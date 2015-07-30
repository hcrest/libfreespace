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

#ifndef FREESPACE_DEVICE_H_
#define FREESPACE_DEVICE_H_

#include "freespace_win32.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Service all async operations on a device.
 *
 * @param device which device
 * @return FREESPACE_SUCCESS if ok
 */
int freespace_private_devicePerform(struct FreespaceDeviceStruct* device);

/**
 * Force a device to close.  Attempt to recover from errors.
 * @param device which device.
 */
void freespace_private_forceCloseDevice(struct FreespaceDeviceStruct* device);

/**
 * Remove the device from the libfreespace API.
 * @param device which device.
 */
void freespace_private_removeDevice(struct FreespaceDeviceStruct* device);

/**
 * Add the device to the libfreespace API.
 * @param device which device.
 */
void freespace_private_insertDevice(struct FreespaceDeviceStruct* device);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_DEVICE_H_ */
