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

#ifndef FREESPACE_CODECS_H_
#define FREESPACE_CODECS_H_

#include "freespace/freespace.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup messages Freespace Messages
 *
 * This page describes the messages that can be sent to and from the Freespace Device.
 * They are represented as structs which can be encoded and decoded from strings.
 */
/**   @ingroup messages 
 * DEPRECATED. This report implements a HID Mouse with 8 buttons and a scroll wheel
 */
struct freespace_MouseMovement {
	// ID = 2
	uint8_t button1;
	uint8_t button2;
	uint8_t button3;
	uint8_t button4;
	uint8_t button5;
	uint8_t button6;
	uint8_t button7;
	uint8_t button8;

	/** Changes in pointer location in dimensionless units. Positive moves pointer right. */
	int16_t deltaX;

	/** Changes in pointer location in dimensionless units. Positive moves pointer down. */
	int16_t deltaY;

	/** Reports scroll wheel motion in detents. Up is positive. */
	int16_t deltaWheel;
};


/** @ingroup messages
 * Decode a MouseMovement message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_MouseMovement struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeMouseMovement(const int8_t* message, int length, struct freespace_MouseMovement* s);

/**   @ingroup messages 
 * DEPRECATED: Indicates buttons that operate a consumer electronics product.
 */
struct freespace_ConsumerControl {
	// ID = 3

	/** A 12-bit field that carries a consumer control usage code */
	int16_t usageID;
	uint8_t usageID11_8;
	uint8_t numPad;

	/** Indicates up to 255 button presses */
	uint8_t functionButtons;
};


/** @ingroup messages
 * Decode a ConsumerControl message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_ConsumerControl struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeConsumerControl(const int8_t* message, int length, struct freespace_ConsumerControl* s);

/**   @ingroup messages 
 * DEPRECATED. Indicates buttons from a computer keyboard or keypad. Refer to the USB HID Usage Tables Document Keyboard Page for more information
 */
struct freespace_KeyboardReport {
	// ID = 4
	uint8_t LCtrl;
	uint8_t LShift;
	uint8_t LAlt;
	uint8_t LGui;
	uint8_t RCtrl;
	uint8_t RShift;
	uint8_t RAlt;
	uint8_t RGui;
	int16_t keyID;
};


/** @ingroup messages
 * Decode a KeyboardReport message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_KeyboardReport struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeKeyboardReport(const int8_t* message, int length, struct freespace_KeyboardReport* s);

/**   @ingroup messages 
 * Reserved for passing message through from the Freespace coprocessor to the USB host.
 */
struct freespace_CoprocessorOutReport {
	// ID = 5
	int16_t payloadLength;
	uint8_t payload[14];
};


/** @ingroup messages
 * Encode a CoprocessorOutReport message.
 *
 * @param s the freespace_CoprocessorOutReport struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeCoprocessorOutReport(const struct freespace_CoprocessorOutReport* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Reserved for proprietary diagnostics, testing, and debugging use.
 */
struct freespace_CoprocessorInReport {
	// ID = 6
	int16_t payloadLength;
	uint8_t payload[14];
};


/** @ingroup messages
 * Decode a CoprocessorInReport message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_CoprocessorInReport struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeCoprocessorInReport(const int8_t* message, int length, struct freespace_CoprocessorInReport* s);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_ConfigurationMessage {
	// ID = 7
	// subID = 0x01

	/** SDA(1)/NORMAL(0) */
	int16_t SDA;
};


/** @ingroup messages
 * Encode a ConfigurationMessage message.
 *
 * @param s the freespace_ConfigurationMessage struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeConfigurationMessage(const struct freespace_ConfigurationMessage* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a PairingMessage message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodePairingMessage(int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a DebugMessage message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeDebugMessage(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from dongle towards the loop to request a chunk of cal data to be sent. This is used in the Zebra system architecture.
 */
struct freespace_FactoryCalibrationReadRequest {
	// ID = 7
	// subID = 0x10
	uint8_t wordOffsetRequested;
};


/** @ingroup messages
 * Encode a FactoryCalibrationReadRequest message.
 *
 * @param s the freespace_FactoryCalibrationReadRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFactoryCalibrationReadRequest(const struct freespace_FactoryCalibrationReadRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from host to dongle to trigger a reset into the bootloader.
The 32-bit loaderkey value is placed at a special location in RAM and the system is reset.
If the loader key has the proper value, the bootloader will then take control.
Otherwise the bootloader will transfer control to the application immediately.
 */
struct freespace_DongleReset {
	// ID = 7
	// subID = 0x14
	uint32_t loaderKey;
};


/** @ingroup messages
 * Encode a DongleReset message.
 *
 * @param s the freespace_DongleReset struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeDongleReset(const struct freespace_DongleReset* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from the host towards the FT dongle to update or request the current status of the dongle
 */
struct freespace_FTDongleStatusRequest {
	// ID = 7
	// subID = 0x15

	/** Power:
	 0:disable DUT power
	 1:enable DUT power */
	uint8_t power;

	/** Reset:
	 0:deassert DUT reset signal
	 1:assert DUT reset signal */
	uint8_t reset;

	/** Presence:
	 0:deassert presence signal
	 1:assert presence signal */
	uint8_t presence;

	/** Operation:
	 0:read status. Values for presence, reset, and power are don't cares
	 1: update status. Status is set to values indicated by presence, power, and reset */
	uint8_t operation;
};


