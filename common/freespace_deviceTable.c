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

#include "freespace/freespace_deviceTable.h"

/*
 * Define the devices recognized by libfreespace.
 * Naming convention:
 *   UserMeaningfulName vN (XXXX)
 *   N = USB interface version number
 *   XXXX = Advertised interfaces:
 *      M = Mouse
 *      K = Keyboard
 *      C = Consumer page
 *      V = Joined multi-axis and vendor-specific
 *      A = Separate multi-axis and vendor-specific (deprecated)
 */
const struct FreespaceDeviceAPI freespace_deviceAPITable[] = {
    { 0x1d5a, 0xc001, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "Piranha", 0xffff},
    { 0x1d5a, 0xc002, 0, 1, 0, {{0, 0},      {0, 0}}, "Piranha bootloader", 0xffff},
    { 0x1d5a, 0xc003, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "Piranha factory test dongle", 0xffff},
    { 0x1d5a, 0xc004, 0, 1, 1, {{0, 0},      {0, 0}}, "Piranha sniffer dongle", 0xffff},
    { 0x1d5a, 0xc005, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "FSRK STM32F10x eval board (E)", 0xffff},
    { 0x1d5a, 0xc006, 0, 1, 0, {{0, 0},      {0, 0}}, "Cortex Bootloader", 0xffff},
    { 0x1d5a, 0xc007, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "FSRK Gen4 Dongle", 0xffff},
    { 0x1d5a, 0xc008, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "SPI to USB adapter board v0", 0xffff},
    { 0x1d5a, 0xc009, 0, 2, 1, {{4, 0xff01}, {8, 1}}, "USB RF Transceiver v0", 0xffff},
    { 0x1d5a, 0xc00a, 1, 2, 1, {{4, 0xff01}, {8, 1}}, "Coprocessor to USB adapter v1 (MA)", 0xffff},
    { 0x1d5a, 0xc00b, 1, 2, 1, {{4, 0xff01}, {8, 1}}, "USB RF Transceiver v1 (MKCA)", 0xffff},
    { 0x1d5a, 0xc00c, 1, 2, 1, {{4, 0xff01}, {8, 1}}, "SPI to USB adapter v1 (MA)", 0xffff},
    { 0x1d5a, 0xc010, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v1 (MV)", 0xffff},
    { 0x1d5a, 0xc011, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v1 (MCV)", 0xffff},
    { 0x1d5a, 0xc012, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v1 (MKV)", 0xffff},
    { 0x1d5a, 0xc013, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v1 (MKCV)", 0xffff},
    { 0x1d5a, 0xc020, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "SPI to USB adapter v1 (MV)", 0xffff},
    { 0x1d5a, 0xc021, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "SPI to USB adapter v1 (V)", 0xffff},
    { 0x1d5a, 0xc030, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "Coprocessor to USB adapter v1 (MV)", 0xffff},
    { 0x1d5a, 0xc031, 1, 1, 1, {{4, 0xff01}, {0, 0}}, "Coprocessor to USB adapter v1 (V)", 0xffff},
    { 0x1d5a, 0xc040, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v2 (MV)", 0xffff},
    { 0x1d5a, 0xc041, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v2 (MCV)", 0xffff},
    { 0x1d5a, 0xc042, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v2 (MKV)", 0xffff},
    { 0x1d5a, 0xc043, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver v2 (MKCV)", 0xffff},
    { 0x1d5a, 0xc044, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver AltRF v2 (MV)", 0xffff},
    { 0x1d5a, 0xc045, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver AltRF v2 (MCV)", 0xffff},
    { 0x1d5a, 0xc046, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver AltRF v2 (MKV)", 0xffff},
    { 0x1d5a, 0xc047, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB RF Transceiver AltRF v2 (MKCV)", 0xffff},
    { 0x1d5a, 0xc050, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "SPI to USB adapter v2 (MV)", 0xffff},
    { 0x1d5a, 0xc051, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "SPI to USB adapter v2 (V)", 0xffff},
    { 0x1d5a, 0xc052, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "SPI to USB adapter v2 (MKCV)", 0xffff},
    { 0x1d5a, 0xc053, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "I2C to USB adapter v2 (SV)", 0xffff},
    { 0x1d5a, 0xc060, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Coprocessor to USB adapter v2 (MV)", 0xffff},
    { 0x1d5a, 0xc061, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Coprocessor to USB adapter v2 (V)", 0xffff},
    { 0x1d5a, 0xc070, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Bluetooth v2 (MV)", 0xffff},
    { 0x1d5a, 0xc071, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Bluetooth v2 (MCV)", 0xffff},
    { 0x1d5a, 0xc022, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Bluetooth v2 (MKV)", 0xffff},
    { 0x1d5a, 0xc073, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Bluetooth v2 (MKCV)", 0xffff},
    { 0x1d5a, 0xc080, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB Freespace Module (MV)", 0xffff},
    { 0x1d5a, 0xc090, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Bluetooth Remote(CKV)", 0xffff},
    { 0x1d5a, 0xc0b0, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MV)", 0xffff},
    { 0x1d5a, 0xc0b1, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MCV)", 0xffff},
    { 0x1d5a, 0xc0b2, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MKV)", 0xffff},
    { 0x1d5a, 0xc0b3, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MKCV)", 0xffff},
    { 0x7045, 0x2860, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Nanosic Smart USB RF Transceiver", 0xffff},
    { 0x1d5a, 0xc0c0, 2, 1, 2, {{4, 0xff01}, {0, 0}}, "RF4CE RemoTI ZID dongle (MKCV)", 0xffff},
    { 0x1d5a, 0xc0d0, 2, 1, 2, {{4, 0xff01}, {0, 0}}, "BLE Scoop (MKCV)", 0xffff},
    { 0x1d5a, 0xc0e0, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "USB Freespace Module (MV)", 0xffff},
    { 0x1d5a, 0xc100, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP160", 0xffff},
    { 0x1d5a, 0xc101, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP260", 0xffff},
    { 0x1d5a, 0xc102, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U160", 0xffff},
    { 0x1d5a, 0xc103, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U260", 0xffff},
    { 0x1d5a, 0xc104, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP160", 0xffff},
    { 0x1d5a, 0xc105, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP260", 0xffff},
    { 0x1d5a, 0xc106, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP261", 0xffff},
    { 0x1d5a, 0xc107, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for FSP262", 0xffff},
    { 0x1d5a, 0xc200, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for FSP275", 0xffff},
    { 0x1d5a, 0xc201, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for FSP276", 0xffff},
    { 0x1d5a, 0xc2b3, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MKCV)", 0xffff},
    { 0x1d5a, 0xc300, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP160", 0xffff},
    { 0x1d5a, 0xc301, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP260", 0xffff},
    { 0x1d5a, 0xc302, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U160", 0xffff},
    { 0x1d5a, 0xc303, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U260", 0xffff},
    { 0x1d5a, 0xc304, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP160", 0xffff},
    { 0x1d5a, 0xc305, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP260", 0xffff},
    { 0x1d5a, 0xc306, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP261", 0xffff},
    { 0x1d5a, 0xc307, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for FSP262", 0xffff},
    { 0x1d5a, 0xc400, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP160", 0xffff},
    { 0x1d5a, 0xc401, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP260", 0xffff},
    { 0x1d5a, 0xc402, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U160", 0xffff},
    { 0x1d5a, 0xc403, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for S2U260", 0xffff},
    { 0x1d5a, 0xc404, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP160", 0xffff},
    { 0x1d5a, 0xc405, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for CP260", 0xffff},
    { 0x1d5a, 0xc406, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for WP261", 0xffff},
    { 0x1d5a, 0xc407, 1, 1, 0, {{0, 0},      {0, 0}}, "USB Bootloader for FSP262", 0xffff},
    { 0x300,  0x30,   1, 1, 2, {{1, 0xff00}, {0, 0}}, "Transceiver", 0xffff},
    { 0x0489, 0xe08c, 1, 1, 2, {{4, 0xff01}, {0, 0}}, "Smart USB RF Transceiver v2 (MKCV)", 0xffff},
};

const int freespace_deviceAPITableNum = sizeof(freespace_deviceAPITable) / sizeof(struct FreespaceDeviceAPI);

