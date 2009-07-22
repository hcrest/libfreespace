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

#include "freespace_discoveryDetail.h"
#include "freespace_deviceMgr.h"
#include <strsafe.h>
#include <malloc.h>

/**
 * Callback for adding Freespace device.
 *
 * @param device The Freespace device for configuration.
 */
typedef void (*freespace_deviceApiCallback)(struct FreespaceDeviceStruct *device);

/**
 * Callback for the DeviceAPI to support multiple simultaneous interfaces
 * for a single top-level device.
 * @param device The device.
 */
static int windowsDeviceApiCallback(struct FreespaceDeviceStruct *device);


/**
 * Figure out which API to use depending on the reported
 * Freespace version.
 */
struct FreespaceDeviceAPI {
    // Set of devices with this version.
    uint16_t idVendor_;
    uint16_t idProduct_;
    USAGE    usage_;
    USAGE    usagePage_;
    freespace_deviceApiCallback    callback_;

    const char* name_;
};

/*
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
static const struct FreespaceDeviceAPI deviceAPITable[] = {
    { 0x1d5a, 0xc001, 4, 0xff01, windowsDeviceApiCallback, "Piranha"},
    { 0x1d5a, 0xc002, 0, 0, NULL, "Piranha bootloader"},
    { 0x1d5a, 0xc003, 4, 0xff01, windowsDeviceApiCallback, "Piranha factory test dongle"},
    { 0x1d5a, 0xc004, 4, 0xff01, NULL, "Piranha sniffer dongle"},
    { 0x1d5a, 0xc005, 4, 0xff01, windowsDeviceApiCallback, "FSRK STM32F10x eval board (E)"},
    { 0x1d5a, 0xc006, 0, 0, NULL, "Cortex Bootloader"},
    { 0x1d5a, 0xc007, 4, 0xff01, windowsDeviceApiCallback, "FSRK Gen4 Dongle"},
    { 0x1d5a, 0xc008, 4, 0xff01, windowsDeviceApiCallback, "SPI to USB adapter board v0"},
    { 0x1d5a, 0xc009, 4, 0xff01, windowsDeviceApiCallback, "USB RF Transceiver v0"},
    { 0x1d5a, 0xc00a, 4, 0xff01, windowsDeviceApiCallback, "Coprocessor to USB adapter v1 (MA)"},
    { 0x1d5a, 0xc00b, 4, 0xff01, windowsDeviceApiCallback, "USB RF Transceiver v1 (MKCA)"},
    { 0x1d5a, 0xc00c, 4, 0xff01, windowsDeviceApiCallback, "SPI to USB adapter v1 (MA)"},
    { 0x1d5a, 0xc010, 4, 0xff01, NULL, "USB RF Transceiver v1 (MV)"},
    { 0x1d5a, 0xc011, 4, 0xff01, NULL, "USB RF Transceiver v1 (MCV)"},
    { 0x1d5a, 0xc012, 4, 0xff01, NULL, "USB RF Transceiver v1 (MKV)"},
    { 0x1d5a, 0xc013, 4, 0xff01, NULL, "USB RF Transceiver v1 (MKCV)"},
    { 0x1d5a, 0xc020, 4, 0xff01, NULL, "SPI to USB adapter v1 (MV)"},
    { 0x1d5a, 0xc021, 4, 0xff01, NULL, "SPI to USB adapter v1 (V)"},
    { 0x1d5a, 0xc030, 4, 0xff01, NULL, "Coprocessor to USB adapter v1 (MV)"},
    { 0x1d5a, 0xc031, 4, 0xff01, NULL, "Coprocessor to USB adapter v1 (V)"},
};

static WCHAR* dupeWCharString(const WCHAR* input) {
    int mallocStrLen = lstrlen(input) + 1;
    WCHAR* out = (WCHAR*) malloc(sizeof(WCHAR) * mallocStrLen);
    if (out != NULL) {
        StringCchCopy(out, mallocStrLen, input);
    }
    return out;
}

/**
 * Get information on the interface specified by the devicePath.
 * @param devicePath path to the device file
 * @param info where the data is returned
 * @return FREESPACE_SUCCESS if ok
 */
