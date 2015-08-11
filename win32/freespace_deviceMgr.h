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

#ifndef FREESPACE_DEVICE_MGR_H_
#define FREESPACE_DEVICE_MGR_H_

#include "freespace_win32.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private device manager API.
 */

/**
 * Create a new Freespace device.
 * @return The new device.
 */
struct FreespaceDeviceStruct* freespace_private_createDevice();

/**
 * Add an initialized device to the master list.
 *
 * @param device the device
 * @return FREESPACE_SUCCESS if ok
 */
int freespace_private_addDevice(struct FreespaceDeviceStruct* device);

/**
 * Free all resources associated with a device.
 * @param device The Freespace device.
 * @return FREESPACE_SUCCESS on success, or Freespace error code.
 */
int freespace_private_freeDevice(struct FreespaceDeviceStruct *device);

/**
 * Get an existing Freespace device using the primary handle index 0.
 * @param ref The reference compatible with discovery associated with the device.
 * @return The Freespace device
 */
struct FreespaceDeviceStruct* freespace_private_getDeviceByRef(FreespaceDeviceRef ref);

/**
 * Get an existing Freespace device.
 * @param id a device ID
 * @return The Freespace device
 */
struct FreespaceDeviceStruct* freespace_private_getDeviceById(FreespaceDeviceId id);

/**
 * Device filter for finding matching devices.
 * @param device The full device structure.
 * @return true on match, false otherwise.
 */
typedef BOOL (*freespace_deviceFilter)(struct FreespaceDeviceStruct* device);

/**
 * Get the list of devices with the mark set to the specified value.
 * @param list The list to populate with the available devices.
 * @param listSize The maximum size of the list.
 * @param listSizeOut The number of devices populated in the list.
 * @param filter The filter to use to determine matching devices.
 * @return 0 on success, error code on failure.
 */
int freespace_private_filterDevices(struct FreespaceDeviceStruct** list, int listSize, int *listSizeOut, freespace_deviceFilter filter);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_DEVICE_MGR_H_ */
