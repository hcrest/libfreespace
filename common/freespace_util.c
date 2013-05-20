/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2010-2013 Hillcrest Laboratories, Inc.
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

#include <freespace/freespace_util.h>

/******************************************************************************
 * freespace_util_getAcceleration
 */
LIBFREESPACE_API int freespace_util_getAcceleration(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    unsigned int axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 1024.0;
        if (meOutPkt->ff0 == 1) {
            offset = 6; // Skip over mouse packet
        }
        if (meOutPkt->ff1 == 0) {
            offset = -1; // Acceleration flag not set
        }
        break;
    case 1:
        scale = 100.0; // Convert to m/s2 too?
        if (meOutPkt->ff0 == 0) {
            offset = -1; // Acceleration flag not set
        }
        break;
    case 2: 
        return -2; // No calibrated acceleration in this format
    case 3:
        scale = 256.0;
        if (meOutPkt->ff0 == 1) {
            offset = 6; // Skip over mouse packet
        }
        if (meOutPkt->ff1 == 0) {
            offset = -1; // Acceleration flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Acceleration flag not set
    }

    // Extract and convert the acceleration data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->x = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
    sensor->y = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
    sensor->z = ((float) axisVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getAngPos
 */
LIBFREESPACE_API int freespace_util_getAngPos(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
    case 3:
        scale = 16384.0;
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over mag
        offset = (meOutPkt->ff5 == 1) ? offset + 2 : offset; // Skip over temperature
        if (meOutPkt->ff6 == 0) {
            offset = -1; // Angular position flag not set
        }
        break;
    case 1:
        scale = 16384.0;
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over mag
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over inclination
        offset = (meOutPkt->ff5 == 1) ? offset + 2 : offset; // Skip over compass
        if (meOutPkt->ff6 == 0) {
            offset = -1; // Angular position flag not set
        }
        break;
    case 2: 
        return -2; // No angular position in this format
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Angular position flag not set
    }

    // Extract and convert the angular position data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->w = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
    sensor->x = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
    sensor->y = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 7] << 8 |  meOutPkt->meData[offset + 6];
    sensor->z = ((float) axisVal) / scale;

    return 0;
}
