/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2009 Hillcrest Laboratories, Inc.
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

#include "freespace_deviceMgr.h"
#include "freespace_device.h"
#include "freespace_discovery.h"
#include "freespace_discoveryDetail.h"
#include <strsafe.h>
#include <malloc.h>

//#define FREESPACE_DISCOVERY_DEBUG
struct LibfreespaceData* freespace_instance_ = NULL;

static int checkDiscovery();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                       )
{
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
            break;
	}
    return TRUE;
}

LIBFREESPACE_API int freespace_init() {
    int rc;

    if (freespace_instance_ != NULL) {
        return FREESPACE_ERROR_BUSY;
    }

    freespace_instance_ = (struct LibfreespaceData*) malloc(sizeof(struct LibfreespaceData));
    if (freespace_instance_ == NULL) {
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }
    memset(freespace_instance_, 0, sizeof(struct LibfreespaceData));

    rc = freespace_private_discoveryThreadInit();
    if (rc != FREESPACE_SUCCESS) {
        return rc;
    }

    return FREESPACE_SUCCESS;
}

LIBFREESPACE_API void freespace_exit() {
    int i;

    // Shut down the discovery code.
    if (freespace_instance_->fdRemovedCallback_) {
        freespace_instance_->fdRemovedCallback_(freespace_private_discoveryEventObject());
    }
    freespace_private_discoveryThreadExit();

    // Free all devices.
    for (i = 0; i < freespace_instance_->deviceCount_; i++) {
        freespace_private_freeDevice(freespace_instance_->devices_[i]);
    }

    free(freespace_instance_);
    freespace_instance_ = NULL;
}

LIBFREESPACE_API int freespace_setDeviceHotplugCallback(freespace_hotplugCallback callback,
                                                        void* cookie) {
    if (freespace_instance_ == NULL) {
        return FREESPACE_ERROR_NOT_FOUND;
    }

    freespace_instance_->hotplugCallback_ = callback;
    freespace_instance_->hotplugCookie_ = cookie;
    return FREESPACE_SUCCESS;
}

static BOOL performHelper(struct FreespaceDeviceStruct* device) {
    freespace_private_devicePerform(device);
    return FALSE;
}

LIBFREESPACE_API int freespace_perform() {
    int rc;

    // Check if the device discovery thread has detected any changes
    // and rescan if so.
    rc = checkDiscovery();

    // Service all of the devices.
    // NOTE: Servicing includes initiating
    freespace_private_filterDevices(NULL, 0, NULL, performHelper);

    return rc;
}

LIBFREESPACE_API void freespace_setFileDescriptorCallbacks(freespace_pollfdAddedCallback addedCallback,
                                                           freespace_pollfdRemovedCallback removedCallback) {
    freespace_instance_->fdAddedCallback_ = addedCallback;
    freespace_instance_->fdRemovedCallback_ = removedCallback;
}

LIBFREESPACE_API int freespace_syncFileDescriptors() {
    if (freespace_instance_->fdAddedCallback_ == NULL) {
        return FREESPACE_SUCCESS;
    }

    freespace_instance_->fdAddedCallback_(freespace_private_discoveryEventObject(), 1);

    freespace_private_filterDevices(NULL, 0, NULL, freespace_private_fdSyncAddFilter);

    return FREESPACE_SUCCESS;
}

LIBFREESPACE_API int freespace_getNextTimeout(int* timeoutMsOut) {
    // TODO
    *timeoutMsOut = 0xffffffff;
    return FREESPACE_SUCCESS;
}

struct FreespaceDeviceStruct* freespace_private_getDeviceByRef(FreespaceDeviceRef ref) {
    int i;
    for (i = 0; i < freespace_instance_->deviceCount_; i++) {
        if (lstrcmp(ref, freespace_instance_->devices_[i]->handle_[0].devicePath) == 0) {
            return freespace_instance_->devices_[i];
        }
    }
    return NULL;
}

struct FreespaceDeviceStruct* freespace_private_getDeviceById(FreespaceDeviceId id) {
    int i;
    for (i = 0; i < freespace_instance_->deviceCount_; i++) {
        if (id == freespace_instance_->devices_[i]->id_) {
            return freespace_instance_->devices_[i];
        }
    }
    return NULL;
}

