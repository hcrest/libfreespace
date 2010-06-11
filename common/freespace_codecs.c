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

#include "freespace/freespace_codecs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
#define STRICT_DECODE_LENGTH 0
#else
#define STRICT_DECODE_LENGTH 0
#endif

#undef CODECS_PRINTF
//#define CODECS_DEBUG
#ifdef CODECS_DEBUG
#define CODECS_PRINTF printf
#else
#define CODECS_PRINTF(...)
#endif


static uint32_t toUint32(const uint8_t * a) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return ((((uint32_t) a[3])) << 24) | ((((uint32_t) a[2])) << 16) | ((((uint32_t) a[1])) << 8) | (uint32_t) a[0];
#else
    return ((((uint32_t) a[0])) << 24) | ((((uint32_t) a[1])) << 16) | ((((uint32_t) a[2])) << 8) | (uint32_t) a[3];
#endif
}

static uint16_t toUint16(const uint8_t * a) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return ((((uint16_t) a[1])) << 8) | (uint16_t) a[0];
#else
    return ((((uint16_t) a[0])) << 8) | (uint16_t) a[1];
#endif
}

static uint8_t toUint8(const uint8_t * a) {
    return (uint8_t) *a;
}

static int32_t toInt32(const uint8_t * a) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return (int32_t) (((((int32_t) a[3])) << 24) | ((((uint32_t) a[2])) << 16) | ((((uint32_t) a[1])) << 8) | (uint32_t) a[0]);
#else
    return (int32_t) (((((int32_t) a[0])) << 24) | ((((uint32_t) a[1])) << 16) | ((((uint32_t) a[2])) << 8) | (uint32_t) a[3]);
#endif
}

static int16_t toInt16(const uint8_t * a) {
#ifdef FREESPACE_LITTLE_ENDIAN
    return (((int16_t) a[1]) << 8) | a[0];
#else
    return (((int16_t) a[0]) << 8) | a[1];
#endif
}