static int getDeviceInfo(const WCHAR* devicePath, struct FreespaceDeviceInterfaceInfo* info) {
    HIDD_ATTRIBUTES HIDAttrib;
    HIDP_CAPS               Capabilities;
    PHIDP_PREPARSED_DATA    HidParsedData;
    HANDLE hHandle;

    hHandle = CreateFile(devicePath,
                         GENERIC_READ, // | GENERIC_WRITE,
                         FILE_SHARE_READ | FILE_SHARE_WRITE,
                         NULL,
                         OPEN_EXISTING,
                         FILE_FLAG_OVERLAPPED,
                         NULL);

    /* 6) Get the Device VID & PID */
    if (hHandle == INVALID_HANDLE_VALUE) {
        return FREESPACE_ERROR_UNEXPECTED;
    }

    HIDAttrib.Size = sizeof(HIDAttrib);
    HidD_GetAttributes(hHandle, &HIDAttrib);
    // TRACE( _T("VendorID = 0x%x, ProductID = 0x%x\n"),HIDAttrib.VendorID, HIDAttrib.ProductID);

    // extract the capabilities info
    if (!HidD_GetPreparsedData(hHandle, &HidParsedData)) {
        // TRACE( _T("Could not get preparsed data!\n"));
        CloseHandle(hHandle);
        return FREESPACE_ERROR_UNEXPECTED;
    }

    if (HidP_GetCaps(HidParsedData ,&Capabilities) != HIDP_STATUS_SUCCESS) {
        // TRACE( _T("Could not get capabilities!\n"));
        CloseHandle(hHandle);
        return FREESPACE_ERROR_UNEXPECTED;
    }

    // close the handle, we are done with it for now
    CloseHandle(hHandle);
    info->idVendor_  = HIDAttrib.VendorID;
    info->idProduct_ = HIDAttrib.ProductID;
    info->usage_     = Capabilities.Usage;
    info->usagePage_ = Capabilities.UsagePage;
    info->inputReportByteLength_  = Capabilities.InputReportByteLength;
    info->outputReportByteLength_ = Capabilities.OutputReportByteLength;
    HidD_FreePreparsedData(HidParsedData);
    return FREESPACE_SUCCESS;
}

int windowsDeviceApiCallback(struct FreespaceDeviceStruct *device) {
    int rc;
    FreespaceDeviceRef wptr;

    // Create a copy of the reference name.
    FreespaceDeviceRef ref = dupeWCharString(device->handle_[0].devicePath);
    if (ref == NULL) {
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }
    device->handle_[1].devicePath = ref;
    DEBUG_WPRINTF(L"Device = %s\n", ref);

    /*
     * Ugly code to find the matching multi-axis HID page
     * FSRK 3.1 products : &col01# to &col02#, &0000#{ to &0001#{
     * FSRK 1.3 products : &col02# to &col03#, &0001#{ to &0002#{
     */
    wptr = wcsstr(device->handle_[1].devicePath, L"&col0");
    if (wptr != NULL) {
        wptr[5] = wptr[5] + 1;
    }
    wptr = wcsstr(device->handle_[1].devicePath, L"#{");
    if (wptr != NULL) {
        wptr[-1] = wptr[-1] + 1;
    }
    DEBUG_WPRINTF(L"         %s\n", ref);
    rc = getDeviceInfo(ref, &device->handle_[1].info_);

    device->handleCount_ = 2;
    return rc;
}

static const struct FreespaceDeviceAPI* getDeviceAPI(const struct FreespaceDeviceInterfaceInfo* info) {
    int i;
    for (i = 0; i < (sizeof(deviceAPITable) / sizeof(struct FreespaceDeviceAPI)); i++) {
        const struct FreespaceDeviceAPI* api = &deviceAPITable[i];
        if (info->idVendor_ == api->idVendor_ && info->idProduct_ == api->idProduct_) {
            if (api->usage_ != 0 && api->usage_ != info->usage_) {
                continue;
            }
            if (api->usagePage_ != 0 && api->usagePage_ != info->usagePage_) {
                continue;
            }
            return api;
        }
    }

    return NULL;
}

static int addNewDevice(FreespaceDeviceRef ref,
                        const struct FreespaceDeviceAPI* api,
                        struct FreespaceDeviceInterfaceInfo* info,
                        struct FreespaceDeviceStruct** deviceOut) {
    struct FreespaceDeviceStruct *device;
    int rc;

    // Create the device
    device = freespace_private_createDevice(api->name_);

    // Create copies of the other malloc fields, or set to NULL.
    device->handle_[0].devicePath = dupeWCharString(ref);
    if (device->handle_[0].devicePath == NULL) {
        return FREESPACE_ERROR_OUT_OF_MEMORY;
    }

    // Populate the relevant information.
    device->handle_[0].info_ = *info;
    device->handleCount_ = 1;

    // Call the device API callback for further configuration.
    if (api->callback_ != NULL) {
        api->callback_(device);
    }

    rc = freespace_private_addDevice(device);

    *deviceOut = device;
    return rc;
}

