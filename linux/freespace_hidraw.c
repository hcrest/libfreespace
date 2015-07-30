/* * libfreespace - library for communicating with Freespace devices
 *
 * Copyright 2015 Hillcrest Labs, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "freespace/freespace.h"
#include "freespace/freespace_deviceTable.h"
#include "freespace_config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <sys/inotify.h>

/**
 * TODO
 *    - support synchronous API
 */

// #define _FREESPACE_DEBUG
// #define _FREESPACE_WARN
// #define _FREESPACE_TRACE

#define LOGF(fmt, lvl, ...) fprintf(stderr, "libfreespace (%20s:%4d): " #lvl " " fmt "\n", __func__, __LINE__, ##__VA_ARGS__);

#ifdef _FREESPACE_WARN
#define WARN(fmt, ...) LOGF(fmt,  WARN, ##__VA_ARGS__)
#else
#define WARN(...)
#endif

#ifdef _FREESPACE_DEBUG
#define DEBUG(fmt, ...) LOGF(fmt, DEBUG, ##__VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifdef _FREESPACE_TRACE
#define TRACE(fmt, ...) LOGF(fmt, TRACE, ##__VA_ARGS__)
#else
#define TRACE(...)
#endif

/**
 * The device state is primarily used to keep track of FreespaceDevice allocations.
 * The state machine looks like the following:
 *
 *     o-->CONNECTED
 *          | ^   |
 *          v |   |
 *        OPENED  |
 *           |    |
 *           v    v
 *         DISCONNECTED
 *
 *  A FreespaceDevice* is allocated when a device is connected.  On the CONNECTED->DISCONNECTED
 *  transition, the hot-plug routine frees the FreespaceDevice*.  On the OPENED->DISCONNECTED
 *  transition, the user may have a reference to a FreespaceDevice*, so it can't be freed then. We
 *  free it on the call to close().
 */

enum FreespaceDeviceState {
    FREESPACE_NONE,
    FREESPACE_CONNECTED,
    FREESPACE_OPENED,
    FREESPACE_DISCONNECTED,
};

#ifdef LIBFREESPACE_THREADED_WRITES
#include <pthread.h>

struct FreespaceDevice;

static const int NUM_MAX_JOBS = 5;
static const int NUM_MAX_FREE_JOBS = 3;

struct FreespaceBGWriteJob {
    int fd;
    int cookie;
    uint8_t message[FREESPACE_MAX_INPUT_MESSAGE_SIZE];
    int length;
    struct FreespaceBGWriteJob * next;
};

struct FreespaceBGWriter {
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;

    struct FreespaceBGWriteJob * head;
    struct FreespaceBGWriteJob * tail;
    struct FreespaceBGWriteJob * free;

    int exitThread;

    int queueLen;
    int numFree;
};

/* Allocate job struct. If the queue is full, drop the next job belonging to
this device. Call with lock held */
static struct FreespaceBGWriteJob * _popFreeJobLocked();
/* Deallocate job struct. Call with lock held */
static int _returnWriteJobLocked(struct FreespaceBGWriteJob *);
/* Pop the next job from the write queue. Call with lock held */
static struct FreespaceBGWriteJob * _popWriteJobLocked();
/* Push a write job to the write_queue. Call with lock held */
static int _pushWriteJobLocked(struct FreespaceBGWriteJob *);
/* Flush all write jobs associated with *devCall with lock held */
static void _flushWriteJobsLocked(struct FreespaceDevice * dev, int l);
/* pthread function for write queue */
static void * _writeThread_fn(void * ptr);

#endif


struct FreespaceDevice {
    FreespaceDeviceId id_; // this id is unique to all connected devices
    enum FreespaceDeviceState state_;

    int fd_;
    int devNum_;
    int cookie_; // this id is unique across all instances
    char hidrawPath_[16];
    struct FreespaceDeviceAPI const * api_;

    freespace_receiveCallback receiveCallback_;
    freespace_receiveMessageCallback receiveMessageCallback_;
    void* receiveCookie_;
    void* receiveMessageCookie_;
};

#define DEV_DIR "/dev"
#define HIDRAW_PREFIX  "hidraw"

#define GET_DEVICE(id, device) \
    struct FreespaceDevice* device = findDeviceById(id); \
    if (device == NULL) { \
        return FREESPACE_ERROR_INVALID_DEVICE; \
    }

#define GET_DEVICE_IF_OPEN(id, device) \
    GET_DEVICE(id, device) \
    switch (device->state_) { \
        case FREESPACE_OPENED: \
            break; \
        case FREESPACE_CONNECTED: \
             /* no error code for "not open" ? */ \
        case FREESPACE_DISCONNECTED: \
            return FREESPACE_ERROR_NO_DEVICE; \
        default:\
            return FREESPACE_ERROR_UNEXPECTED;\
    }


