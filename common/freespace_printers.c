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

static void printUnknown(const char* name, const char* buffer, int length) {
    int i;
    printf("%s(", name);
    for (i = 0; i < length; ++i) {
        printf("%02x ", (uint8_t) buffer[i]);
    }
    printf(")\n");
}

void freespace_printMessage(FILE* fp, const char* message, int length) {
    struct freespace_message s;
    int rc = freespace_decode_message((const int8_t*) message, length, &s);
    if (rc != 0) {
        printUnknown("unknown", message, length);
    }
    freespace_printMessageStruct(fp, &s);
}

void freespace_printMessageStruct(FILE* fp, struct freespace_message* s) {
    switch(s->messageType) {
    case FREESPACE_MESSAGE_MOUSEMOVEMENT:
        freespace_printMouseMovement(fp, &(s->mouseMovement));
        break;
    case FREESPACE_MESSAGE_CONSUMERCONTROL:
        freespace_printConsumerControl(fp, &(s->consumerControl));
        break;
    case FREESPACE_MESSAGE_KEYBOARDREPORT:
        freespace_printKeyboardReport(fp, &(s->keyboardReport));
        break;
    case FREESPACE_MESSAGE_COPROCESSORINREPORT:
        freespace_printCoprocessorInReport(fp, &(s->coprocessorInReport));
        break;
    case FREESPACE_MESSAGE_GEN4SDAFORMAT:
        freespace_printGen4SDAFormat(fp, &(s->gen4SDAFormat));
        break;
    case FREESPACE_MESSAGE_FACTORYCALIBRATIONREADDATA:
        freespace_printFactoryCalibrationReadData(fp, &(s->factoryCalibrationReadData));
        break;
    case FREESPACE_MESSAGE_FTDONGLESTATUSRESPONSE:
        freespace_printFTDongleStatusResponse(fp, &(s->fTDongleStatusResponse));
        break;
    case FREESPACE_MESSAGE_STATISTICSRESPONSE:
        freespace_printStatisticsResponse(fp, &(s->statisticsResponse));
        break;
    case FREESPACE_MESSAGE_ZEBRASYSTEMTESTRESPONSE:
        freespace_printZebraSystemTestResponse(fp, &(s->zebraSystemTestResponse));
        break;
    case FREESPACE_MESSAGE_PRODUCTIDRESPONSE:
        freespace_printProductIDResponse(fp, &(s->productIDResponse));
        break;
    case FREESPACE_MESSAGE_LINKSTATUS:
        freespace_printLinkStatus(fp, &(s->linkStatus));
        break;
    case FREESPACE_MESSAGE_SPIOPERATIONRESPONSE:
        freespace_printSPIOperationResponse(fp, &(s->sPIOperationResponse));
        break;
    case FREESPACE_MESSAGE_EVENTREPORTCONFIGURATIONRESPONSE:
        freespace_printEventReportConfigurationResponse(fp, &(s->eventReportConfigurationResponse));
        break;
    case FREESPACE_MESSAGE_EVENTREPORT:
        freespace_printEventReport(fp, &(s->eventReport));
        break;
    case FREESPACE_MESSAGE_UNKNOWNCRSNOTIFICATIONRESPONSE:
        freespace_printUnknownCRSNotificationResponse(fp, &(s->unknownCRSNotificationResponse));
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
    case FREESPACE_MESSAGE_FSP2COPROCESSORMESSAGE:
        freespace_printFSP2CoprocessorMessage(fp, &(s->fSP2CoprocessorMessage));
        break;
    case FREESPACE_MESSAGE_LOOPBOOTLOADERSTATUS:
        freespace_printLoopBootloaderStatus(fp, &(s->loopBootloaderStatus));
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
    case FREESPACE_MESSAGE_BUTTONSTATE:
        freespace_printButtonState(fp, &(s->buttonState));
        break;
    case FREESPACE_MESSAGE_SCROLLMOTION:
        freespace_printScrollMotion(fp, &(s->scrollMotion));
        break;
    default:
        return;
    }
}
LIBFREESPACE_API int freespace_printMouseMovementStr(char* dest, int maxlen, const struct freespace_MouseMovement* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "MouseMovement(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel);
#else
    n = sprintf(dest, "MouseMovement(button1=%d button2=%d button3=%d button4=%d button5=%d button6=%d button7=%d button8=%d deltaX=%d deltaY=%d deltaWheel=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->button6, s->button7, s->button8, s->deltaX, s->deltaY, s->deltaWheel);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printMouseMovement(FILE* fp, const struct freespace_MouseMovement* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printMouseMovementStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printConsumerControlStr(char* dest, int maxlen, const struct freespace_ConsumerControl* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ConsumerControl(usageID=%d usageID11_8=%d numPad=%d functionButtons=%d)", s->usageID, s->usageID11_8, s->numPad, s->functionButtons);
#else
    n = sprintf(dest, "ConsumerControl(usageID=%d usageID11_8=%d numPad=%d functionButtons=%d)", s->usageID, s->usageID11_8, s->numPad, s->functionButtons);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printConsumerControl(FILE* fp, const struct freespace_ConsumerControl* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printConsumerControlStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printKeyboardReportStr(char* dest, int maxlen, const struct freespace_KeyboardReport* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "KeyboardReport(LCtrl=%d LShift=%d LAlt=%d LGui=%d RCtrl=%d RShift=%d RAlt=%d RGui=%d keyID=%d)", s->LCtrl, s->LShift, s->LAlt, s->LGui, s->RCtrl, s->RShift, s->RAlt, s->RGui, s->keyID);
#else
    n = sprintf(dest, "KeyboardReport(LCtrl=%d LShift=%d LAlt=%d LGui=%d RCtrl=%d RShift=%d RAlt=%d RGui=%d keyID=%d)", s->LCtrl, s->LShift, s->LAlt, s->LGui, s->RCtrl, s->RShift, s->RAlt, s->RGui, s->keyID);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printKeyboardReport(FILE* fp, const struct freespace_KeyboardReport* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printKeyboardReportStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
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


LIBFREESPACE_API int freespace_printConfigurationMessageStr(char* dest, int maxlen, const struct freespace_ConfigurationMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ConfigurationMessage(SDA=%d)", s->SDA);
#else
    n = sprintf(dest, "ConfigurationMessage(SDA=%d)", s->SDA);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printConfigurationMessage(FILE* fp, const struct freespace_ConfigurationMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printConfigurationMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFactoryCalibrationReadRequestStr(char* dest, int maxlen, const struct freespace_FactoryCalibrationReadRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FactoryCalibrationReadRequest(wordOffsetRequested=%d)", s->wordOffsetRequested);
#else
    n = sprintf(dest, "FactoryCalibrationReadRequest(wordOffsetRequested=%d)", s->wordOffsetRequested);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFactoryCalibrationReadRequest(FILE* fp, const struct freespace_FactoryCalibrationReadRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFactoryCalibrationReadRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printDongleResetStr(char* dest, int maxlen, const struct freespace_DongleReset* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "DongleReset(loaderKey=%d)", s->loaderKey);
#else
    n = sprintf(dest, "DongleReset(loaderKey=%d)", s->loaderKey);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printDongleReset(FILE* fp, const struct freespace_DongleReset* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printDongleResetStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFTDongleStatusRequestStr(char* dest, int maxlen, const struct freespace_FTDongleStatusRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FTDongleStatusRequest(power=%d reset=%d presence=%d operation=%d)", s->power, s->reset, s->presence, s->operation);
#else
    n = sprintf(dest, "FTDongleStatusRequest(power=%d reset=%d presence=%d operation=%d)", s->power, s->reset, s->presence, s->operation);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFTDongleStatusRequest(FILE* fp, const struct freespace_FTDongleStatusRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFTDongleStatusRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printZebraSystemTestStr(char* dest, int maxlen, const struct freespace_ZebraSystemTest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ZebraSystemTest(reset=%d mode=%d pcSequence=%d)", s->reset, s->mode, s->pcSequence);
#else
    n = sprintf(dest, "ZebraSystemTest(reset=%d mode=%d pcSequence=%d)", s->reset, s->mode, s->pcSequence);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printZebraSystemTest(FILE* fp, const struct freespace_ZebraSystemTest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printZebraSystemTestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printLoopLEDSetRequestStr(char* dest, int maxlen, const struct freespace_LoopLEDSetRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LoopLEDSetRequest(onOff=%d selectLED=%d)", s->onOff, s->selectLED);
#else
    n = sprintf(dest, "LoopLEDSetRequest(onOff=%d selectLED=%d)", s->onOff, s->selectLED);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLoopLEDSetRequest(FILE* fp, const struct freespace_LoopLEDSetRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLoopLEDSetRequestStr(str, sizeof(str), s);
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


LIBFREESPACE_API int freespace_printSPIOperationMessageStr(char* dest, int maxlen, const struct freespace_SPIOperationMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "SPIOperationMessage(byte0=%d byte1=%d byte2=%d byte3=%d byte4=%d byte5=%d)", s->byte0, s->byte1, s->byte2, s->byte3, s->byte4, s->byte5);
#else
    n = sprintf(dest, "SPIOperationMessage(byte0=%d byte1=%d byte2=%d byte3=%d byte4=%d byte5=%d)", s->byte0, s->byte1, s->byte2, s->byte3, s->byte4, s->byte5);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printSPIOperationMessage(FILE* fp, const struct freespace_SPIOperationMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printSPIOperationMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printEventReportConfigSetRequestStr(char* dest, int maxlen, const struct freespace_EventReportConfigSetRequest* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "EventReportConfigSetRequest(intc=%d xyMov=%d acSt=%d reset=%d motDr=%d wom=%d motOv=%d acEv=%d sdaDr=%d sdaOv=%d cfgSt=%d)", s->intc, s->xyMov, s->acSt, s->reset, s->motDr, s->wom, s->motOv, s->acEv, s->sdaDr, s->sdaOv, s->cfgSt);
#else
    n = sprintf(dest, "EventReportConfigSetRequest(intc=%d xyMov=%d acSt=%d reset=%d motDr=%d wom=%d motOv=%d acEv=%d sdaDr=%d sdaOv=%d cfgSt=%d)", s->intc, s->xyMov, s->acSt, s->reset, s->motDr, s->wom, s->motOv, s->acEv, s->sdaDr, s->sdaOv, s->cfgSt);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printEventReportConfigSetRequest(FILE* fp, const struct freespace_EventReportConfigSetRequest* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printEventReportConfigSetRequestStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printUnknownCRSNotificationStr(char* dest, int maxlen, const struct freespace_UnknownCRSNotification* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "UnknownCRSNotification(unknownReportID=%d unknownSubMessageID=%d)", s->unknownReportID, s->unknownSubMessageID);
#else
    n = sprintf(dest, "UnknownCRSNotification(unknownReportID=%d unknownSubMessageID=%d)", s->unknownReportID, s->unknownSubMessageID);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printUnknownCRSNotification(FILE* fp, const struct freespace_UnknownCRSNotification* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printUnknownCRSNotificationStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "FRSLoopReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSLoopReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
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
    n = sprintf_s(dest, maxlen, "FRSLoopWriteData(wordOffset=%d)", s->wordOffset);
#else
    n = sprintf(dest, "FRSLoopWriteData(wordOffset=%d)", s->wordOffset);
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
    n = sprintf_s(dest, maxlen, "FRSDongleReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSDongleReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
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
    n = sprintf_s(dest, maxlen, "FRSDongleWriteData(wordOffset=%d)", s->wordOffset);
#else
    n = sprintf(dest, "FRSDongleWriteData(wordOffset=%d)", s->wordOffset);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
#else
    n = sprintf(dest, "FRSEFlashReadRequest(wordOffset=%d FRStype=%d)", s->wordOffset, s->FRStype);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashWriteData(wordOffset=%d)", s->wordOffset);
#else
    n = sprintf(dest, "FRSEFlashWriteData(wordOffset=%d)", s->wordOffset);
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


LIBFREESPACE_API int freespace_printLoopBootloaderCommandStr(char* dest, int maxlen, const struct freespace_LoopBootloaderCommand* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LoopBootloaderCommand(loaderKeyCommand=%d)", s->loaderKeyCommand);
#else
    n = sprintf(dest, "LoopBootloaderCommand(loaderKeyCommand=%d)", s->loaderKeyCommand);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLoopBootloaderCommand(FILE* fp, const struct freespace_LoopBootloaderCommand* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLoopBootloaderCommandStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printGen4SDAFormatStr(char* dest, int maxlen, const struct freespace_Gen4SDAFormat* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "Gen4SDAFormat(reportCount=%d left=%d right=%d scroll=%d hold=%d triangle=%d deltaWheel=%d accelX=%d accelY=%d accelZ=%d rotationX=%d rotationY=%d rotationZ=%d xMov=%d yMov=%d sampleBase=%d command=%d)", s->reportCount, s->left, s->right, s->scroll, s->hold, s->triangle, s->deltaWheel, s->accelX, s->accelY, s->accelZ, s->rotationX, s->rotationY, s->rotationZ, s->xMov, s->yMov, s->sampleBase, s->command);
#else
    n = sprintf(dest, "Gen4SDAFormat(reportCount=%d left=%d right=%d scroll=%d hold=%d triangle=%d deltaWheel=%d accelX=%d accelY=%d accelZ=%d rotationX=%d rotationY=%d rotationZ=%d xMov=%d yMov=%d sampleBase=%d command=%d)", s->reportCount, s->left, s->right, s->scroll, s->hold, s->triangle, s->deltaWheel, s->accelX, s->accelY, s->accelZ, s->rotationX, s->rotationY, s->rotationZ, s->xMov, s->yMov, s->sampleBase, s->command);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printGen4SDAFormat(FILE* fp, const struct freespace_Gen4SDAFormat* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printGen4SDAFormatStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFactoryCalibrationReadDataStr(char* dest, int maxlen, const struct freespace_FactoryCalibrationReadData* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FactoryCalibrationReadData(wordOffset=%d dataLength=%d)", s->wordOffset, s->dataLength);
#else
    n = sprintf(dest, "FactoryCalibrationReadData(wordOffset=%d dataLength=%d)", s->wordOffset, s->dataLength);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFactoryCalibrationReadData(FILE* fp, const struct freespace_FactoryCalibrationReadData* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFactoryCalibrationReadDataStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printFTDongleStatusResponseStr(char* dest, int maxlen, const struct freespace_FTDongleStatusResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FTDongleStatusResponse(power=%d reset=%d presence=%d status=%d)", s->power, s->reset, s->presence, s->status);
#else
    n = sprintf(dest, "FTDongleStatusResponse(power=%d reset=%d presence=%d status=%d)", s->power, s->reset, s->presence, s->status);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFTDongleStatusResponse(FILE* fp, const struct freespace_FTDongleStatusResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFTDongleStatusResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printStatisticsResponseStr(char* dest, int maxlen, const struct freespace_StatisticsResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "StatisticsResponse(stackSpace=%d runTime=%d)", s->stackSpace, s->runTime);
#else
    n = sprintf(dest, "StatisticsResponse(stackSpace=%d runTime=%d)", s->stackSpace, s->runTime);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printStatisticsResponse(FILE* fp, const struct freespace_StatisticsResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printStatisticsResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printZebraSystemTestResponseStr(char* dest, int maxlen, const struct freespace_ZebraSystemTestResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ZebraSystemTestResponse(reset=%d mode=%d pcSequence=%d dongleOutSequence=%d rfSequence=%d dongleInSequence=%d)", s->reset, s->mode, s->pcSequence, s->dongleOutSequence, s->rfSequence, s->dongleInSequence);
#else
    n = sprintf(dest, "ZebraSystemTestResponse(reset=%d mode=%d pcSequence=%d dongleOutSequence=%d rfSequence=%d dongleInSequence=%d)", s->reset, s->mode, s->pcSequence, s->dongleOutSequence, s->rfSequence, s->dongleInSequence);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printZebraSystemTestResponse(FILE* fp, const struct freespace_ZebraSystemTestResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printZebraSystemTestResponseStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "ProductIDResponse(swPartNumber=%d swBuildNumber=%d swicn=%d swVersionPatch=%d swVersionMinor=%d swVersionMajor=%d hwPlatformID=%d hwRevision=%d serialNumber=%d deviceClass=%d invalidNS=%d)", s->swPartNumber, s->swBuildNumber, s->swicn, s->swVersionPatch, s->swVersionMinor, s->swVersionMajor, s->hwPlatformID, s->hwRevision, s->serialNumber, s->deviceClass, s->invalidNS);
#else
    n = sprintf(dest, "ProductIDResponse(swPartNumber=%d swBuildNumber=%d swicn=%d swVersionPatch=%d swVersionMinor=%d swVersionMajor=%d hwPlatformID=%d hwRevision=%d serialNumber=%d deviceClass=%d invalidNS=%d)", s->swPartNumber, s->swBuildNumber, s->swicn, s->swVersionPatch, s->swVersionMinor, s->swVersionMajor, s->hwPlatformID, s->hwRevision, s->serialNumber, s->deviceClass, s->invalidNS);
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


LIBFREESPACE_API int freespace_printSPIOperationResponseStr(char* dest, int maxlen, const struct freespace_SPIOperationResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "SPIOperationResponse(byte0=%d byte1=%d byte2=%d byte3=%d byte4=%d byte5=%d byte6=%d byte7=%d byte8=%d byte9=%d byte10=%d byte11=%d byte12=%d byte13=%d byte14=%d byte15=%d byte16=%d byte17=%d byte18=%d byte19=%d byte20=%d byte21=%d byte22=%d byte23=%d length=%d)", s->byte0, s->byte1, s->byte2, s->byte3, s->byte4, s->byte5, s->byte6, s->byte7, s->byte8, s->byte9, s->byte10, s->byte11, s->byte12, s->byte13, s->byte14, s->byte15, s->byte16, s->byte17, s->byte18, s->byte19, s->byte20, s->byte21, s->byte22, s->byte23, s->length);
#else
    n = sprintf(dest, "SPIOperationResponse(byte0=%d byte1=%d byte2=%d byte3=%d byte4=%d byte5=%d byte6=%d byte7=%d byte8=%d byte9=%d byte10=%d byte11=%d byte12=%d byte13=%d byte14=%d byte15=%d byte16=%d byte17=%d byte18=%d byte19=%d byte20=%d byte21=%d byte22=%d byte23=%d length=%d)", s->byte0, s->byte1, s->byte2, s->byte3, s->byte4, s->byte5, s->byte6, s->byte7, s->byte8, s->byte9, s->byte10, s->byte11, s->byte12, s->byte13, s->byte14, s->byte15, s->byte16, s->byte17, s->byte18, s->byte19, s->byte20, s->byte21, s->byte22, s->byte23, s->length);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printSPIOperationResponse(FILE* fp, const struct freespace_SPIOperationResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printSPIOperationResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printEventReportConfigurationResponseStr(char* dest, int maxlen, const struct freespace_EventReportConfigurationResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "EventReportConfigurationResponse(intc=%d xyMov=%d acSt=%d reset=%d motDr=%d wom=%d motOv=%d acEv=%d sdaDr=%d sdaOv=%d cfgSt=%d)", s->intc, s->xyMov, s->acSt, s->reset, s->motDr, s->wom, s->motOv, s->acEv, s->sdaDr, s->sdaOv, s->cfgSt);
#else
    n = sprintf(dest, "EventReportConfigurationResponse(intc=%d xyMov=%d acSt=%d reset=%d motDr=%d wom=%d motOv=%d acEv=%d sdaDr=%d sdaOv=%d cfgSt=%d)", s->intc, s->xyMov, s->acSt, s->reset, s->motDr, s->wom, s->motOv, s->acEv, s->sdaDr, s->sdaOv, s->cfgSt);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printEventReportConfigurationResponse(FILE* fp, const struct freespace_EventReportConfigurationResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printEventReportConfigurationResponseStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printEventReportStr(char* dest, int maxlen, const struct freespace_EventReport* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "EventReport(register0=%d register1=%d register2=%d register3=%d register4=%d register5=%d register6=%d register7=%d)", s->register0, s->register1, s->register2, s->register3, s->register4, s->register5, s->register6, s->register7);
#else
    n = sprintf(dest, "EventReport(register0=%d register1=%d register2=%d register3=%d register4=%d register5=%d register6=%d register7=%d)", s->register0, s->register1, s->register2, s->register3, s->register4, s->register5, s->register6, s->register7);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printEventReport(FILE* fp, const struct freespace_EventReport* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printEventReportStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printUnknownCRSNotificationResponseStr(char* dest, int maxlen, const struct freespace_UnknownCRSNotificationResponse* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "UnknownCRSNotificationResponse(unknownReportID=%d unknownSubMessageID=%d)", s->unknownReportID, s->unknownSubMessageID);
#else
    n = sprintf(dest, "UnknownCRSNotificationResponse(unknownReportID=%d unknownSubMessageID=%d)", s->unknownReportID, s->unknownSubMessageID);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printUnknownCRSNotificationResponse(FILE* fp, const struct freespace_UnknownCRSNotificationResponse* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printUnknownCRSNotificationResponseStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "FRSLoopReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
#else
    n = sprintf(dest, "FRSLoopReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
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
    n = sprintf_s(dest, maxlen, "FRSDongleReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
#else
    n = sprintf(dest, "FRSDongleReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
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
    n = sprintf_s(dest, maxlen, "FRSEFlashReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
#else
    n = sprintf(dest, "FRSEFlashReadResponse(wordOffset=%d status=%d dataLength=%d)", s->wordOffset, s->status, s->dataLength);
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


LIBFREESPACE_API int freespace_printFSP2CoprocessorMessageStr(char* dest, int maxlen, const struct freespace_FSP2CoprocessorMessage* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "FSP2CoprocessorMessage(length=%d)", s->length);
#else
    n = sprintf(dest, "FSP2CoprocessorMessage(length=%d)", s->length);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printFSP2CoprocessorMessage(FILE* fp, const struct freespace_FSP2CoprocessorMessage* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printFSP2CoprocessorMessageStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printLoopBootloaderStatusStr(char* dest, int maxlen, const struct freespace_LoopBootloaderStatus* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "LoopBootloaderStatus(normalLaunch=%d internalAppValid=%d internalAppNotValid=%d flashProtectionChanged=%d upgradeStarted=%d validateStarted=%d errorOccured=%d errorCode=%d)", s->normalLaunch, s->internalAppValid, s->internalAppNotValid, s->flashProtectionChanged, s->upgradeStarted, s->validateStarted, s->errorOccured, s->errorCode);
#else
    n = sprintf(dest, "LoopBootloaderStatus(normalLaunch=%d internalAppValid=%d internalAppNotValid=%d flashProtectionChanged=%d upgradeStarted=%d validateStarted=%d errorOccured=%d errorCode=%d)", s->normalLaunch, s->internalAppValid, s->internalAppNotValid, s->flashProtectionChanged, s->upgradeStarted, s->validateStarted, s->errorOccured, s->errorCode);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printLoopBootloaderStatus(FILE* fp, const struct freespace_LoopBootloaderStatus* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printLoopBootloaderStatusStr(str, sizeof(str), s);
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
    n = sprintf_s(dest, maxlen, "BodyFrame(button1=%d button2=%d button3=%d button4=%d button5=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearAccelX=%d linearAccelY=%d linearAccelZ=%d angularVelX=%d angularVelY=%d angularVelZ=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearAccelX, s->linearAccelY, s->linearAccelZ, s->angularVelX, s->angularVelY, s->angularVelZ);
#else
    n = sprintf(dest, "BodyFrame(button1=%d button2=%d button3=%d button4=%d button5=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearAccelX=%d linearAccelY=%d linearAccelZ=%d angularVelX=%d angularVelY=%d angularVelZ=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearAccelX, s->linearAccelY, s->linearAccelZ, s->angularVelX, s->angularVelY, s->angularVelZ);
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
    n = sprintf_s(dest, maxlen, "UserFrame(button1=%d button2=%d button3=%d button4=%d button5=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearPosX=%d linearPosY=%d linearPosZ=%d angularPosA=%d angularPosB=%d angularPosC=%d angularPosD=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearPosX, s->linearPosY, s->linearPosZ, s->angularPosA, s->angularPosB, s->angularPosC, s->angularPosD);
#else
    n = sprintf(dest, "UserFrame(button1=%d button2=%d button3=%d button4=%d button5=%d deltaX=%d deltaY=%d deltaWheel=%d sequenceNumber=%d linearPosX=%d linearPosY=%d linearPosZ=%d angularPosA=%d angularPosB=%d angularPosC=%d angularPosD=%d)", s->button1, s->button2, s->button3, s->button4, s->button5, s->deltaX, s->deltaY, s->deltaWheel, s->sequenceNumber, s->linearPosX, s->linearPosY, s->linearPosZ, s->angularPosA, s->angularPosB, s->angularPosC, s->angularPosD);
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


LIBFREESPACE_API int freespace_printButtonStateStr(char* dest, int maxlen, const struct freespace_ButtonState* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ButtonState(leftButton=%d rightButton=%d scrollButton=%d park=%d)", s->leftButton, s->rightButton, s->scrollButton, s->park);
#else
    n = sprintf(dest, "ButtonState(leftButton=%d rightButton=%d scrollButton=%d park=%d)", s->leftButton, s->rightButton, s->scrollButton, s->park);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printButtonState(FILE* fp, const struct freespace_ButtonState* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printButtonStateStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}


LIBFREESPACE_API int freespace_printScrollMotionStr(char* dest, int maxlen, const struct freespace_ScrollMotion* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "ScrollMotion(deltaWheel=%d)", s->deltaWheel);
#else
    n = sprintf(dest, "ScrollMotion(deltaWheel=%d)", s->deltaWheel);
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}

LIBFREESPACE_API int freespace_printScrollMotion(FILE* fp, const struct freespace_ScrollMotion* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_printScrollMotionStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%s\n", str);
}

