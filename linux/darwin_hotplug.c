/* * libfreespace - library for communicating with Freespace devices
 *
 * Copyright 2009-15 Hillcrest Laboratories, Inc.
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

#include "hotplug.h"
#include "freespace/freespace.h"

#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <mach/mach_port.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <IOKit/usb/IOUSBLib.h>


static mach_port_t  freespace_darwin_mp = 0; /* master port */
static CFRunLoopRef freespace_darwin_acfl = NULL; /* async cf loop */

/* async event thread */
static pthread_t freespace_darwin_et;

static int readFd_ = -1;
static int writeFd_ = -1;


static void darwin_device_event (void *ptr, io_iterator_t devices) {
    io_service_t device;

    // Reset the notifications
    while ((device = IOIteratorNext (devices)) != 0) {
        IOObjectRelease (device);
    }

    // Let the other thread know something happened
    write(writeFd_, "1", 1);
}


static void *event_thread_main (void *arg0) {
    IOReturn kresult;
    io_service_t device;

    /* hotplug (device removal) source */
    CFRunLoopSourceRef     notification_cfsource;
    IONotificationPortRef  notification_port;
    io_iterator_t          rem_device_iterator;
    io_iterator_t          add_device_iterator;

    CFRetain (CFRunLoopGetCurrent ());

    /* add the notification port to the run loop */
    notification_port     = IONotificationPortCreate (freespace_darwin_mp);
    notification_cfsource = IONotificationPortGetRunLoopSource (notification_port);
    CFRunLoopAddSource(CFRunLoopGetCurrent (), notification_cfsource, kCFRunLoopDefaultMode);

    /* create notifications for removed devices */
    kresult = IOServiceAddMatchingNotification (notification_port, kIOTerminatedNotification,
                                                IOServiceMatching(kIOUSBDeviceClassName),
                                                (IOServiceMatchingCallback) darwin_device_event,
                                                NULL, &rem_device_iterator);

    if (kresult != kIOReturnSuccess) {
        pthread_exit ((void *)kresult);
    }
    /* arm notifiers */
    while ((device = IOIteratorNext (rem_device_iterator)) != 0) {
        IOObjectRelease (device);
    }

    /* create notifications for added devices */
    kresult = IOServiceAddMatchingNotification (notification_port, kIOFirstMatchNotification,
                                                IOServiceMatching(kIOUSBDeviceClassName),
                                                (IOServiceMatchingCallback) darwin_device_event,
                                                NULL, &add_device_iterator);

    if (kresult != kIOReturnSuccess) {
        pthread_exit ((void *)kresult);
    }
    /* arm notifiers */
    while ((device = IOIteratorNext (add_device_iterator)) != 0) {
        IOObjectRelease (device);
    }

    /* let the main thread know about the async runloop */
    freespace_darwin_acfl = CFRunLoopGetCurrent ();

    /* run the runloop */
    CFRunLoopRun();

    /* delete notification port */
    CFRunLoopSourceInvalidate (notification_cfsource);
    IONotificationPortDestroy (notification_port);

    CFRelease (CFRunLoopGetCurrent ());

    freespace_darwin_acfl = NULL;

    pthread_exit (0);
}


int freespace_hotplug_init() {
    IOReturn kresult;
    int fds[2];
    int rc;

    /* Create the master port for talking to IOKit */
    if (!freespace_darwin_mp) {
        kresult = IOMasterPort (MACH_PORT_NULL, &freespace_darwin_mp);

        if (kresult != kIOReturnSuccess || !freespace_darwin_mp) {
            return kresult;
        }
    }

    // Don't allow init to be called twice.
    if (readFd_ != -1 || writeFd_ != -1) {
        return FREESPACE_ERROR_UNEXPECTED;
    }

    // Open up a pipe for sending/receiving hotplug events
    rc = pipe(fds);
    if (rc < 0) {
        return rc;
    }
    writeFd_ = fds[1];
    readFd_ = fds[0];

    // Set the socket to non-blocking
    rc = fcntl(readFd_, F_SETFL, O_NONBLOCK);
    if (rc < 0) {
        close(readFd_);
        close(writeFd_);
        return FREESPACE_ERROR_UNEXPECTED;
    }

    // Write something to the readFD to get the initial scan rolling
    write(writeFd_, "1", 1);

    // Start the notification thread
    pthread_create (&freespace_darwin_et, NULL, event_thread_main, NULL);

    // Make sure the Run loop has been started
    while (!freespace_darwin_acfl) {
        usleep (10);
    }

    return 0;
}

void freespace_hotplug_exit() {
    void *ret;

    /* stop the async runloop */
    CFRunLoopStop (freespace_darwin_acfl);
    pthread_join (freespace_darwin_et, &ret);

    if (freespace_darwin_mp) {
        mach_port_deallocate(mach_task_self(), freespace_darwin_mp);
    }

    freespace_darwin_mp = 0;

    close(readFd_);
    close(writeFd_);
    readFd_ = -1;
    writeFd_ = -1;
}

int freespace_hotplug_timeout() {
    return -1;
}

int freespace_hotplug_getFD() {
    return readFd_;
}

int freespace_hotplug_perform(int* recheck) {
    char buf[16];
    int rc;
    *recheck = 0;

    rc = read(readFd_, buf, sizeof(buf));
    if (rc > 0) {
        // If any data was read, we received an event - rescan the devices
        *recheck = 1;
        return FREESPACE_SUCCESS;
    }

    // We didn't read anything - was this an error?
    if (errno == EAGAIN) {
        return FREESPACE_SUCCESS;
    } else {
        return FREESPACE_ERROR_IO;
    }
}
