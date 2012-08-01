/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2009-2010 Hillcrest Laboratories, Inc.
 *
 * libfreespace is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "../include/freespace/freespace.h"
#include "../include/freespace/freespace_deviceTable.h"
#include "hotplug.h"
#include "../config.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/**
 * TODO
 *    - bluetooth write support on Ubunutu/hidp.. wtf?
 * 	  - add sync support?
 */

//#define _FREESPACE_WARN
//#define _FREESPACE_DEBUG
//#define _FREESPACE_TRACE

#define LOGF(fmt, lvl, ...) printf("libfreespace (freespace_hidraw.c:%d): " #lvl " " fmt "\n", __LINE__, __VA_ARGS__);

#ifdef _FREESPACE_WARN
#define WARN(fmt, ...) LOGF(fmt, WARN, __VA_ARGS__)
#else
#define WARN(...)
#endif

#ifdef _FREESPACE_DEBUG
#define DEBUG(fmt, ...) LOGF(fmt, DEBUG, __VA_ARGS__)
#else
#define DEBUG(...)
#endif

#ifdef _FREESPACE_TRACE
#define TRACE(fmt, ...) LOGF(fmt, TRACE, __VA_ARGS__)
#else
#define TRACE(...)
#endif

#define FREESPACE_RECEIVE_QUEUE_SIZE 8 // Could be tuned better. 3-4 might be good enough

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
 *  transition, the hotplug routine frees the FreespaceDevice*.  On the OPENED->DISCONNECTED
 *  transition, the user may have a reference to a FreespaceDevice*, so it can't be freed then. We
 *  free it on the call to close().
 */

enum FreespaceDeviceState {
	FREESPACE_NONE,
    FREESPACE_CONNECTED,
    FREESPACE_OPENED,
    FREESPACE_DISCONNECTED,
};

struct FreespaceDevice {
    FreespaceDeviceId id_;
    enum FreespaceDeviceState state_;

    int fd_;
    int num_;
    char hidrawPath_[16];
    struct FreespaceDeviceAPI const * api_;

    freespace_receiveCallback receiveCallback_;
    freespace_receiveMessageCallback receiveMessageCallback_;
    void* receiveCookie_;
    void* receiveMessageCookie_;
};


#define DEV_DIR "/dev/"
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

/* global variables */
static int numDevices = 0;
static FreespaceDeviceId nextFreeIndex = 0;
static struct FreespaceDevice* devices[FREESPACE_MAXIMUM_DEVICE_COUNT];

static freespace_pollfdAddedCallback userAddedCallback = NULL;
static freespace_pollfdRemovedCallback userRemovedCallback = NULL;
static freespace_hotplugCallback hotplugCallback = NULL;
static void* hotplugCookie;

static int _scanDevices();
static int _pollDevice(struct FreespaceDevice * device);
static int _disconnect(struct FreespaceDevice * device);
static void _deallocateDevice(struct FreespaceDevice* device);

const char* freespace_version() {
    return VERSION;
}

static struct FreespaceDevice* findDeviceById(FreespaceDeviceId id) {
    int i;
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        if (devices[i] != NULL && devices[i]->id_ == id) {
            return devices[i];
        }
    }

    return NULL;
}

int freespace_init() {

	int rc = 0;

	memset(&devices, 0, sizeof(devices));

	rc = freespace_hotplug_init();
	if (rc < 0) {
		return rc;
	}

    return FREESPACE_SUCCESS;
}

void freespace_exit() {
	// TODO
}


int freespace_setDeviceHotplugCallback(freespace_hotplugCallback callback,
                                       void* cookie) {
    hotplugCallback = callback;
    hotplugCookie = cookie;
    return FREESPACE_SUCCESS;
}

