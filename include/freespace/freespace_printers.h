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

#ifndef FREESPACE_PRINTERS_H_
#define FREESPACE_PRINTERS_H_

#include "freespace/freespace.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "freespace_codecs.h"
#include <stdio.h>


/**
 * Pretty print a Freespace message to the terminal.
 *
 * @param fp the file pointer to print into
 * @param message the HID message
 * @param length the length of the message
 */
LIBFREESPACE_API void freespace_printMessage(FILE* fp, const uint8_t* message, int length);

LIBFREESPACE_API void freespace_printMessageStruct(FILE* fp, struct freespace_message* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printMouseMovementStr(char* dest, int maxlen, const struct freespace_MouseMovement* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printMouseMovement(FILE* fp, const struct freespace_MouseMovement* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printConsumerControlStr(char* dest, int maxlen, const struct freespace_ConsumerControl* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printConsumerControl(FILE* fp, const struct freespace_ConsumerControl* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printKeyboardReportStr(char* dest, int maxlen, const struct freespace_KeyboardReport* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printKeyboardReport(FILE* fp, const struct freespace_KeyboardReport* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printCoprocessorOutReportStr(char* dest, int maxlen, const struct freespace_CoprocessorOutReport* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printCoprocessorOutReport(FILE* fp, const struct freespace_CoprocessorOutReport* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printCoprocessorInReportStr(char* dest, int maxlen, const struct freespace_CoprocessorInReport* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printCoprocessorInReport(FILE* fp, const struct freespace_CoprocessorInReport* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printConfigurationMessageStr(char* dest, int maxlen, const struct freespace_ConfigurationMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printConfigurationMessage(FILE* fp, const struct freespace_ConfigurationMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFactoryCalibrationReadRequestStr(char* dest, int maxlen, const struct freespace_FactoryCalibrationReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFactoryCalibrationReadRequest(FILE* fp, const struct freespace_FactoryCalibrationReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleResetStr(char* dest, int maxlen, const struct freespace_DongleReset* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleReset(FILE* fp, const struct freespace_DongleReset* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFTDongleStatusRequestStr(char* dest, int maxlen, const struct freespace_FTDongleStatusRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFTDongleStatusRequest(FILE* fp, const struct freespace_FTDongleStatusRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printZebraSystemTestStr(char* dest, int maxlen, const struct freespace_ZebraSystemTest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printZebraSystemTest(FILE* fp, const struct freespace_ZebraSystemTest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopLEDSetRequestStr(char* dest, int maxlen, const struct freespace_LoopLEDSetRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopLEDSetRequest(FILE* fp, const struct freespace_LoopLEDSetRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFrequencyFixRequestStr(char* dest, int maxlen, const struct freespace_FrequencyFixRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFrequencyFixRequest(FILE* fp, const struct freespace_FrequencyFixRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSoftwareResetMessageStr(char* dest, int maxlen, const struct freespace_SoftwareResetMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSoftwareResetMessage(FILE* fp, const struct freespace_SoftwareResetMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessageStr(char* dest, int maxlen, const struct freespace_DongleRFSupressHomeFrequencyMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessage(FILE* fp, const struct freespace_DongleRFSupressHomeFrequencyMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSPIOperationMessageStr(char* dest, int maxlen, const struct freespace_SPIOperationMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSPIOperationMessage(FILE* fp, const struct freespace_SPIOperationMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReportConfigSetRequestStr(char* dest, int maxlen, const struct freespace_EventReportConfigSetRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReportConfigSetRequest(FILE* fp, const struct freespace_EventReportConfigSetRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUnknownCRSNotificationStr(char* dest, int maxlen, const struct freespace_UnknownCRSNotification* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUnknownCRSNotification(FILE* fp, const struct freespace_UnknownCRSNotification* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadRequest(FILE* fp, const struct freespace_FRSLoopReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteRequest(FILE* fp, const struct freespace_FRSLoopWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteDataStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteData(FILE* fp, const struct freespace_FRSLoopWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadRequest(FILE* fp, const struct freespace_FRSDongleReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteRequest(FILE* fp, const struct freespace_FRSDongleWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteDataStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteData(FILE* fp, const struct freespace_FRSDongleWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadRequest(FILE* fp, const struct freespace_FRSEFlashReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteRequest(FILE* fp, const struct freespace_FRSEFlashWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteDataStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteData(FILE* fp, const struct freespace_FRSEFlashWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopBootloaderCommandStr(char* dest, int maxlen, const struct freespace_LoopBootloaderCommand* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopBootloaderCommand(FILE* fp, const struct freespace_LoopBootloaderCommand* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printGen4SDAFormatStr(char* dest, int maxlen, const struct freespace_Gen4SDAFormat* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printGen4SDAFormat(FILE* fp, const struct freespace_Gen4SDAFormat* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFactoryCalibrationReadDataStr(char* dest, int maxlen, const struct freespace_FactoryCalibrationReadData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFactoryCalibrationReadData(FILE* fp, const struct freespace_FactoryCalibrationReadData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFTDongleStatusResponseStr(char* dest, int maxlen, const struct freespace_FTDongleStatusResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFTDongleStatusResponse(FILE* fp, const struct freespace_FTDongleStatusResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printStatisticsResponseStr(char* dest, int maxlen, const struct freespace_StatisticsResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printStatisticsResponse(FILE* fp, const struct freespace_StatisticsResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printZebraSystemTestResponseStr(char* dest, int maxlen, const struct freespace_ZebraSystemTestResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printZebraSystemTestResponse(FILE* fp, const struct freespace_ZebraSystemTestResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDResponseStr(char* dest, int maxlen, const struct freespace_ProductIDResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDResponse(FILE* fp, const struct freespace_ProductIDResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkStatusStr(char* dest, int maxlen, const struct freespace_LinkStatus* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkStatus(FILE* fp, const struct freespace_LinkStatus* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSPIOperationResponseStr(char* dest, int maxlen, const struct freespace_SPIOperationResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSPIOperationResponse(FILE* fp, const struct freespace_SPIOperationResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReportConfigurationResponseStr(char* dest, int maxlen, const struct freespace_EventReportConfigurationResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReportConfigurationResponse(FILE* fp, const struct freespace_EventReportConfigurationResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReportStr(char* dest, int maxlen, const struct freespace_EventReport* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printEventReport(FILE* fp, const struct freespace_EventReport* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUnknownCRSNotificationResponseStr(char* dest, int maxlen, const struct freespace_UnknownCRSNotificationResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUnknownCRSNotificationResponse(FILE* fp, const struct freespace_UnknownCRSNotificationResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadResponse(FILE* fp, const struct freespace_FRSLoopReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteResponse(FILE* fp, const struct freespace_FRSLoopWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadResponse(FILE* fp, const struct freespace_FRSDongleReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteResponse(FILE* fp, const struct freespace_FRSDongleWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadResponse(FILE* fp, const struct freespace_FRSEFlashReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteResponse(FILE* fp, const struct freespace_FRSEFlashWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFSP2CoprocessorMessageStr(char* dest, int maxlen, const struct freespace_FSP2CoprocessorMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFSP2CoprocessorMessage(FILE* fp, const struct freespace_FSP2CoprocessorMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopBootloaderStatusStr(char* dest, int maxlen, const struct freespace_LoopBootloaderStatus* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLoopBootloaderStatus(FILE* fp, const struct freespace_LoopBootloaderStatus* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevelStr(char* dest, int maxlen, const struct freespace_BatteryLevel* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevel(FILE* fp, const struct freespace_BatteryLevel* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyFrameStr(char* dest, int maxlen, const struct freespace_BodyFrame* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyFrame(FILE* fp, const struct freespace_BodyFrame* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUserFrameStr(char* dest, int maxlen, const struct freespace_UserFrame* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUserFrame(FILE* fp, const struct freespace_UserFrame* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataMotionControlStr(char* dest, int maxlen, const struct freespace_DataMotionControl* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataMotionControl(FILE* fp, const struct freespace_DataMotionControl* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonStateStr(char* dest, int maxlen, const struct freespace_ButtonState* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonState(FILE* fp, const struct freespace_ButtonState* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printScrollMotionStr(char* dest, int maxlen, const struct freespace_ScrollMotion* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printScrollMotion(FILE* fp, const struct freespace_ScrollMotion* s);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_PRINTERS_H_ */
