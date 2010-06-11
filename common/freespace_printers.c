/*
 * This file is part of libfreespace.
 * 
 * Copyright (c) 2009-2010 Hillcrest Laboratories, Inc. 
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

void freespace_printMessage(FILE* fp, const uint8_t* message, int length, uint8_t ver) {
    struct freespace_message s;
    int rc = freespace_decode_message(message, length, &s, ver);
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
    case FREESPACE_MESSAGE_ALWAYSONRESPONSE:
        freespace_printAlwaysOnResponse(fp, &(s->alwaysOnResponse));
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
    case FREESPACE_MESSAGE_DATAMODERESPONSE:
        freespace_printDataModeResponse(fp, &(s->dataModeResponse));
        break;
    case FREESPACE_MESSAGE_BUTTONTESTMODERESPONSE:
        freespace_printButtonTestModeResponse(fp, &(s->buttonTestModeResponse));
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
    case FREESPACE_MESSAGE_FRSWRITERESPONSE:
        freespace_printFRSWriteResponse(fp, &(s->fRSWriteResponse));
        break;
    case FREESPACE_MESSAGE_FRSREADRESPONSE:
        freespace_printFRSReadResponse(fp, &(s->fRSReadResponse));
        break;
    case FREESPACE_MESSAGE_PERRESPONSE:
        freespace_printPerResponse(fp, &(s->perResponse));
        break;
    case FREESPACE_MESSAGE_BODYUSERFRAME:
        freespace_printBodyUserFrame(fp, &(s->bodyUserFrame));
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
    n = sprintf_s(dest, maxlen, "CoprocessorOutReport(payloadLength=%d )", s->payloadLength);
#else
    n = sprintf(dest, "CoprocessorOutReport(payloadLength=%d )", s->payloadLength);
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
    n = sprintf_s(dest, maxlen, "CoprocessorInReport(payloadLength=%d )", s->payloadLength);
#else
    n = sprintf(dest, "CoprocessorInReport(payloadLength=%d )", s->payloadLength);
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


LIBFREESPACE_API int freespace_printPairingMessageStr(char* dest, int maxlen, const struct freespace_PairingMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "PairingMessage()");
#else
    n = sprintf(dest, "PairingMessage()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printPairingMessage(FILE* fp, const struct freespace_PairingMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printPairingMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printProductIDRequestStr(char* dest, int maxlen, const struct freespace_ProductIDRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ProductIDRequest()");
#else
    n = sprintf(dest, "ProductIDRequest()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printProductIDRequest(FILE* fp, const struct freespace_ProductIDRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printProductIDRequestStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "LEDSetRequest(selectLED=%d onOff=%d)", s->selectLED, s->onOff);
#else
    n = sprintf(dest, "LEDSetRequest(selectLED=%d onOff=%d)", s->selectLED, s->onOff);
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


LIBFREESPACE_API int freespace_printAlwaysOnRequestStr(char* dest, int maxlen, const struct freespace_AlwaysOnRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "AlwaysOnRequest()");
#else
    n = sprintf(dest, "AlwaysOnRequest()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printAlwaysOnRequest(FILE* fp, const struct freespace_AlwaysOnRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printAlwaysOnRequestStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "FrequencyFixRequest(channel3=%d channel2=%d channel1=%d channel0=%d channel4=%d device=%d)", s->channel3, s->channel2, s->channel1, s->channel0, s->channel4, s->device);
#else
    n = sprintf(dest, "FrequencyFixRequest(channel3=%d channel2=%d channel1=%d channel0=%d channel4=%d device=%d)", s->channel3, s->channel2, s->channel1, s->channel0, s->channel4, s->device);
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


LIBFREESPACE_API int freespace_printDongleRFDisableMessageStr(char* dest, int maxlen, const struct freespace_DongleRFDisableMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DongleRFDisableMessage()");
#else
    n = sprintf(dest, "DongleRFDisableMessage()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDongleRFDisableMessage(FILE* fp, const struct freespace_DongleRFDisableMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDongleRFDisableMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printTxDisableMessageStr(char* dest, int maxlen, const struct freespace_TxDisableMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "TxDisableMessage()");
#else
    n = sprintf(dest, "TxDisableMessage()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printTxDisableMessage(FILE* fp, const struct freespace_TxDisableMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printTxDisableMessageStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "DongleRFSupressHomeFrequencyMessage(high=%d low=%d)", s->high, s->low);
#else
    n = sprintf(dest, "DongleRFSupressHomeFrequencyMessage(high=%d low=%d)", s->high, s->low);
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
    n = sprintf_s(dest, maxlen, "FRSLoopReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSLoopReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
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
    n = sprintf_s(dest, maxlen, "FRSDongleReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSDongleReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSEFlashReadRequest(BlockSize=%d wordOffset=%d FRStype=%d)", s->BlockSize, s->wordOffset, s->FRStype);
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


LIBFREESPACE_API int freespace_printDongleRFEnableMessageStr(char* dest, int maxlen, const struct freespace_DongleRFEnableMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DongleRFEnableMessage()");
#else
    n = sprintf(dest, "DongleRFEnableMessage()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDongleRFEnableMessage(FILE* fp, const struct freespace_DongleRFEnableMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDongleRFEnableMessageStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "DataModeRequest(status=%d disableFreespace=%d inhibitPowerManager=%d enableUserPosition=%d enableBodyMotion=%d SDA=%d aggregate=%d enableMouseMovement=%d)", s->status, s->disableFreespace, s->inhibitPowerManager, s->enableUserPosition, s->enableBodyMotion, s->SDA, s->aggregate, s->enableMouseMovement);
#else
    n = sprintf(dest, "DataModeRequest(status=%d disableFreespace=%d inhibitPowerManager=%d enableUserPosition=%d enableBodyMotion=%d SDA=%d aggregate=%d enableMouseMovement=%d)", s->status, s->disableFreespace, s->inhibitPowerManager, s->enableUserPosition, s->enableBodyMotion, s->SDA, s->aggregate, s->enableMouseMovement);
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


LIBFREESPACE_API int freespace_printButtonTestModeRequestStr(char* dest, int maxlen, const struct freespace_ButtonTestModeRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ButtonTestModeRequest(enable=%d)", s->enable);
#else
    n = sprintf(dest, "ButtonTestModeRequest(enable=%d)", s->enable);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printButtonTestModeRequest(FILE* fp, const struct freespace_ButtonTestModeRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printButtonTestModeRequestStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "PairingResponse(autoPairing=%d pairing=%d success=%d)", s->autoPairing, s->pairing, s->success);
#else
    n = sprintf(dest, "PairingResponse(autoPairing=%d pairing=%d success=%d)", s->autoPairing, s->pairing, s->success);
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
    n = sprintf_s(dest, maxlen, "ProductIDResponse(swPartNumber=%d invalidNS=%d serialNumber=%d startup=%d swVersionMajor=%d swicn=%d swBuildNumber=%d swVersionPatch=%d swVersionMinor=%d deviceClass=%d)", s->swPartNumber, s->invalidNS, s->serialNumber, s->startup, s->swVersionMajor, s->swicn, s->swBuildNumber, s->swVersionPatch, s->swVersionMinor, s->deviceClass);
#else
    n = sprintf(dest, "ProductIDResponse(swPartNumber=%d invalidNS=%d serialNumber=%d startup=%d swVersionMajor=%d swicn=%d swBuildNumber=%d swVersionPatch=%d swVersionMinor=%d deviceClass=%d)", s->swPartNumber, s->invalidNS, s->serialNumber, s->startup, s->swVersionMajor, s->swicn, s->swBuildNumber, s->swVersionPatch, s->swVersionMinor, s->deviceClass);
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
    n = sprintf_s(dest, maxlen, "LinkStatus(status=%d resetStatus=%d mode=%d txDisabled=%d)", s->status, s->resetStatus, s->mode, s->txDisabled);
#else
    n = sprintf(dest, "LinkStatus(status=%d resetStatus=%d mode=%d txDisabled=%d)", s->status, s->resetStatus, s->mode, s->txDisabled);
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


LIBFREESPACE_API int freespace_printAlwaysOnResponseStr(char* dest, int maxlen, const struct freespace_AlwaysOnResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "AlwaysOnResponse()");
#else
    n = sprintf(dest, "AlwaysOnResponse()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printAlwaysOnResponse(FILE* fp, const struct freespace_AlwaysOnResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printAlwaysOnResponseStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "FRSLoopReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
#else
    n = sprintf(dest, "FRSLoopReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
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
    n = sprintf_s(dest, maxlen, "FRSLoopWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
#else
    n = sprintf(dest, "FRSLoopWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
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
    n = sprintf_s(dest, maxlen, "FRSDongleReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
#else
    n = sprintf(dest, "FRSDongleReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
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
    n = sprintf_s(dest, maxlen, "FRSDongleWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
#else
    n = sprintf(dest, "FRSDongleWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
#else
    n = sprintf(dest, "FRSEFlashReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
#else
    n = sprintf(dest, "FRSEFlashWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
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
    n = sprintf_s(dest, maxlen, "DataModeResponse(disableFreespace=%d inhibitPowerManager=%d enableUserPosition=%d enableBodyMotion=%d SDA=%d aggregate=%d enableMouseMovement=%d)", s->disableFreespace, s->inhibitPowerManager, s->enableUserPosition, s->enableBodyMotion, s->SDA, s->aggregate, s->enableMouseMovement);
#else
    n = sprintf(dest, "DataModeResponse(disableFreespace=%d inhibitPowerManager=%d enableUserPosition=%d enableBodyMotion=%d SDA=%d aggregate=%d enableMouseMovement=%d)", s->disableFreespace, s->inhibitPowerManager, s->enableUserPosition, s->enableBodyMotion, s->SDA, s->aggregate, s->enableMouseMovement);
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


LIBFREESPACE_API int freespace_printButtonTestModeResponseStr(char* dest, int maxlen, const struct freespace_ButtonTestModeResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ButtonTestModeResponse(status=%d press=%d button=%d)", s->status, s->press, s->button);
#else
    n = sprintf(dest, "ButtonTestModeResponse(status=%d press=%d button=%d)", s->status, s->press, s->button);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printButtonTestModeResponse(FILE* fp, const struct freespace_ButtonTestModeResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printButtonTestModeResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printBatteryLevelRequestStr(char* dest, int maxlen, const struct freespace_BatteryLevelRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "BatteryLevelRequest()");
#else
    n = sprintf(dest, "BatteryLevelRequest()");
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printBatteryLevelRequest(FILE* fp, const struct freespace_BatteryLevelRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printBatteryLevelRequestStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "BodyFrame(angularVelZ=%d linearAccelZ=%d sequenceNumber=%d deltaX=%d deltaY=%d linearAccelY=%d linearAccelX=%d angularVelX=%d button3=%d button2=%d button1=%d angularVelY=%d button7=%d button6=%d button5=%d button4=%d button8=%d deltaWheel=%d)", s->angularVelZ, s->linearAccelZ, s->sequenceNumber, s->deltaX, s->deltaY, s->linearAccelY, s->linearAccelX, s->angularVelX, s->button3, s->button2, s->button1, s->angularVelY, s->button7, s->button6, s->button5, s->button4, s->button8, s->deltaWheel);
#else
    n = sprintf(dest, "BodyFrame(angularVelZ=%d linearAccelZ=%d sequenceNumber=%d deltaX=%d deltaY=%d linearAccelY=%d linearAccelX=%d angularVelX=%d button3=%d button2=%d button1=%d angularVelY=%d button7=%d button6=%d button5=%d button4=%d button8=%d deltaWheel=%d)", s->angularVelZ, s->linearAccelZ, s->sequenceNumber, s->deltaX, s->deltaY, s->linearAccelY, s->linearAccelX, s->angularVelX, s->button3, s->button2, s->button1, s->angularVelY, s->button7, s->button6, s->button5, s->button4, s->button8, s->deltaWheel);
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
    n = sprintf_s(dest, maxlen, "UserFrame(angularPosC=%d sequenceNumber=%d deltaX=%d deltaY=%d angularPosD=%d angularPosA=%d linearPosZ=%d linearPosY=%d linearPosX=%d button3=%d button2=%d button1=%d angularPosB=%d button7=%d button6=%d button5=%d button4=%d button8=%d deltaWheel=%d)", s->angularPosC, s->sequenceNumber, s->deltaX, s->deltaY, s->angularPosD, s->angularPosA, s->linearPosZ, s->linearPosY, s->linearPosX, s->button3, s->button2, s->button1, s->angularPosB, s->button7, s->button6, s->button5, s->button4, s->button8, s->deltaWheel);
#else
    n = sprintf(dest, "UserFrame(angularPosC=%d sequenceNumber=%d deltaX=%d deltaY=%d angularPosD=%d angularPosA=%d linearPosZ=%d linearPosY=%d linearPosX=%d button3=%d button2=%d button1=%d angularPosB=%d button7=%d button6=%d button5=%d button4=%d button8=%d deltaWheel=%d)", s->angularPosC, s->sequenceNumber, s->deltaX, s->deltaY, s->angularPosD, s->angularPosA, s->linearPosZ, s->linearPosY, s->linearPosX, s->button3, s->button2, s->button1, s->angularPosB, s->button7, s->button6, s->button5, s->button4, s->button8, s->deltaWheel);
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
    n = sprintf_s(dest, maxlen, "DataMotionControl(enableMouseMovement=%d enableUserPosition=%d disableFreespace=%d inhibitPowerManager=%d enableBodyMotion=%d)", s->enableMouseMovement, s->enableUserPosition, s->disableFreespace, s->inhibitPowerManager, s->enableBodyMotion);
#else
    n = sprintf(dest, "DataMotionControl(enableMouseMovement=%d enableUserPosition=%d disableFreespace=%d inhibitPowerManager=%d enableBodyMotion=%d)", s->enableMouseMovement, s->enableUserPosition, s->disableFreespace, s->inhibitPowerManager, s->enableBodyMotion);
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


LIBFREESPACE_API int freespace_printFRSWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSWriteResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
#else
    n = sprintf(dest, "FRSWriteResponse(status=%d wordOffset=%d)", s->status, s->wordOffset);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSWriteResponse(FILE* fp, const struct freespace_FRSWriteResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSWriteResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSReadResponseStr(char* dest, int maxlen, const struct freespace_FRSReadResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
#else
    n = sprintf(dest, "FRSReadResponse(status=%d FRStype=%d wordOffset=%d  dataLength=%d)", s->status, s->FRStype, s->wordOffset, s->dataLength);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSReadResponse(FILE* fp, const struct freespace_FRSReadResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSReadResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printPerResponseStr(char* dest, int maxlen, const struct freespace_PerResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "PerResponse(count=%d msError=%d frError=%d smError=%d)", s->count, s->msError, s->frError, s->smError);
#else
    n = sprintf(dest, "PerResponse(count=%d msError=%d frError=%d smError=%d)", s->count, s->msError, s->frError, s->smError);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printPerResponse(FILE* fp, const struct freespace_PerResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printPerResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSWriteRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#else
    n = sprintf(dest, "FRSWriteRequest(length=%d FRStype=%d)", s->length, s->FRStype);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSWriteRequest(FILE* fp, const struct freespace_FRSWriteRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSWriteRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSWriteDataStr(char* dest, int maxlen, const struct freespace_FRSWriteData* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#else
    n = sprintf(dest, "FRSWriteData(wordOffset=%d data=%d)", s->wordOffset, s->data);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSWriteData(FILE* fp, const struct freespace_FRSWriteData* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSWriteDataStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFRSReadRequestStr(char* dest, int maxlen, const struct freespace_FRSReadRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FRSReadRequest(BlockSize=%d FRStype=%d readOffset=%d)", s->BlockSize, s->FRStype, s->readOffset);
#else
    n = sprintf(dest, "FRSReadRequest(BlockSize=%d FRStype=%d readOffset=%d)", s->BlockSize, s->FRStype, s->readOffset);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFRSReadRequest(FILE* fp, const struct freespace_FRSReadRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFRSReadRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printPerRequestStr(char* dest, int maxlen, const struct freespace_PerRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "PerRequest( op=%d)", s->op);
#else
    n = sprintf(dest, "PerRequest( op=%d)", s->op);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printPerRequest(FILE* fp, const struct freespace_PerRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printPerRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printBodyUserFrameStr(char* dest, int maxlen, const struct freespace_BodyUserFrame* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "BodyUserFrame(sequenceNumber=%d angularVelZ=%d angularVelX=%d angularVelY=%d linearPosZ=%d deltaX=%d deltaY=%d angularPosD=%d angularPosC=%d angularPosB=%d linearPosY=%d linearPosX=%d angularPosA=%d deltaWheel=%d linearAccelZ=%d linearAccelY=%d linearAccelX=%d button3=%d button2=%d button1=%d button7=%d button6=%d button5=%d button4=%d button8=%d)", s->sequenceNumber, s->angularVelZ, s->angularVelX, s->angularVelY, s->linearPosZ, s->deltaX, s->deltaY, s->angularPosD, s->angularPosC, s->angularPosB, s->linearPosY, s->linearPosX, s->angularPosA, s->deltaWheel, s->linearAccelZ, s->linearAccelY, s->linearAccelX, s->button3, s->button2, s->button1, s->button7, s->button6, s->button5, s->button4, s->button8);
#else
    n = sprintf(dest, "BodyUserFrame(sequenceNumber=%d angularVelZ=%d angularVelX=%d angularVelY=%d linearPosZ=%d deltaX=%d deltaY=%d angularPosD=%d angularPosC=%d angularPosB=%d linearPosY=%d linearPosX=%d angularPosA=%d deltaWheel=%d linearAccelZ=%d linearAccelY=%d linearAccelX=%d button3=%d button2=%d button1=%d button7=%d button6=%d button5=%d button4=%d button8=%d)", s->sequenceNumber, s->angularVelZ, s->angularVelX, s->angularVelY, s->linearPosZ, s->deltaX, s->deltaY, s->angularPosD, s->angularPosC, s->angularPosB, s->linearPosY, s->linearPosX, s->angularPosA, s->deltaWheel, s->linearAccelZ, s->linearAccelY, s->linearAccelX, s->button3, s->button2, s->button1, s->button7, s->button6, s->button5, s->button4, s->button8);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printBodyUserFrame(FILE* fp, const struct freespace_BodyUserFrame* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printBodyUserFrameStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}

