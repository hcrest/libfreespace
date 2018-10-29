/* * libfreespace - library for communicating with Freespace devices
 *
 * Copyright 2013-15 Hillcrest Laboratories, Inc.
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

#include <freespace/freespace_util.h>

/******************************************************************************
 * freespace_util_getAcceleration
 */
LIBFREESPACE_API int freespace_util_getAcceleration(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 1024.0; // Q10
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        if (meOutPkt->ff1 == 0) {
            offset = -1; // Acceleration flag not set
        }
        break;
    case 1:
        scale = 100.0; // 0.01g
        if (meOutPkt->ff0 == 0) {
            offset = -1; // Acceleration flag not set
        }
        break;
    case 2: 
        return -2; // No calibrated acceleration in this format
    case 3:
        scale = 256.0; // Q8
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
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
 * freespace_util_getAccNoGravity
 */
LIBFREESPACE_API int freespace_util_getAccNoGravity(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 1024.0; // Q10
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        if (meOutPkt->ff2 == 0) {
            offset = -1; // Acceleration No Grav flag not set
        }
        break;
    case 1:
        scale = 100.0; // 0.01g
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        if (meOutPkt->ff1 == 0) {
            offset = -1; // Acceleration No Grav flag not set
        }
        break;
    case 2: 
        return -2; // No calibrated acceleration no gravity in this format
    case 3:
        scale = 256.0; // Q8
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        if (meOutPkt->ff2 == 0) {
            offset = -1; // Acceleration No Grav flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Acceleration No Grav flag not set
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
 * freespace_util_getAngularVelocity
 */
LIBFREESPACE_API int freespace_util_getAngularVelocity(struct freespace_MotionEngineOutput const * meOutPkt,
                                                       struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 1024.0; // Q10
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        if (meOutPkt->ff3 == 0) {
            offset = -1; // Ang Vel flag not set
        }
        break;
    case 1:
        scale = 100.0; // 0.1 deg/s
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        if (meOutPkt->ff2 == 0) {
            offset = -1; // Ang Vel flag not set
        }
        break;
    case 2: 
        return -2; // No calibrated angular velocity in this format
    case 3:
        scale = 512.0; // Q9
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        if (meOutPkt->ff3 == 0) {
            offset = -1; // Ang Vel flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Ang Vel flag not set
    }

    // Extract and convert the angular velocity data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->x = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
    sensor->y = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
    sensor->z = ((float) axisVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getMagnetometer
 */
LIBFREESPACE_API int freespace_util_getMagnetometer(struct freespace_MotionEngineOutput const * meOutPkt,
                                                    struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 4096.0; // Q12
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over ang vel
        if (meOutPkt->ff4 == 0) {
            offset = -1; // Mag flag not set
        }
        break;
    case 1:
        scale = 1000.0; // 0.001 gauss
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over ang vel
        if (meOutPkt->ff3 == 0) {
            offset = -1; // Mag flag not set
        }
        break;
    case 2: 
        return -2; // No calibrated magnetometer in this format
    case 3:
        scale = 32.0; // Q5
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over ang vel
        if (meOutPkt->ff4 == 0) {
            offset = -1; // Mag flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Mag flag not set
    }

    // Extract and convert the magnetometer data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->x = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
    sensor->y = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
    sensor->z = ((float) axisVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getTemperature
 */
LIBFREESPACE_API int freespace_util_getTemperature(struct freespace_MotionEngineOutput const * meOutPkt,
                                                   struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 0:
        scale = 128.0; // Q7
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over mag
        if (meOutPkt->ff5 == 0) {
            offset = -1; // Temperature flag not set
        }
        break;
    case 1:
    case 2: 
        return -2; // No calibrated temperature in this format
    case 3:
        scale = 128.0; // Q7
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over mouse
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over mag
        if (meOutPkt->ff5 == 0) {
            offset = -1; // Mag flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Temperature flag not set
    }

    // Extract and convert the temperature data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->w = ((float) axisVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getInclination
 */
LIBFREESPACE_API int freespace_util_getInclination(struct freespace_MotionEngineOutput const * meOutPkt,
                                                   struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 1:
        scale = 10.0; // 0.1 degrees
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over mag
        if (meOutPkt->ff4 == 0) {
            offset = -1; // Inclination flag not set
        }
        break;
    case 0:
    case 2:
    case 3:
        return -2; // No calibrated inclination in this format
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Inclination flag not set
    }

    // Extract and convert the inclination data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->x = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
    sensor->y = ((float) axisVal) / scale;
    axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
    sensor->z = ((float) axisVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getCompassHeading
 */
LIBFREESPACE_API int freespace_util_getCompassHeading(struct freespace_MotionEngineOutput const * meOutPkt,
                                                      struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int16_t axisVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 1:
        scale = 10.0; // 0.1 degrees
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over mag
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over inclination
        if (meOutPkt->ff5 == 0) {
            offset = -1; // Compass heading flag not set
        }
        break;
    case 0:
    case 2:
    case 3:
        return -2; // No calibrated compass heading in this format
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Compass heading flag not set
    }

    // Extract and convert the compass heading data
    axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
    sensor->x = ((float) axisVal) / scale;

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
    if (meOutPkt->formatSelect == 1) {
        axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
        sensor->x = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
        sensor->y = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
        sensor->z = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 7] << 8 |  meOutPkt->meData[offset + 6];
        sensor->w = ((float) axisVal) / scale;
    } else {
        axisVal = meOutPkt->meData[offset + 1] << 8 |  meOutPkt->meData[offset + 0];
        sensor->w = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 3] << 8 |  meOutPkt->meData[offset + 2];
        sensor->x = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 5] << 8 |  meOutPkt->meData[offset + 4];
        sensor->y = ((float) axisVal) / scale;
        axisVal = meOutPkt->meData[offset + 7] << 8 |  meOutPkt->meData[offset + 6];
        sensor->z = ((float) axisVal) / scale;
    }

    return 0;
}

/******************************************************************************
 * freespace_util_getActClass
 */
LIBFREESPACE_API int freespace_util_getActClass(struct freespace_MotionEngineOutput const * meOutPkt,
                                                struct MultiAxisSensor * sensor) {

    int offset = 0; // default value of 0; if < 0 then offset is not valid
    int8_t flagVal;
    float scale;

    switch(meOutPkt->formatSelect) {
    case 1:
        scale = 1.0; // Q0
        offset = (meOutPkt->ff0 == 1) ? offset + 6 : offset; // Skip over acc
        offset = (meOutPkt->ff1 == 1) ? offset + 6 : offset; // Skip over lin acc
        offset = (meOutPkt->ff2 == 1) ? offset + 6 : offset; // Skip over ang vel
        offset = (meOutPkt->ff3 == 1) ? offset + 6 : offset; // Skip over mag
        offset = (meOutPkt->ff4 == 1) ? offset + 6 : offset; // Skip over inclination
        offset = (meOutPkt->ff5 == 1) ? offset + 2 : offset; // Skip over compass
        offset = (meOutPkt->ff6 == 1) ? offset + 8 : offset; // Skip over ang pos
        if (meOutPkt->ff7 == 0) {
            offset = -1; // Act class flag not set
        }
        break;
    case 0:
    case 2:
    case 3:
        return -2; // No activity classification in this format
    default:
        return -3; // The format number was unrecognized
    }

    if (offset < 0) {
        return -1; // Act class flag not set
    }

    flagVal = meOutPkt->meData[offset + 0]; // Act Class Flags
    sensor->x = ((float) flagVal) / scale;
    flagVal = meOutPkt->meData[offset + 1]; // Power Mgmt Flags
    sensor->y = ((float) flagVal) / scale;

    return 0;
}

/******************************************************************************
 * freespace_util_getMouseAndButtons
 */
LIBFREESPACE_API int freespace_util_getMouseAndButtons(struct freespace_MotionEngineOutput const * meOutPkt,
                                                       struct MultiAxisSensor * sensor) {

    switch (meOutPkt->formatSelect) {
    case 0:
        if (meOutPkt->ff0 == 0) {
            return -1; // Mouse flag not set
        }
        break;
    case 1:
    case 2:
        return -2; // No mouse data in this format
    case 3:
        if (meOutPkt->ff0 == 0) {
            return -1; // Mouse flag not set
        }
        break;
    default:
        return -3; // The format number was unrecognized
    }

    // Extract and convert the mouse data
    sensor->x = meOutPkt->meData[1]; // Delta X
    sensor->y = meOutPkt->meData[3]; // Delta Y
    sensor->z = meOutPkt->meData[5]; // Delta Wheel
    sensor->w = meOutPkt->meData[0]; // Button bits

    return 0;
}

