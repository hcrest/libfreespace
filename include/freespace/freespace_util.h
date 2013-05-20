/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2013 Hillcrest Laboratories, Inc.
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

#ifndef FREESPACE_UTIL_H_
#define FREESPACE_UTIL_H_

#include "freespace/freespace_codecs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup util Utility Functions API
 *
 * This page describes the utility functions API for supporting
 * communications and operations with Freespace(r) devices.
 */

/** This struct is used to exchange values for a sensor.
 * It contains 4 axes so that it can hold quaternions.
 * It is used as a general container for sensors with 1 to 4 axes.
 */
struct MultiAxisSensor {
    float w; /** temperature or quaternion w */
    float x; /** x axis */
    float y; /** y axis */
    float z; /** z axis */
};

/** @ingroup util
 *
 * Get the acceleration values from a MEOut packet
 *
 * @param meOutPkt a pointer to the MEOut packet to extract the acceleration from
 * @param sensor a pointer to where to store the extraced values
 * @return 0 if successful
 *         -1 if the format flag was not set for the acceleration field
 *         -2 if the meOutPkt does not contain acceleration at all
 *         -3 if the format select number is unrecognized
 */
LIBFREESPACE_API int freespace_util_getAcceleration(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor);

/** @ingroup util
 *
 * Get the angular position values from a MEOut packet
 *
 * @param meOutPkt a pointer to the MEOut packet to extract the angular position from
 * @param sensor a pointer to where to store the extraced values
 * @return 0 if successful
 *         -1 if the format flag was not set for the angular position field
 *         -2 if the meOutPkt does not contain angular position at all
 *         -3 if the format select number is unrecognized
 */
LIBFREESPACE_API int freespace_util_getAngPos(struct freespace_MotionEngineOutput const * meOutPkt,
                                              struct MultiAxisSensor * sensor);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_H_ */