struct freespace_context {
    int numDevices;
    int nextFreeIndex;
    int connectedDevices; // bitmap of connected devices
    struct FreespaceDevice * devices[FREESPACE_MAXIMUM_DEVICE_COUNT];

    int inotify_fd;
    int inotify_wd;

    freespace_pollfdAddedCallback userAddedCallback;
    freespace_pollfdRemovedCallback userRemovedCallback;
    freespace_hotplugCallback hotplugCallback;
    void* hotplugCookie;

#ifdef LIBFREESPACE_THREADED_WRITES
    struct FreespaceBGWriter writer;
#endif
};

/* global variables */
static struct freespace_context ctx_;

/* local functions */
static int _inotify_init();
static int _inotify_process();
static int _readDevice(struct FreespaceDevice * device);
static int _disconnect(struct FreespaceDevice * device);
static void _deallocateDevice(struct FreespaceDevice* device);
static int _write(int fd, const uint8_t* message, int length);
static int _scanAllDevices();

const char* freespace_version() {
    return LIBFREESPACE_VERSION;
}

static struct FreespaceDevice* findDeviceById(FreespaceDeviceId id) {
    int i;
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        if (ctx_.devices[i] != NULL && ctx_.devices[i]->id_ == id) {
            return ctx_.devices[i];
        }
    }

    return NULL;
}

// Initialize inotify
int freespace_init() {
    int rc = 0;
    memset(&ctx_, 0, sizeof(ctx_));
    rc = _inotify_init();
    if (rc != 0) {
        return rc;
    }

#ifdef LIBFREESPACE_THREADED_WRITES
    rc = pthread_create(&ctx_.writer.thread, NULL, &_writeThread_fn, NULL);
    //pthread_setname_np(ctx_.writer.thread, "libfreespace-write");

    if (rc < 0) {
        WARN("pthread_create failed: %s", strerror(errno));
        return FREESPACE_ERROR_COULD_NOT_CREATE_THREAD;
    }

    pthread_mutex_init(&ctx_.writer.mutex, NULL);
    pthread_cond_init(&ctx_.writer.cond, NULL);
#endif

    return FREESPACE_SUCCESS;
}

// Disconnect, deallocate device and remove all callbacks
void freespace_exit() {
    int i;
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        struct FreespaceDevice * device = ctx_.devices[i];
        if (device == NULL) {
            continue;
        }

        if (device->state_ != FREESPACE_DISCONNECTED) {
            _disconnect(device);
        }

        if (ctx_.devices[i]) {
            _deallocateDevice(ctx_.devices[i]);
        }
    }

    if (ctx_.inotify_fd > 0) {
        if (ctx_.userRemovedCallback) {
            ctx_.userRemovedCallback(ctx_.inotify_fd);
        }
    }

#ifdef LIBFREESPACE_THREADED_WRITES
    // Signal the thread to shutdown...
    ctx_.writer.exitThread = 1;
    pthread_cond_signal(&ctx_.writer.cond);

    pthread_join(ctx_.writer.thread, NULL);
    pthread_mutex_destroy(&ctx_.writer.mutex);
    pthread_cond_destroy(&ctx_.writer.cond);
#endif

    return;
}


int freespace_setDeviceHotplugCallback(freespace_hotplugCallback callback,
                                       void* cookie) {
    ctx_.hotplugCallback = callback;
    ctx_.hotplugCookie = cookie;
    return FREESPACE_SUCCESS;
}

int freespace_getDeviceList(FreespaceDeviceId* idList,
                            int maxIds,
                            int* numIds) {
    int i;
    int rc;
    *numIds = 0;

    rc = _inotify_process();
    if (rc != FREESPACE_SUCCESS) {
        return rc;
    }

    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT && *numIds < maxIds; i++) {
        if (ctx_.devices[i] != NULL) {
            idList[*numIds] = ctx_.devices[i]->id_;
            *numIds = *numIds + 1;
        }
    }

    return FREESPACE_SUCCESS;
}

int freespace_getDeviceInfo(FreespaceDeviceId id,
                            struct FreespaceDeviceInfo* info) {
    GET_DEVICE(id, device);

    info->vendor = device->api_->idVendor_;
    info->product = device->api_->idProduct_;
    info->name = device->api_->name_;
    info->hVer = device->api_->hVer_;
    return FREESPACE_SUCCESS;
}

