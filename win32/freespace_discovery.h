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

#ifndef FREESPACE_DISCOVERY_H_
#define FREESPACE_DISCOVERY_H_

#include "freespace_win32.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the discovery process.
 * @return 0 on success, or Freespace error code.
 */
int freespace_private_discoveryThreadInit();

/**
 * Exit the discovery process.
 * @return 0 on success, or Freespace error code.
 */
int freespace_private_discoveryThreadExit();

/*
 * Determine if the status of the connected devices has changed.
 * @return true if rescan is requested, false otherwise.
 */
BOOL freespace_private_discoveryStatusChanged();

/*
 * Get the handle used for device discovery.
 * @return The handle to be used for polling.
 */
HANDLE freespace_private_discoveryEventObject();

/*
 * Get the status of the independend discovery thread.
 * @return 0 on thread running normally, or Freespace error code.
 */
int freespace_private_discoveryGetThreadStatus();

/**
 * Request a rescan of the devices.  
 * Can be used to help recover from device errors.
 */
void freespace_private_requestDeviceRescan();

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_DISCOVERY_H_ */