/** @ingroup messages
 * Encode a FTDongleStatusRequest message.
 *
 * @param s the freespace_FTDongleStatusRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFTDongleStatusRequest(const struct freespace_FTDongleStatusRequest* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a StatisticsRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeStatisticsRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message is used during Zebra Sytem test to collect link (RF+USB) strength statistics.
 */
struct freespace_ZebraSystemTest {
	// ID = 7
	// subID = 0x1F

	/** 1 to reset, 0 for normal */
	uint8_t reset;

	/** 0: RF+USB, 1: USB */
	uint8_t mode;
	int16_t pcSequence;
};


/** @ingroup messages
 * Encode a ZebraSystemTest message.
 *
 * @param s the freespace_ZebraSystemTest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeZebraSystemTest(const struct freespace_ZebraSystemTest* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a ProductIDRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeProductIDRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This request causes the Loop to set a status LED to a particular value
 */
struct freespace_LoopLEDSetRequest {
	// ID = 7
	// subID = 0x22

	/** 0: Off, 1: On */
	uint8_t onOff;

	/** LED Select: 0-green(all devices)
	 1-red(all devices)
	 2-yellow(all devices)
	 3-blue(all devices)
	 4-FTA green
	 5-FTA red
	 6-S2U yellow
	 7-S2U blue
	 8-Dominion LED PWM
	 9-Dominion LED1
	 10-Dominion LED2 */
	uint8_t selectLED;
};


/** @ingroup messages
 * Encode a LoopLEDSetRequest message.
 *
 * @param s the freespace_LoopLEDSetRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeLoopLEDSetRequest(const struct freespace_LoopLEDSetRequest* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a AlwaysOnRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeAlwaysOnRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message causes the FR frequencies of the selected device to be fixed at channels 0-4. The last byte selects the device.
	 When the loop is selected it is put into a mode where it does not require the dongle to transmit and where it does not go to sleep.
 */
struct freespace_FrequencyFixRequest {
	// ID = 7
	// subID = 0x32
	uint8_t channel0;
	uint8_t channel1;
	uint8_t channel2;
	uint8_t channel3;
	uint8_t channel4;

	/** 1 for dongle, 2 for loop */
	uint8_t device;
};


/** @ingroup messages
 * Encode a FrequencyFixRequest message.
 *
 * @param s the freespace_FrequencyFixRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFrequencyFixRequest(const struct freespace_FrequencyFixRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message software resets the selected device.
 */
struct freespace_SoftwareResetMessage {
	// ID = 7
	// subID = 0x33

	/** 1 for dongle */
	uint8_t device;
};


/** @ingroup messages
 * Encode a SoftwareResetMessage message.
 *
 * @param s the freespace_SoftwareResetMessage struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeSoftwareResetMessage(const struct freespace_SoftwareResetMessage* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a DongleRFDisableMessage message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeDongleRFDisableMessage(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message is for RF Home frequency supression on the dongle.

	802.11 defines the peak of a channel to cover the range +-11MHz from the center frequency of the channel.
	Hillcrest adds an extra 1MHz to this boundary, so Low and High should be -12MHz from the center channel of the 802.11
	and +12MHz from the center channel of 802.11 respectively. These values must be in the range [1,82].
	To disable home frequency suppression, set either Low or High to be out-of-range. 0xFF is the preferred value for disabling suppression.
 */
struct freespace_DongleRFSupressHomeFrequencyMessage {
	// ID = 7
	// subID = 0x35
	uint8_t low;
	uint8_t high;
};


/** @ingroup messages
 * Encode a DongleRFSupressHomeFrequencyMessage message.
 *
 * @param s the freespace_DongleRFSupressHomeFrequencyMessage struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeDongleRFSupressHomeFrequencyMessage(const struct freespace_DongleRFSupressHomeFrequencyMessage* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message carries a generic SPI operation to be sent to a test module, such as FSRK-E.
	The SPI response message is sent back to the host.
 */
struct freespace_SPIOperationMessage {
	// ID = 7
	// subID = 0x36
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
};


/** @ingroup messages
 * Encode a SPIOperationMessage message.
 *
 * @param s the freespace_SPIOperationMessage struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeSPIOperationMessage(const struct freespace_SPIOperationMessage* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message is sent from the host to an FSRK-E adapter test module,
	 to configure which events will cause the test module to send an event report message to the host.
	 A bit set to one indicates the reporting for that event is enabled.
 */
struct freespace_EventReportConfigSetRequest {
	// ID = 7
	// subID = 0x37

	/** Summary interrupt detected. */
	uint8_t intc;

	/** X-Y pointer movement data ready. */
	uint8_t xyMov;

	/** Activity classification status change */
	uint8_t acSt;

	/** Test module reset detected. */
	uint8_t reset;

	/** Motion data ready */
	uint8_t motDr;

	/** Wake-on-motion detected. */
	uint8_t wom;

	/** Motion data overflow */
	uint8_t motOv;

	/** Activity classification event detected. */
	uint8_t acEv;

	/** SDA data ready. */
	uint8_t sdaDr;

	/** SDA data overflow. */
	uint8_t sdaOv;

	/** Configuration StatusChange */
	uint8_t cfgSt;
};


/** @ingroup messages
 * Encode a EventReportConfigSetRequest message.
 *
 * @param s the freespace_EventReportConfigSetRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeEventReportConfigSetRequest(const struct freespace_EventReportConfigSetRequest* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a EventReportConfigGetRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeEventReportConfigGetRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message is sent from a base station handheld. It indicates that the base station did not
	 recognize a CRS message that the handheld had sent. This message will never be sent over USB.
 */