// This hidraw implementation handles only async messages
int freespace_openDevice(FreespaceDeviceId id) {
    GET_DEVICE(id, device);

    if (device->state_ == FREESPACE_DISCONNECTED) {
        return FREESPACE_ERROR_NO_DEVICE;
    }

    if (device->state_ == FREESPACE_OPENED) {
        return FREESPACE_SUCCESS;
    }

    if (device->state_ != FREESPACE_CONNECTED) {
        return FREESPACE_ERROR_UNEXPECTED;
    }

    device->fd_ = open(device->hidrawPath_, O_RDWR | O_NONBLOCK);
    if (device->fd_ < 0) {
        WARN("Failed opening %s: %s", device->hidrawPath_, strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    // flush the device
    uint8_t buf[1024];
    while (read(device->fd_, buf, sizeof(buf)) > 0);

    if (ctx_.userAddedCallback) {
        ctx_.userAddedCallback(device->fd_, POLLIN);
    }

    device->state_ = FREESPACE_OPENED;
    return FREESPACE_SUCCESS;
}

void freespace_closeDevice(FreespaceDeviceId id) {
    struct FreespaceDevice* device = findDeviceById(id);
    if (device == NULL) {
        DEBUG("closeDevice() -- failed to get device %d", id);
        return;
    }

    if (device->state_ == FREESPACE_CONNECTED) {
        TRACE("closeDevice() that is not opened");
        // not open
        return;
    }

    if (device->state_ == FREESPACE_OPENED) {
        DEBUG("closeDevice() opened device");
#ifdef LIBFREESPACE_THREADED_WRITES
        pthread_mutex_lock(&ctx_.writer.mutex );
        _flushWriteJobsLocked(device, -1);
        pthread_mutex_unlock(&ctx_.writer.mutex);
#endif
        // return the device to the "connected" state
        if (device->fd_ > 0) {
            if (ctx_.userRemovedCallback) {
                ctx_.userRemovedCallback(device->fd_);
            }
            close(device->fd_);
            device->fd_ = -1;
        }
        device->state_ = FREESPACE_CONNECTED;
        return;
    }

    if (device->state_ == FREESPACE_DISCONNECTED) {
        DEBUG("closeDevice() already disconnected");
        // device is disconnected...
        // we've been waiting for this close() to deallocate it.
        _deallocateDevice(device);
        return;
    }
    DEBUG("Closed device %d", id);
}

int freespace_private_send(FreespaceDeviceId id, const uint8_t* message, int length) {
    return FREESPACE_ERROR_UINIMPLEMENTED;
}

int freespace_sendMessage(FreespaceDeviceId id, struct freespace_message* message) {
    int rc;
    uint8_t msgBuf[FREESPACE_MAX_OUTPUT_MESSAGE_SIZE];
    GET_DEVICE_IF_OPEN(id, device);
    
    // Address is reserved for now and must be set to 0 by the caller.
    if (message->dest == 0) {
        message->dest = FREESPACE_RESERVED_ADDRESS;
    }
    
    message->ver = device->api_->hVer_;

    rc = freespace_encode_message(message, msgBuf, FREESPACE_MAX_OUTPUT_MESSAGE_SIZE);
    if (rc <= FREESPACE_SUCCESS) {
        return rc;
    }
    
    return freespace_private_send(id, msgBuf, rc);
}

int freespace_private_read(FreespaceDeviceId id,
                           uint8_t* message,
                           int maxLength,
                           unsigned int timeoutMs,
                           int* actualLength) {
    GET_DEVICE_IF_OPEN(id, device);

    // TODO
    return FREESPACE_ERROR_UINIMPLEMENTED;
}

int freespace_readMessage(FreespaceDeviceId id,
                          struct freespace_message* message,
                          unsigned int timeoutMs) {
    GET_DEVICE_IF_OPEN(id, device);
    return FREESPACE_ERROR_UINIMPLEMENTED;

}

int freespace_flush(FreespaceDeviceId id) {
    // TODO
    return FREESPACE_ERROR_UINIMPLEMENTED;

}

int _write(int fd, const uint8_t* message, int length) {
    int rc = write(fd, message, length);
    if (rc < 0) {
        if (errno == ENOENT || errno == ENODEV) {
            // disconnected.... hot-plug will catch this later
            return FREESPACE_ERROR_NO_DEVICE;
        }

        if (errno == ETIMEDOUT) {
            return FREESPACE_ERROR_TIMEOUT;
        }

        WARN("Write failed: %s", strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    if (rc != length) {
        WARN("Write failed. Wrote %d bytes of %d", rc, length);
        return FREESPACE_ERROR_IO;
    }

    return FREESPACE_SUCCESS;
}

int freespace_private_sendAsync(FreespaceDeviceId id,
                                const uint8_t* message,
                                int length,
                                unsigned int timeoutMs,
                                freespace_sendCallback callback,
                                void* cookie) {
#ifndef LIBFREESPACE_THREADED_WRITES

    GET_DEVICE_IF_OPEN(id, device);
    return _write(device->fd_, message, length);
#else
    ssize_t rc;
    struct FreespaceBGWriteJob * job;

    GET_DEVICE_IF_OPEN(id, device);

    pthread_mutex_lock(&ctx_.writer.mutex );
    job = _popFreeJobLocked(device);
    if (!job) {
        if (ctx_.writer.queueLen + 1 > NUM_MAX_JOBS) {
            // our queue is full, we need to evict a job belonging to this device
            _flushWriteJobsLocked(device, 1);
            // there should be a job in our free pool now
            job = _popFreeJobLocked(device);
        } else {
            // ok, create a new job
            job = malloc(sizeof(struct FreespaceBGWriteJob));
        }
    }

    if (job) {
        memset(job, 0, sizeof(*job));
        // we'll use the fd to
        job->fd = device->fd_;
        job->cookie = device->cookie_;
        memcpy(job->message, message, length);
        job->length = length;

        rc = _pushWriteJobLocked(job);
    } else {
        WARN("error allocating a write job");
    }

    pthread_mutex_unlock(&ctx_.writer.mutex);
    pthread_cond_signal(&ctx_.writer.cond);
    return rc;
#endif
}

int freespace_sendMessageAsync(FreespaceDeviceId id,
                               struct freespace_message* message,
                               unsigned int timeoutMs,
                               freespace_sendCallback callback,
                               void* cookie) {

    int rc;
    uint8_t msgBuf[FREESPACE_MAX_OUTPUT_MESSAGE_SIZE];
    GET_DEVICE_IF_OPEN(id, device);

    // Address is reserved for now and must be set to 0 by the caller.
    if (message->dest == 0) {
        message->dest = FREESPACE_RESERVED_ADDRESS;
    }
    message->ver = device->api_->hVer_;

    rc = freespace_encode_message(message, msgBuf, FREESPACE_MAX_OUTPUT_MESSAGE_SIZE);
    if (rc <= FREESPACE_SUCCESS) {
        return rc;
    }

    return freespace_private_sendAsync(id, msgBuf, rc, timeoutMs, callback, cookie);
}

int freespace_getNextTimeout(int* timeoutMsOut) {
    // TODO
    *timeoutMsOut = -1;
    return FREESPACE_SUCCESS;
}

int freespace_perform() {
    int i;
    int n;
    int nfds;
    int rc;
    static int needToRescan = 1;

    // Initial scan of all devices
    if (needToRescan) {
        _scanAllDevices();
        needToRescan = 0;
    }

    struct pollfd fds[FREESPACE_MAXIMUM_DEVICE_COUNT + 1];

    // Populate fds[]
    fds[0].fd = ctx_.inotify_fd;
    fds[0].events = POLLIN;

    for (i = 0, n = 0; n < ctx_.numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; ++i) {
        struct FreespaceDevice * device = ctx_.devices[i];
        if (!device) {
            continue;
        }

        ++n;
        fds[n].fd = device->fd_;
        fds[n].events = POLLIN | POLLHUP | POLLERR;
    }

    // Poll open file descriptors
    nfds = poll(fds, ctx_.numDevices + 1, 0);

    i = 0;
    n = 0;
    if (nfds > 0 ) {
        // inotify events
        if (fds[0].revents & POLLIN) {

            fds[0].revents = 0;
            --nfds;
            if ((rc = _inotify_process())) {
                return rc;
            }
        }

        // Other device->fd_
        while (nfds > 0) {
            while (n < ctx_.numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT) {
                struct FreespaceDevice * device = ctx_.devices[i];
                if (!device) {
                    ++i;
                    continue;
                }
                ++n;
                ++i;

                if (fds[n].revents) {
                    if (fds[n].revents & (POLLHUP | POLLERR)) {
                        DEBUG("Disconnect device %d", device->id_);
                        rc = _disconnect(device);
                    }
                    if (fds[n].revents & POLLIN) {
                        if (device->state_ == FREESPACE_OPENED) {
                            rc = _readDevice(device);
                        }
                    }
                    if (rc) {
                        return rc;
                    }
                    --nfds;
                    break;
                }
            }
        }
    } else if (nfds < 0 ) {
        WARN("poll() failed: %s", strerror(errno));
        return FREESPACE_ERROR_UNEXPECTED;
    }

    return FREESPACE_SUCCESS;
}

void freespace_setFileDescriptorCallbacks(freespace_pollfdAddedCallback addedCallback,
                                          freespace_pollfdRemovedCallback removedCallback) {
    ctx_.userAddedCallback = addedCallback;
    ctx_.userRemovedCallback = removedCallback;
}

int freespace_syncFileDescriptors() {
    int i;
    int n;

    if (ctx_.userAddedCallback == NULL) {
        return FREESPACE_SUCCESS;
    }

    // Add the hot-plug inotify's fd
    ctx_.userAddedCallback(ctx_.inotify_fd, POLLIN);

    i = 0;
    n = 0;
    for (; n < ctx_.numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        struct FreespaceDevice * device = ctx_.devices[i];
        if (device) {
            n++;
            if (device->state_ == FREESPACE_OPENED) {
                // assert(device->fd_ > 0);
                ctx_.userAddedCallback(device->fd_, POLLIN);
            }
        }
    }

    return FREESPACE_SUCCESS;
}

int freespace_private_setReceiveCallback(FreespaceDeviceId id,
                                         freespace_receiveCallback callback,
                                         void* cookie) {
    GET_DEVICE(id, device);

    device->receiveCallback_ = callback;
    device->receiveCookie_ = cookie;

    return FREESPACE_SUCCESS;
}

int freespace_setReceiveMessageCallback(FreespaceDeviceId id,
                                        freespace_receiveMessageCallback callback,
                                        void* cookie) {
    GET_DEVICE(id, device);

    device->receiveMessageCallback_ = callback;
    device->receiveMessageCookie_ = cookie;

    return FREESPACE_SUCCESS;
}

static int _readDevice(struct FreespaceDevice * device) {
    ssize_t rc;
    uint8_t buf[FREESPACE_MAX_OUTPUT_MESSAGE_SIZE];

    while (1) {
        rc = read(device->fd_, buf, sizeof(buf));
        if (rc < 0) {
            if (errno == EAGAIN) {
                // no more data
                break;
            }

            if (errno == ENOENT || errno == ENODEV) {
                // Disconnected.... hot-plug will catch this later and notify
                return FREESPACE_ERROR_NO_DEVICE;
            }

            WARN("Failed reading %s: %s", device->hidrawPath_, strerror(errno));
            return FREESPACE_ERROR_IO;
        }

        if (rc == 0) { // EOF
            // Disconnected.... hot-plug will catch this later and notify
            return FREESPACE_ERROR_NO_DEVICE;
        }

        if (device->receiveCallback_) {
            device->receiveCallback_(device->id_, buf, (int) rc, device->receiveCookie_, FREESPACE_SUCCESS);
        }

        if (device->receiveMessageCallback_) {
            struct freespace_message m;

            rc = freespace_decode_message(buf, rc, &m, device->api_->hVer_);

            device->receiveMessageCallback_(
                    device->id_,
                    rc == FREESPACE_SUCCESS ? &m : NULL,
                    device->receiveMessageCookie_, rc);
        }
    }
    return FREESPACE_SUCCESS;
}

// check if device at hidraw path is a Freespace device.
static int _isFreespaceDevice(const char * path, struct FreespaceDeviceAPI const ** API) {

    int i;
    int rc;
    int stringFound = 0;
    struct hidraw_devinfo info;
    int fd = open(path, O_RDONLY);

    *API = 0;
    if (fd < 0) {
        if (errno != EAGAIN) {
            WARN("Failed opening %s: %s", path, strerror(errno));
            return FREESPACE_ERROR_IO;
        }
        return FREESPACE_SUCCESS;
    }

    rc = ioctl(fd, HIDIOCGRAWINFO, &info);
    if (rc < 0) {
        WARN("Failed getting hidraw info for %s: %s", path, strerror(errno));
        close(fd);
        return FREESPACE_ERROR_IO;
    }

    {
        // Search for 06 01 FF 09 04 A1 in the HID descriptor
        #define FREESPACE_HID_STRING_LEN 6
        const __u8 FREESPACE_HID_STRING[FREESPACE_HID_STRING_LEN] = {0x06, 0x01, 0xFF, 0x09, 0x04, 0xA1};
        struct hidraw_report_descriptor descriptor;

        rc = ioctl(fd, HIDIOCGRDESCSIZE, &descriptor.size);
        if (rc < 0) {
            DEBUG("HIDIOCGRDESCSIZE %s: %s", path, strerror(errno));
            close(fd);
            return FREESPACE_ERROR_IO;
        }
        rc = ioctl(fd, HIDIOCGRDESC, &descriptor);
        if (rc < 0) {
            DEBUG("HIDIOCGRDESC %s: %s", path, strerror(errno));
            close(fd);
            return FREESPACE_ERROR_IO;
        }

        for (i = 0; i < (int)(descriptor.size - FREESPACE_HID_STRING_LEN); ++i) {
            if (memcmp(FREESPACE_HID_STRING, &descriptor.value[i], FREESPACE_HID_STRING_LEN) == 0) {
                stringFound = 1;
                break;
            }
        }
    }

    if (stringFound) {
        TRACE("Freespace device found: %s", path);
        for (i = 0; i < freespace_deviceAPITableNum; i++) {
            struct FreespaceDeviceAPI const * api = &freespace_deviceAPITable[i];
            if (api->idVendor_ != info.vendor) {
                continue;
            }

            if ((api->idProduct_ & api->mask_) != (info.product & api->mask_)) {
                continue;
            }

            *API = api;
            break;
        }
    }

    close(fd);
    return FREESPACE_SUCCESS;
}

static int _allocateNewDevice(struct FreespaceDevice** out_device) {
    struct FreespaceDevice* device;
    *out_device = 0;

    if (ctx_.nextFreeIndex == -1) {
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }

    device = (struct FreespaceDevice*) malloc(sizeof(struct FreespaceDevice));
    if (device == NULL) {
        // Out of memory.
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }
    memset(device, 0, sizeof(struct FreespaceDevice));
    device->cookie_ = ++ctx_.numDevices;

    ctx_.devices[ctx_.nextFreeIndex] = device;
    if (ctx_.numDevices < FREESPACE_MAXIMUM_DEVICE_COUNT) {
        while (ctx_.devices[ctx_.nextFreeIndex] != NULL) {
            ctx_.nextFreeIndex++;
            if (ctx_.nextFreeIndex == FREESPACE_MAXIMUM_DEVICE_COUNT) {
                ctx_.nextFreeIndex = 0;
            }
        }
    } else {
        ctx_.nextFreeIndex = -1;
    }

    * out_device = device;
    return FREESPACE_SUCCESS;
}


static FreespaceDeviceId _assignId() {
    int i;
    
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; ++i) {
        if ((ctx_.connectedDevices & (1 << i)) == 0) {
            ctx_.connectedDevices |= (1 << i);
            DEBUG("Device ID %d is connected", i);
            return i;
        }
    }
    return -1;
}

static int _scanDevice(const char * devName) {

    int rc, i, n, devNum;
    char absPath[NAME_MAX] = "";
    struct FreespaceDevice * device;
    struct FreespaceDeviceAPI const * API = 0;

    // get <num> from hidraw<num>
    if (sscanf(devName, "hidraw%u", &devNum) != 1) {
        return FREESPACE_ERROR_UNEXPECTED;
    }

    for (i = 0, n = 0; n < ctx_.numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        device = ctx_.devices[i];

        if (device == 0) {
            continue;
        }

        n++;
        if (device->devNum_ != devNum) {
            continue;
        }

        switch (device->state_) {
            case FREESPACE_OPENED:
            case FREESPACE_CONNECTED:
                // known device
                return FREESPACE_SUCCESS;

            case FREESPACE_DISCONNECTED:
                // this is a "ghost" device that close() has not been called on.
                return FREESPACE_SUCCESS;

            default:
                WARN("unexpected state: %d", (int) device->state_);
                return FREESPACE_ERROR_UNEXPECTED;
        }
    }

    snprintf(absPath, sizeof(absPath), "%s/%s", DEV_DIR, devName);
    if ((rc = access(absPath, R_OK | W_OK))) {
        // can't access this file, just skip
        DEBUG(" -- %s: %s", absPath, strerror(errno));
        return FREESPACE_SUCCESS;
    }

    rc = _isFreespaceDevice(absPath, &API);
    if (!API) {
        TRACE("Not a freespace device: %s", absPath);
        return rc;
    }

    // Allocate a device
    {

        int rc = _allocateNewDevice(&device);
        if (rc != FREESPACE_SUCCESS) {
            return rc;
        }

        device->state_ = FREESPACE_CONNECTED;
        device->fd_ = -1;
        device->id_ = _assignId();
        device->devNum_ = devNum;
        strncpy(device->hidrawPath_, absPath, sizeof(device->hidrawPath_));
        device->api_ = API;

        if (ctx_.hotplugCallback) {
            ctx_.hotplugCallback(FREESPACE_HOTPLUG_INSERTION, device->id_, ctx_.hotplugCookie);
        }
    }

    DEBUG("Found freespace device at %s. ** Num devices: %d **", absPath, ctx_.numDevices);
    return FREESPACE_SUCCESS;
}

// Check whether a hidraw device is added/removed to/from the device directory /dev)
static int _scanAllDevices() {
    TRACE("Scanning all hidraw devices");
    // Check if a device has been added (iterate all of /dev)
    DIR* dev_dir = opendir(DEV_DIR);
    if (dev_dir) {
        struct dirent*  ent;

        while ( (ent = readdir(dev_dir)) != NULL ) {
            if (strncmp(ent->d_name, HIDRAW_PREFIX, strlen(HIDRAW_PREFIX)) != 0) {
                continue;
            }

            _scanDevice(ent->d_name);
        }
    } else {
        WARN("Failed opening %s", DEV_DIR);
        return FREESPACE_ERROR_ACCESS;
    }

    // TODO handle the case where devices drop when in the "connected" but not "opened" state...
    closedir(dev_dir);
    return FREESPACE_SUCCESS;
}

// Create and initialize inotify instance
// Add watch to about events specified by when new file is created or deleted in
// the device directory (/dev)
static int _inotify_init() {
    int rc;

    ctx_.inotify_fd = inotify_init();
    if (ctx_.inotify_fd < 0) {
        WARN("Failed inotify_init: %s", strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    rc = fcntl(ctx_.inotify_fd, F_SETFL, O_NONBLOCK);  // Set to non-blocking
    if (rc < 0) {
        WARN("Failed inotify -> non block: %s", strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    // watch for files added or permissions changed under /dev
    ctx_.inotify_wd = inotify_add_watch(ctx_.inotify_fd, DEV_DIR, IN_CREATE | IN_ATTRIB);
    if (ctx_.inotify_wd < 0) {
        WARN("Failed inotify_add_watch: %s", strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    if (ctx_.userAddedCallback) {
        ctx_.userAddedCallback(ctx_.inotify_fd, POLLIN);
    }
    return FREESPACE_SUCCESS;
}

static int _inotify_process() {

    // Process inotify events
    char buf[sizeof(struct inotify_event) + NAME_MAX + 1];

    // inotify returns only 1 event at a time.
    int rc = read(ctx_.inotify_fd, buf, sizeof(buf));

    if (rc < 0) {
        if (errno == EAGAIN) {
            // done!
            return FREESPACE_SUCCESS;
        }
        WARN("inotify: fd read error: %s", strerror(errno));
        return FREESPACE_ERROR_IO;
    }

    struct inotify_event * event = (struct inotify_event *) (buf);

    if (event->wd != ctx_.inotify_wd) {
        WARN("inotify: watchdog does not match! -- %d != %d", event->wd, ctx_.inotify_wd);
        return FREESPACE_ERROR_IO;
    }

    if (event->len > sizeof(buf)) {
        TRACE("inotify: event read length violation. event size: %u, buffer size: %zu",
              event->len, sizeof(buf));
        return FREESPACE_ERROR_IO;
    }

    if (strncmp(event->name, HIDRAW_PREFIX, strlen(HIDRAW_PREFIX)) != 0) {
        TRACE("inotify: skip event - %s/%s:%04x ", DEV_DIR, event->name, event->mask);
        return FREESPACE_SUCCESS;
    }

    DEBUG("inotify: handle event - %s/%s:%04x ", DEV_DIR, event->name, event->mask);
    if (event->mask & (IN_CREATE | IN_ATTRIB)) {
        return _scanDevice(event->name);
    }

    return FREESPACE_SUCCESS;
}

static void _deallocateDevice(struct FreespaceDevice* device) {
    int i;
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        if (ctx_.devices[i] == device) {
            if (ctx_.nextFreeIndex == -1) {
                ctx_.nextFreeIndex = i;
            }

#if 1 // this should not be necessary.
            if (device->fd_ > 0) {
                DEBUG("Deallocate device (%s) -- fd still open!", device->hidrawPath_)
                if (ctx_.userRemovedCallback) {
                    ctx_.userRemovedCallback(device->fd_);
                }
                close(device->fd_);
                device->fd_ = -1;
            }
#endif
            free(device);
            ctx_.devices[i] = NULL;
            ctx_.numDevices--;
            DEBUG("Freed device. ** Num devices: %d **", ctx_.numDevices);
            return;
        }
    }

    WARN("Could not deallocate %p", device);
}

static int _disconnect(struct FreespaceDevice * device) {
    DEBUG("Freespace device (%d) at %s disconnected", device->id_, device->hidrawPath_);

#ifdef LIBFREESPACE_THREADED_WRITES
    pthread_mutex_lock(&ctx_.writer.mutex );
    _flushWriteJobsLocked(device, -1);
    pthread_mutex_unlock(&ctx_.writer.mutex);
#endif
    // device is currently in use, we can't delete it outright
    if (device->state_ == FREESPACE_OPENED) {
        if (device->fd_ > 0) {
            if (ctx_.userRemovedCallback) {
                ctx_.userRemovedCallback(device->fd_);
            }
            close(device->fd_);
            device->fd_ = -1;
        }

        // Indicate that the device is disconnected so that its ID can be reused
        ctx_.connectedDevices &= ~((int)(1 << device->id_));
        WARN("Device ID %d is disconnected", device->id_);

        device->state_ = FREESPACE_DISCONNECTED;
        TRACE("*** Sending removal notification for device %d while opened", device->id_);
        if (ctx_.hotplugCallback) {
            ctx_.hotplugCallback(FREESPACE_HOTPLUG_REMOVAL, device->id_, ctx_.hotplugCookie);
        }

        // we have to wait for closeDevice() to deallocate this device.
        return FREESPACE_SUCCESS;
    }

    if (device->state_ == FREESPACE_CONNECTED) {
        int id = device->id_;
        if (device->fd_ > 0) {
            if (ctx_.userRemovedCallback) {
                ctx_.userRemovedCallback(device->fd_);
            }
            close(device->fd_);
            device->fd_ = -1;
        }

        // Indicate that the device is disconnected so that its ID can be reused
        ctx_.connectedDevices &= ~((int)(1 << device->id_));
        WARN("Device ID %d is disconnected", device->id_);

        _deallocateDevice(device);
        device = NULL;

        TRACE("*** Sending removal notification for device %d while connected", id);
        if (ctx_.hotplugCallback) {
            ctx_.hotplugCallback(FREESPACE_HOTPLUG_REMOVAL, id, ctx_.hotplugCookie);
        }

        return FREESPACE_SUCCESS;
    }

    return FREESPACE_ERROR_UNEXPECTED;
}

#ifdef LIBFREESPACE_THREADED_WRITES

static struct FreespaceBGWriteJob * _popFreeJobLocked() {
    struct FreespaceBGWriteJob * j;
    if (ctx_.writer.free != NULL) {
        j = ctx_.writer.free;
        ctx_.writer.free = ctx_.writer.free->next;
        j->next = NULL;
        return j;
    }

    return NULL;
}

static int _returnWriteJobLocked(struct FreespaceBGWriteJob * j) {
    if (ctx_.writer.numFree + 1 > NUM_MAX_FREE_JOBS) {
        free(j);
        return 0;
    }

    ctx_.writer.numFree++;
    j->next = ctx_.writer.free;
    ctx_.writer.free = j;
    return FREESPACE_SUCCESS;
}


static struct FreespaceBGWriteJob * _popWriteJobLocked() {
    struct FreespaceBGWriteJob * j = NULL;

    if (ctx_.writer.head != NULL) {
        j = ctx_.writer.head;
        ctx_.writer.head = j->next;
        if (ctx_.writer.head == NULL) {
            ctx_.writer.tail = NULL;
        }
    }

    ctx_.writer.queueLen--;
    return j;
}

static int _pushWriteJobLocked(struct FreespaceBGWriteJob * j) {
    if (ctx_.writer.tail == NULL) {
        ctx_.writer.head = j;
        ctx_.writer.tail = j;
    } else {
        ctx_.writer.tail->next = j;
        ctx_.writer.tail = j;
    }

    ctx_.writer.queueLen++;
    return FREESPACE_SUCCESS;
}

static void _flushWriteJobsLocked(struct FreespaceDevice * dev, int limit) {
    struct FreespaceBGWriteJob * j = ctx_.writer.head;
    struct FreespaceBGWriteJob * prev = NULL;
    struct FreespaceBGWriteJob * next = NULL;

    if (j == NULL) {
        return;
    }

    while (j && limit) {
        if (j->cookie != dev->cookie_) {
            // keep this job
            prev = j;
            j = j->next;
        }

        // we have to remove j from the linked list
        if (prev == NULL) {
            ctx_.writer.head = j->next;
        }

        next = j->next;
        _returnWriteJobLocked(j);
        if (limit > 0) {
            limit--;
        }
        if (prev) {
            prev->next = next;
        }
        j = next;
    }
}

static void * _writeThread_fn(void * ptr) {
    while (ctx_.writer.exitThread == 0) {
          struct FreespaceBGWriteJob * j;

         // Lock mutex and then wait for signal to relase mutex
          pthread_mutex_lock(&ctx_.writer.mutex );
          // wait for signal..
          pthread_cond_wait(&ctx_.writer.cond, &ctx_.writer.mutex );
          while (ctx_.writer.exitThread == 0 && (j = _popWriteJobLocked()) != NULL) {
              pthread_mutex_unlock(&ctx_.writer.mutex);
              _write(j->fd, j->message, j->length);
              pthread_mutex_lock(&ctx_.writer.mutex );
              _returnWriteJobLocked(j);
          }

          pthread_mutex_unlock(&ctx_.writer.mutex);
    }

    return 0;
}

#endif