int freespace_private_scanAndAddDevices() {
    const struct FreespaceDeviceAPI* api;
    int rc = FREESPACE_SUCCESS;
    struct FreespaceDeviceInterfaceInfo info;
    struct FreespaceDeviceStruct* device;
    HDEVINFO hardwareDeviceInfo;                /* handle to structure containing all attached HID Device information */
    PSP_DEVICE_INTERFACE_DETAIL_DATA functionClassDeviceData = NULL; /* device info data */
    SP_DEVICE_INTERFACE_DATA deviceInfoData;
    DWORD Index;
    DWORD requiredLength;
    DWORD predictedLength;

    // 1) Get the HID Globally Unique ID from the OS
    GUID cls;
    HidD_GetHidGuid(&cls);

    // 2) Get an array of structures containing information about
    // all attached and enumerated HIDs
    hardwareDeviceInfo = SetupDiGetClassDevs(&cls,
                                             NULL,
                                             NULL,
                                             DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hardwareDeviceInfo == INVALID_HANDLE_VALUE) {
        return FREESPACE_ERROR_UNEXPECTED;
    }

    // Perform some initialization
    // set to the size of the structure that will contain the device info data
    deviceInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    Index = 0; // Index into hardwareDeviceInfo

    /* 3) Step through the attached device list 1 by 1 and examine
       each of the attached devices.  When there are no more entries in
       the array of structures, the function will return */
    for (;;) {
        /* free the memory allocated for functionClassDeviceData */
        if(functionClassDeviceData != NULL) {
            free(functionClassDeviceData);
            functionClassDeviceData = NULL;
        }

        // 3A) Get information about each HID device in turn
        // The current device is doned by Index
        if (! SetupDiEnumDeviceInterfaces(hardwareDeviceInfo,
                                          0, // No care about specific PDOs
                                          &cls,
                                          Index,
                                          &deviceInfoData)) {
            if (ERROR_NO_MORE_ITEMS == GetLastError()) {
                // Last entry found.  Successful termination.
            } else {
                // Unknown error!
                if (hardwareDeviceInfo != INVALID_HANDLE_VALUE) {
                    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
                }
                rc = FREESPACE_ERROR_UNEXPECTED;
            }
            break;
        }
        Index++;

        /* 3B) Get the size of the DEVICE_INTERFACE_DETAIL_DATA
           structure.  The first call will return an error condition,
           but we'll get the size of the strucure */
        requiredLength = 0;
        predictedLength = 0;
        SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo,
                                        &deviceInfoData,
                                        NULL,
                                        0,
                                        &requiredLength,
                                        NULL);

        /* 3C) allocate memory for the hardwareDeviceInfo structure */
        predictedLength = requiredLength;
        functionClassDeviceData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(predictedLength);
        if (functionClassDeviceData) {
            /* set the size parameter of the structure */
            functionClassDeviceData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        } else {
            rc = FREESPACE_ERROR_OUT_OF_MEMORY;
            break;
        }

        /* 3D) Now call the function with the correct size parameter.  This
           function will return data from one of the array members that
           Step #2 pointed to.  This way we can start to identify the
           attributes of particular HID devices.  */
        if (! SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo,
                                              &deviceInfoData,
                                              functionClassDeviceData,
                                              predictedLength,
                                              &requiredLength,
                                              NULL) ) {
            rc = FREESPACE_ERROR_UNEXPECTED;
            break;
        }

        // Check if we know about the device.
        device = freespace_private_getDeviceByRef(functionClassDeviceData->DevicePath);
        if (device) {
            // Yes, so mark it.
            device->status_ = FREESPACE_DISCOVERY_STATUS_EXISTING;
        } else {
            // Unknown device, so get more info on it.
            rc = getDeviceInfo(functionClassDeviceData->DevicePath, &info);
            if (rc != FREESPACE_SUCCESS) {
                continue;
            }

            // Check if it is a supported Freespace device.
            api = getDeviceAPI(&info);
            if (api != NULL) {
                rc = addNewDevice(functionClassDeviceData->DevicePath, api, &info, &device);
                if (rc == FREESPACE_SUCCESS) {
                    device->status_ = FREESPACE_DISCOVERY_STATUS_ADDED;
                } else {
                    // Something very strange happened.
                    if (hardwareDeviceInfo != INVALID_HANDLE_VALUE) {
                        SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
                    }
                    return rc;
                }
            }
        }
    }

    /* 4) Free the allocated memory */
    if (hardwareDeviceInfo != INVALID_HANDLE_VALUE) {
        SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
    }

    return FREESPACE_SUCCESS;
}