int freespace_private_filterDevices(struct FreespaceDeviceStruct** list, int listSize, int *listSizeOut, freespace_deviceFilter filter) {
    int i;
    int itemsAdded = 0;

    if (filter == NULL) {
        return FREESPACE_SUCCESS;
    }

    for (i = 0; i < freespace_instance_->deviceCount_; i++) {
        struct FreespaceDeviceStruct* device = freespace_instance_->devices_[i];
        if (filter(device)) {
            if (itemsAdded < listSize && list != NULL) {
                list[itemsAdded] = device;
                itemsAdded++;
            }
        }
    }
    if (listSizeOut != NULL) {
        *listSizeOut = itemsAdded;
    }
    return FREESPACE_SUCCESS;
}

LIBFREESPACE_API int freespace_getDeviceList(FreespaceDeviceId* list, int listSize, int *listSizeOut) {
    int i;
    int rc;

    // Check if the device list has changed.
    rc = checkDiscovery();
    if (rc != FREESPACE_SUCCESS) {
        return rc;
    }

    // Return the devices.
    for (*listSizeOut = 0, i = 0; *listSizeOut < listSize && i < freespace_instance_->deviceCount_; (*listSizeOut)++) {
        list[*listSizeOut] = freespace_instance_->devices_[i]->id_;
        i++;
    }

    return FREESPACE_SUCCESS;
}

int freespace_private_addDevice(struct FreespaceDeviceStruct* device) {
    int idx;

    // Initialize the overlapped I/O
    for (idx = 0; idx < device->handleCount_; idx++) {
        HANDLE waitEvent;
        struct FreespaceSubStruct* s = &device->handle_[idx];

        waitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (waitEvent == NULL) {
            return FREESPACE_ERROR_UNEXPECTED;
        }
        s->readOverlapped_.hEvent = waitEvent;
        s->readOverlapped_.Offset = 0;
        s->readOverlapped_.OffsetHigh = 0;
        s->readStatus_ = FALSE;
    }

    // Add the device to list.
    freespace_instance_->devices_[freespace_instance_->deviceCount_] = device;
    freespace_instance_->deviceCount_++;

    // Notify insertion
    if (freespace_instance_->hotplugCallback_ != NULL) {
        freespace_instance_->hotplugCallback_(FREESPACE_HOTPLUG_INSERTION, device->id_, freespace_instance_->hotplugCookie_);
    }

    return FREESPACE_SUCCESS;
}

static BOOL filterInitialize(struct FreespaceDeviceStruct* device) {
    device->status_ = FREESPACE_DISCOVERY_STATUS_UNKNOWN;
    return FALSE;
}

static BOOL filterSweep(struct FreespaceDeviceStruct* device) {
    // Device has been removed if it existed before and
    // wasn't marked as existing now.
    return device->status_ == FREESPACE_DISCOVERY_STATUS_UNKNOWN;
}

int checkDiscovery() {
    if (freespace_private_discoveryStatusChanged()) {
        int i;
        int rc;
        struct FreespaceDeviceStruct* list[FREESPACE_MAXIMUM_DEVICE_COUNT];
        int listLength = 0;

        // Mark and sweep the device list.
        freespace_private_filterDevices(NULL, 0, NULL, filterInitialize);

        // Mark everything that still exists and add new devices
        rc = freespace_private_scanAndAddDevices();
        if (rc != FREESPACE_SUCCESS) {
            return rc;
        }
        // Note: freespace_private_scanAndAddDevices already called the callback for added devices.

        // Collect the removed devices.
        freespace_private_filterDevices(list, FREESPACE_MAXIMUM_DEVICE_COUNT, &listLength, filterSweep);

        // Remove them from the device list so that future API calls fail to this device
        // fail. See callbacks after this loop.
        for (i = 0; i < listLength; i++) {
            int idx;
            for (idx = 0; idx < freespace_instance_->deviceCount_; idx++) {
                if (list[i] == freespace_instance_->devices_[idx]) {
                    memcpy(&freespace_instance_->devices_[idx], &freespace_instance_->devices_[idx + 1], (freespace_instance_->deviceCount_ - idx - 1) * sizeof(struct FreespaceDeviceStruct*));
                    freespace_instance_->deviceCount_--;
                }
            }
        }

        // Call the removal callbacks and free the device structures.
        for (i = 0; i < listLength; i++) {
            struct FreespaceDeviceStruct* device = list[i];

            if (freespace_instance_->hotplugCallback_ != NULL) {
                freespace_instance_->hotplugCallback_(FREESPACE_HOTPLUG_REMOVAL, device->id_, freespace_instance_->hotplugCookie_);
            }

            freespace_private_freeDevice(device);
        }
    }

    return freespace_private_discoveryGetThreadStatus();
}