int freespace_getDeviceList(FreespaceDeviceId* idList,
                            int maxIds,
                            int* numIds) {
    int i;
    int rc;
    *numIds = 0;

    rc = _scanDevices();
    if (rc != FREESPACE_SUCCESS) {
        return rc;
    }

    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT && *numIds < maxIds; i++) {
        if (devices[i] != NULL) {
            idList[*numIds] = devices[i]->id_;
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

// this hidraw impl handles only async messages
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

    if (userAddedCallback) {
    	userAddedCallback(device->fd_, POLLIN);
    }

    device->state_ = FREESPACE_OPENED;
    return FREESPACE_SUCCESS;
}

void freespace_closeDevice(FreespaceDeviceId id) {
	struct FreespaceDevice* device = findDeviceById(id);
	if (device == NULL) {
		return;
	}

	if (device->state_ == FREESPACE_CONNECTED) {
		// not open
		return;
	}

	if (device->state_ == FREESPACE_OPENED) {
		// return the device to the "connected" state
		if (device->fd_ > 0) {
			if (userRemovedCallback) {
				userRemovedCallback(device->fd_);
			}
			close(device->fd_);
			device->fd_ = -1;
		}
		device->state_ = FREESPACE_CONNECTED;
	}

	if (device->state_ == FREESPACE_DISCONNECTED) {
		// device is disconnected...
		// we've been waiting for this close() to deallocate it.
		_deallocateDevice(device);
		return;
	}
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
    int rc;
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

int freespace_private_sendAsync(FreespaceDeviceId id,
                                const uint8_t* message,
                                int length,
                                unsigned int timeoutMs,
                                freespace_sendCallback callback,
                                void* cookie) {
    ssize_t rc;
    GET_DEVICE_IF_OPEN(id, device);

    TRACE(" Write >> ", "" );
    rc = write(device->fd_, message, length);
    TRACE(" Write << ", "" );

    if (rc < 0) {
    	if (errno == ENOENT || errno == ENODEV) {
    		// disconnected.... hotplug will catch this later
    		return FREESPACE_ERROR_NO_DEVICE;
    	}
    	WARN("Failed writing to %s: %s", device->hidrawPath_, strerror(errno));
    	return FREESPACE_ERROR_IO;
    }

    if (rc == 0) {
    	WARN("Failed writing to %s. Wrote 0 bytes... busy?", device->hidrawPath_, rc, length);
    	return FREESPACE_ERROR_BUSY;
    }

    if (rc != length) {
    	WARN("Failed writing to %s. Wrote %d bytes of %d", device->hidrawPath_, rc, length);
    	return FREESPACE_ERROR_IO;
    }

    TRACE("Wrote %d bytes to %s", length, device->hidrawPath_);
    return FREESPACE_SUCCESS;
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

	_scanDevices();

	i = 0;
	n = 0;
	for (; n < numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
		struct FreespaceDevice * device = devices[i];
		if (!device) {
			continue;
		}
		n++;

		switch (device->state_) {
			case FREESPACE_CONNECTED:
				// no-op
				break;

			case FREESPACE_OPENED:
				_pollDevice(device);
				break;

			case FREESPACE_DISCONNECTED:
				// no-op
				break;
		}
	}

    return FREESPACE_SUCCESS;
}

void freespace_setFileDescriptorCallbacks(freespace_pollfdAddedCallback addedCallback,
                                          freespace_pollfdRemovedCallback removedCallback) {
    userAddedCallback = addedCallback;
    userRemovedCallback = removedCallback;
}

int freespace_syncFileDescriptors() {
    int i;
	int n;

    if (userAddedCallback == NULL) {
        return FREESPACE_SUCCESS;
    }

    // Add the hotplug code's fd
    userAddedCallback(freespace_hotplug_getFD(), POLLIN);

	i = 0;
	n = 0;
	for (; n < numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
		struct FreespaceDevice * device = devices[i];
		if (device) {
			n++;
			if (device->state_ == FREESPACE_OPENED) {
				// assert(device->fd_ > 0);
				userAddedCallback(device->fd_, POLLIN);
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
    int rc;
    GET_DEVICE(id, device);

    device->receiveMessageCallback_ = callback;
    device->receiveMessageCookie_ = cookie;

    return FREESPACE_SUCCESS;
}

static int _pollDevice(struct FreespaceDevice * device) {
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
				// disconnected.... hotplug will catch this later and notify
				return FREESPACE_ERROR_NO_DEVICE;
			}

			WARN("Failed reading %s: %s", device->hidrawPath_, strerror(errno));
			return FREESPACE_ERROR_IO;
		}

		if (rc == 0) { // EOF
			// disconnected.... hotplug will catch this later and notify
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

	for (i = 0; i < freespace_deviceAPITableNum; i++) {
		if (freespace_deviceAPITable[i].idVendor_ != info.vendor) {
			continue;
		}

		if (freespace_deviceAPITable[i].idProduct_ != (info.product & 0xffff )) {
			continue;
		}

		{
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

			TRACE("%s  - descriptor size: %d",  path,  descriptor.size);

			// TODO parse descriptors and do something intelligent....
			// Really, really crude matching for now..
			if (descriptor.size == 161) { // scoop
				*API = &freespace_deviceAPITable[i];
			} else if (descriptor.size == 174) { // Roku Broadcom Remote
				*API = &freespace_deviceAPITable[i];
			} else if (descriptor.size == 200) { // Ozmo Remote
				*API = &freespace_deviceAPITable[i];
			}

		}
		break;
	}

	close(fd);
	return FREESPACE_SUCCESS;
}

static int _allocateNewDevice(struct FreespaceDevice** out_device) {
	struct FreespaceDevice* device;
	*out_device = 0;

	if (nextFreeIndex == -1) {
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }

	device = (struct FreespaceDevice*) malloc(sizeof(struct FreespaceDevice));
	if (device == NULL) {
		// Out of memory.
		return FREESPACE_ERROR_OUT_OF_MEMORY;
	}
	memset(device, 0, sizeof(struct FreespaceDevice));
    numDevices++;

    devices[nextFreeIndex] = device;
    if (numDevices < FREESPACE_MAXIMUM_DEVICE_COUNT) {
        while (devices[nextFreeIndex] != NULL) {
            nextFreeIndex++;
            if (nextFreeIndex == FREESPACE_MAXIMUM_DEVICE_COUNT) {
                nextFreeIndex = 0;
            }
        }
    } else {
        nextFreeIndex = -1;
    }

    * out_device = device;
    return FREESPACE_SUCCESS;
}

// check if /dev/hidraw<num> path already belongs to an existing device
static int _isNewDevice(int num, int * isNew) {

	int i = 0;
	int n = 0;

	for (; n < numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
		struct FreespaceDevice * device = devices[i];

		if (device == 0) {
			continue;
		}

		n++;
		if (device->num_ != num) {
			continue;
		}

		switch (device->state_) {
			case FREESPACE_OPENED:
			case FREESPACE_CONNECTED:
				*isNew = 0; // /dev/hidraw<num>is already known
				return FREESPACE_SUCCESS;

			case FREESPACE_DISCONNECTED:
				*isNew = 1; // this is a "ghost" device that close() has not been called on.
				return FREESPACE_SUCCESS;

			default:
				break;
		}
	}

	// does not match one of our current devices
	*isNew = 1;
	return FREESPACE_SUCCESS;
}

// return <num> from /dev/hidraw<num>
static int _parseNum(const char * path) {
	return atoi(path + sizeof(HIDRAW_PREFIX) - 1);
}

static FreespaceDeviceId _assignId() {
	static int ctr = 0;
	return ctr++;
}


static int _scanDevices() {
	int i, n;
	int rc;
    int needToRescan = 0;

    // Check if the devices need to be rescanned.
    rc = freespace_hotplug_perform(&needToRescan);

    if (rc != FREESPACE_SUCCESS || !needToRescan) {
        return rc;
    }

	TRACE("Scanning hidraw devices", "");

	// Check if any of our connected or open devices have dropped
	i = 0;
	n = 0;
	for (; n < numDevices && i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
		struct FreespaceDevice * device = devices[i];

		if (!device) {
			continue;
		}

		n++;
		if (device->state_!= FREESPACE_OPENED && device->state_!= FREESPACE_CONNECTED) {
			continue;
		}

		// check if the file is still present
		 struct stat fileStat;
		 if(stat(device->hidrawPath_, &fileStat) < 0) {
			if (errno == ENOENT || errno == ENODEV) { // disconnected..
				_disconnect(device);
				continue;
			}

			if (errno != EAGAIN) {
				WARN("Stat failed on %s: %s", device->hidrawPath_, strerror(errno));
				return FREESPACE_ERROR_IO;
			}
		 }

		 TRACE("%s not disconnected", device->hidrawPath_);
    }

	// Check if a device has been added (iterate all of /dev)
	DIR*    dev_dir = opendir(DEV_DIR);
	if (dev_dir) {
        struct dirent*  ent;
	    char path[NAME_MAX];
	    strncpy(path, DEV_DIR, strlen(DEV_DIR));
	    int num;

        while ( (ent = readdir(dev_dir)) != NULL ) {
			struct FreespaceDeviceAPI const * API = 0;
			int isNew = 0;

            if (strncmp(ent->d_name, HIDRAW_PREFIX, strlen(HIDRAW_PREFIX)) != 0) {
            	continue;
            }


            num = _parseNum(ent->d_name);
    		_isNewDevice(num, &isNew);

    		if (!isNew) {
    			continue;
    		}

            // append the file name and store an abs path
            strncpy(&path[strlen(DEV_DIR)], ent->d_name, ent->d_reclen);
    		_isFreespaceDevice(path, &API);
    		if (!API) {
    			continue;
    			TRACE("Not a freespace device: %s", path);
    		}

    		// allocate a device
    		{

    			struct FreespaceDevice * device;
    			rc = _allocateNewDevice(&device);
    			if (rc != FREESPACE_SUCCESS) {
    				return rc;
    			}

    			device->state_ = FREESPACE_CONNECTED;
    			device->fd_ = -1;
    		    device->id_ = _assignId();
    		    device->num_ = num;
    			strncpy(device->hidrawPath_, path, sizeof(device->hidrawPath_));
    			device->api_ = API;

                if (hotplugCallback) {
                    hotplugCallback(FREESPACE_HOTPLUG_INSERTION, device->id_, hotplugCookie);
                }
    		}

    		DEBUG("Found freespace device at %s. Num devices: %d", path, numDevices);
        }
	} else {
		WARN("Failed opening %s", DEV_DIR);
	}

	// TODO handle the case where devices drop when in the "connected" but not "opened" state...
	closedir(dev_dir);
	return FREESPACE_SUCCESS;
}

static void _deallocateDevice(struct FreespaceDevice* device) {
    int i;
    for (i = 0; i < FREESPACE_MAXIMUM_DEVICE_COUNT; i++) {
        if (devices[i] == device) {
            if (nextFreeIndex == -1) {
                nextFreeIndex = i;
            }

#if 1 // this should not be necessary.
        	if (device->fd_ > 0) {
        		DEBUG("Deallocate device (%s) -- fd still open!", device->hidrawPath_)
    			if (userRemovedCallback) {
    				userRemovedCallback(device->fd_);
    			}
    			close(device->fd_);
    			device->fd_ = -1;
    		}
#endif
            free(device);
            devices[i] = NULL;
            numDevices--;
			DEBUG("Freed device. Num devices: %d", numDevices);
            return;
        }
    }

    WARN("Could not deallocate %p", device);
}

static int _disconnect(struct FreespaceDevice * device) {
	DEBUG("Freespace device (%d) at %s disconnected", device->id_, device->hidrawPath_);

    // device is currently in use, we can't delete it outright
    if (device->state_ == FREESPACE_OPENED) {
    	if (device->fd_ > 0) {
			if (userRemovedCallback) {
				userRemovedCallback(device->fd_);
			}
			close(device->fd_);
			device->fd_ = -1;
		}


		device->state_ = FREESPACE_DISCONNECTED;
		if (hotplugCallback) {
			hotplugCallback(FREESPACE_HOTPLUG_REMOVAL, device->id_, hotplugCookie);
		}

		// we have to wait for closeDevice() to deallocate this device.
		return FREESPACE_SUCCESS;
    }

    if (device->state_ == FREESPACE_CONNECTED) {
    	int id = device->id_;
    	if (device->fd_ > 0) {
			if (userRemovedCallback) {
				userRemovedCallback(device->fd_);
			}
			close(device->fd_);
			device->fd_ = -1;
		}
    	_deallocateDevice(device);
    	device = NULL;

		if (hotplugCallback) {
			hotplugCallback(FREESPACE_HOTPLUG_REMOVAL, id, hotplugCookie);
		}

    	return FREESPACE_SUCCESS;
    }

	return FREESPACE_ERROR_UNEXPECTED;
}