struct freespace_UnknownCRSNotification {
	// ID = 7
	// subID = 0x39

	/** This is the HID report ID of the unknown message received by the base station. */
	uint8_t unknownReportID;

	/** This is the HID sub-message ID of the unknown message received by the base station. */
	uint8_t unknownSubMessageID;
};


/** @ingroup messages
 * Encode a UnknownCRSNotification message.
 *
 * @param s the freespace_UnknownCRSNotification struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeUnknownCRSNotification(const struct freespace_UnknownCRSNotification* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from dongle towards the loop to request flash record to be sent.
	The data sent starts from the word offset and continues through to the end of the record.
 */
struct freespace_FRSLoopReadRequest {
	// ID = 7
	// subID = 0x3A
	int16_t wordOffset;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSLoopReadRequest message.
 *
 * @param s the freespace_FRSLoopReadRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSLoopReadRequest(const struct freespace_FRSLoopReadRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from the host towards the loop to initiate a flash record write.
	A length of 0 will cause the record to be invalidated.
 */
struct freespace_FRSLoopWriteRequest {
	// ID = 7
	// subID = 0x3D
	int16_t length;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSLoopWriteRequest message.
 *
 * @param s the freespace_FRSLoopWriteRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSLoopWriteRequest(const struct freespace_FRSLoopWriteRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This message is sent from the host towards the loop to write data to the record a previous write request indicated.
 */
struct freespace_FRSLoopWriteData {
	// ID = 7
	// subID = 0x3F
	int16_t wordOffset;
	uint8_t data[4];
};


/** @ingroup messages
 * Encode a FRSLoopWriteData message.
 *
 * @param s the freespace_FRSLoopWriteData struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSLoopWriteData(const struct freespace_FRSLoopWriteData* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSDongleReadRequest {
	// ID = 7
	// subID = 0x3B
	int16_t wordOffset;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSDongleReadRequest message.
 *
 * @param s the freespace_FRSDongleReadRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSDongleReadRequest(const struct freespace_FRSDongleReadRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSDongleWriteRequest {
	// ID = 7
	// subID = 0x3E
	int16_t length;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSDongleWriteRequest message.
 *
 * @param s the freespace_FRSDongleWriteRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSDongleWriteRequest(const struct freespace_FRSDongleWriteRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSDongleWriteData {
	// ID = 7
	// subID = 0x40
	int16_t wordOffset;
	uint8_t data[4];
};


/** @ingroup messages
 * Encode a FRSDongleWriteData message.
 *
 * @param s the freespace_FRSDongleWriteData struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSDongleWriteData(const struct freespace_FRSDongleWriteData* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSEFlashReadRequest {
	// ID = 7
	// subID = 0x41
	int16_t wordOffset;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSEFlashReadRequest message.
 *
 * @param s the freespace_FRSEFlashReadRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSEFlashReadRequest(const struct freespace_FRSEFlashReadRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSEFlashWriteRequest {
	// ID = 7
	// subID = 0x42
	int16_t length;
	int16_t FRStype;
};


/** @ingroup messages
 * Encode a FRSEFlashWriteRequest message.
 *
 * @param s the freespace_FRSEFlashWriteRequest struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSEFlashWriteRequest(const struct freespace_FRSEFlashWriteRequest* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSEFlashWriteData {
	// ID = 7
	// subID = 0x43
	int16_t wordOffset;
	uint8_t data[4];
};


/** @ingroup messages
 * Encode a FRSEFlashWriteData message.
 *
 * @param s the freespace_FRSEFlashWriteData struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeFRSEFlashWriteData(const struct freespace_FRSEFlashWriteData* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * This is sent from host to a handheld(loop) to trigger a reset into the bootloader.
	The 32-bit loaderkey command value is placed at a special location in RAM and the system is reset.
	If the loaderkey has the proper value, the bootloader will act on the command.
	Otherwise the bootloader will transfer control to the application immediately.
 */
struct freespace_LoopBootloaderCommand {
	// ID = 7
	// subID = 0x44

	/** Loader Key Commands:
	     Launch application: 0xb9e3aa6e 
	     Upgrade Image:      0x461c5592 
	     Validate Image:     0xc452801e */
	uint32_t loaderKeyCommand;
};


/** @ingroup messages
 * Encode a LoopBootloaderCommand message.
 *
 * @param s the freespace_LoopBootloaderCommand struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeLoopBootloaderCommand(const struct freespace_LoopBootloaderCommand* s, int8_t* message, int maxlength);


/** @ingroup messages
 * Encode a LoopBootloaderStatusRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeLoopBootloaderStatusRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * The Gen4 SDA format is used by FSRK Gen4 dongle, which has USB product ID = 0xC007.
 */
struct freespace_Gen4SDAFormat {
	// ID = 8
	// subID = 0x01
	int16_t reportCount;
	uint8_t left;
	uint8_t right;
	uint8_t scroll;
	uint8_t hold;
	uint8_t triangle;
	int16_t deltaWheel;
	int16_t accelX;
	int16_t accelY;
	int16_t accelZ;
	int16_t rotationX;
	int16_t rotationY;
	int16_t rotationZ;
	int16_t xMov;
	int16_t yMov;
	uint32_t sampleBase;
	uint16_t command;
};


/** @ingroup messages
 * Decode a Gen4SDAFormat message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_Gen4SDAFormat struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeGen4SDAFormat(const int8_t* message, int length, struct freespace_Gen4SDAFormat* s);

/**   @ingroup messages 
 * This is sent from the loop to the dongle to convey the factory calibration data. This is only used by the Zebra architecture products.
 */
struct freespace_FactoryCalibrationReadData {
	// ID = 8
	// subID = 0x10

	/** wordOffset indicates the number of words offset from the beginning of the factory calibration data the message begins. */
	int16_t wordOffset;

	/** Indicates the number of data words contained within the message, typically 5 words. */
	uint8_t dataLength;
	uint8_t factCalData[20];
};


/** @ingroup messages
 * Decode a FactoryCalibrationReadData message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FactoryCalibrationReadData struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFactoryCalibrationReadData(const int8_t* message, int length, struct freespace_FactoryCalibrationReadData* s);

/**   @ingroup messages 
 * This is sent from the FT dongle towards the host to indicate the current status of the dongle
 */
struct freespace_FTDongleStatusResponse {
	// ID = 8
	// subID = 0x15

	/** 0: DUT power is disabled, 1: DUT power is enabled */
	uint8_t power;

	/** 0: DUT reset is deasserted, 1: DUT is reset asserted */
	uint8_t reset;

	/** 0: presence signal is deasserted, 1: presence signal is asserted */
	uint8_t presence;

	/** Status:
	    -0xFF: This is not a factory calibration dongle. Not guaranteed to be supported.
	    -0x00: Idle. The factory calibration dongle is waiting to be connected to a Loop.
	    -0x01: Connecting. The factory calibration dongle is currently debouncing its Loop presence detect line or synchronizing with a Loop.
	    -0x02: Connected. The factory calibration dongle is fully connected to a Loop. Communications have been established and command/response messages can be sent to/received from the Loop. */
	uint8_t status;
};


/** @ingroup messages
 * Decode a FTDongleStatusResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FTDongleStatusResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFTDongleStatusResponse(const int8_t* message, int length, struct freespace_FTDongleStatusResponse* s);

/**   @ingroup messages 
 * This message is sent in response to a statistics request.
 */
struct freespace_StatisticsResponse {
	// ID = 8
	// subID = 0x16
	int16_t stackSpace;
	int16_t runTime;
};


/** @ingroup messages
 * Decode a StatisticsResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_StatisticsResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeStatisticsResponse(const int8_t* message, int length, struct freespace_StatisticsResponse* s);

/**   @ingroup messages 
 * This message is used during Zebra System test to collection link(RF+USB) strength statistics.
 */
struct freespace_ZebraSystemTestResponse {
	// ID = 8
	// subID = 0x1F

	/** 1 to reset, 0 for normal */
	uint8_t reset;

	/** 0 - RF+USB, 1 - USB */
	uint8_t mode;
	uint16_t pcSequence;
	uint16_t dongleOutSequence;
	uint16_t rfSequence;
	uint16_t dongleInSequence;
};


/** @ingroup messages
 * Decode a ZebraSystemTestResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_ZebraSystemTestResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeZebraSystemTestResponse(const int8_t* message, int length, struct freespace_ZebraSystemTestResponse* s);

/**   @ingroup messages 
 * This is sent from the polled device towards the host to convey the product ID information.
 */
struct freespace_ProductIDResponse {
	// ID = 8
	// subID = 0x20
	uint32_t swPartNumber;
	uint32_t swBuildNumber;
	uint32_t swicn;
	int16_t swVersionPatch;
	int16_t swVersionMinor;
	int16_t swVersionMajor;
	int16_t hwPlatformID;
	int16_t hwRevision;
	uint32_t serialNumber;

	/** The device class represents the characteristics of the device providing the product ID. 
	 0: device type not known.
	 1: non-data-generating device. The Zebra dongle device class is 1
	 2: data-generating device. The Zebra loop device class is 2. */
	uint8_t deviceClass;

	/** 0: read serial number is valid, 1 read serial number is invalid; retry read until valid. */
	uint8_t invalidNS;
};


/** @ingroup messages
 * Decode a ProductIDResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_ProductIDResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeProductIDResponse(const int8_t* message, int length, struct freespace_ProductIDResponse* s);

/**   @ingroup messages 
 * This message is sent from a compliance test-ready dongle to indicate the dongle's current status.
 */
struct freespace_LinkStatus {
	// ID = 8
	// subID = 0x30
	uint8_t status;
	uint8_t mode;
	uint8_t resetStatus;
};


/** @ingroup messages
 * Decode a LinkStatus message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_LinkStatus struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeLinkStatus(const int8_t* message, int length, struct freespace_LinkStatus* s);

/**   @ingroup messages 
 * This message is sent in response to an SPI operation request generic out message.
	 This format is used for the 27 byte (Gen4) size generic in message.
 */
struct freespace_SPIOperationResponse {
	// ID = 8
	// subID = 0x36
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
	uint8_t byte9;
	uint8_t byte10;
	uint8_t byte11;
	uint8_t byte12;
	uint8_t byte13;
	uint8_t byte14;
	uint8_t byte15;
	uint8_t byte16;
	uint8_t byte17;
	uint8_t byte18;
	uint8_t byte19;
	uint8_t byte20;
	uint8_t byte21;
	uint8_t byte22;
	uint8_t byte23;
	int16_t length;
};


/** @ingroup messages
 * Decode a SPIOperationResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_SPIOperationResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeSPIOperationResponse(const int8_t* message, int length, struct freespace_SPIOperationResponse* s);

/**   @ingroup messages 
 * This message is sent from the FSRK-E adapter test module to the host in response to a request for the current configuration for event reporting.
 */
struct freespace_EventReportConfigurationResponse {
	// ID = 8
	// subID = 0x37

	/** Summary interrupt detected. */
	uint8_t intc;

	/** X-Y pointer movement data ready. */
	uint8_t xyMov;

	/** Activity classification status change */
	uint8_t acSt;

	/** Test module reset detected. */
	uint8_t reset;

	/** Motion data ready */
	uint8_t motDr;

	/** Wake-on-motion detected. */
	uint8_t wom;

	/** Motion data overflow */
	uint8_t motOv;

	/** Activity classification event detected. */
	uint8_t acEv;

	/** SDA data ready. */
	uint8_t sdaDr;

	/** SDA data overflow. */
	uint8_t sdaOv;

	/** Configuration StatusChange */
	uint8_t cfgSt;
};


/** @ingroup messages
 * Decode a EventReportConfigurationResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_EventReportConfigurationResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeEventReportConfigurationResponse(const int8_t* message, int length, struct freespace_EventReportConfigurationResponse* s);

/**   @ingroup messages 
 * This message is sent from the FSRK-E adapter test module to report that a particular
	 event, for which event reporting has been enabled, has occured.
	 The contents of the report are a set of 8 register data reports. Each 2 bytes.
	 The selection of registers reported is determined by the adapter board firmware.
 */
struct freespace_EventReport {
	// ID = 8
	// subID = 0x38
	uint16_t register0;
	uint16_t register1;
	uint16_t register2;
	uint16_t register3;
	uint16_t register4;
	uint16_t register5;
	uint16_t register6;
	uint16_t register7;
};


/** @ingroup messages
 * Decode a EventReport message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_EventReport struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeEventReport(const int8_t* message, int length, struct freespace_EventReport* s);

/**   @ingroup messages 
 * This message is sent from a handheld to a base station. It indicates that the handheld did not recognize a CRS message that the base station had sent.
	 This message will never be sent over USB
 */
struct freespace_UnknownCRSNotificationResponse {
	// ID = 8
	// subID = 0x39

	/** This is the HID report ID of the unknown message received by the handheld. */
	int16_t unknownReportID;

	/** This is the HID sub-message ID of the unknown message received by the handheld. */
	int16_t unknownSubMessageID;
};


/** @ingroup messages
 * Decode a UnknownCRSNotificationResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_UnknownCRSNotificationResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeUnknownCRSNotificationResponse(const int8_t* message, int length, struct freespace_UnknownCRSNotificationResponse* s);

/**   @ingroup messages 
 * This is sent from the loop to the dongle to convey an FSR record.
 */
struct freespace_FRSLoopReadResponse {
	// ID = 8
	// subID = 0x3A

	/** Word Offset indicates the number of words the data is offset from the beginning of the record */
	int16_t wordOffset;
	uint8_t data[20];

	/** Status:
	0: no error
	1: unrecognized FRS type
	2: busy
	3: read completed
	4: offset out of range
	5: record empty */
	uint8_t status;

	/** Data Length indicates the number of data words contained within the message, typically 5 words */
	uint8_t dataLength;
};


/** @ingroup messages
 * Decode a FRSLoopReadResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSLoopReadResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSLoopReadResponse(const int8_t* message, int length, struct freespace_FRSLoopReadResponse* s);

/**   @ingroup messages 
 * This is sent from the loop to the host to indicate status of the write operation.
 */
struct freespace_FRSLoopWriteResponse {
	// ID = 8
	// subID = 0x3D
	int16_t wordOffset;

	/** Status/Error:
	0: word received
	1: unrecognized FRS type
	2: busy
	3: write completed
	4: write mode entered already
	5: write failed
	6: data received while not in write mode
	7: invalid length
	8: record valid (the complete record passed internal validation checks)
	9:record invalid (the complete record failed internal validation checks) */
	uint8_t status;
};


/** @ingroup messages
 * Decode a FRSLoopWriteResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSLoopWriteResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSLoopWriteResponse(const int8_t* message, int length, struct freespace_FRSLoopWriteResponse* s);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSDongleReadResponse {
	// ID = 8
	// subID = 0x3B
	int16_t wordOffset;
	uint8_t data[20];
	uint8_t status;
	uint8_t dataLength;
};


/** @ingroup messages
 * Decode a FRSDongleReadResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSDongleReadResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSDongleReadResponse(const int8_t* message, int length, struct freespace_FRSDongleReadResponse* s);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSDongleWriteResponse {
	// ID = 8
	// subID = 0x3E
	int16_t wordOffset;
	uint8_t status;
};


/** @ingroup messages
 * Decode a FRSDongleWriteResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSDongleWriteResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSDongleWriteResponse(const int8_t* message, int length, struct freespace_FRSDongleWriteResponse* s);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSEFlashReadResponse {
	// ID = 8
	// subID = 0x41
	int16_t wordOffset;
	uint8_t data[20];
	uint8_t status;
	uint8_t dataLength;
};


/** @ingroup messages
 * Decode a FRSEFlashReadResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSEFlashReadResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSEFlashReadResponse(const int8_t* message, int length, struct freespace_FRSEFlashReadResponse* s);

/**   @ingroup messages 
 * Undocumented Message
 */
struct freespace_FRSEFlashWriteResponse {
	// ID = 8
	// subID = 0x42
	int16_t wordOffset;
	uint8_t status;
};


/** @ingroup messages
 * Decode a FRSEFlashWriteResponse message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FRSEFlashWriteResponse struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFRSEFlashWriteResponse(const int8_t* message, int length, struct freespace_FRSEFlashWriteResponse* s);

/**   @ingroup messages 
 * This message is sent from the COP2USB adapter test module to the host.
	 It carries the entire message receieved over the coprocessor interface.
	 This message is used to test the FSP to coprocessor data flow.
 */
struct freespace_FSP2CoprocessorMessage {
	// ID = 8
	// subID = 0x3C

	/** Length is the length of the payload in bytes */
	uint8_t length;
	uint8_t payload[16];
};


/** @ingroup messages
 * Decode a FSP2CoprocessorMessage message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_FSP2CoprocessorMessage struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeFSP2CoprocessorMessage(const int8_t* message, int length, struct freespace_FSP2CoprocessorMessage* s);

/**   @ingroup messages 
 * This report conveys status and error information from the device bootloader.
 */
struct freespace_LoopBootloaderStatus {
	// ID = 8
	// subID = 0x43
	uint8_t normalLaunch;
	uint8_t internalAppValid;
	uint8_t internalAppNotValid;
	uint8_t flashProtectionChanged;
	uint8_t upgradeStarted;
	uint8_t validateStarted;
	uint8_t errorOccured;

	/** Bootloader Error Codes:
    0: no error
    1: bad flash
    2: unsupported version
    3: incompatible hardware
    4: invalid image
    5: flash erase error
    6: flash write error
    7: bad encryption key
    8: CRC error
    9: bad write
    10: overflow error */
	uint8_t errorCode;
};


/** @ingroup messages
 * Decode a LoopBootloaderStatus message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_LoopBootloaderStatus struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeLoopBootloaderStatus(const int8_t* message, int length, struct freespace_LoopBootloaderStatus* s);


/** @ingroup messages
 * Encode a BatteryLevelRequest message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeBatteryLevelRequest(int8_t* message, int maxlength);

/**   @ingroup messages 
 * Indicates the battery strength of the handheld unit.
 */
struct freespace_BatteryLevel {
	// ID = 10

	/** A percentage of the operating voltage range (0-100%) */
	int16_t batteryStrength;
};


/** @ingroup messages
 * Decode a BatteryLevel message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_BatteryLevel struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeBatteryLevel(const int8_t* message, int length, struct freespace_BatteryLevel* s);

/**   @ingroup messages 
 * Conveys the motion relative to the body frame of the Freespace handheld device. 
 The data have been processed to remove tremor and other unwanted side effects.
 */
struct freespace_BodyFrame {
	// ID = 32
	uint8_t button1;
	uint8_t button2;
	uint8_t button3;
	uint8_t button4;
	uint8_t button5;
	int16_t deltaX;
	int16_t deltaY;
	int16_t deltaWheel;

	/** A monotonically increasing integer generated by the Freespace sensor board at a nominal rate of 125 Hz.
	Can be used to correlate body frame messages with the user frame messages */
	uint32_t sequenceNumber;

	/** Linear Acceleration is reported in SI units (cm/s^2) with an exponent of -1. X is positive forward. Y is positive right. Z is positive down wrt handheld frame of reference. */
	int16_t linearAccelX;
	int16_t linearAccelY;
	int16_t linearAccelZ;

	/** Angular Velocity is reported in units of rad/s with an exponent of -3. X positive is tilt right(roll). Y positive it tilt up(pitch). Z positive is turn right(yaw) wrt the handheld device frame of reference. */
	int16_t angularVelX;
	int16_t angularVelY;
	int16_t angularVelZ;
};


/** @ingroup messages
 * Decode a BodyFrame message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_BodyFrame struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeBodyFrame(const int8_t* message, int length, struct freespace_BodyFrame* s);

/**   @ingroup messages 
 * Conveys the handheld device position and orientation with respect to a user frame of reference. 
The gravity acceleration vector points up along the negative Z  axis.
 */
struct freespace_UserFrame {
	// ID = 33
	uint8_t button1;
	uint8_t button2;
	uint8_t button3;
	uint8_t button4;
	uint8_t button5;
	int16_t deltaX;
	int16_t deltaY;
	int16_t deltaWheel;

	/** Correlates the position report with the Body Frame Motion Report */
	uint32_t sequenceNumber;

	/** Linear Offset is in units of meters. X positive is right. Y positive is near. Z positive is down wrt the user frame of reference. */
	int16_t linearPosX;
	int16_t linearPosY;
	int16_t linearPosZ;

	/** Angular Position is in dimensionless units. The axes are given in quaternion form where A, B, C, D represent the real, i, j, and k coefficients. */
	int16_t angularPosA;
	int16_t angularPosB;
	int16_t angularPosC;
	int16_t angularPosD;
};


/** @ingroup messages
 * Decode a UserFrame message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_UserFrame struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeUserFrame(const int8_t* message, int length, struct freespace_UserFrame* s);

/**   @ingroup messages 
 * This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features.
 */
struct freespace_DataMotionControl {
	// ID = 34

	/** Enable Body Motion: when set to 1 enables Body Frame Motion reports. */
	uint8_t enableBodyMotion;

	/** Enable User Position: when set to 1 enables User Frame Position reports */
	uint8_t enableUserPosition;

	/** Inhibit Power Manager: when set to 1 disables the power management feature that automatically stops sending motion reports after a period of no motion. */
	uint8_t inhibitPowerManager;

	/** Enable Mouse Movement: when set to 1 enables Mouse Movement reports. */
	uint8_t enableMouseMovement;

	/** Disable Freespace: when set to 1 disables the Freespace motion sensing system to conserve power. No pointer or motion reports are sent regardless of the value of the other bits. */
	uint8_t disableFreespace;
};


/** @ingroup messages
 * Encode a DataMotionControl message.
 *
 * @param s the freespace_DataMotionControl struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encodeDataMotionControl(const struct freespace_DataMotionControl* s, int8_t* message, int maxlength);

/**   @ingroup messages 
 * The button state report is sent from the handheld to the base station over the CRS link to indicate a change in the state of the buttons on the handheld.
 */
struct freespace_ButtonState {
	// ID = 128
	uint8_t leftButton;
	uint8_t rightButton;
	uint8_t scrollButton;
	uint8_t park;
};


/** @ingroup messages
 * Decode a ButtonState message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_ButtonState struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeButtonState(const int8_t* message, int length, struct freespace_ButtonState* s);

/**   @ingroup messages 
 * The button state report is sent from the handheld to the base station over the CRS link to indicate a change in the state of the buttons on the handheld.
 */
struct freespace_ScrollMotion {
	// ID = 129
	int16_t deltaWheel;
};


/** @ingroup messages
 * Decode a ScrollMotion message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_ScrollMotion struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decodeScrollMotion(const int8_t* message, int length, struct freespace_ScrollMotion* s);


/** @ingroup messages
 * An enumeration for all the types of messages that can exist. Used in freespace_message
 * to determine the type of message contained in the union
 */
enum MessageTypes {
    FREESPACE_MESSAGE_MOUSEMOVEMENT = 0,
    FREESPACE_MESSAGE_CONSUMERCONTROL = 1,
    FREESPACE_MESSAGE_KEYBOARDREPORT = 2,
    FREESPACE_MESSAGE_COPROCESSOROUTREPORT = 3,
    FREESPACE_MESSAGE_COPROCESSORINREPORT = 4,
    FREESPACE_MESSAGE_CONFIGURATIONMESSAGE = 5,
    FREESPACE_MESSAGE_FACTORYCALIBRATIONREADREQUEST = 6,
    FREESPACE_MESSAGE_DONGLERESET = 7,
    FREESPACE_MESSAGE_FTDONGLESTATUSREQUEST = 8,
    FREESPACE_MESSAGE_ZEBRASYSTEMTEST = 9,
    FREESPACE_MESSAGE_LOOPLEDSETREQUEST = 10,
    FREESPACE_MESSAGE_FREQUENCYFIXREQUEST = 11,
    FREESPACE_MESSAGE_SOFTWARERESETMESSAGE = 12,
    FREESPACE_MESSAGE_DONGLERFSUPRESSHOMEFREQUENCYMESSAGE = 13,
    FREESPACE_MESSAGE_SPIOPERATIONMESSAGE = 14,
    FREESPACE_MESSAGE_EVENTREPORTCONFIGSETREQUEST = 15,
    FREESPACE_MESSAGE_UNKNOWNCRSNOTIFICATION = 16,
    FREESPACE_MESSAGE_FRSLOOPREADREQUEST = 17,
    FREESPACE_MESSAGE_FRSLOOPWRITEREQUEST = 18,
    FREESPACE_MESSAGE_FRSLOOPWRITEDATA = 19,
    FREESPACE_MESSAGE_FRSDONGLEREADREQUEST = 20,
    FREESPACE_MESSAGE_FRSDONGLEWRITEREQUEST = 21,
    FREESPACE_MESSAGE_FRSDONGLEWRITEDATA = 22,
    FREESPACE_MESSAGE_FRSEFLASHREADREQUEST = 23,
    FREESPACE_MESSAGE_FRSEFLASHWRITEREQUEST = 24,
    FREESPACE_MESSAGE_FRSEFLASHWRITEDATA = 25,
    FREESPACE_MESSAGE_LOOPBOOTLOADERCOMMAND = 26,
    FREESPACE_MESSAGE_GEN4SDAFORMAT = 27,
    FREESPACE_MESSAGE_FACTORYCALIBRATIONREADDATA = 28,
    FREESPACE_MESSAGE_FTDONGLESTATUSRESPONSE = 29,
    FREESPACE_MESSAGE_STATISTICSRESPONSE = 30,
    FREESPACE_MESSAGE_ZEBRASYSTEMTESTRESPONSE = 31,
    FREESPACE_MESSAGE_PRODUCTIDRESPONSE = 32,
    FREESPACE_MESSAGE_LINKSTATUS = 33,
    FREESPACE_MESSAGE_SPIOPERATIONRESPONSE = 34,
    FREESPACE_MESSAGE_EVENTREPORTCONFIGURATIONRESPONSE = 35,
    FREESPACE_MESSAGE_EVENTREPORT = 36,
    FREESPACE_MESSAGE_UNKNOWNCRSNOTIFICATIONRESPONSE = 37,
    FREESPACE_MESSAGE_FRSLOOPREADRESPONSE = 38,
    FREESPACE_MESSAGE_FRSLOOPWRITERESPONSE = 39,
    FREESPACE_MESSAGE_FRSDONGLEREADRESPONSE = 40,
    FREESPACE_MESSAGE_FRSDONGLEWRITERESPONSE = 41,
    FREESPACE_MESSAGE_FRSEFLASHREADRESPONSE = 42,
    FREESPACE_MESSAGE_FRSEFLASHWRITERESPONSE = 43,
    FREESPACE_MESSAGE_FSP2COPROCESSORMESSAGE = 44,
    FREESPACE_MESSAGE_LOOPBOOTLOADERSTATUS = 45,
    FREESPACE_MESSAGE_BATTERYLEVEL = 46,
    FREESPACE_MESSAGE_BODYFRAME = 47,
    FREESPACE_MESSAGE_USERFRAME = 48,
    FREESPACE_MESSAGE_DATAMOTIONCONTROL = 49,
    FREESPACE_MESSAGE_BUTTONSTATE = 50,
    FREESPACE_MESSAGE_SCROLLMOTION = 51,
};

/** @ingroup messages
 * freespace_message has an enum which defines the type of the message contained
 * and a union of all the possible message structs. 
 */
struct freespace_message {
    int messageType;
    union {
		struct freespace_MouseMovement mouseMovement;
		struct freespace_ConsumerControl consumerControl;
		struct freespace_KeyboardReport keyboardReport;
		struct freespace_CoprocessorOutReport coprocessorOutReport;
		struct freespace_CoprocessorInReport coprocessorInReport;
		struct freespace_ConfigurationMessage configurationMessage;
		struct freespace_FactoryCalibrationReadRequest factoryCalibrationReadRequest;
		struct freespace_DongleReset dongleReset;
		struct freespace_FTDongleStatusRequest fTDongleStatusRequest;
		struct freespace_ZebraSystemTest zebraSystemTest;
		struct freespace_LoopLEDSetRequest loopLEDSetRequest;
		struct freespace_FrequencyFixRequest frequencyFixRequest;
		struct freespace_SoftwareResetMessage softwareResetMessage;
		struct freespace_DongleRFSupressHomeFrequencyMessage dongleRFSupressHomeFrequencyMessage;
		struct freespace_SPIOperationMessage sPIOperationMessage;
		struct freespace_EventReportConfigSetRequest eventReportConfigSetRequest;
		struct freespace_UnknownCRSNotification unknownCRSNotification;
		struct freespace_FRSLoopReadRequest fRSLoopReadRequest;
		struct freespace_FRSLoopWriteRequest fRSLoopWriteRequest;
		struct freespace_FRSLoopWriteData fRSLoopWriteData;
		struct freespace_FRSDongleReadRequest fRSDongleReadRequest;
		struct freespace_FRSDongleWriteRequest fRSDongleWriteRequest;
		struct freespace_FRSDongleWriteData fRSDongleWriteData;
		struct freespace_FRSEFlashReadRequest fRSEFlashReadRequest;
		struct freespace_FRSEFlashWriteRequest fRSEFlashWriteRequest;
		struct freespace_FRSEFlashWriteData fRSEFlashWriteData;
		struct freespace_LoopBootloaderCommand loopBootloaderCommand;
		struct freespace_Gen4SDAFormat gen4SDAFormat;
		struct freespace_FactoryCalibrationReadData factoryCalibrationReadData;
		struct freespace_FTDongleStatusResponse fTDongleStatusResponse;
		struct freespace_StatisticsResponse statisticsResponse;
		struct freespace_ZebraSystemTestResponse zebraSystemTestResponse;
		struct freespace_ProductIDResponse productIDResponse;
		struct freespace_LinkStatus linkStatus;
		struct freespace_SPIOperationResponse sPIOperationResponse;
		struct freespace_EventReportConfigurationResponse eventReportConfigurationResponse;
		struct freespace_EventReport eventReport;
		struct freespace_UnknownCRSNotificationResponse unknownCRSNotificationResponse;
		struct freespace_FRSLoopReadResponse fRSLoopReadResponse;
		struct freespace_FRSLoopWriteResponse fRSLoopWriteResponse;
		struct freespace_FRSDongleReadResponse fRSDongleReadResponse;
		struct freespace_FRSDongleWriteResponse fRSDongleWriteResponse;
		struct freespace_FRSEFlashReadResponse fRSEFlashReadResponse;
		struct freespace_FRSEFlashWriteResponse fRSEFlashWriteResponse;
		struct freespace_FSP2CoprocessorMessage fSP2CoprocessorMessage;
		struct freespace_LoopBootloaderStatus loopBootloaderStatus;
		struct freespace_BatteryLevel batteryLevel;
		struct freespace_BodyFrame bodyFrame;
		struct freespace_UserFrame userFrame;
		struct freespace_DataMotionControl dataMotionControl;
		struct freespace_ButtonState buttonState;
		struct freespace_ScrollMotion scrollMotion;
    };
};

/** @ingroup messages
 * Decode an arbitrary message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_message struct to decode into
 * @return FREESPACE_SUCESS or an error code
 */
LIBFREESPACE_API int freespace_decode_message(const int8_t* message, int length, struct freespace_message* s);

/** @ingroup messages
 * Encode an arbitrary message.
 *
 * @param s the freespace_message struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @return the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encode_message(const struct freespace_message* s, int8_t* message, int maxlength);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_CODECS_H_ */
