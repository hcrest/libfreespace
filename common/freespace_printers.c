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

#include "freespace/freespace_printers.h"
#include <string.h>

static void printUnknown(const char* name, const uint8_t* buffer, int length) {
    int i;
    printf("%s(", name);
    for (i = 0; i < length; ++i) {
        printf("%02x ", (uint8_t) buffer[i]);
    }
    printf(")\n");
}

void freespace_printMessage(FILE* fp, const uint8_t* message, int length) {
    struct freespace_message s;
    int rc = freespace_decode_message(message, length, &s);
    if (rc != 0) {
        printUnknown("unknown", message, length);
    }
    freespace_printMessageStruct(fp, &s);
}

void freespace_printMessageStruct(FILE* fp, struct freespace_message* s) {
    switch(s->messageType) {
    case FREESPACE_MESSAGE_COPROCESSORINREPORT:
        freespace_printCoprocessorInReport(fp, &(s->coprocessorInReport));
        break;
    case FREESPACE_MESSAGE_PAIRINGRESPONSE:
        freespace_printPairingResponse(fp, &(s->pairingResponse));
        break;
    case FREESPACE_MESSAGE_PRODUCTIDRESPONSE:
        freespace_printProductIDResponse(fp, &(s->productIDResponse));
        break;
    case FREESPACE_MESSAGE_LINKSTATUS:
        freespace_printLinkStatus(fp, &(s->linkStatus));
        break;
    case FREESPACE_MESSAGE_FRSLOOPREADRESPONSE:
        freespace_printFRSLoopReadResponse(fp, &(s->fRSLoopReadResponse));
        break;
    case FREESPACE_MESSAGE_FRSLOOPWRITERESPONSE:
        freespace_printFRSLoopWriteResponse(fp, &(s->fRSLoopWriteResponse));
        break;
    case FREESPACE_MESSAGE_FRSDONGLEREADRESPONSE:
        freespace_printFRSDongleReadResponse(fp, &(s->fRSDongleReadResponse));
        break;
    case FREESPACE_MESSAGE_FRSDONGLEWRITERESPONSE:
        freespace_printFRSDongleWriteResponse(fp, &(s->fRSDongleWriteResponse));
        break;
    case FREESPACE_MESSAGE_FRSEFLASHREADRESPONSE:
        freespace_printFRSEFlashReadResponse(fp, &(s->fRSEFlashReadResponse));
        break;
    case FREESPACE_MESSAGE_FRSEFLASHWRITERESPONSE:
        freespace_printFRSEFlashWriteResponse(fp, &(s->fRSEFlashWriteResponse));
        break;
    case FREESPACE_MESSAGE_BATTERYLEVEL:
        freespace_printBatteryLevel(fp, &(s->batteryLevel));
        break;
    case FREESPACE_MESSAGE_BODYFRAME:
        freespace_printBodyFrame(fp, &(s->bodyFrame));
        break;
    case FREESPACE_MESSAGE_USERFRAME:
        freespace_printUserFrame(fp, &(s->userFrame));
        break;
    default:
        return;
    }
}
LIBFREESPACE_API int freespace_printCoprocessorOutReportStr(char* dest, int maxlen, const struct freespace_CoprocessorOutReport* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "CoprocessorOutReport(payloadLength=%d)", s->payloadLength);
#else
    n = sprintf(dest, "CoprocessorOutReport(payloadLength=%d)", s->payloadLength);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printCoprocessorOutReport(FILE* fp, const struct freespace_CoprocessorOutReport* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printCoprocessorOutReportStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printCoprocessorInReportStr(char* dest, int maxlen, const struct freespace_CoprocessorInReport* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "CoprocessorInReport(payloadLength=%d)", s->payloadLength);
#else
    n = sprintf(dest, "CoprocessorInReport(payloadLength=%d)", s->payloadLength);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printCoprocessorInReport(FILE* fp, const struct freespace_CoprocessorInReport* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printCoprocessorInReportStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printLEDSetRequestStr(char* dest, int maxlen, const struct freespace_LEDSetRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LEDSetRequest(onOff=%d selectLED=%d)", s->onOff, s->selectLED);
#else
    n = sprintf(dest, "LEDSetRequest(onOff=%d selectLED=%d)", s->onOff, s->selectLED);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLEDSetRequest(FILE* fp, const struct freespace_LEDSetRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLEDSetRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printLinkQualityRequestStr(char* dest, int maxlen, const struct freespace_LinkQualityRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LinkQualityRequest(enable=%d)", s->enable);
#else
    n = sprintf(dest, "LinkQualityRequest(enable=%d)", s->enable);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLinkQualityRequest(FILE* fp, const struct freespace_LinkQualityRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLinkQualityRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFrequencyFixRequestStr(char* dest, int maxlen, const struct freespace_FrequencyFixRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FrequencyFixRequest(channel0=%d channel1=%d channel2=%d channel3=%d channel4=%d device=%d)", s->channel0, s->channel1, s->channel2, s->channel3, s->channel4, s->device);
#else
    n = sprintf(dest, "FrequencyFixRequest(channel0=%d channel1=%d channel2=%d channel3=%d channel4=%d device=%d)", s->channel0, s->channel1, s->channel2, s->channel3, s->channel4, s->device);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFrequencyFixRequest(FILE* fp, const struct freespace_FrequencyFixRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFrequencyFixRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printSoftwareResetMessageStr(char* dest, int maxlen, const struct freespace_SoftwareResetMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "SoftwareResetMessage(device=%d)", s->device);
#else
    n = sprintf(dest, "SoftwareResetMessage(device=%d)", s->device);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printSoftwareResetMessage(FILE* fp, const struct freespace_SoftwareResetMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printSoftwareResetMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessageStr(char* dest, int maxlen, const struct freespace_DongleRFSupressHomeFrequencyMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DongleRFSupressHomeFrequencyMessage(low=%d high=%d)", s->low, s->high);
#else
    n = sprintf(dest, "DongleRFSupressHomeFrequencyMessage(low=%d high=%d)", s->low, s->high);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessage(FILE* fp, const struct freespace_DongleRFSupressHomeFrequencyMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDongleRFSupressHomeFrequencyMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSLoopReadRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopReadRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSLoopReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#else
    n = sprintf(dest, "FRSLoopReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSLoopReadRequest(FILE* fp, const struct freespace_FRSLoopReadRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSLoopReadRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSLoopWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSLoopWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#else
    n = sprintf(dest, "FRSLoopWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSLoopWriteRequest(FILE* fp, const struct freespace_FRSLoopWriteRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSLoopWriteRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSLoopWriteDataStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteData* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSLoopWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#else
    n = sprintf(dest, "FRSLoopWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSLoopWriteData(FILE* fp, const struct freespace_FRSLoopWriteData* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSLoopWriteDataStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSDongleReadRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleReadRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSDongleReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#else
    n = sprintf(dest, "FRSDongleReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSDongleReadRequest(FILE* fp, const struct freespace_FRSDongleReadRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSDongleReadRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSDongleWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSDongleWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#else
    n = sprintf(dest, "FRSDongleWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSDongleWriteRequest(FILE* fp, const struct freespace_FRSDongleWriteRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSDongleWriteRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSDongleWriteDataStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteData* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSDongleWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#else
    n = sprintf(dest, "FRSDongleWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSDongleWriteData(FILE* fp, const struct freespace_FRSDongleWriteData* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSDongleWriteDataStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSEFlashReadRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSEFlashReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#else
    n = sprintf(dest, "FRSEFlashReadRequest(wordOffset=%d FRStype=%d BlockSize=%d)", s->wordOffset, s->FRStype, s->BlockSize);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSEFlashReadRequest(FILE* fp, const struct freespace_FRSEFlashReadRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSEFlashReadRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSEFlashWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSEFlashWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#else
    n = sprintf(dest, "FRSEFlashWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSEFlashWriteRequest(FILE* fp, const struct freespace_FRSEFlashWriteRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSEFlashWriteRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSEFlashWriteDataStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteData* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSEFlashWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#else
    n = sprintf(dest, "FRSEFlashWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSEFlashWriteData(FILE* fp, const struct freespace_FRSEFlashWriteData* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSEFlashWriteDataStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printDataModeRequestStr(char* dest, int maxlen, const struct freespace_DataModeRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DataModeRequest(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#else
    n = sprintf(dest, "DataModeRequest(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDataModeRequest(FILE* fp, const struct freespace_DataModeRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDataModeRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printPairingResponseStr(char* dest, int maxlen, const struct freespace_PairingResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "PairingResponse(pairing=%d autoPairing=%d success=%d)", s->pairing, s->autoPairing, s->success);
#else
    n = sprintf(dest, "PairingResponse(pairing=%d autoPairing=%d success=%d)", s->pairing, s->autoPairing, s->success);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printPairingResponse(FILE* fp, const struct freespace_PairingResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printPairingResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printProductIDResponseStr(char* dest, int maxlen, const struct freespace_ProductIDResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ProductIDResponse(swPartNumber=%d swBuildNumber=%d swicn=%d swVersionPatch=%d swVersionMinor=%d swVersionMajor=%d serialNumber=%d deviceClass=%d invalidNS=%d)", s->swPartNumber, s->swBuildNumber, s->swicn, s->swVersionPatch, s->swVersionMinor, s->swVersionMajor, s->serialNumber, s->deviceClass, s->invalidNS);
#else
    n = sprintf(dest, "ProductIDResponse(swPartNumber=%d swBuildNumber=%d swicn=%d swVersionPatch=%d swVersionMinor=%d swVersionMajor=%d serialNumber=%d deviceClass=%d invalidNS=%d)", s->swPartNumber, s->swBuildNumber, s->swicn, s->swVersionPatch, s->swVersionMinor, s->swVersionMajor, s->serialNumber, s->deviceClass, s->invalidNS);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printProductIDResponse(FILE* fp, const struct freespace_ProductIDResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printProductIDResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printLinkStatusStr(char* dest, int maxlen, const struct freespace_LinkStatus* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LinkStatus(status=%d mode=%d resetStatus=%d)", s->status, s->mode, s->resetStatus);
#else
    n = sprintf(dest, "LinkStatus(status=%d mode=%d resetStatus=%d)", s->status, s->mode, s->resetStatus);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLinkStatus(FILE* fp, const struct freespace_LinkStatus* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLinkStatusStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSLoopReadResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopReadResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSLoopReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#else
    n = sprintf(dest, "FRSLoopReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSLoopReadResponse(FILE* fp, const struct freespace_FRSLoopReadResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSLoopReadResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSLoopWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSLoopWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#else
    n = sprintf(dest, "FRSLoopWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSLoopWriteResponse(FILE* fp, const struct freespace_FRSLoopWriteResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSLoopWriteResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSDongleReadResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleReadResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSDongleReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#else
    n = sprintf(dest, "FRSDongleReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSDongleReadResponse(FILE* fp, const struct freespace_FRSDongleReadResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSDongleReadResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSDongleWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSDongleWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#else
    n = sprintf(dest, "FRSDongleWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSDongleWriteResponse(FILE* fp, const struct freespace_FRSDongleWriteResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSDongleWriteResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSEFlashReadResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSEFlashReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#else
    n = sprintf(dest, "FRSEFlashReadResponse(wordOffset=%d status=%d dataLength=%d FRStype=%d)", s->wordOffset, s->status, s->dataLength, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSEFlashReadResponse(FILE* fp, const struct freespace_FRSEFlashReadResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSEFlashReadResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSEFlashWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSEFlashWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#else
    n = sprintf(dest, "FRSEFlashWriteResponse(wordOffset=%d status=%d)", s->wordOffset, s->status);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSEFlashWriteResponse(FILE* fp, const struct freespace_FRSEFlashWriteResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSEFlashWriteResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printDataModeResponseStr(char* dest, int maxlen, const struct freespace_DataModeResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DataModeResponse(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#else
    n = sprintf(dest, "DataModeResponse(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDataModeResponse(FILE* fp, const struct freespace_DataModeResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDataModeResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printBatteryLevelStr(char* dest, int maxlen, const struct freespace_BatteryLevel* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "BatteryLevel(batteryStrength=%d)", s->batteryStrength);
#else
    n = sprintf(dest, "BatteryLevel(batteryStrength=%d)", s->batteryStrength);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printBatteryLevel(FILE* fp, const struct freespace_BatteryLevel* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printBatteryLevelStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printBodyFrameStr(char* dest, int maxlen, const struct freespace_BodyFrame* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "BodyFrame(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearAccelX=%d linearAccelY=%d linearAccelZ=%d angularVelX=%d angularVelY=%d angularVelZ=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearAccelX, s->linearAccelY, s->linearAccelZ, s->angularVelX, s->angularVelY, s->angularVelZ);
#else
    n = sprintf(dest, "BodyFrame(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearAccelX=%d linearAccelY=%d linearAccelZ=%d angularVelX=%d angularVelY=%d angularVelZ=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearAccelX, s->linearAccelY, s->linearAccelZ, s->angularVelX, s->angularVelY, s->angularVelZ);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printBodyFrame(FILE* fp, const struct freespace_BodyFrame* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printBodyFrameStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printUserFrameStr(char* dest, int maxlen, const struct freespace_UserFrame* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "UserFrame(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearPosX=%d linearPosY=%d linearPosZ=%d angularPosA=%d angularPosB=%d angularPosC=%d angularPosD=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearPosX, s->linearPosY, s->linearPosZ, s->angularPosA, s->angularPosB, s->angularPosC, s->angularPosD);
#else
    n = sprintf(dest, "UserFrame(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearPosX=%d linearPosY=%d linearPosZ=%d angularPosA=%d angularPosB=%d angularPosC=%d angularPosD=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearPosX, s->linearPosY, s->linearPosZ, s->angularPosA, s->angularPosB, s->angularPosC, s->angularPosD);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printUserFrame(FILE* fp, const struct freespace_UserFrame* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printUserFrameStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printDataMotionControlStr(char* dest, int maxlen, const struct freespace_DataMotionControl* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DataMotionControl(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#else
    n = sprintf(dest, "DataMotionControl(enableBodyMotion=%d enableUserPosition=%d inhibitPowerManager=%d enableMouseMovement=%d disableFreespace=%d)", s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDataMotionControl(FILE* fp, const struct freespace_DataMotionControl* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDataMotionControlStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}

