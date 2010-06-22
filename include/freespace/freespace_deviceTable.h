/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2010 Hillcrest Laboratories, Inc.
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
 
#ifndef FREESPACE_DEVICE_TABLE_H_
#define FREESPACE_DEVICE_TABLE_H_


#ifdef _WIN32
#include "win32_stdint.h"
#include <windows.h>
#include <hidsdi.h>
#else
#include <stdint.h>
#define USAGE uint16_t
#endif

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
 * This must be updated when devices are added or subtraced from the device
 * table.
 */
#define FREESPACE_DEVICES_COUNT 41

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
};

/**
 * @ingroup device
 * Stores information for all Freespace devices.
 */
extern const struct FreespaceDeviceAPI freespace_deviceAPITable[FREESPACE_DEVICES_COUNT];

#ifdef __cplusplus
}
#endif

#endif // FREESPACE_DEVICE_TABLE_H_

