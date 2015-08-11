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

#ifndef FREESPACE_DISCOVERY_DETAIL_H_
#define FREESPACE_DISCOVERY_DETAIL_H_

#include "freespace_win32.h"
#include "freespace_device.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Mangle the device path into a unique device identifier.  The resulting
 * identifier must be the same for the multiple ports of the same device.
 * @param devicePath The source device path
 * @return The unique ID which must be freed when no longer needed.
 */
WCHAR* freespace_private_generateUniqueId(FreespaceDeviceRef devicePath);

/**
 * Populate all devices currently present in the system.
 * If a new device is found and the hotplug callback is specified,
 * then call the hotplug callback.  This scan process must correctly identify
 * devices that were both removed and inserted since the last scan.
 * @return FREESPACE_SUCCESS or error.
 */
int freespace_private_scanAndAddDevices();

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_DISCOVERY_DETAIL_H_ */

