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

#ifndef _HOTPLUG_H_
#define _HOTPLUG_H_


/**
 * Initialize the hotplug file descriptor
 */
int freespace_hotplug_init();

/**
 * Cleanup hotplug initializations and close the hotplug
 * file descriptor
 */
void freespace_hotplug_exit();

/**
 * Return a timeout if the hotplug
 * code should be called back based on a timer
 * rather than a file descriptor activating.
 *
 * @return the time in milliseconds. Negative means wait forever
 */
int freespace_hotplug_timeout();


/**
 * Get the file descriptor for hotplug events
 * This is for use by poll or select
 */
int freespace_hotplug_getFD();

/**
 * Handle hotplug event
 * @param recheck set to one when to rescan for new devices.
 * Returns FREESPACE_SUCCESS if some kind of hotplug event occurred
 * Returns an error code otherwise
 */
int freespace_hotplug_perform(int* recheck);

#endif // _HOTPLUG_H_
