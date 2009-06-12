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

#ifndef FREESPACE_DISCOVERY_DETAIL_H_
#define FREESPACE_DISCOVERY_DETAIL_H_

#include "freespace_win32.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Populate all devices currently present in the system.
 * If a new device is found and the hotplug callback is specified,
 * then call the hotplug callback.
 * @return FREESPACE_SUCCESS or error.
 */
int freespace_private_scanAndAddDevices();

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_DISCOVERY_DETAIL_H_ */

