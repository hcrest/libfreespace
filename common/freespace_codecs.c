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

#include "freespace/freespace_codecs.h"
#include <string.h>
#include <stdio.h>

#ifdef WIN32
#define STRICT_DECODE_LENGTH 0
#else
#define STRICT_DECODE_LENGTH -1
#endif


static uint32_t toUInt32(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return (((uint32_t) ((uint8_t) d)) << 24) | (((uint32_t) ((uint8_t) c)) << 16) | (((uint32_t) ((uint8_t) b)) << 8) | (uint32_t) a;
#else
    return (((uint32_t) ((uint8_t) a)) << 24) | (((uint32_t) ((uint8_t) b)) << 16) | (((uint32_t) ((uint8_t) c)) << 8) | (uint32_t) d;
#endif
}

static int16_t toInt(uint8_t a, uint8_t b) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return (((int16_t) b) << 8) | a;
#else
    return (((int16_t) a) << 8) | b;
#endif
}

static uint8_t getBit(uint8_t a, uint16_t whichBit) {
    return (uint8_t) (a >> whichBit) & 0x01;
}

static uint8_t getNibble(uint8_t a, uint16_t whichNibble) {
    return (uint8_t) (a >> (whichNibble*4)) & 0x0F;
}

static uint8_t byteFromBits(uint8_t lsb, 
                            uint8_t b,
                            uint8_t c,
                            uint8_t d,
                            uint8_t e,
                            uint8_t f,
                            uint8_t g,
                            uint8_t msb) {
    return (uint8_t) (((msb & 0x01) << 7) | ((g & 0x01) << 6) | ((f & 0x01) << 5) | ((e & 0x01) << 4) | ((d & 0x01) << 3) | ((c & 0x01) << 2) | ((b & 0x01) << 1) | (lsb & 0x01));
}


