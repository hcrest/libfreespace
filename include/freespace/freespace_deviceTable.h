/* * libfreespace - library for communicating with Freespace devices
 *
 * Copyright 2010-15 Hillcrest Laboratories, Inc.
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

#ifndef FREESPACE_DEVICE_TABLE_H_
#define FREESPACE_DEVICE_TABLE_H_


#ifdef _WIN32
#include <stdint.h>
#include <windows.h>

#if _MSC_VER >= 1600
// With VC2010 and the WinDDK 7600, setting the include path to be have the
// WinDDK inc/api directory causes build failure.
// The workaround is to only have the /inc directory in the path, and add
// the api prefix to hidsdi.h.
#include <api/hidsdi.h>
#else
#include <hidsdi.h>
#endif


#else
#include <stdint.h>
#define USAGE uint16_t
#endif

#include "freespace/freespace.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#endif

/**
 * Define the maximum number of handles (interfaces) per device that
 * can be joined together as a single virtual device.
 * Changing this value requires changing instances of freespace_deviceAPI.
 * This may be Windows-specific.
 */
#define FREESPACE_HANDLE_COUNT_MAX 2

/**
 * @ingroup device
 * A specific usage available for a given freespace_deviceAPI.
 */
struct FreespaceDeviceUsageAPI {
    USAGE    usage_;
    USAGE    usagePage_;
};

/**
 * @ingroup device
 * Figure out which API to use depending on the reported Freespace version.
 */
struct FreespaceDeviceAPI {
    // Set of devices with this version.
    uint16_t    idVendor_;
    uint16_t    idProduct_;
    int         controlInterfaceNumber_;
    int         usageCount_;
	int         hVer_; // HID messaging protocol used for a particular product ID
    struct      FreespaceDeviceUsageAPI usages_[FREESPACE_HANDLE_COUNT_MAX];
    const char* name_;
    int         mask_; // Used for masking idProduct_
};

/**
 * @ingroup device
 * Stores information for all Freespace devices.
 */
extern const struct FreespaceDeviceAPI freespace_deviceAPITable[];

/**
 * Defines the number of devices in the API table.  This can't be done as a sizeof the array
 * because the externed definition of the array is not dimensioned.
 */
extern const int freespace_deviceAPITableNum;

#ifdef __cplusplus
}
#endif

#endif // FREESPACE_DEVICE_TABLE_H_