static int8_t toInt8(const uint8_t * a) {
    return (int8_t) *a;
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

LIBFREESPACE_API int freespace_encodeCoprocessorOutReport(const struct freespace_CoprocessorOutReport* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 16) {
				CODECS_PRINTF("freespace_CoprocessorOutReport encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 5;
			message[0 + offset] = s->payloadLength >> 0;
			message[1 + offset] = s->payload[0] >> 0;
			message[2 + offset] = s->payload[1] >> 0;
			message[3 + offset] = s->payload[2] >> 0;
			message[4 + offset] = s->payload[3] >> 0;
			message[5 + offset] = s->payload[4] >> 0;
			message[6 + offset] = s->payload[5] >> 0;
			message[7 + offset] = s->payload[6] >> 0;
			message[8 + offset] = s->payload[7] >> 0;
			message[9 + offset] = s->payload[8] >> 0;
			message[10 + offset] = s->payload[9] >> 0;
			message[11 + offset] = s->payload[10] >> 0;
			message[12 + offset] = s->payload[11] >> 0;
			message[13 + offset] = s->payload[12] >> 0;
			message[14 + offset] = s->payload[13] >> 0;
			return 15 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_decodeCoprocessorInReport(const uint8_t* message, int length, struct freespace_CoprocessorInReport* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 16) || (!STRICT_DECODE_LENGTH && length < 16)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "CoprocessorInReport", 16, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 6) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->payloadLength = toUint8(&message[0 + offset]);
			s->payload[0] = toUint8(&message[1 + offset]);
			s->payload[1] = toUint8(&message[2 + offset]);
			s->payload[2] = toUint8(&message[3 + offset]);
			s->payload[3] = toUint8(&message[4 + offset]);
			s->payload[4] = toUint8(&message[5 + offset]);
			s->payload[5] = toUint8(&message[6 + offset]);
			s->payload[6] = toUint8(&message[7 + offset]);
			s->payload[7] = toUint8(&message[8 + offset]);
			s->payload[8] = toUint8(&message[9 + offset]);
			s->payload[9] = toUint8(&message[10 + offset]);
			s->payload[10] = toUint8(&message[11 + offset]);
			s->payload[11] = toUint8(&message[12 + offset]);
			s->payload[12] = toUint8(&message[13 + offset]);
			s->payload[13] = toUint8(&message[14 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_encodePairingMessage(const struct freespace_PairingMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_PairingMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 13;
			return 7 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_PairingMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 2;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeProductIDRequest(const struct freespace_ProductIDRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_ProductIDRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 32;
			return 7 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_ProductIDRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 9;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeLEDSetRequest(const struct freespace_LEDSetRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_LEDSetRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 34;
			message[1 + offset] = s->onOff >> 0;
			message[2 + offset] = s->selectLED >> 0;
			return 7 + offset;
		case 2:
			if (maxlength < 11) {
				CODECS_PRINTF("freespace_LEDSetRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 10;
			message[1 + offset] = s->onOff >> 0;
			message[2 + offset] = s->selectLED >> 0;
			message[1] = 7 + offset;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeLinkQualityRequest(const struct freespace_LinkQualityRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_LinkQualityRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 48;
			message[1 + offset] = s->enable >> 0;
			return 7 + offset;
		case 2:
			if (maxlength < 11) {
				CODECS_PRINTF("freespace_LinkQualityRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 3;
			message[1 + offset] = s->enable >> 0;
			message[1] = 7 + offset;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeAlwaysOnRequest(const struct freespace_AlwaysOnRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_AlwaysOnRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 49;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFrequencyFixRequest(const struct freespace_FrequencyFixRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FrequencyFixRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 50;
			message[1 + offset] = s->channel0 >> 0;
			message[2 + offset] = s->channel1 >> 0;
			message[3 + offset] = s->channel2 >> 0;
			message[4 + offset] = s->channel3 >> 0;
			message[5 + offset] = s->channel4 >> 0;
			message[6 + offset] = s->device >> 0;
			return 7 + offset;
		case 2:
			if (maxlength < 10) {
				CODECS_PRINTF("freespace_FrequencyFixRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 11;
			message[1 + offset] = s->channel0 >> 0;
			message[2 + offset] = s->channel1 >> 0;
			message[3 + offset] = s->channel2 >> 0;
			message[4 + offset] = s->channel3 >> 0;
			message[5 + offset] = s->channel4 >> 0;
			message[1] = 6 + offset;
			return 6 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeSoftwareResetMessage(const struct freespace_SoftwareResetMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_SoftwareResetMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 51;
			message[1 + offset] = s->device >> 0;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeDongleRFDisableMessage(const struct freespace_DongleRFDisableMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_DongleRFDisableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 52;
			return 7 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_DongleRFDisableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 13;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeTxDisableMessage(const struct freespace_TxDisableMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 2) {
				CODECS_PRINTF("freespace_TxDisableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 80;
			return 1 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_TxDisableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 14;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeDongleRFSupressHomeFrequencyMessage(const struct freespace_DongleRFSupressHomeFrequencyMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_DongleRFSupressHomeFrequencyMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 53;
			message[1 + offset] = s->low >> 0;
			message[2 + offset] = s->high >> 0;
			return 7 + offset;
		case 2:
			if (maxlength < 11) {
				CODECS_PRINTF("freespace_DongleRFSupressHomeFrequencyMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 15;
			message[1 + offset] = s->low >> 0;
			message[2 + offset] = s->high >> 0;
			message[1] = 7 + offset;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSLoopReadRequest(const struct freespace_FRSLoopReadRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSLoopReadRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 58;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			message[5 + offset] = s->BlockSize >> 0;
			message[6 + offset] = s->BlockSize >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSLoopWriteRequest(const struct freespace_FRSLoopWriteRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSLoopWriteRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 61;
			message[1 + offset] = s->length >> 0;
			message[2 + offset] = s->length >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSLoopWriteData(const struct freespace_FRSLoopWriteData* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSLoopWriteData encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 63;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->data >> 0;
			message[4 + offset] = s->data >> 8;
			message[5 + offset] = s->data >> 16;
			message[6 + offset] = s->data >> 24;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSDongleReadRequest(const struct freespace_FRSDongleReadRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSDongleReadRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 59;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			message[5 + offset] = s->BlockSize >> 0;
			message[6 + offset] = s->BlockSize >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSDongleWriteRequest(const struct freespace_FRSDongleWriteRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSDongleWriteRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 62;
			message[1 + offset] = s->length >> 0;
			message[2 + offset] = s->length >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSDongleWriteData(const struct freespace_FRSDongleWriteData* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSDongleWriteData encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 64;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->data >> 0;
			message[4 + offset] = s->data >> 8;
			message[5 + offset] = s->data >> 16;
			message[6 + offset] = s->data >> 24;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSEFlashReadRequest(const struct freespace_FRSEFlashReadRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSEFlashReadRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 65;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			message[5 + offset] = s->BlockSize >> 0;
			message[6 + offset] = s->BlockSize >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSEFlashWriteRequest(const struct freespace_FRSEFlashWriteRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSEFlashWriteRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 66;
			message[1 + offset] = s->length >> 0;
			message[2 + offset] = s->length >> 8;
			message[3 + offset] = s->FRStype >> 0;
			message[4 + offset] = s->FRStype >> 8;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSEFlashWriteData(const struct freespace_FRSEFlashWriteData* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_FRSEFlashWriteData encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 67;
			message[1 + offset] = s->wordOffset >> 0;
			message[2 + offset] = s->wordOffset >> 8;
			message[3 + offset] = s->data >> 0;
			message[4 + offset] = s->data >> 8;
			message[5 + offset] = s->data >> 16;
			message[6 + offset] = s->data >> 24;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeDongleRFEnableMessage(const struct freespace_DongleRFEnableMessage* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 8) {
				CODECS_PRINTF("freespace_DongleRFEnableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 71;
			return 7 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_DongleRFEnableMessage encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 12;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeDataModeRequest(const struct freespace_DataModeRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 3) {
				CODECS_PRINTF("freespace_DataModeRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 73;
			message[1 + offset] = byteFromBits(s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace, s->SDA, 0, s->status);
			return 2 + offset;
		case 2:
			if (maxlength < 6) {
				CODECS_PRINTF("freespace_DataModeRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 4;
			message[1 + offset] = byteFromBits(s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace, s->SDA, s->aggregate, s->status);
			message[1] = 2 + offset;
			return 2 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeButtonTestModeRequest(const struct freespace_ButtonTestModeRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 3) {
				CODECS_PRINTF("freespace_ButtonTestModeRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[0 + offset] = (uint8_t) 81;
			message[1 + offset] = s->enable >> 0;
			return 2 + offset;
		case 2:
			if (maxlength < 6) {
				CODECS_PRINTF("freespace_ButtonTestModeRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 17;
			message[1 + offset] = s->enable >> 0;
			message[1] = 2 + offset;
			return 2 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_decodePairingResponse(const uint8_t* message, int length, struct freespace_PairingResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "PairingResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 13) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->pairing = (uint8_t) ((message[1 + offset] >> 0) & 0x01);
			s->autoPairing = (uint8_t) ((message[1 + offset] >> 1) & 0x01);
			s->success = (uint8_t) ((message[1 + offset] >> 2) & 0x01);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 12) || (!STRICT_DECODE_LENGTH && length < 12)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "PairingResponse", 12, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 2) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->pairing = (uint8_t) ((message[1 + offset] >> 0) & 0x01);
			s->autoPairing = (uint8_t) ((message[1 + offset] >> 1) & 0x01);
			s->success = (uint8_t) ((message[1 + offset] >> 2) & 0x01);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeProductIDResponse(const uint8_t* message, int length, struct freespace_ProductIDResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "ProductIDResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 32) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->swPartNumber = toUint32(&message[1 + offset]);
			s->swBuildNumber = toUint32(&message[5 + offset]);
			s->swicn = toUint32(&message[9 + offset]);
			s->swVersionPatch = toUint16(&message[13 + offset]);
			s->swVersionMinor = toUint8(&message[15 + offset]);
			s->swVersionMajor = toUint8(&message[16 + offset]);
			s->serialNumber = toUint32(&message[19 + offset]);
			s->deviceClass = (uint8_t) ((message[23 + offset] >> 0) & 0x7F);
			s->invalidNS = getBit(message[23 + offset], 7);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 22) || (!STRICT_DECODE_LENGTH && length < 22)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "ProductIDResponse", 22, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 6) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 9) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->deviceClass = (uint8_t) ((message[1 + offset] >> 0) & 0x3F);
			s->startup = getBit(message[1 + offset], 6);
			s->invalidNS = getBit(message[1 + offset], 7);
			s->swVersionMajor = toUint8(&message[2 + offset]);
			s->swVersionMinor = toUint8(&message[3 + offset]);
			s->swPartNumber = toUint32(&message[4 + offset]);
			s->swBuildNumber = toUint32(&message[8 + offset]);
			s->serialNumber = toUint32(&message[12 + offset]);
			s->swVersionPatch = toUint16(&message[16 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeLinkStatus(const uint8_t* message, int length, struct freespace_LinkStatus* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "LinkStatus", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 48) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->status = toUint8(&message[1 + offset]);
			s->mode = toUint8(&message[2 + offset]);
			s->resetStatus = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 31) || (!STRICT_DECODE_LENGTH && length < 31)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "LinkStatus", 31, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 3) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->status = toUint8(&message[1 + offset]);
			s->mode = toUint8(&message[2 + offset]);
			s->resetStatus = toUint8(&message[3 + offset]);
			s->txDisabled = toUint8(&message[4 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeAlwaysOnResponse(const uint8_t* message, int length, struct freespace_AlwaysOnResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "AlwaysOnResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 49) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSLoopReadResponse(const uint8_t* message, int length, struct freespace_FRSLoopReadResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSLoopReadResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 58) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->data[0] = toUint32(&message[3 + offset]);
			s->data[1] = toUint32(&message[7 + offset]);
			s->data[2] = toUint32(&message[11 + offset]);
			s->data[3] = toUint32(&message[15 + offset]);
			s->data[4] = toUint32(&message[19 + offset]);
    s->status = getNibble(message[23 + offset], 0);
    s->dataLength = getNibble(message[23 + offset], 1);
			s->FRStype = toUint16(&message[24 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSLoopWriteResponse(const uint8_t* message, int length, struct freespace_FRSLoopWriteResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSLoopWriteResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 61) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->status = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSDongleReadResponse(const uint8_t* message, int length, struct freespace_FRSDongleReadResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSDongleReadResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 59) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->data[0] = toUint32(&message[3 + offset]);
			s->data[1] = toUint32(&message[7 + offset]);
			s->data[2] = toUint32(&message[11 + offset]);
			s->data[3] = toUint32(&message[15 + offset]);
			s->data[4] = toUint32(&message[19 + offset]);
    s->status = getNibble(message[23 + offset], 0);
    s->dataLength = getNibble(message[23 + offset], 1);
			s->FRStype = toUint16(&message[24 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSDongleWriteResponse(const uint8_t* message, int length, struct freespace_FRSDongleWriteResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSDongleWriteResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 62) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->status = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSEFlashReadResponse(const uint8_t* message, int length, struct freespace_FRSEFlashReadResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSEFlashReadResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 65) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->data[0] = toUint32(&message[3 + offset]);
			s->data[1] = toUint32(&message[7 + offset]);
			s->data[2] = toUint32(&message[11 + offset]);
			s->data[3] = toUint32(&message[15 + offset]);
			s->data[4] = toUint32(&message[19 + offset]);
    s->status = getNibble(message[23 + offset], 0);
    s->dataLength = getNibble(message[23 + offset], 1);
			s->FRStype = toUint16(&message[24 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSEFlashWriteResponse(const uint8_t* message, int length, struct freespace_FRSEFlashWriteResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 27) || (!STRICT_DECODE_LENGTH && length < 27)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSEFlashWriteResponse", 27, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 66) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->wordOffset = toUint16(&message[1 + offset]);
			s->status = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeDataModeResponse(const uint8_t* message, int length, struct freespace_DataModeResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 3) || (!STRICT_DECODE_LENGTH && length < 3)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "DataModeResponse", 3, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 73) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->enableBodyMotion = getBit(message[1 + offset], 0);
			s->enableUserPosition = getBit(message[1 + offset], 1);
			s->inhibitPowerManager = getBit(message[1 + offset], 2);
			s->enableMouseMovement = getBit(message[1 + offset], 3);
			s->disableFreespace = getBit(message[1 + offset], 4);
			s->SDA = getBit(message[1 + offset], 5);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 6) || (!STRICT_DECODE_LENGTH && length < 6)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "DataModeResponse", 6, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 4) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->enableBodyMotion = getBit(message[1 + offset], 0);
			s->enableUserPosition = getBit(message[1 + offset], 1);
			s->inhibitPowerManager = getBit(message[1 + offset], 2);
			s->enableMouseMovement = getBit(message[1 + offset], 3);
			s->disableFreespace = getBit(message[1 + offset], 4);
			s->SDA = getBit(message[1 + offset], 5);
			s->aggregate = getBit(message[1 + offset], 6);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeButtonTestModeResponse(const uint8_t* message, int length, struct freespace_ButtonTestModeResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 5) || (!STRICT_DECODE_LENGTH && length < 5)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "ButtonTestModeResponse", 5, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

            if ((uint8_t) message[offset] != 81) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->status = toUint8(&message[1 + offset]);
			s->button = toUint8(&message[2 + offset]);
			s->press = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 8) || (!STRICT_DECODE_LENGTH && length < 8)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "ButtonTestModeResponse", 8, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 17) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->status = toUint8(&message[1 + offset]);
			s->button = toUint8(&message[2 + offset]);
			s->press = toUint8(&message[3 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_encodeBatteryLevelRequest(const struct freespace_BatteryLevelRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 2) {
				CODECS_PRINTF("freespace_BatteryLevelRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 9;
			return 1 + offset;
		case 2:
			if (maxlength < 5) {
				CODECS_PRINTF("freespace_BatteryLevelRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 5;
			message[1] = 1 + offset;
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_decodeBatteryLevel(const uint8_t* message, int length, struct freespace_BatteryLevel* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 4) || (!STRICT_DECODE_LENGTH && length < 4)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "BatteryLevel", 4, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 10) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->batteryStrength = toUint8(&message[0 + offset]);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 6) || (!STRICT_DECODE_LENGTH && length < 6)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "BatteryLevel", 6, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->batteryStrength = toUint8(&message[1 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeBodyFrame(const uint8_t* message, int length, struct freespace_BodyFrame* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 21) || (!STRICT_DECODE_LENGTH && length < 21)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "BodyFrame", 21, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 32) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->button1 = getBit(message[0 + offset], 0);
			s->button2 = getBit(message[0 + offset], 1);
			s->button3 = getBit(message[0 + offset], 2);
			s->button4 = getBit(message[0 + offset], 3);
			s->button5 = getBit(message[0 + offset], 4);
			s->button6 = getBit(message[0 + offset], 5);
			s->button7 = getBit(message[0 + offset], 6);
			s->button8 = getBit(message[0 + offset], 7);
			s->deltaX = toInt8(&message[1 + offset]);
			s->deltaY = toInt8(&message[2 + offset]);
			s->deltaWheel = toInt8(&message[3 + offset]);
			s->sequenceNumber = toUint16(&message[4 + offset]);
			s->linearAccelX = toInt16(&message[8 + offset]);
			s->linearAccelY = toInt16(&message[10 + offset]);
			s->linearAccelZ = toInt16(&message[12 + offset]);
			s->angularVelX = toInt16(&message[14 + offset]);
			s->angularVelY = toInt16(&message[16 + offset]);
			s->angularVelZ = toInt16(&message[18 + offset]);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 22) || (!STRICT_DECODE_LENGTH && length < 22)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "BodyFrame", 22, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 32) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];
			s->button1 = getBit(message[0 + offset], 0);
			s->button2 = getBit(message[0 + offset], 1);
			s->button3 = getBit(message[0 + offset], 2);
			s->button4 = getBit(message[0 + offset], 3);
			s->button5 = getBit(message[0 + offset], 4);
			s->button6 = getBit(message[0 + offset], 5);
			s->button7 = getBit(message[0 + offset], 6);
			s->button8 = getBit(message[0 + offset], 7);
			s->deltaX = toInt8(&message[1 + offset]);
			s->deltaY = toInt8(&message[2 + offset]);
			s->deltaWheel = toInt8(&message[3 + offset]);
			s->sequenceNumber = toUint16(&message[4 + offset]);
			s->linearAccelX = toInt16(&message[6 + offset]);
			s->linearAccelY = toInt16(&message[8 + offset]);
			s->linearAccelZ = toInt16(&message[10 + offset]);
			s->angularVelX = toInt16(&message[12 + offset]);
			s->angularVelY = toInt16(&message[14 + offset]);
			s->angularVelZ = toInt16(&message[16 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeUserFrame(const uint8_t* message, int length, struct freespace_UserFrame* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 1:
            if ((STRICT_DECODE_LENGTH && length != 23) || (!STRICT_DECODE_LENGTH && length < 23)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "UserFrame", 23, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 33) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->button1 = getBit(message[0 + offset], 0);
			s->button2 = getBit(message[0 + offset], 1);
			s->button3 = getBit(message[0 + offset], 2);
			s->button4 = getBit(message[0 + offset], 3);
			s->button5 = getBit(message[0 + offset], 4);
			s->button6 = getBit(message[0 + offset], 5);
			s->button7 = getBit(message[0 + offset], 6);
			s->button8 = getBit(message[0 + offset], 7);
			s->deltaX = toInt8(&message[1 + offset]);
			s->deltaY = toInt8(&message[2 + offset]);
			s->deltaWheel = toInt8(&message[3 + offset]);
			s->sequenceNumber = toUint16(&message[4 + offset]);
			s->linearPosX = toInt16(&message[8 + offset]);
			s->linearPosY = toInt16(&message[10 + offset]);
			s->linearPosZ = toInt16(&message[12 + offset]);
			s->angularPosA = toInt16(&message[14 + offset]);
			s->angularPosB = toInt16(&message[16 + offset]);
			s->angularPosC = toInt16(&message[18 + offset]);
			s->angularPosD = toInt16(&message[20 + offset]);
			return FREESPACE_SUCCESS;
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 24) || (!STRICT_DECODE_LENGTH && length < 24)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "UserFrame", 24, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 33) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];
			s->button1 = getBit(message[0 + offset], 0);
			s->button2 = getBit(message[0 + offset], 1);
			s->button3 = getBit(message[0 + offset], 2);
			s->button4 = getBit(message[0 + offset], 3);
			s->button5 = getBit(message[0 + offset], 4);
			s->button6 = getBit(message[0 + offset], 5);
			s->button7 = getBit(message[0 + offset], 6);
			s->button8 = getBit(message[0 + offset], 7);
			s->deltaX = toInt8(&message[1 + offset]);
			s->deltaY = toInt8(&message[2 + offset]);
			s->deltaWheel = toInt8(&message[3 + offset]);
			s->sequenceNumber = toUint16(&message[4 + offset]);
			s->linearPosX = toInt16(&message[6 + offset]);
			s->linearPosY = toInt16(&message[8 + offset]);
			s->linearPosZ = toInt16(&message[10 + offset]);
			s->angularPosB = toInt16(&message[12 + offset]);
			s->angularPosC = toInt16(&message[14 + offset]);
			s->angularPosD = toInt16(&message[16 + offset]);
			s->angularPosA = (int16_t) sqrt(268435456 - ((s->angularPosB * s->angularPosB) + (s->angularPosC * s->angularPosC) + (s->angularPosD * s->angularPosD)));
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_encodeDataMotionControl(const struct freespace_DataMotionControl* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 1:
			if (maxlength < 2) {
				CODECS_PRINTF("freespace_DataMotionControl encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 34;
			message[0 + offset] = byteFromBits(s->enableBodyMotion, s->enableUserPosition, s->inhibitPowerManager, s->enableMouseMovement, s->disableFreespace, 0, 0, 0);
			return 1 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_decodeFRSWriteResponse(const uint8_t* message, int length, struct freespace_FRSWriteResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 8) || (!STRICT_DECODE_LENGTH && length < 8)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSWriteResponse", 8, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 5) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 6) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->status = toUint8(&message[1 + offset]);
			s->wordOffset = toUint16(&message[2 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodeFRSReadResponse(const uint8_t* message, int length, struct freespace_FRSReadResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 22) || (!STRICT_DECODE_LENGTH && length < 22)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "FRSReadResponse", 22, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 6) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 8) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
    s->status = getNibble(message[1 + offset], 0);
    s->dataLength = getNibble(message[1 + offset], 1);
			s->wordOffset = toUint16(&message[2 + offset]);
			s->data[0] = toUint32(&message[4 + offset]);
			s->data[1] = toUint32(&message[8 + offset]);
			s->data[2] = toUint32(&message[12 + offset]);
			s->FRStype = toUint16(&message[16 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_decodePerResponse(const uint8_t* message, int length, struct freespace_PerResponse* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 22) || (!STRICT_DECODE_LENGTH && length < 22)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "PerResponse", 22, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 6) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];

            if ((uint8_t) message[offset] != 16) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			s->count = toUint32(&message[2 + offset]);
			s->msError = toUint32(&message[6 + offset]);
			s->smError = toUint32(&message[10 + offset]);
			s->frError = toUint32(&message[14 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}

LIBFREESPACE_API int freespace_encodeFRSWriteRequest(const struct freespace_FRSWriteRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 2:
			if (maxlength < 10) {
				CODECS_PRINTF("freespace_FRSWriteRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 6;
			message[2 + offset] = s->length >> 0;
			message[3 + offset] = s->length >> 8;
			message[4 + offset] = s->FRStype >> 0;
			message[5 + offset] = s->FRStype >> 8;
			message[1] = 6 + offset;
			return 6 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSWriteData(const struct freespace_FRSWriteData* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 2:
			if (maxlength < 12) {
				CODECS_PRINTF("freespace_FRSWriteData encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 7;
			message[2 + offset] = s->wordOffset >> 0;
			message[3 + offset] = s->wordOffset >> 8;
			message[4 + offset] = s->data >> 0;
			message[5 + offset] = s->data >> 8;
			message[6 + offset] = s->data >> 16;
			message[7 + offset] = s->data >> 24;
			message[1] = 8 + offset;
			return 8 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodeFRSReadRequest(const struct freespace_FRSReadRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 2:
			if (maxlength < 12) {
				CODECS_PRINTF("freespace_FRSReadRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 8;
			message[2 + offset] = s->readOffset >> 0;
			message[3 + offset] = s->readOffset >> 8;
			message[4 + offset] = s->FRStype >> 0;
			message[5 + offset] = s->FRStype >> 8;
			message[6 + offset] = s->BlockSize >> 0;
			message[7 + offset] = s->BlockSize >> 8;
			message[1] = 8 + offset;
			return 8 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_encodePerRequest(const struct freespace_PerRequest* s, uint8_t* message, int maxlength) {

	uint8_t offset = 1;

	switch(s->ver) {
		case 2:
			if (maxlength < 11) {
				CODECS_PRINTF("freespace_PerRequest encode(<INVALID LENGTH>)\n");
				return FREESPACE_ERROR_BUFFER_TOO_SMALL;
			}
			message[0] = (uint8_t) 7;
			message[2] = s->dest;
			message[3] = 0;
			offset = 4;
			message[0 + offset] = (uint8_t) 16;
			message[1 + offset] = s->op >> 0;
			message[2 + offset] = s->payload[0] >> 0;
			message[3 + offset] = s->payload[1] >> 0;
			message[4 + offset] = s->payload[2] >> 0;
			message[5 + offset] = s->payload[3] >> 0;
			message[6 + offset] = s->payload[4] >> 0;
			message[1] = 7 + offset;
			return 7 + offset;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}}

LIBFREESPACE_API int freespace_decodeBodyUserFrame(const uint8_t* message, int length, struct freespace_BodyUserFrame* s, uint8_t ver) {
	uint8_t offset = 1;

	s->ver = ver;

	switch(ver) {
		case 2:
            if ((STRICT_DECODE_LENGTH && length != 36) || (!STRICT_DECODE_LENGTH && length < 36)) {
                CODECS_PRINTF("Length mismatch for %s.  Expected %d.  Got %d.\n", "BodyUserFrame", 36, length);
                return FREESPACE_ERROR_BUFFER_TOO_SMALL;
            }
            if ((uint8_t) message[0] != 34) {
                return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
			offset = 4;
			s->len = message[1];
			s->dest = message[2];
			s->src = message[3];
			s->button1 = getBit(message[0 + offset], 0);
			s->button2 = getBit(message[0 + offset], 1);
			s->button3 = getBit(message[0 + offset], 2);
			s->button4 = getBit(message[0 + offset], 3);
			s->button5 = getBit(message[0 + offset], 4);
			s->button6 = getBit(message[0 + offset], 5);
			s->button7 = getBit(message[0 + offset], 6);
			s->button8 = getBit(message[0 + offset], 7);
			s->deltaX = toInt8(&message[1 + offset]);
			s->deltaY = toInt8(&message[2 + offset]);
			s->deltaWheel = toInt8(&message[3 + offset]);
			s->sequenceNumber = toUint16(&message[4 + offset]);
			s->linearAccelX = toInt16(&message[6 + offset]);
			s->linearAccelY = toInt16(&message[8 + offset]);
			s->linearAccelZ = toInt16(&message[10 + offset]);
			s->angularVelX = toInt16(&message[12 + offset]);
			s->angularVelY = toInt16(&message[14 + offset]);
			s->angularVelZ = toInt16(&message[16 + offset]);
			s->linearPosX = toInt16(&message[18 + offset]);
			s->linearPosY = toInt16(&message[20 + offset]);
			s->linearPosZ = toInt16(&message[22 + offset]);
			s->angularPosB = toInt16(&message[24 + offset]);
			s->angularPosC = toInt16(&message[26 + offset]);
			s->angularPosD = toInt16(&message[28 + offset]);
			s->angularPosA = toInt16(&message[30 + offset]);
			return FREESPACE_SUCCESS;
		default:
			return  FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
	}
}


LIBFREESPACE_API int freespace_decode_message(const uint8_t* message, int length, struct freespace_message* s, uint8_t ver) {
    if (length == 0) {
        return -1;
    }

    switch (ver) {
		case 0:
			switch(message[0]) {
                default:
                    return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
		case 1:
			switch(message[0]) {
				case 6:
                    s->messageType = FREESPACE_MESSAGE_COPROCESSORINREPORT;
                    return freespace_decodeCoprocessorInReport(message, length, &(s->coprocessorInReport), ver);
				case 8:
                    switch (message[1]) {
                        case 13:
                            s->messageType = FREESPACE_MESSAGE_PAIRINGRESPONSE;
                            return freespace_decodePairingResponse(message, length, &(s->pairingResponse), ver);
                        case 32:
                            s->messageType = FREESPACE_MESSAGE_PRODUCTIDRESPONSE;
                            return freespace_decodeProductIDResponse(message, length, &(s->productIDResponse), ver);
                        case 48:
                            s->messageType = FREESPACE_MESSAGE_LINKSTATUS;
                            return freespace_decodeLinkStatus(message, length, &(s->linkStatus), ver);
                        case 49:
                            s->messageType = FREESPACE_MESSAGE_ALWAYSONRESPONSE;
                            return freespace_decodeAlwaysOnResponse(message, length, &(s->alwaysOnResponse), ver);
                        case 58:
                            s->messageType = FREESPACE_MESSAGE_FRSLOOPREADRESPONSE;
                            return freespace_decodeFRSLoopReadResponse(message, length, &(s->fRSLoopReadResponse), ver);
                        case 61:
                            s->messageType = FREESPACE_MESSAGE_FRSLOOPWRITERESPONSE;
                            return freespace_decodeFRSLoopWriteResponse(message, length, &(s->fRSLoopWriteResponse), ver);
                        case 59:
                            s->messageType = FREESPACE_MESSAGE_FRSDONGLEREADRESPONSE;
                            return freespace_decodeFRSDongleReadResponse(message, length, &(s->fRSDongleReadResponse), ver);
                        case 62:
                            s->messageType = FREESPACE_MESSAGE_FRSDONGLEWRITERESPONSE;
                            return freespace_decodeFRSDongleWriteResponse(message, length, &(s->fRSDongleWriteResponse), ver);
                        case 65:
                            s->messageType = FREESPACE_MESSAGE_FRSEFLASHREADRESPONSE;
                            return freespace_decodeFRSEFlashReadResponse(message, length, &(s->fRSEFlashReadResponse), ver);
                        case 66:
                            s->messageType = FREESPACE_MESSAGE_FRSEFLASHWRITERESPONSE;
                            return freespace_decodeFRSEFlashWriteResponse(message, length, &(s->fRSEFlashWriteResponse), ver);
                        case 73:
                            s->messageType = FREESPACE_MESSAGE_DATAMODERESPONSE;
                            return freespace_decodeDataModeResponse(message, length, &(s->dataModeResponse), ver);
                        case 81:
                            s->messageType = FREESPACE_MESSAGE_BUTTONTESTMODERESPONSE;
                            return freespace_decodeButtonTestModeResponse(message, length, &(s->buttonTestModeResponse), ver);
                        default:
                            return FREESPACE_ERROR_MALFORMED_MESSAGE;
                    }
				case 10:
                    s->messageType = FREESPACE_MESSAGE_BATTERYLEVEL;
                    return freespace_decodeBatteryLevel(message, length, &(s->batteryLevel), ver);
				case 32:
                    s->messageType = FREESPACE_MESSAGE_BODYFRAME;
                    return freespace_decodeBodyFrame(message, length, &(s->bodyFrame), ver);
				case 33:
                    s->messageType = FREESPACE_MESSAGE_USERFRAME;
                    return freespace_decodeUserFrame(message, length, &(s->userFrame), ver);
                default:
                    return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }
		case 2:
			switch(message[0]) {
				case 5:
                    switch (message[4]) {
                        case 2:
                            s->messageType = FREESPACE_MESSAGE_PAIRINGRESPONSE;
                            return freespace_decodePairingResponse(message, length, &(s->pairingResponse), ver);
                        case 3:
                            s->messageType = FREESPACE_MESSAGE_LINKSTATUS;
                            return freespace_decodeLinkStatus(message, length, &(s->linkStatus), ver);
                        case 4:
                            s->messageType = FREESPACE_MESSAGE_DATAMODERESPONSE;
                            return freespace_decodeDataModeResponse(message, length, &(s->dataModeResponse), ver);
                        case 17:
                            s->messageType = FREESPACE_MESSAGE_BUTTONTESTMODERESPONSE;
                            return freespace_decodeButtonTestModeResponse(message, length, &(s->buttonTestModeResponse), ver);
                        case 5:
                            s->messageType = FREESPACE_MESSAGE_BATTERYLEVEL;
                            return freespace_decodeBatteryLevel(message, length, &(s->batteryLevel), ver);
                        case 6:
                            s->messageType = FREESPACE_MESSAGE_FRSWRITERESPONSE;
                            return freespace_decodeFRSWriteResponse(message, length, &(s->fRSWriteResponse), ver);
                        default:
                            return FREESPACE_ERROR_MALFORMED_MESSAGE;
                    }
				case 6:
                    switch (message[4]) {
                        case 9:
                            s->messageType = FREESPACE_MESSAGE_PRODUCTIDRESPONSE;
                            return freespace_decodeProductIDResponse(message, length, &(s->productIDResponse), ver);
                        case 8:
                            s->messageType = FREESPACE_MESSAGE_FRSREADRESPONSE;
                            return freespace_decodeFRSReadResponse(message, length, &(s->fRSReadResponse), ver);
                        case 16:
                            s->messageType = FREESPACE_MESSAGE_PERRESPONSE;
                            return freespace_decodePerResponse(message, length, &(s->perResponse), ver);
                        default:
                            return FREESPACE_ERROR_MALFORMED_MESSAGE;
                    }
				case 32:
                    s->messageType = FREESPACE_MESSAGE_BODYFRAME;
                    return freespace_decodeBodyFrame(message, length, &(s->bodyFrame), ver);
				case 33:
                    s->messageType = FREESPACE_MESSAGE_USERFRAME;
                    return freespace_decodeUserFrame(message, length, &(s->userFrame), ver);
				case 34:
                    s->messageType = FREESPACE_MESSAGE_BODYUSERFRAME;
                    return freespace_decodeBodyUserFrame(message, length, &(s->bodyUserFrame), ver);
                default:
                    return FREESPACE_ERROR_MALFORMED_MESSAGE;
            }

    default:
        return FREESPACE_ERROR_INVALID_HID_PROTOCOL_VERSION;
    }
}

LIBFREESPACE_API int freespace_encode_message(const uint8_t hVer, struct freespace_message* message, uint8_t* msgBuf, int maxlength, uint8_t dest) {
    switch (message->messageType) {
        case FREESPACE_MESSAGE_COPROCESSOROUTREPORT:
            message->coprocessorOutReport.ver = hVer;
            message->coprocessorOutReport.dest = dest;
            message->coprocessorOutReport.src = 0;
            return freespace_encodeCoprocessorOutReport(&(message->coprocessorOutReport), msgBuf, maxlength);
        case FREESPACE_MESSAGE_PAIRINGMESSAGE:
            message->pairingMessage.ver = hVer;
            message->pairingMessage.dest = dest;
            message->pairingMessage.src = 0;
            return freespace_encodePairingMessage(&(message->pairingMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_PRODUCTIDREQUEST:
            message->productIDRequest.ver = hVer;
            message->productIDRequest.dest = dest;
            message->productIDRequest.src = 0;
            return freespace_encodeProductIDRequest(&(message->productIDRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_LEDSETREQUEST:
            message->lEDSetRequest.ver = hVer;
            message->lEDSetRequest.dest = dest;
            message->lEDSetRequest.src = 0;
            return freespace_encodeLEDSetRequest(&(message->lEDSetRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_LINKQUALITYREQUEST:
            message->linkQualityRequest.ver = hVer;
            message->linkQualityRequest.dest = dest;
            message->linkQualityRequest.src = 0;
            return freespace_encodeLinkQualityRequest(&(message->linkQualityRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_ALWAYSONREQUEST:
            message->alwaysOnRequest.ver = hVer;
            message->alwaysOnRequest.dest = dest;
            message->alwaysOnRequest.src = 0;
            return freespace_encodeAlwaysOnRequest(&(message->alwaysOnRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FREQUENCYFIXREQUEST:
            message->frequencyFixRequest.ver = hVer;
            message->frequencyFixRequest.dest = dest;
            message->frequencyFixRequest.src = 0;
            return freespace_encodeFrequencyFixRequest(&(message->frequencyFixRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_SOFTWARERESETMESSAGE:
            message->softwareResetMessage.ver = hVer;
            message->softwareResetMessage.dest = dest;
            message->softwareResetMessage.src = 0;
            return freespace_encodeSoftwareResetMessage(&(message->softwareResetMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_DONGLERFDISABLEMESSAGE:
            message->dongleRFDisableMessage.ver = hVer;
            message->dongleRFDisableMessage.dest = dest;
            message->dongleRFDisableMessage.src = 0;
            return freespace_encodeDongleRFDisableMessage(&(message->dongleRFDisableMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_TXDISABLEMESSAGE:
            message->txDisableMessage.ver = hVer;
            message->txDisableMessage.dest = dest;
            message->txDisableMessage.src = 0;
            return freespace_encodeTxDisableMessage(&(message->txDisableMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_DONGLERFSUPRESSHOMEFREQUENCYMESSAGE:
            message->dongleRFSupressHomeFrequencyMessage.ver = hVer;
            message->dongleRFSupressHomeFrequencyMessage.dest = dest;
            message->dongleRFSupressHomeFrequencyMessage.src = 0;
            return freespace_encodeDongleRFSupressHomeFrequencyMessage(&(message->dongleRFSupressHomeFrequencyMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSLOOPREADREQUEST:
            message->fRSLoopReadRequest.ver = hVer;
            message->fRSLoopReadRequest.dest = dest;
            message->fRSLoopReadRequest.src = 0;
            return freespace_encodeFRSLoopReadRequest(&(message->fRSLoopReadRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSLOOPWRITEREQUEST:
            message->fRSLoopWriteRequest.ver = hVer;
            message->fRSLoopWriteRequest.dest = dest;
            message->fRSLoopWriteRequest.src = 0;
            return freespace_encodeFRSLoopWriteRequest(&(message->fRSLoopWriteRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSLOOPWRITEDATA:
            message->fRSLoopWriteData.ver = hVer;
            message->fRSLoopWriteData.dest = dest;
            message->fRSLoopWriteData.src = 0;
            return freespace_encodeFRSLoopWriteData(&(message->fRSLoopWriteData), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSDONGLEREADREQUEST:
            message->fRSDongleReadRequest.ver = hVer;
            message->fRSDongleReadRequest.dest = dest;
            message->fRSDongleReadRequest.src = 0;
            return freespace_encodeFRSDongleReadRequest(&(message->fRSDongleReadRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSDONGLEWRITEREQUEST:
            message->fRSDongleWriteRequest.ver = hVer;
            message->fRSDongleWriteRequest.dest = dest;
            message->fRSDongleWriteRequest.src = 0;
            return freespace_encodeFRSDongleWriteRequest(&(message->fRSDongleWriteRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSDONGLEWRITEDATA:
            message->fRSDongleWriteData.ver = hVer;
            message->fRSDongleWriteData.dest = dest;
            message->fRSDongleWriteData.src = 0;
            return freespace_encodeFRSDongleWriteData(&(message->fRSDongleWriteData), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSEFLASHREADREQUEST:
            message->fRSEFlashReadRequest.ver = hVer;
            message->fRSEFlashReadRequest.dest = dest;
            message->fRSEFlashReadRequest.src = 0;
            return freespace_encodeFRSEFlashReadRequest(&(message->fRSEFlashReadRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSEFLASHWRITEREQUEST:
            message->fRSEFlashWriteRequest.ver = hVer;
            message->fRSEFlashWriteRequest.dest = dest;
            message->fRSEFlashWriteRequest.src = 0;
            return freespace_encodeFRSEFlashWriteRequest(&(message->fRSEFlashWriteRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSEFLASHWRITEDATA:
            message->fRSEFlashWriteData.ver = hVer;
            message->fRSEFlashWriteData.dest = dest;
            message->fRSEFlashWriteData.src = 0;
            return freespace_encodeFRSEFlashWriteData(&(message->fRSEFlashWriteData), msgBuf, maxlength);
        case FREESPACE_MESSAGE_DONGLERFENABLEMESSAGE:
            message->dongleRFEnableMessage.ver = hVer;
            message->dongleRFEnableMessage.dest = dest;
            message->dongleRFEnableMessage.src = 0;
            return freespace_encodeDongleRFEnableMessage(&(message->dongleRFEnableMessage), msgBuf, maxlength);
        case FREESPACE_MESSAGE_DATAMODEREQUEST:
            message->dataModeRequest.ver = hVer;
            message->dataModeRequest.dest = dest;
            message->dataModeRequest.src = 0;
            return freespace_encodeDataModeRequest(&(message->dataModeRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_BUTTONTESTMODEREQUEST:
            message->buttonTestModeRequest.ver = hVer;
            message->buttonTestModeRequest.dest = dest;
            message->buttonTestModeRequest.src = 0;
            return freespace_encodeButtonTestModeRequest(&(message->buttonTestModeRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_BATTERYLEVELREQUEST:
            message->batteryLevelRequest.ver = hVer;
            message->batteryLevelRequest.dest = dest;
            message->batteryLevelRequest.src = 0;
            return freespace_encodeBatteryLevelRequest(&(message->batteryLevelRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_DATAMOTIONCONTROL:
            message->dataMotionControl.ver = hVer;
            message->dataMotionControl.dest = dest;
            message->dataMotionControl.src = 0;
            return freespace_encodeDataMotionControl(&(message->dataMotionControl), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSWRITEREQUEST:
            message->fRSWriteRequest.ver = hVer;
            message->fRSWriteRequest.dest = dest;
            message->fRSWriteRequest.src = 0;
            return freespace_encodeFRSWriteRequest(&(message->fRSWriteRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSWRITEDATA:
            message->fRSWriteData.ver = hVer;
            message->fRSWriteData.dest = dest;
            message->fRSWriteData.src = 0;
            return freespace_encodeFRSWriteData(&(message->fRSWriteData), msgBuf, maxlength);
        case FREESPACE_MESSAGE_FRSREADREQUEST:
            message->fRSReadRequest.ver = hVer;
            message->fRSReadRequest.dest = dest;
            message->fRSReadRequest.src = 0;
            return freespace_encodeFRSReadRequest(&(message->fRSReadRequest), msgBuf, maxlength);
        case FREESPACE_MESSAGE_PERREQUEST:
            message->perRequest.ver = hVer;
            message->perRequest.dest = dest;
            message->perRequest.src = 0;
            return freespace_encodePerRequest(&(message->perRequest), msgBuf, maxlength);
        default:
            return -1;
        }
}