LIBFREESPACE_API int freespace_decodeMouseMovement(const int8_t* message, int length, struct freespace_MouseMovement* s) {
    if ((STRICT_DECODE_LENGTH && length != 5) || (!STRICT_DECODE_LENGTH && length < 5)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 2) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->button1 = getBit(message[1], 0);
    s->button2 = getBit(message[1], 1);
    s->button3 = getBit(message[1], 2);
    s->button4 = getBit(message[1], 3);
    s->button5 = getBit(message[1], 4);
    s->button6 = getBit(message[1], 5);
    s->button7 = getBit(message[1], 6);
    s->button8 = getBit(message[1], 7);
    s->deltaX = (int16_t) message[2];
    s->deltaY = (int16_t) message[3];
    s->deltaWheel = (int16_t) message[4];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeConsumerControl(const int8_t* message, int length, struct freespace_ConsumerControl* s) {
    if ((STRICT_DECODE_LENGTH && length != 4) || (!STRICT_DECODE_LENGTH && length < 4)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 3) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->usageID = (int16_t) message[1];
    s->usageID11_8 = getNibble(message[2], 0);
    s->numPad = getNibble(message[2], 1);
    s->functionButtons = (uint8_t) message[3];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeKeyboardReport(const int8_t* message, int length, struct freespace_KeyboardReport* s) {
    if ((STRICT_DECODE_LENGTH && length != 4) || (!STRICT_DECODE_LENGTH && length < 4)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 4) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->LCtrl = getBit(message[1], 0);
    s->LShift = getBit(message[1], 1);
    s->LAlt = getBit(message[1], 2);
    s->LGui = getBit(message[1], 3);
    s->RCtrl = getBit(message[1], 4);
    s->RShift = getBit(message[1], 5);
    s->RAlt = getBit(message[1], 6);
    s->RGui = getBit(message[1], 7);
    s->keyID = (int16_t) message[3];
	return FREESPACE_SUCCESS;
}

LIBFREESPACE_API int freespace_encodeCoprocessorOutReport(const struct freespace_CoprocessorOutReport* s, int8_t* message, int maxlength) {
	if (maxlength < 16) {
		printf("freespace_CoprocessorOutReport encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 5;
	message[1] = (int8_t) s->payloadLength;
	memcpy(message + 2, s->payload, 14);
	return 16;
}


LIBFREESPACE_API int freespace_decodeCoprocessorInReport(const int8_t* message, int length, struct freespace_CoprocessorInReport* s) {
    if ((STRICT_DECODE_LENGTH && length != 16) || (!STRICT_DECODE_LENGTH && length < 16)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 6) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->payloadLength = (int16_t) message[1];
	memcpy(s->payload, &message[2], 14);
	return FREESPACE_SUCCESS;
}

LIBFREESPACE_API int freespace_encodeConfigurationMessage(const struct freespace_ConfigurationMessage* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_ConfigurationMessage encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 1;
	message[2] = (int8_t) s->SDA;
	return 8;
}


LIBFREESPACE_API int freespace_encodePairingMessage(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_PairingMessage encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 13;
    return 8;
}


LIBFREESPACE_API int freespace_encodeDebugMessage(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_DebugMessage encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 15;
    return 8;
}

LIBFREESPACE_API int freespace_encodeFactoryCalibrationReadRequest(const struct freespace_FactoryCalibrationReadRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FactoryCalibrationReadRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 16;
	message[2] = (int8_t) s->wordOffsetRequested;
	return 8;
}

LIBFREESPACE_API int freespace_encodeDongleReset(const struct freespace_DongleReset* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_DongleReset encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 20;
	message[2] = (int8_t) (s->loaderKey >> 0);
	message[3] = (int8_t) (s->loaderKey >> 8);
	message[4] = (int8_t) (s->loaderKey >> 16);
	message[5] = (int8_t) (s->loaderKey >> 24);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFTDongleStatusRequest(const struct freespace_FTDongleStatusRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FTDongleStatusRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 21;
	message[2] = byteFromBits(s->power, s->reset, s->presence, 0, 0, 0, 0, s->operation);
	return 8;
}


LIBFREESPACE_API int freespace_encodeStatisticsRequest(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_StatisticsRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 22;
    return 8;
}

LIBFREESPACE_API int freespace_encodeZebraSystemTest(const struct freespace_ZebraSystemTest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_ZebraSystemTest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 31;
	message[2] = byteFromBits(s->reset, s->mode, s->mode >> 1, 0, 0, 0, 0, 0);
	message[4] = (int8_t) (s->pcSequence >> 0);
	message[5] = (int8_t) (s->pcSequence >> 8);
	return 8;
}


LIBFREESPACE_API int freespace_encodeProductIDRequest(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_ProductIDRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 32;
    return 8;
}

LIBFREESPACE_API int freespace_encodeLoopLEDSetRequest(const struct freespace_LoopLEDSetRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_LoopLEDSetRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 34;
	message[2] = (int8_t) s->onOff;
	message[3] = (int8_t) s->selectLED;
	return 8;
}


LIBFREESPACE_API int freespace_encodeAlwaysOnRequest(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_AlwaysOnRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 49;
    return 8;
}

LIBFREESPACE_API int freespace_encodeFrequencyFixRequest(const struct freespace_FrequencyFixRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FrequencyFixRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 50;
	message[2] = (int8_t) s->channel0;
	message[3] = (int8_t) s->channel1;
	message[4] = (int8_t) s->channel2;
	message[5] = (int8_t) s->channel3;
	message[6] = (int8_t) s->channel4;
	message[7] = (int8_t) s->device;
	return 8;
}

LIBFREESPACE_API int freespace_encodeSoftwareResetMessage(const struct freespace_SoftwareResetMessage* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_SoftwareResetMessage encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 51;
	message[2] = (int8_t) s->device;
	return 8;
}


LIBFREESPACE_API int freespace_encodeDongleRFDisableMessage(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_DongleRFDisableMessage encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 52;
    return 8;
}

LIBFREESPACE_API int freespace_encodeDongleRFSupressHomeFrequencyMessage(const struct freespace_DongleRFSupressHomeFrequencyMessage* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_DongleRFSupressHomeFrequencyMessage encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 53;
	message[2] = (int8_t) s->low;
	message[3] = (int8_t) s->high;
	return 8;
}

LIBFREESPACE_API int freespace_encodeSPIOperationMessage(const struct freespace_SPIOperationMessage* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_SPIOperationMessage encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 54;
	message[2] = (int8_t) s->byte0;
	message[3] = (int8_t) s->byte1;
	message[4] = (int8_t) s->byte2;
	message[5] = (int8_t) s->byte3;
	message[6] = (int8_t) s->byte4;
	message[7] = (int8_t) s->byte5;
	return 8;
}

LIBFREESPACE_API int freespace_encodeEventReportConfigSetRequest(const struct freespace_EventReportConfigSetRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_EventReportConfigSetRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 55;
	message[2] = byteFromBits(s->intc, s->xyMov, s->acSt, s->reset, s->motDr, s->wom, s->motOv, s->acEv);
	message[3] = byteFromBits(s->sdaDr, s->sdaOv, s->cfgSt, 0, 0, 0, 0, 0);
	return 8;
}


LIBFREESPACE_API int freespace_encodeEventReportConfigGetRequest(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_EventReportConfigGetRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 56;
    return 8;
}

LIBFREESPACE_API int freespace_encodeUnknownCRSNotification(const struct freespace_UnknownCRSNotification* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_UnknownCRSNotification encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 57;
	message[2] = (int8_t) s->unknownReportID;
	message[3] = (int8_t) s->unknownSubMessageID;
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSLoopReadRequest(const struct freespace_FRSLoopReadRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSLoopReadRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 58;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSLoopWriteRequest(const struct freespace_FRSLoopWriteRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSLoopWriteRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 61;
	message[2] = (int8_t) (s->length >> 0);
	message[3] = (int8_t) (s->length >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSLoopWriteData(const struct freespace_FRSLoopWriteData* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSLoopWriteData encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 63;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	memcpy(message + 4, s->data, 4);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSDongleReadRequest(const struct freespace_FRSDongleReadRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSDongleReadRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 59;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSDongleWriteRequest(const struct freespace_FRSDongleWriteRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSDongleWriteRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 62;
	message[2] = (int8_t) (s->length >> 0);
	message[3] = (int8_t) (s->length >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSDongleWriteData(const struct freespace_FRSDongleWriteData* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSDongleWriteData encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 64;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	memcpy(message + 4, s->data, 4);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSEFlashReadRequest(const struct freespace_FRSEFlashReadRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSEFlashReadRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 65;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSEFlashWriteRequest(const struct freespace_FRSEFlashWriteRequest* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSEFlashWriteRequest encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 66;
	message[2] = (int8_t) (s->length >> 0);
	message[3] = (int8_t) (s->length >> 8);
	message[4] = (int8_t) (s->FRStype >> 0);
	message[5] = (int8_t) (s->FRStype >> 8);
	return 8;
}

LIBFREESPACE_API int freespace_encodeFRSEFlashWriteData(const struct freespace_FRSEFlashWriteData* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_FRSEFlashWriteData encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 67;
	message[2] = (int8_t) (s->wordOffset >> 0);
	message[3] = (int8_t) (s->wordOffset >> 8);
	memcpy(message + 4, s->data, 4);
	return 8;
}

LIBFREESPACE_API int freespace_encodeLoopBootloaderCommand(const struct freespace_LoopBootloaderCommand* s, int8_t* message, int maxlength) {
	if (maxlength < 8) {
		printf("freespace_LoopBootloaderCommand encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 7;
	message[1] = (int8_t) 68;
	message[2] = (int8_t) (s->loaderKeyCommand >> 0);
	message[3] = (int8_t) (s->loaderKeyCommand >> 8);
	message[4] = (int8_t) (s->loaderKeyCommand >> 16);
	message[5] = (int8_t) (s->loaderKeyCommand >> 24);
	return 8;
}


LIBFREESPACE_API int freespace_encodeLoopBootloaderStatusRequest(int8_t* message, int maxlength) {
    if (maxlength < 8) {
        printf("freespace_LoopBootloaderStatusRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 7;
    message[1] = (int8_t) 69;
    return 8;
}


LIBFREESPACE_API int freespace_decodeGen4SDAFormat(const int8_t* message, int length, struct freespace_Gen4SDAFormat* s) {
    if ((STRICT_DECODE_LENGTH && length != 25) || (!STRICT_DECODE_LENGTH && length < 25)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 1) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->reportCount = (int16_t) message[2];
    s->left = getBit(message[3], 0);
    s->right = getBit(message[3], 1);
    s->scroll = getBit(message[3], 2);
    s->hold = getBit(message[3], 3);
    s->triangle = getBit(message[3], 6);
    s->deltaWheel = (int16_t) message[4];
	s->accelX = toInt(message[5], message[6]);
	s->accelY = toInt(message[7], message[8]);
	s->accelZ = toInt(message[9], message[10]);
	s->rotationX = toInt(message[11], message[12]);
	s->rotationY = toInt(message[13], message[14]);
	s->rotationZ = toInt(message[15], message[16]);
    s->xMov = (int16_t) message[17];
    s->yMov = (int16_t) message[18];
	s->sampleBase = toUInt32(message[19], message[20], message[21], message[22]);
	s->command = toInt(message[23], message[24]);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFactoryCalibrationReadData(const int8_t* message, int length, struct freespace_FactoryCalibrationReadData* s) {
    if ((STRICT_DECODE_LENGTH && length != 47) || (!STRICT_DECODE_LENGTH && length < 47)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 16) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->wordOffset = (int16_t) message[2];
    s->dataLength = getNibble(message[3], 0);
	memcpy(s->factCalData, &message[4], 20);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFTDongleStatusResponse(const int8_t* message, int length, struct freespace_FTDongleStatusResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 21) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->power = getBit(message[2], 0);
    s->reset = getBit(message[2], 1);
    s->presence = getBit(message[2], 2);
    s->status = (uint8_t) message[3];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeStatisticsResponse(const int8_t* message, int length, struct freespace_StatisticsResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 22) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->stackSpace = toInt(message[2], message[3]);
	s->runTime = toInt(message[4], message[5]);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeZebraSystemTestResponse(const int8_t* message, int length, struct freespace_ZebraSystemTestResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 31) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->reset = getBit(message[2], 0);
    s->mode = (uint8_t) ((message[2] >> 1) & 0x03);
	s->pcSequence = toInt(message[4], message[5]);
	s->dongleOutSequence = toInt(message[6], message[7]);
	s->rfSequence = toInt(message[8], message[9]);
	s->dongleInSequence = toInt(message[10], message[11]);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeProductIDResponse(const int8_t* message, int length, struct freespace_ProductIDResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 32) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->swPartNumber = toUInt32(message[2], message[3], message[4], message[5]);
	s->swBuildNumber = toUInt32(message[6], message[7], message[8], message[9]);
	s->swicn = toUInt32(message[10], message[11], message[12], message[13]);
	s->swVersionPatch = toInt(message[14], message[15]);
    s->swVersionMinor = (int16_t) message[16];
    s->swVersionMajor = (int16_t) message[17];
    s->hwPlatformID = (int16_t) message[18];
    s->hwRevision = (int16_t) message[19];
	s->serialNumber = toUInt32(message[20], message[21], message[22], message[23]);
    s->deviceClass = (uint8_t) ((message[24] >> 0) & 0x7F);
    s->invalidNS = getBit(message[24], 7);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeLinkStatus(const int8_t* message, int length, struct freespace_LinkStatus* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 48) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->status = (uint8_t) message[2];
    s->mode = (uint8_t) message[3];
    s->resetStatus = (uint8_t) message[4];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeSPIOperationResponse(const int8_t* message, int length, struct freespace_SPIOperationResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 54) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->byte0 = (uint8_t) message[2];
    s->byte1 = (uint8_t) message[3];
    s->byte2 = (uint8_t) message[4];
    s->byte3 = (uint8_t) message[5];
    s->byte4 = (uint8_t) message[6];
    s->byte5 = (uint8_t) message[7];
    s->byte6 = (uint8_t) message[8];
    s->byte7 = (uint8_t) message[9];
    s->byte8 = (uint8_t) message[10];
    s->byte9 = (uint8_t) message[11];
    s->byte10 = (uint8_t) message[12];
    s->byte11 = (uint8_t) message[13];
    s->byte12 = (uint8_t) message[14];
    s->byte13 = (uint8_t) message[15];
    s->byte14 = (uint8_t) message[16];
    s->byte15 = (uint8_t) message[17];
    s->byte16 = (uint8_t) message[18];
    s->byte17 = (uint8_t) message[19];
    s->byte18 = (uint8_t) message[20];
    s->byte19 = (uint8_t) message[21];
    s->byte20 = (uint8_t) message[22];
    s->byte21 = (uint8_t) message[23];
    s->byte22 = (uint8_t) message[24];
    s->byte23 = (uint8_t) message[25];
    s->length = (int16_t) message[26];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeEventReportConfigurationResponse(const int8_t* message, int length, struct freespace_EventReportConfigurationResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 55) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->intc = getBit(message[2], 0);
    s->xyMov = getBit(message[2], 1);
    s->acSt = getBit(message[2], 2);
    s->reset = getBit(message[2], 3);
    s->motDr = getBit(message[2], 4);
    s->wom = getBit(message[2], 5);
    s->motOv = getBit(message[2], 6);
    s->acEv = getBit(message[2], 7);
    s->sdaDr = getBit(message[3], 0);
    s->sdaOv = getBit(message[3], 1);
    s->cfgSt = getBit(message[3], 2);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeEventReport(const int8_t* message, int length, struct freespace_EventReport* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 56) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->register0 = toInt(message[2], message[3]);
	s->register1 = toInt(message[4], message[5]);
	s->register2 = toInt(message[6], message[7]);
	s->register3 = toInt(message[8], message[9]);
	s->register4 = toInt(message[10], message[11]);
	s->register5 = toInt(message[12], message[13]);
	s->register6 = toInt(message[14], message[15]);
	s->register7 = toInt(message[16], message[17]);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeUnknownCRSNotificationResponse(const int8_t* message, int length, struct freespace_UnknownCRSNotificationResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 57) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->unknownReportID = (int16_t) message[2];
    s->unknownSubMessageID = (int16_t) message[3];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSLoopReadResponse(const int8_t* message, int length, struct freespace_FRSLoopReadResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 26) || (!STRICT_DECODE_LENGTH && length < 26)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 58) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
	memcpy(s->data, &message[4], 19);
    s->status = getNibble(message[23], 0);
    s->dataLength = getNibble(message[23], 1);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSLoopWriteResponse(const int8_t* message, int length, struct freespace_FRSLoopWriteResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 61) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
    s->status = (uint8_t) message[4];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSDongleReadResponse(const int8_t* message, int length, struct freespace_FRSDongleReadResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 26) || (!STRICT_DECODE_LENGTH && length < 26)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 59) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
	memcpy(s->data, &message[4], 19);
    s->status = getNibble(message[23], 0);
    s->dataLength = getNibble(message[23], 1);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSDongleWriteResponse(const int8_t* message, int length, struct freespace_FRSDongleWriteResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 62) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
    s->status = (uint8_t) message[4];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSEFlashReadResponse(const int8_t* message, int length, struct freespace_FRSEFlashReadResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 26) || (!STRICT_DECODE_LENGTH && length < 26)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 65) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
	memcpy(s->data, &message[4], 19);
    s->status = getNibble(message[23], 0);
    s->dataLength = getNibble(message[23], 1);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFRSEFlashWriteResponse(const int8_t* message, int length, struct freespace_FRSEFlashWriteResponse* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 66) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
	s->wordOffset = toInt(message[2], message[3]);
    s->status = (uint8_t) message[4];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeFSP2CoprocessorMessage(const int8_t* message, int length, struct freespace_FSP2CoprocessorMessage* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 60) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->length = (uint8_t) message[2];
	memcpy(s->payload, &message[3], 16);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeLoopBootloaderStatus(const int8_t* message, int length, struct freespace_LoopBootloaderStatus* s) {
    if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 8) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }

    if ((uint8_t) message[1] != 67) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->normalLaunch = getBit(message[4], 0);
    s->internalAppValid = getBit(message[4], 1);
    s->internalAppNotValid = getBit(message[4], 2);
    s->flashProtectionChanged = getBit(message[4], 3);
    s->upgradeStarted = getBit(message[4], 4);
    s->validateStarted = getBit(message[4], 5);
    s->errorOccured = getBit(message[4], 6);
    s->errorCode = (uint8_t) message[8];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_encodeBatteryLevelRequest(int8_t* message, int maxlength) {
    if (maxlength < 2) {
        printf("freespace_BatteryLevelRequest encode(<INVALID LENGTH>)\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (int8_t) 9;
    return 2;
}


LIBFREESPACE_API int freespace_decodeBatteryLevel(const int8_t* message, int length, struct freespace_BatteryLevel* s) {
    if ((STRICT_DECODE_LENGTH && length != 4) || (!STRICT_DECODE_LENGTH && length < 4)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 10) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->batteryStrength = (int16_t) message[1];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeBodyFrame(const int8_t* message, int length, struct freespace_BodyFrame* s) {
    if ((STRICT_DECODE_LENGTH && length != 21) || (!STRICT_DECODE_LENGTH && length < 21)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 32) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->button1 = getBit(message[1], 0);
    s->button2 = getBit(message[1], 1);
    s->button3 = getBit(message[1], 2);
    s->button4 = getBit(message[1], 3);
    s->button5 = getBit(message[1], 4);
    s->deltaX = (int16_t) message[2];
    s->deltaY = (int16_t) message[3];
    s->deltaWheel = (int16_t) message[4];
	s->sequenceNumber = toUInt32(message[5], message[6], message[7], message[8]);
	s->linearAccelX = toInt(message[9], message[10]);
	s->linearAccelY = toInt(message[11], message[12]);
	s->linearAccelZ = toInt(message[13], message[14]);
	s->angularVelX = toInt(message[15], message[16]);
	s->angularVelY = toInt(message[17], message[18]);
	s->angularVelZ = toInt(message[19], message[20]);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeUserFrame(const int8_t* message, int length, struct freespace_UserFrame* s) {
    if ((STRICT_DECODE_LENGTH && length != 23) || (!STRICT_DECODE_LENGTH && length < 23)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 33) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->button1 = getBit(message[1], 0);
    s->button2 = getBit(message[1], 1);
    s->button3 = getBit(message[1], 2);
    s->button4 = getBit(message[1], 3);
    s->button5 = getBit(message[1], 4);
    s->deltaX = (int16_t) message[2];
    s->deltaY = (int16_t) message[3];
    s->deltaWheel = (int16_t) message[4];
	s->sequenceNumber = toUInt32(message[5], message[6], message[7], message[8]);
	s->linearPosX = toInt(message[9], message[10]);
	s->linearPosY = toInt(message[11], message[12]);
	s->linearPosZ = toInt(message[13], message[14]);
	s->angularPosA = toInt(message[15], message[16]);
	s->angularPosB = toInt(message[17], message[18]);
	s->angularPosC = toInt(message[19], message[20]);
	s->angularPosD = toInt(message[21], message[22]);
	return FREESPACE_SUCCESS;
}

LIBFREESPACE_API int freespace_encodeDataMotionControl(const struct freespace_DataMotionControl* s, int8_t* message, int maxlength) {
	if (maxlength < 2) {
		printf("freespace_DataMotionControl encode(<INVALID LENGTH>)\n");
		return -1;
	}
	message[0] = (int8_t) 34;
	message[1] = byteFromBits(s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace, 0, 0, 0);
	return 2;
}


LIBFREESPACE_API int freespace_decodeButtonState(const int8_t* message, int length, struct freespace_ButtonState* s) {
    if ((STRICT_DECODE_LENGTH && length != 2) || (!STRICT_DECODE_LENGTH && length < 2)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 128) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->leftButton = getBit(message[1], 0);
    s->rightButton = getBit(message[1], 1);
    s->scrollButton = getBit(message[1], 2);
    s->park = getBit(message[1], 3);
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decodeScrollMotion(const int8_t* message, int length, struct freespace_ScrollMotion* s) {
    if ((STRICT_DECODE_LENGTH && length != 2) || (!STRICT_DECODE_LENGTH && length < 2)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != 129) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
    s->deltaWheel = (int16_t) message[1];
	return FREESPACE_SUCCESS;
}


LIBFREESPACE_API int freespace_decode_message(const int8_t* message, int length, struct freespace_message* s) {
    if (length == 0) {
        return -1;
    }

    switch ((uint8_t) message[0]) {
	case 2:
        {
            s->messageType = FREESPACE_MESSAGE_MOUSEMOVEMENT;
            return freespace_decodeMouseMovement(message, length, &(s->mouseMovement));
            
        }
        break;
	case 3:
        {
            s->messageType = FREESPACE_MESSAGE_CONSUMERCONTROL;
            return freespace_decodeConsumerControl(message, length, &(s->consumerControl));
            
        }
        break;
	case 4:
        {
            s->messageType = FREESPACE_MESSAGE_KEYBOARDREPORT;
            return freespace_decodeKeyboardReport(message, length, &(s->keyboardReport));
            
        }
        break;
	case 6:
        {
            s->messageType = FREESPACE_MESSAGE_COPROCESSORINREPORT;
            return freespace_decodeCoprocessorInReport(message, length, &(s->coprocessorInReport));
            
        }
        break;
	case 8:
        switch ((uint8_t) message[1]) {
        case 1:
            {
                s->messageType = FREESPACE_MESSAGE_GEN4SDAFORMAT;
                return freespace_decodeGen4SDAFormat(message, length, &(s->gen4SDAFormat));
            }
            break;
        case 16:
            {
                s->messageType = FREESPACE_MESSAGE_FACTORYCALIBRATIONREADDATA;
                return freespace_decodeFactoryCalibrationReadData(message, length, &(s->factoryCalibrationReadData));
            }
            break;
        case 21:
            {
                s->messageType = FREESPACE_MESSAGE_FTDONGLESTATUSRESPONSE;
                return freespace_decodeFTDongleStatusResponse(message, length, &(s->fTDongleStatusResponse));
            }
            break;
        case 22:
            {
                s->messageType = FREESPACE_MESSAGE_STATISTICSRESPONSE;
                return freespace_decodeStatisticsResponse(message, length, &(s->statisticsResponse));
            }
            break;
        case 31:
            {
                s->messageType = FREESPACE_MESSAGE_ZEBRASYSTEMTESTRESPONSE;
                return freespace_decodeZebraSystemTestResponse(message, length, &(s->zebraSystemTestResponse));
            }
            break;
        case 32:
            {
                s->messageType = FREESPACE_MESSAGE_PRODUCTIDRESPONSE;
                return freespace_decodeProductIDResponse(message, length, &(s->productIDResponse));
            }
            break;
        case 48:
            {
                s->messageType = FREESPACE_MESSAGE_LINKSTATUS;
                return freespace_decodeLinkStatus(message, length, &(s->linkStatus));
            }
            break;
        case 54:
            {
                s->messageType = FREESPACE_MESSAGE_SPIOPERATIONRESPONSE;
                return freespace_decodeSPIOperationResponse(message, length, &(s->sPIOperationResponse));
            }
            break;
        case 55:
            {
                s->messageType = FREESPACE_MESSAGE_EVENTREPORTCONFIGURATIONRESPONSE;
                return freespace_decodeEventReportConfigurationResponse(message, length, &(s->eventReportConfigurationResponse));
            }
            break;
        case 56:
            {
                s->messageType = FREESPACE_MESSAGE_EVENTREPORT;
                return freespace_decodeEventReport(message, length, &(s->eventReport));
            }
            break;
        case 57:
            {
                s->messageType = FREESPACE_MESSAGE_UNKNOWNCRSNOTIFICATIONRESPONSE;
                return freespace_decodeUnknownCRSNotificationResponse(message, length, &(s->unknownCRSNotificationResponse));
            }
            break;
        case 58:
            {
                s->messageType = FREESPACE_MESSAGE_FRSLOOPREADRESPONSE;
                return freespace_decodeFRSLoopReadResponse(message, length, &(s->fRSLoopReadResponse));
            }
            break;
        case 61:
            {
                s->messageType = FREESPACE_MESSAGE_FRSLOOPWRITERESPONSE;
                return freespace_decodeFRSLoopWriteResponse(message, length, &(s->fRSLoopWriteResponse));
            }
            break;
        case 59:
            {
                s->messageType = FREESPACE_MESSAGE_FRSDONGLEREADRESPONSE;
                return freespace_decodeFRSDongleReadResponse(message, length, &(s->fRSDongleReadResponse));
            }
            break;
        case 62:
            {
                s->messageType = FREESPACE_MESSAGE_FRSDONGLEWRITERESPONSE;
                return freespace_decodeFRSDongleWriteResponse(message, length, &(s->fRSDongleWriteResponse));
            }
            break;
        case 65:
            {
                s->messageType = FREESPACE_MESSAGE_FRSEFLASHREADRESPONSE;
                return freespace_decodeFRSEFlashReadResponse(message, length, &(s->fRSEFlashReadResponse));
            }
            break;
        case 66:
            {
                s->messageType = FREESPACE_MESSAGE_FRSEFLASHWRITERESPONSE;
                return freespace_decodeFRSEFlashWriteResponse(message, length, &(s->fRSEFlashWriteResponse));
            }
            break;
        case 60:
            {
                s->messageType = FREESPACE_MESSAGE_FSP2COPROCESSORMESSAGE;
                return freespace_decodeFSP2CoprocessorMessage(message, length, &(s->fSP2CoprocessorMessage));
            }
            break;
        case 67:
            {
                s->messageType = FREESPACE_MESSAGE_LOOPBOOTLOADERSTATUS;
                return freespace_decodeLoopBootloaderStatus(message, length, &(s->loopBootloaderStatus));
            }
            break;
        default:
            return -1;

        }
        break;
	case 10:
        {
            s->messageType = FREESPACE_MESSAGE_BATTERYLEVEL;
            return freespace_decodeBatteryLevel(message, length, &(s->batteryLevel));
            
        }
        break;
	case 32:
        {
            s->messageType = FREESPACE_MESSAGE_BODYFRAME;
            return freespace_decodeBodyFrame(message, length, &(s->bodyFrame));
            
        }
        break;
	case 33:
        {
            s->messageType = FREESPACE_MESSAGE_USERFRAME;
            return freespace_decodeUserFrame(message, length, &(s->userFrame));
            
        }
        break;
	case 128:
        {
            s->messageType = FREESPACE_MESSAGE_BUTTONSTATE;
            return freespace_decodeButtonState(message, length, &(s->buttonState));
            
        }
        break;
	case 129:
        {
            s->messageType = FREESPACE_MESSAGE_SCROLLMOTION;
            return freespace_decodeScrollMotion(message, length, &(s->scrollMotion));
            
        }
        break;

    default:
        return -1;
    }
}

LIBFREESPACE_API int freespace_encode_message(const struct freespace_message* s, int8_t* message, int maxlength) {
    switch (s->messageType) {
    case FREESPACE_MESSAGE_COPROCESSOROUTREPORT:
        return freespace_encodeCoprocessorOutReport(&(s->coprocessorOutReport), message, maxlength);
    case FREESPACE_MESSAGE_CONFIGURATIONMESSAGE:
        return freespace_encodeConfigurationMessage(&(s->configurationMessage), message, maxlength);
    case FREESPACE_MESSAGE_FACTORYCALIBRATIONREADREQUEST:
        return freespace_encodeFactoryCalibrationReadRequest(&(s->factoryCalibrationReadRequest), message, maxlength);
    case FREESPACE_MESSAGE_DONGLERESET:
        return freespace_encodeDongleReset(&(s->dongleReset), message, maxlength);
    case FREESPACE_MESSAGE_FTDONGLESTATUSREQUEST:
        return freespace_encodeFTDongleStatusRequest(&(s->fTDongleStatusRequest), message, maxlength);
    case FREESPACE_MESSAGE_ZEBRASYSTEMTEST:
        return freespace_encodeZebraSystemTest(&(s->zebraSystemTest), message, maxlength);
    case FREESPACE_MESSAGE_LOOPLEDSETREQUEST:
        return freespace_encodeLoopLEDSetRequest(&(s->loopLEDSetRequest), message, maxlength);
    case FREESPACE_MESSAGE_FREQUENCYFIXREQUEST:
        return freespace_encodeFrequencyFixRequest(&(s->frequencyFixRequest), message, maxlength);
    case FREESPACE_MESSAGE_SOFTWARERESETMESSAGE:
        return freespace_encodeSoftwareResetMessage(&(s->softwareResetMessage), message, maxlength);
    case FREESPACE_MESSAGE_DONGLERFSUPRESSHOMEFREQUENCYMESSAGE:
        return freespace_encodeDongleRFSupressHomeFrequencyMessage(&(s->dongleRFSupressHomeFrequencyMessage), message, maxlength);
    case FREESPACE_MESSAGE_SPIOPERATIONMESSAGE:
        return freespace_encodeSPIOperationMessage(&(s->sPIOperationMessage), message, maxlength);
    case FREESPACE_MESSAGE_EVENTREPORTCONFIGSETREQUEST:
        return freespace_encodeEventReportConfigSetRequest(&(s->eventReportConfigSetRequest), message, maxlength);
    case FREESPACE_MESSAGE_UNKNOWNCRSNOTIFICATION:
        return freespace_encodeUnknownCRSNotification(&(s->unknownCRSNotification), message, maxlength);
    case FREESPACE_MESSAGE_FRSLOOPREADREQUEST:
        return freespace_encodeFRSLoopReadRequest(&(s->fRSLoopReadRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSLOOPWRITEREQUEST:
        return freespace_encodeFRSLoopWriteRequest(&(s->fRSLoopWriteRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSLOOPWRITEDATA:
        return freespace_encodeFRSLoopWriteData(&(s->fRSLoopWriteData), message, maxlength);
    case FREESPACE_MESSAGE_FRSDONGLEREADREQUEST:
        return freespace_encodeFRSDongleReadRequest(&(s->fRSDongleReadRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSDONGLEWRITEREQUEST:
        return freespace_encodeFRSDongleWriteRequest(&(s->fRSDongleWriteRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSDONGLEWRITEDATA:
        return freespace_encodeFRSDongleWriteData(&(s->fRSDongleWriteData), message, maxlength);
    case FREESPACE_MESSAGE_FRSEFLASHREADREQUEST:
        return freespace_encodeFRSEFlashReadRequest(&(s->fRSEFlashReadRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSEFLASHWRITEREQUEST:
        return freespace_encodeFRSEFlashWriteRequest(&(s->fRSEFlashWriteRequest), message, maxlength);
    case FREESPACE_MESSAGE_FRSEFLASHWRITEDATA:
        return freespace_encodeFRSEFlashWriteData(&(s->fRSEFlashWriteData), message, maxlength);
    case FREESPACE_MESSAGE_LOOPBOOTLOADERCOMMAND:
        return freespace_encodeLoopBootloaderCommand(&(s->loopBootloaderCommand), message, maxlength);
    case FREESPACE_MESSAGE_DATAMOTIONCONTROL:
        return freespace_encodeDataMotionControl(&(s->dataMotionControl), message, maxlength);
    default:
        return -1;
    }
}