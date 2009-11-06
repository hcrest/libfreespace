#!/usr/bin/env python
#
# This file is part of libfreespace.
#  
# Copyright (c) 2009 Hillcrest Laboratories, Inc. 
# 
# libfreespace is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# Define some constants for convenience
name = 'name'
size = 'size'
bits = 'bits'
nibbles = 'nibbles'
cType = 'cType'
id = 'id'
RESERVED = 'RESERVED'
ConstantID = 'constID'
SubMessageID = 'subId'
Documentation = 'comment'


# ---------------------------------------------------------------------------------------
# -------------------------------- Message Class ----------------------------------------
# ---------------------------------------------------------------------------------------


class Message:
    def __init__(self, name="", encode=False, decode=False, shouldGenerate=True):
        self.name=name
        self.encode = encode
        self.decode = decode
        self.Fields = []
        self.Documentation = "Undocumented Message"
        self.enumName = "FREESPACE_MESSAGE_" + self.name.upper()
        self.structName = self.name[0].lower() + self.name[1:]
        self.shouldGenerate = shouldGenerate
    
    def getMessageSize(self):
        toRet = 1 # Add one for the opening message type byte
        if self.ID.has_key('subId'):
            toRet += self.ID['subId']['size']
        for element in self.Fields:
            toRet += element['size']
        return toRet
    
    def hasUnReservedFields(self):
        if len(self.Fields) > 0:
            for field in self.Fields:
                if field['name'] != 'RESERVED':
                    return True
        return False
        
messages = []

# ---------------------------------------------------------------------------------------
# -------------------------------- HID Reports ------------------------------------------
# ---------------------------------------------------------------------------------------
    
    
# ---------------------------------------------------------------------------------------
# Mouse Movement Message
MouseMovement = Message("MouseMovement", decode=True)
MouseMovement.Documentation = "DEPRECATED. This report implements a HID Mouse with 8 buttons and a scroll wheel"
MouseMovement.ID = {
    ConstantID:2
}
MouseMovement.Fields = [
    {name:"buttons",    size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:'button6'},{name:'button7'},{name:'button8'}]},
    {name:"deltaX",     size:1, cType:'int8_t', Documentation:"Changes in pointer location in dimensionless units. Positive moves pointer right."},
    {name:"deltaY",     size:1, cType:'int8_t', Documentation:"Changes in pointer location in dimensionless units. Positive moves pointer down."},
    {name:"deltaWheel", size:1, cType:'int8_t', Documentation:"Reports scroll wheel motion in detents. Up is positive."}
]

messages.append(MouseMovement)

# ---------------------------------------------------------------------------------------
# Consumer Control Report
ConsumerControl = Message("ConsumerControl", decode=True)
ConsumerControl.Documentation = "DEPRECATED: Indicates buttons that operate a consumer electronics product."
ConsumerControl.ID = {
    ConstantID:3
}
ConsumerControl.Fields = [
    {name:"usageID",         size:1, cType:'uint8_t', Documentation:"A 12-bit field that carries a consumer control usage code"},
    {name:"numAndUsage",     size:1, nibbles:[{name:'usageID11_8'}, {name:'numPad'}]},
    {name:"functionButtons", size:1, cType:'uint8_t', Documentation:"Indicates up to 255 button presses"}
]

messages.append(ConsumerControl)

# ---------------------------------------------------------------------------------------
# Keyboard Report
KeyboardReport = Message("KeyboardReport", decode=True)
KeyboardReport.Documentation = "DEPRECATED. Indicates buttons from a computer keyboard or keypad. Refer to the USB HID Usage Tables Document Keyboard Page for more information"
KeyboardReport.ID = {
    ConstantID:4
}
KeyboardReport.Fields = [
    {name:"modifiers", size:1, bits:[{name:'LCtrl'},{name:'LShift'},{name:'LAlt'},{name:'LGui'},{name:'RCtrl'},{name:'RShift'},{name:'RAlt'},{name:'RGui'}]},
    {name:RESERVED,    size:1},
    {name:"keyID",     size:1, cType:'int16_t'}
]

messages.append(KeyboardReport)

# ---------------------------------------------------------------------------------------
# Coprocessor Pass-through Out Report
CoprocessorOutReport = Message("CoprocessorOutReport", encode=True)
CoprocessorOutReport.Documentation = "Reserved for passing message through from the Freespace coprocessor to the USB host."
CoprocessorOutReport.ID = {
    ConstantID:5
}
CoprocessorOutReport.Fields = [
    {name:"payloadLength", size:1, cType:'int16_t'},
    {name:"payload",       size:14}
]

messages.append(CoprocessorOutReport)

# ---------------------------------------------------------------------------------------
# Coprocessor Pass-through In Report
CoprocessorInReport = Message("CoprocessorInReport", decode=True)
CoprocessorInReport.Documentation = "Reserved for proprietary diagnostics, testing, and debugging use."
CoprocessorInReport.ID = {
    ConstantID:6
}
CoprocessorInReport.Fields = [
    {name:"payloadLength", size:1, cType:'uint8_t'},
    {name:"payload",       size:14}
]

messages.append(CoprocessorInReport)

# ---------------------------------------------------------------------------------------
# Battery Level Request Message
BatteryLevelRequest = Message("BatteryLevelRequest", encode=True)
BatteryLevelRequest.Documentation = "Sent by the host to request the battery status of the handheld unit."
BatteryLevelRequest.ID = {
    ConstantID:9
}
BatteryLevelRequest.Fields = [
    {name:RESERVED, size:1}
]

messages.append(BatteryLevelRequest)

# ---------------------------------------------------------------------------------------
# Battery Level Message
BatteryLevel = Message("BatteryLevel", decode=True)
BatteryLevel.Documentation = "Indicates the battery strength of the handheld unit."
BatteryLevel.ID = {
    ConstantID:10
}
BatteryLevel.Fields = [
    {name:"batteryStrength", size:1, cType:'uint8_t', Documentation:"A percentage of the operating voltage range (0-100%)"},
    {name:RESERVED,        size:2}
]

messages.append(BatteryLevel)

# ---------------------------------------------------------------------------------------
# Body Frame Message
BodyFrameMessage = Message("BodyFrame", decode=True)
BodyFrameMessage.Documentation = "Conveys the motion relative to the body frame of the Freespace handheld device. \n The data have been processed to remove tremor and other unwanted side effects."
BodyFrameMessage.ID = {
    ConstantID:32
}
BodyFrameMessage.Fields = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:RESERVED},{name:RESERVED},{name:RESERVED}]},
    {name:"deltaX",         size:1, cType:'int8_t'},
    {name:"deltaY",         size:1, cType:'int8_t'},
    {name:"deltaWheel",     size:1, cType:'int8_t'},
    {name:"sequenceNumber", size:4, cType:'uint32_t', Documentation:"A monotonically increasing integer generated by the Freespace sensor board at a nominal rate of 125 Hz.\n\tCan be used to correlate body frame messages with the user frame messages"},
    {name:"linearAccelX",   size:2, cType:'int16_t', Documentation: "Linear Acceleration is reported in SI units (cm/s^2) with an exponent of -1. X is positive forward. Y is positive right. Z is positive down wrt handheld frame of reference."},
    {name:"linearAccelY",   size:2, cType:'int16_t'},
    {name:"linearAccelZ",   size:2, cType:'int16_t'},
    {name:"angularVelX",    size:2, cType:'int16_t', Documentation: "Angular Velocity is reported in units of rad/s with an exponent of -3. X positive is tilt right(roll). Y positive it tilt up(pitch). Z positive is turn right(yaw) wrt the handheld device frame of reference."},
    {name:"angularVelY",    size:2, cType:'int16_t'},
    {name:"angularVelZ",    size:2, cType:'int16_t'}
]

messages.append(BodyFrameMessage)

# ---------------------------------------------------------------------------------------
# User Frame Message
UserFrameMessage = Message("UserFrame", decode=True)
UserFrameMessage.Documentation = "Conveys the handheld device position and orientation with respect to a user frame of reference. \nThe gravity acceleration vector points up along the negative Z  axis."
UserFrameMessage.ID = {
    ConstantID:33
}
UserFrameMessage.Fields = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:RESERVED},{name:RESERVED},{name:RESERVED}]},
    {name:"deltaX",         size:1, cType:'int8_t'},
    {name:"deltaY",         size:1, cType:'int8_t'},
    {name:"deltaWheel",     size:1, cType:'int8_t'},
    {name:"sequenceNumber", size:4, cType:'uint32_t', Documentation:"Correlates the position report with the Body Frame Motion Report"},
    {name:"linearPosX",     size:2, cType:'int16_t', Documentation:"Linear Offset is in units of meters. X positive is right. Y positive is near. Z positive is down wrt the user frame of reference."},
    {name:"linearPosY",     size:2, cType:'int16_t'},
    {name:"linearPosZ",     size:2, cType:'int16_t'},
    {name:"angularPosA",    size:2, cType:'int16_t', Documentation:"Angular Position is in dimensionless units. The axes are given in quaternion form where A, B, C, D represent the real, i, j, and k coefficients."},
    {name:"angularPosB",    size:2, cType:'int16_t'},
    {name:"angularPosC",    size:2, cType:'int16_t'},
    {name:"angularPosD",    size:2, cType:'int16_t'}
]

messages.append(UserFrameMessage)

# ---------------------------------------------------------------------------------------
# Data Motion Control Message
DataMotion = Message("DataMotionControl", encode=True)
DataMotion.Documentation = "This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features."
DataMotion.ID = {
    ConstantID:34
}
DataMotion.Fields = [
    {name:"flags", size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 enables Body Frame Motion reports."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 enables User Frame Position reports"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 disables the power management feature that automatically stops sending motion reports after a period of no motion."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 enables Mouse Movement reports."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 disables the Freespace motion sensing system to conserve power. No pointer or motion reports are sent regardless of the value of the other bits."},
                                 {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]}
]

messages.append(DataMotion)

# ---------------------------------------------------------------------------------------
# ---------------------- Generic Out Reports (ID 7) -------------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# Configuration Message
ConfigurationMessage = Message("ConfigurationMessage", encode=True)
ConfigurationMessage.Documenation = "Used by the host to configure both the loop and dongle."
ConfigurationMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:1}
}
ConfigurationMessage.Fields = [
    {name:"SDA", size:1, cType:'int16_t', Documentation:"SDA(1)/NORMAL(0)"},
    {name:RESERVED,   size:5}
]

messages.append(ConfigurationMessage)

# ---------------------------------------------------------------------------------------
# Pairing Message
PairingMessage = Message("PairingMessage", encode=True)
PairingMessage.Documentation = "Used by the host to put the dongle into normal pairing mode."
PairingMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:13}
}
PairingMessage.Fields = [
    {name:RESERVED,   size:6}
]

messages.append(PairingMessage)

# ---------------------------------------------------------------------------------------
# Debug Message
DebugMessage = Message("DebugMessage", encode=True)
DebugMessage.Documentation = "Used for debug messages."
DebugMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:15}
}
DebugMessage.Fields = [
    {name:RESERVED,   size:6}
]

messages.append(DebugMessage)

# ---------------------------------------------------------------------------------------
# Factory Calibration Read Request Message
FactoryCalibrationReadReq = Message("FactoryCalibrationReadRequest", encode=True)
FactoryCalibrationReadReq.Documentation = "This is sent from dongle towards the loop to request a chunk of cal data to be sent. This is used in the Zebra system architecture."
FactoryCalibrationReadReq.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:16}
}
FactoryCalibrationReadReq.Fields = [
    {name:"wordOffsetRequested", size:1, cType:'uint8_t'},
    {name:RESERVED,   size:5}
]

messages.append(FactoryCalibrationReadReq)

# ---------------------------------------------------------------------------------------
# Dongle Reset to Bootloader
DongleReset = Message("DongleReset", encode=True)
DongleReset.Documentation = "This is sent from host to dongle to trigger a reset into the bootloader.\nThe 32-bit loaderkey value is placed at a special location in RAM and the system is reset.\nIf the loader key has the proper value, the bootloader will then take control.\nOtherwise the bootloader will transfer control to the application immediately."
DongleReset.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:20}
}
DongleReset.Fields = [
    {name:"loaderKey", size:4, cType:'uint32_t'},
    {name:RESERVED,    size:2}
]

messages.append(DongleReset)

# ---------------------------------------------------------------------------------------
# Factory Test Dongle Status Request
FTDongleStatusRequest = Message("FTDongleStatusRequest", encode=True)
FTDongleStatusRequest.Documentation = "This is sent from the host towards the FT dongle to update or request the current status of the dongle"
FTDongleStatusRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:21}
}
FTDongleStatusRequest.Fields = [
    {name:'flags', size:1, bits:[{name:'power',     Documentation:"Power:\n\t 0:disable DUT power\n\t 1:enable DUT power"},
                                 {name:'reset',     Documentation:"Reset:\n\t 0:deassert DUT reset signal\n\t 1:assert DUT reset signal"},
                                 {name:'presence',  Documentation:"Presence:\n\t 0:deassert presence signal\n\t 1:assert presence signal"},
                                 {name:RESERVED},
                                 {name:RESERVED},
                                 {name:RESERVED},
                                 {name:RESERVED},
                                 {name:'operation', Documentation:"Operation:\n\t 0:read status. Values for presence, reset, and power are don't cares\n\t 1: update status. Status is set to values indicated by presence, power, and reset"}]},
    {name:RESERVED, size:5}
]

messages.append(FTDongleStatusRequest)

# ---------------------------------------------------------------------------------------
# Statistics Request
StatisticsRequest = Message("StatisticsRequest", encode=True)
StatisticsRequest.Documentation = "This is sent from the host towards the loop to request a statistics report."
StatisticsRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:22}
}
StatisticsRequest.Fields = [
    {name:RESERVED, size:6}
]

messages.append(StatisticsRequest)

# ---------------------------------------------------------------------------------------
# Zebra System Test
ZebraSystemTest = Message("ZebraSystemTest", encode=True)
ZebraSystemTest.Documentation = "This message is used during Zebra Sytem test to collect link (RF+USB) strength statistics."
ZebraSystemTest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:31}
}
ZebraSystemTest.Fields = [
    {name:'config',     size:1, bits:[{name:'reset',         Documentation:"1 to reset, 0 for normal"},
                                      {name:'mode',  size:2, Documentation:"0: RF+USB, 1: USB"},
                                      {name:RESERVED},{name:RESERVED},{name:RESERVED},{name:RESERVED},{name:RESERVED}]},
    {name:RESERVED,     size:1},
    {name:"pcSequence", size:2, cType:'int16_t'},
    {name:RESERVED,     size:2}
]

messages.append(ZebraSystemTest)

# ---------------------------------------------------------------------------------------
# Product ID Request Message
ProductIDRequest = Message("ProductIDRequest", encode=True)
ProductIDRequest.Documentation = "This is sent from the host to the attached device(dongle) to request the product ID information. The dongle will forward this request to the Loop."
ProductIDRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:32}
}
ProductIDRequest.Fields = [
    {name:RESERVED, size:6}
]

messages.append(ProductIDRequest)

# ---------------------------------------------------------------------------------------
# Loop LED Set Request
LoopLEDSetRequest = Message("LoopLEDSetRequest", encode=True)
LoopLEDSetRequest.Documentation = "This request causes the Loop to set a status LED to a particular value"
LoopLEDSetRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:34}
}
LoopLEDSetRequest.Fields = [
    {name:'onOff',     size:1, cType:'uint8_t', Documentation:"0: Off, 1: On"},
    {name:'selectLED', size:1, cType:'uint8_t', Documentation:"LED Select: 0-green(all devices)\n\t 1-red(all devices)\n\t 2-yellow(all devices)\n\t 3-blue(all devices)\n\t 4-FTA green\n\t 5-FTA red\n\t 6-S2U yellow\n\t 7-S2U blue\n\t 8-Dominion LED PWM\n\t 9-Dominion LED1\n\t 10-Dominion LED2"},
    {name:RESERVED,    size:4}
]

messages.append(LoopLEDSetRequest)

# ---------------------------------------------------------------------------------------
# Always On Request Message
AlwaysOnRequest = Message("AlwaysOnRequest", encode=True)
AlwaysOnRequest.Documentation = "This message forces the Loop into an always on state. It is relayed to the Loop from the dongle."
AlwaysOnRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:49}
}
AlwaysOnRequest.Fields = [
    {name:RESERVED,    size:6}
]

messages.append(AlwaysOnRequest)

# ---------------------------------------------------------------------------------------
# Frequency Fix Request Message
FrequencyFixRequest = Message("FrequencyFixRequest", encode=True)
FrequencyFixRequest.Documentation = "This message causes the FR frequencies of the selected device to be fixed at channels 0-4. The last byte selects the device.\n\t When the loop is selected it is put into a mode where it does not require the dongle to transmit and where it does not go to sleep."
FrequencyFixRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:50}
}
FrequencyFixRequest.Fields = [
    {name:'channel0', size:1, cType:'uint8_t'},
    {name:'channel1', size:1, cType:'uint8_t'},
    {name:'channel2', size:1, cType:'uint8_t'},
    {name:'channel3', size:1, cType:'uint8_t'},
    {name:'channel4', size:1, cType:'uint8_t'},
    {name:'device',   size:1, cType:'uint8_t', Documentation:"1 for dongle, 2 for loop"}
]

messages.append(FrequencyFixRequest)

# ---------------------------------------------------------------------------------------
# Software Reset Message
SoftwareReset = Message("SoftwareResetMessage", encode=True)
SoftwareReset.Documentation = "This message software resets the selected device."
SoftwareReset.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:51}
}
SoftwareReset.Fields = [
    {name:"device", size:1, cType:'uint8_t', Documentation:"1 for dongle"},
    {name:RESERVED, size:5}
]

messages.append(SoftwareReset)

# ---------------------------------------------------------------------------------------
# Dongle RF Disable Message
DongleRFDisableMessage = Message("DongleRFDisableMessage", encode=True)
DongleRFDisableMessage.Documentation = "This message disables the RF on the dongle. The RF is disabled until the next soft/hard reset."
DongleRFDisableMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:52}
}
DongleRFDisableMessage.Fields = [
    {name:RESERVED, size:6}
]

messages.append(DongleRFDisableMessage)

# ---------------------------------------------------------------------------------------
# Dongle RF Supress Home Frequency Message
RFSupressMessage = Message("DongleRFSupressHomeFrequencyMessage", encode=True)
RFSupressMessage.Documentation = "This message is for RF Home frequency supression on the dongle.\n\n\t\
802.11 defines the peak of a channel to cover the range +-11MHz from the center frequency of the channel.\n\t\
Hillcrest adds an extra 1MHz to this boundary, so Low and High should be -12MHz from the center channel of the 802.11\n\t\
and +12MHz from the center channel of 802.11 respectively. These values must be in the range [1,82].\n\t\
To disable home frequency suppression, set either Low or High to be out-of-range. 0xFF is the preferred value for disabling suppression."
RFSupressMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:53}
}
RFSupressMessage.Fields = [
    {name:'low',  size:1, cType:'uint8_t'},
    {name:'high', size:1, cType:'uint8_t'},                        
    {name:RESERVED, size:4}
]

messages.append(RFSupressMessage)

# ---------------------------------------------------------------------------------------
# SPI Operation Message
SPIOperationMessage = Message("SPIOperationMessage", encode=True)
SPIOperationMessage.Documentation = "This message carries a generic SPI operation to be sent to a test module, such as FSRK-E.\n\tThe SPI response message is sent back to the host."
SPIOperationMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:54}
}
SPIOperationMessage.Fields = [
    {name:'byte0', size:1, cType:'uint8_t'},
    {name:'byte1', size:1, cType:'uint8_t'},
    {name:'byte2', size:1, cType:'uint8_t'},
    {name:'byte3', size:1, cType:'uint8_t'},
    {name:'byte4', size:1, cType:'uint8_t'},
    {name:'byte5', size:1, cType:'uint8_t'}
]

messages.append(SPIOperationMessage)

# ---------------------------------------------------------------------------------------
# Event Report Configuration Set Request
EventReportConfigSetRequest = Message("EventReportConfigSetRequest", encode=True)
EventReportConfigSetRequest.Documentation = "This message is sent from the host to an FSRK-E adapter test module,\n\t to configure which events will cause the test module to send an event report message to the host.\n\t A bit set to one indicates the reporting for that event is enabled."
EventReportConfigSetRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:55}
}
EventReportConfigSetRequest.Fields = [
    {name:'flags0', size:1, bits:[{name:'intc',  Documentation:"Summary interrupt detected."},
                                  {name:'xyMov', Documentation:"X-Y pointer movement data ready."},
                                  {name:'acSt',  Documentation:"Activity classification status change"},
                                  {name:'reset', Documentation:"Test module reset detected."},
                                  {name:'motDr', Documentation:"Motion data ready"},
                                  {name:'wom',   Documentation:"Wake-on-motion detected."},
                                  {name:'motOv', Documentation:"Motion data overflow"},
                                  {name:'acEv',  Documentation:"Activity classification event detected."}]},
    {name:'flags1', size:1, bits:[{name:'sdaDr', Documentation:"SDA data ready."},
                                  {name:'sdaOv', Documentation:"SDA data overflow."},
                                  {name:'cfgSt', Documentation:"Configuration StatusChange"},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED}]},
    {name:RESERVED, size:4}
]

messages.append(EventReportConfigSetRequest)

# ---------------------------------------------------------------------------------------
# Event Report Configuration Get Request
EventReportConfigGetRequest = Message("EventReportConfigGetRequest", encode=True)
EventReportConfigGetRequest.Documentation = "This message is sent from the host to an FSRK-E adapter test module to request the current configuration for event reports."
EventReportConfigGetRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:56}
}
EventReportConfigGetRequest.Fields = [
    {name:RESERVED, size:6}
]

messages.append(EventReportConfigGetRequest)

# ---------------------------------------------------------------------------------------
# Unknown CRS Notification
UnknownCRSNotification = Message("UnknownCRSNotification", encode=True)
UnknownCRSNotification.Documentation = "This message is sent from a base station handheld. It indicates that the base station did not\n\t recognize a CRS message that the handheld had sent. This message will never be sent over USB."
UnknownCRSNotification.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:57}
}
UnknownCRSNotification.Fields = [
    {name:'unknownReportID',     size:1, cType:'uint8_t', Documentation:"This is the HID report ID of the unknown message received by the base station."},
    {name:'unknownSubMessageID', size:1, cType:'uint8_t', Documentation:"This is the HID sub-message ID of the unknown message received by the base station."},
    {name:RESERVED, size:4}
]

messages.append(UnknownCRSNotification)

# ---------------------------------------------------------------------------------------
# FRS Read Loop Request Message
FRSLoopReadRequest = Message("FRSLoopReadRequest", encode=True)
FRSLoopReadRequest.Documentation = "This is sent from dongle towards the loop to request flash record to be sent.\n\tThe data sent starts from the word offset and continues through to the end of the record."
FRSLoopReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSLoopReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Write Loop Request Message
FRSLoopWriteRequest = Message("FRSLoopWriteRequest", encode=True)
FRSLoopWriteRequest.Documentation = "This is sent from the host towards the loop to initiate a flash record write.\n\tA length of 0 will cause the record to be invalidated."
FRSLoopWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:61}
}
FRSLoopWriteRequest.Fields = [
    {name:"length", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSLoopWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Write Loop Data Message
FRSLoopWriteData = Message("FRSLoopWriteData", encode=True)
FRSLoopWriteData.Documentation = "This message is sent from the host towards the loop to write data to the record a previous write request indicated."
FRSLoopWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:63}
}
FRSLoopWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"data",       size:4}
]

messages.append(FRSLoopWriteData)

# ---------------------------------------------------------------------------------------
# FRS Read Dongle Request Message
FRSDongleReadRequest = Message("FRSDongleReadRequest", encode=True)
FRSDongleReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSDongleReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Write Dongle Request Message
FRSDongleWriteRequest = Message("FRSDongleWriteRequest", encode=True)
FRSDongleWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteRequest.Fields = [
    {name:"length", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSDongleWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Write Dongle Data Message
FRSDongleWriteData = Message("FRSDongleWriteData", encode=True)
FRSDongleWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:64}
}
FRSDongleWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"data",       size:4}
]

messages.append(FRSDongleWriteData)

# ---------------------------------------------------------------------------------------
# FRS Read EFlash Request Message
FRSEFlashReadRequest = Message("FRSEFlashReadRequest", encode=True)
FRSEFlashReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSEFlashReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Write EFlash Request Message
FRSEFlashWriteRequest = Message("FRSEFlashWriteRequest", encode=True)
FRSEFlashWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteRequest.Fields = [
    {name:"length", size:2, cType:'int16_t'},
    {name:"FRStype",    size:2, cType:'int16_t'},
    {name:RESERVED,   size:2}
]

messages.append(FRSEFlashWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Write EFlash Data Message
FRSEFlashWriteData = Message("FRSEFlashWriteData", encode=True)
FRSEFlashWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:67}
}
FRSEFlashWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'int16_t'},
    {name:"data",       size:4}
]

messages.append(FRSEFlashWriteData)

# ---------------------------------------------------------------------------------------
# Loop Bootloader Command
LoopBootloaderCommand = Message("LoopBootloaderCommand", encode=True)
LoopBootloaderCommand.Documentation = "This is sent from host to a handheld(loop) to trigger a reset into the bootloader.\n\t\
The 32-bit loaderkey command value is placed at a special location in RAM and the system is reset.\n\t\
If the loaderkey has the proper value, the bootloader will act on the command.\n\t\
Otherwise the bootloader will transfer control to the application immediately."
LoopBootloaderCommand.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:68}
}
LoopBootloaderCommand.Fields = [
    {name:"loaderKeyCommand", size:4, cType:'uint32_t', Documentation:"Loader Key Commands:\n\t \
    Launch application: 0xb9e3aa6e \n\t \
    Upgrade Image:      0x461c5592 \n\t \
    Validate Image:     0xc452801e"},
    {name:RESERVED, size:2}
]

messages.append(LoopBootloaderCommand)

# ---------------------------------------------------------------------------------------
# Loop Bootloader Status Request
LoopBootloaderStatusRequest = Message("LoopBootloaderStatusRequest", encode=True)
LoopBootloaderStatusRequest.Documentation = "This is sent from host to a handheld to request that it send a bootloader status report message."
LoopBootloaderStatusRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:69}
}
LoopBootloaderStatusRequest.Fields = [
    {name:RESERVED, size:6}
]

messages.append(LoopBootloaderStatusRequest)

# ---------------------------------------------------------------------------------------
# ------------------------- Generic In Reports (ID 8) -----------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# Gen4 SDA Format
Gen4SDAFormat = Message("Gen4SDAFormat", decode=True)
Gen4SDAFormat.Documentation = "The Gen4 SDA format is used by FSRK Gen4 dongle, which has USB product ID = 0xC007."
Gen4SDAFormat.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:1}
}
Gen4SDAFormat.Fields = [
    {name:'reportCount', size:1, cType:'int16_t'},
    {name:'buttons',     size:1, bits:[{name:'left'},{name:'right'},{name:'scroll'},{name:'hold'},{name:RESERVED},{name:RESERVED},{name:'triangle'},{name:RESERVED}]},
    {name:'deltaWheel',  size:1, cType:'int8_t'},
    {name:'accelX',      size:2, cType:'int16_t'},
    {name:'accelY',      size:2, cType:'int16_t'},
    {name:'accelZ',      size:2, cType:'int16_t'},
    {name:'rotationX',   size:2, cType:'int16_t'},
    {name:'rotationY',   size:2, cType:'int16_t'},
    {name:'rotationZ',   size:2, cType:'int16_t'},
    {name:'xMov',        size:1, cType:'int8_t'},
    {name:'yMov',        size:1, cType:'int8_t'},
    {name:'sampleBase',  size:4, cType:'uint32_t'},
    {name:'command',     size:2, cType:'uint16_t'}
]

messages.append(Gen4SDAFormat)

# ---------------------------------------------------------------------------------------
# CRC Error Indication Message
CRCErrorIndication = Message("CRCErrorIndication", decode=True)
CRCErrorIndication.Documentation = "The factory test dongle will send this message when a CRC message occurred over the wired factory test interface."
CRCErrorIndication.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:4}
}
CRCErrorIndication.Fields = [
    {name:RESERVED,     size:25}
]

messages.append(CRCErrorIndication)

# ---------------------------------------------------------------------------------------
# Factory Calibration Read Data
FactoryCalibrationReadData = Message("FactoryCalibrationReadData", decode=True)
FactoryCalibrationReadData.Documentation = "This is sent from the loop to the dongle to convey the factory calibration data. This is only used by the Zebra architecture products."
FactoryCalibrationReadData.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:16}
}
FactoryCalibrationReadData.Fields = [
    {name:"wordOffset",  size:1, cType:'uint8_t', Documentation:"wordOffset indicates the number of words offset from the beginning of the factory calibration data the message begins."},
    {name:"nibs",        size:1, nibbles:[{name:'dataLength', Documentation:"Indicates the number of data words contained within the message, typically 5 words."}, {name:RESERVED}]},
    {name:"factCalData", size:20},
    {name:RESERVED,      size:23}
]

messages.append(FactoryCalibrationReadData)

# ---------------------------------------------------------------------------------------
# Factory Test Dongle Status Response
FTDongleStatusResponse = Message("FTDongleStatusResponse", decode=True)
FTDongleStatusResponse.Documentation = "This is sent from the FT dongle towards the host to indicate the current status of the dongle"
FTDongleStatusResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:21}
}
FTDongleStatusResponse.Fields = [
    {name:'flags', size:1, bits:[{name:'power',    Documentation:"0: DUT power is disabled, 1: DUT power is enabled"},
                                 {name:'reset',    Documentation:"0: DUT reset is deasserted, 1: DUT is reset asserted"},
                                 {name:'presence', Documentation:"0: presence signal is deasserted, 1: presence signal is asserted"}]},
    {name:'status', size:1, cType:'uint8_t', Documentation:"Status:\n\t\
    -0xFF: This is not a factory calibration dongle. Not guaranteed to be supported.\n\t\
    -0x00: Idle. The factory calibration dongle is waiting to be connected to a Loop.\n\t\
    -0x01: Connecting. The factory calibration dongle is currently debouncing its Loop presence detect line or synchronizing with a Loop.\n\t\
    -0x02: Connected. The factory calibration dongle is fully connected to a Loop. Communications have been established and command/response messages can be sent to/received from the Loop."},
    {name:RESERVED, size:23}
]

messages.append(FTDongleStatusResponse)

# ---------------------------------------------------------------------------------------
# Statistics Response
StatisticsResponse = Message("StatisticsResponse", decode=True)
StatisticsResponse.Documentation = "This message is sent in response to a statistics request."
StatisticsResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:22}
}
StatisticsResponse.Fields = [
    {name:'stackSpace', size:2, cType:'int16_t'},
    {name:'runTime',    size:2, cType:'int16_t'},
    {name:RESERVED,     size:21}
]

messages.append(StatisticsResponse)

# ---------------------------------------------------------------------------------------
# Zebra System Test
ZebraSystemTestResponse = Message("ZebraSystemTestResponse", decode=True)
ZebraSystemTestResponse.Documentation = "This message is used during Zebra System test to collection link(RF+USB) strength statistics."
ZebraSystemTestResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:31}
}
ZebraSystemTestResponse.Fields = [
    {name:'modeReset',         size:1, bits:[{name:'reset',        Documentation:"1 to reset, 0 for normal"},
                                             {name:'mode', size:2, Documentation:"0 - RF+USB, 1 - USB"},
                                             {name:RESERVED},{name:RESERVED},{name:RESERVED},{name:RESERVED},{name:RESERVED}]},
    {name:RESERVED,            size:1},
    {name:'pcSequence',        size:2, cType:'uint16_t'},
    {name:'dongleOutSequence', size:2, cType:'uint16_t'},
    {name:'rfSequence',        size:2, cType:'uint16_t'},
    {name:'dongleInSequence',  size:2, cType:'uint16_t'},
    {name:RESERVED,            size:15}
]

messages.append(ZebraSystemTestResponse)

# ---------------------------------------------------------------------------------------
# Product ID Response Message
ProductIDResponse = Message("ProductIDResponse", decode=True)
ProductIDResponse.Documentation = "This is sent from the polled device towards the host to convey the product ID information."
ProductIDResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:32}
}
ProductIDResponse.Fields = [
    {name:'swPartNumber',   size:4, cType:'uint32_t'},
    {name:'swBuildNumber',  size:4, cType:'uint32_t'},
    {name:'swicn',          size:4, cType:'uint32_t'},
    {name:'swVersionPatch', size:2, cType:'int16_t'},
    {name:'swVersionMinor', size:1, cType:'uint8_t'},
    {name:'swVersionMajor', size:1, cType:'uint8_t'},
    {name:'hwPlatformID',   size:1, cType:'uint8_t'},
    {name:'hwRevision',     size:1, cType:'uint8_t'},
    {name:'serialNumber',   size:4, cType:'uint32_t'},
    {name:'deviceClass',    size:1, bits:[{name:'deviceClass', size:7, Documentation:"The device class represents the characteristics of the device providing the product ID. \n\t 0: device type not known.\n\t 1: non-data-generating device. The Zebra dongle device class is 1\n\t 2: data-generating device. The Zebra loop device class is 2."},
                                          {name:'invalidNS',            Documentation:"0: read serial number is valid, 1 read serial number is invalid; retry read until valid."}]},
    {name:RESERVED,         size:2}
]

messages.append(ProductIDResponse)

# ---------------------------------------------------------------------------------------
# Link Quality Status Message
LinkStatusMessage = Message("LinkStatus", decode=True)
LinkStatusMessage.Documentation = "This message is sent from a compliance test-ready dongle to indicate the dongle's current status."
LinkStatusMessage.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:48}
}
LinkStatusMessage.Fields = [
    {name:"status",     size:1, cType:'uint8_t'},
    {name:"mode",       size:1, cType:'uint8_t'},
    {name:"resetStatus",size:1, cType:'uint8_t'},
    {name:RESERVED,     size:22}
]

messages.append(LinkStatusMessage)

# ---------------------------------------------------------------------------------------
# Always On Response Message
AlwaysOnResponse = Message("AlwaysOnResponse", decode=True)
AlwaysOnResponse.Documentation = "This message is sent from a the loop to acknowledge an always on mode request message."
AlwaysOnResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:49}
}
AlwaysOnResponse.Fields = [
    {name:RESERVED,     size:25}
]

messages.append(AlwaysOnResponse)

# ---------------------------------------------------------------------------------------
# SPI Operation Response Message
SPIOperationResponse = Message("SPIOperationResponse", decode=True)
SPIOperationResponse.Documentation = "This message is sent in response to an SPI operation request generic out message.\n\t This format is used for the 27 byte (Gen4) size generic in message."
SPIOperationResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:54}
}
SPIOperationResponse.Fields = [
    {name:'byte0',  size:1, cType:'uint8_t'},
    {name:'byte1',  size:1, cType:'uint8_t'},
    {name:'byte2',  size:1, cType:'uint8_t'},
    {name:'byte3',  size:1, cType:'uint8_t'},
    {name:'byte4',  size:1, cType:'uint8_t'},
    {name:'byte5',  size:1, cType:'uint8_t'},
    {name:'byte6',  size:1, cType:'uint8_t'},
    {name:'byte7',  size:1, cType:'uint8_t'},
    {name:'byte8',  size:1, cType:'uint8_t'},
    {name:'byte9',  size:1, cType:'uint8_t'},
    {name:'byte10', size:1, cType:'uint8_t'},
    {name:'byte11', size:1, cType:'uint8_t'},
    {name:'byte12', size:1, cType:'uint8_t'},
    {name:'byte13', size:1, cType:'uint8_t'},
    {name:'byte14', size:1, cType:'uint8_t'},
    {name:'byte15', size:1, cType:'uint8_t'},
    {name:'byte16', size:1, cType:'uint8_t'},
    {name:'byte17', size:1, cType:'uint8_t'},
    {name:'byte18', size:1, cType:'uint8_t'},
    {name:'byte19', size:1, cType:'uint8_t'},
    {name:'byte20', size:1, cType:'uint8_t'},
    {name:'byte21', size:1, cType:'uint8_t'},
    {name:'byte22', size:1, cType:'uint8_t'},
    {name:'byte23', size:1, cType:'uint8_t'},
    {name:'length', size:1, cType:'uint8_t'},
]

messages.append(SPIOperationResponse)

# ---------------------------------------------------------------------------------------
# Event Report Configuration Response
EventReportConfigurationResponse = Message("EventReportConfigurationResponse", decode=True)
EventReportConfigurationResponse.Documentation = "This message is sent from the FSRK-E adapter test module to the host in response to a request for the current configuration for event reporting."
EventReportConfigurationResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:55}
}
EventReportConfigurationResponse.Fields = [
    {name:'flags0', size:1, bits:[{name:'intc',  Documentation:"Summary interrupt detected."},
                                  {name:'xyMov', Documentation:"X-Y pointer movement data ready."},
                                  {name:'acSt',  Documentation:"Activity classification status change"},
                                  {name:'reset', Documentation:"Test module reset detected."},
                                  {name:'motDr', Documentation:"Motion data ready"},
                                  {name:'wom',   Documentation:"Wake-on-motion detected."},
                                  {name:'motOv', Documentation:"Motion data overflow"},
                                  {name:'acEv',  Documentation:"Activity classification event detected."}]},
    {name:'flags1', size:1, bits:[{name:'sdaDr', Documentation:"SDA data ready."},
                                  {name:'sdaOv', Documentation:"SDA data overflow."},
                                  {name:'cfgSt', Documentation:"Configuration StatusChange"},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED},
                                  {name:RESERVED}]},
    {name:RESERVED, size:23}    
]

messages.append(EventReportConfigurationResponse)

# ---------------------------------------------------------------------------------------
# Event Report Message
EventReport = Message("EventReport", decode=True)
EventReport.Documentation = "This message is sent from the FSRK-E adapter test module to report that a particular\n\t \
event, for which event reporting has been enabled, has occured.\n\t \
The contents of the report are a set of 8 register data reports. Each 2 bytes.\n\t \
The selection of registers reported is determined by the adapter board firmware."
EventReport.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:56}
}
EventReport.Fields = [
    {name:'register0', size:2, cType:'uint16_t'},
    {name:'register1', size:2, cType:'uint16_t'},
    {name:'register2', size:2, cType:'uint16_t'},
    {name:'register3', size:2, cType:'uint16_t'},
    {name:'register4', size:2, cType:'uint16_t'},
    {name:'register5', size:2, cType:'uint16_t'},
    {name:'register6', size:2, cType:'uint16_t'},
    {name:'register7', size:2, cType:'uint16_t'},
    {name:RESERVED,    size:9}    
]

messages.append(EventReport)

# ---------------------------------------------------------------------------------------
# Unknown CRS Notification
UnknownCRSNotificationResponse = Message("UnknownCRSNotificationResponse", decode=True)
UnknownCRSNotificationResponse.Documentation = "This message is sent from a handheld to a base station. It indicates that the handheld did not recognize a CRS message that the base station had sent.\n\t This message will never be sent over USB"
UnknownCRSNotificationResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:57}
}
UnknownCRSNotificationResponse.Fields = [
    {name:"unknownReportID",     size:1, cType:'uint8_t', Documentation:"This is the HID report ID of the unknown message received by the handheld."},
    {name:"unknownSubMessageID", size:1, cType:'uint8_t', Documentation:"This is the HID sub-message ID of the unknown message received by the handheld."},
    {name:RESERVED,     size:23}    
]

messages.append(UnknownCRSNotificationResponse)

# ---------------------------------------------------------------------------------------
# FRS Read Loop Response Message
FRSLoopReadResponse = Message("FRSLoopReadResponse", decode=True)
FRSLoopReadResponse.Documentation = "This is sent from the loop to the dongle to convey an FSR record."
FRSLoopReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status', Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:RESERVED,     size:2}    
]

messages.append(FRSLoopReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Write Loop Response Message
FRSLoopWriteResponse = Message("FRSLoopWriteResponse", decode=True)
FRSLoopWriteResponse.Documentation = "This is sent from the loop to the host to indicate status of the write operation."
FRSLoopWriteResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:61}
}
FRSLoopWriteResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t'},
    {name:"status",     size:1,  cType:'uint8_t', Documentation:"Status/Error:\n\t\
0: word received\n\t\
1: unrecognized FRS type\n\t\
2: busy\n\t\
3: write completed\n\t\
4: write mode entered already\n\t\
5: write failed\n\t\
6: data received while not in write mode\n\t\
7: invalid length\n\t\
8: record valid (the complete record passed internal validation checks)\n\t\
9:record invalid (the complete record failed internal validation checks)"},
    {name:RESERVED,     size:22}
]

messages.append(FRSLoopWriteResponse)

# ---------------------------------------------------------------------------------------
# FRS Read Dongle Response Message
FRSDongleReadResponse = Message("FRSDongleReadResponse", decode=True)
FRSDongleReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t'},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status'},{name:'dataLength'}]},
    {name:RESERVED,     size:2}    
]

messages.append(FRSDongleReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Write Dongle Response Message
FRSDongleWriteResponse = Message("FRSDongleWriteResponse", decode=True)
FRSDongleWriteResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t'},
    {name:"status",     size:1,  cType:'uint8_t'},
    {name:RESERVED,     size:22}
]

messages.append(FRSDongleWriteResponse)

# ---------------------------------------------------------------------------------------
# FRS Read EFlash Response Message
FRSEFlashReadResponse = Message("FRSEFlashReadResponse", decode=True)
FRSEFlashReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t'},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status'},{name:'dataLength'}]},
    {name:RESERVED,     size:2}    
]

messages.append(FRSEFlashReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Write EFlash Response Message
FRSEFlashWriteResponse = Message("FRSEFlashWriteResponse", decode=True)
FRSEFlashWriteResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t'},
    {name:"status",     size:1,  cType:'uint8_t'},
    {name:RESERVED,     size:22}
]

messages.append(FRSEFlashWriteResponse)

# ---------------------------------------------------------------------------------------
# FSP2Coprocessor Message
FSP2CoprocessorMessage = Message("FSP2CoprocessorMessage", decode=True)
FSP2CoprocessorMessage.Documentation = "This message is sent from the COP2USB adapter test module to the host.\n\t \
It carries the entire message receieved over the coprocessor interface.\n\t \
This message is used to test the FSP to coprocessor data flow."
FSP2CoprocessorMessage.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:60}
}
FSP2CoprocessorMessage.Fields = [
    {name:'length',  size:1, cType:'uint8_t', Documentation:"Length is the length of the payload in bytes"},
    {name:'payload', size:16},
    {name:RESERVED,  size:8}    
]

messages.append(FSP2CoprocessorMessage)

# ---------------------------------------------------------------------------------------
# Loop Bootloader Status
LoopBootloaderStatus = Message("LoopBootloaderStatus", decode=True)
LoopBootloaderStatus.Documentation = "This report conveys status and error information from the device bootloader."
LoopBootloaderStatus.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:67}
}
LoopBootloaderStatus.Fields = [
    {name:RESERVED,    size:2},
    {name:'flags',     size:1, bits:[{name:'normalLaunch'},
                                     {name:'internalAppValid'},
                                     {name:'internalAppNotValid'},
                                     {name:'flashProtectionChanged'},
                                     {name:'upgradeStarted'},
                                     {name:'validateStarted'},
                                     {name:'errorOccured'},
                                     {name:RESERVED}]},
    {name:RESERVED,    size:3},
    {name:'errorCode', size:1, cType:'uint8_t', Documentation:"\
Bootloader Error Codes:\n\
    0: no error\n\
    1: bad flash\n\
    2: unsupported version\n\
    3: incompatible hardware\n\
    4: invalid image\n\
    5: flash erase error\n\
    6: flash write error\n\
    7: bad encryption key\n\
    8: CRC error\n\
    9: bad write\n\
    10: overflow error"},
    {name:RESERVED,    size:18}
]

messages.append(LoopBootloaderStatus)

# ---------------------------------------------------------------------------------------
# ------------------------- Extended Out Report (ID 15) ---------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# SPI Operation Request
SPIOperationRequest = Message("SPIOperationRequest", encode=True, shouldGenerate=False)
SPIOperationRequest.Documentation = "The SPI operation request is sent from the host to the FSRK to perform the specified SPI transaction.\n\t\
This format carries up to 24 bytes of 12 16-bit words. The FSRK sends a Generic In SPI operation response with the data returned from the SPI transaction."
SPIOperationRequest.ID = {
    ConstantID:15,
    SubMessageID:{size:1, id:2}
}
SPIOperationRequest.Fields = [
    {name:'length', size:1, cType:'uint8_t'},
    {name:RESERVED, size:1},
    {name:'byte0',  size:1, cType:'uint8_t'},
    {name:'byte1',  size:1, cType:'uint8_t'},
    {name:'byte2',  size:1, cType:'uint8_t'},
    {name:'byte3',  size:1, cType:'uint8_t'},
    {name:'byte4',  size:1, cType:'uint8_t'},
    {name:'byte5',  size:1, cType:'uint8_t'},
    {name:'byte6',  size:1, cType:'uint8_t'},
    {name:'byte7',  size:1, cType:'uint8_t'},
    {name:'byte8',  size:1, cType:'uint8_t'},
    {name:'byte9',  size:1, cType:'uint8_t'},
    {name:'byte10', size:1, cType:'uint8_t'},
    {name:'byte11', size:1, cType:'uint8_t'},
    {name:'byte12', size:1, cType:'uint8_t'},
    {name:'byte13', size:1, cType:'uint8_t'},
    {name:'byte14', size:1, cType:'uint8_t'},
    {name:'byte15', size:1, cType:'uint8_t'},
    {name:'byte16', size:1, cType:'uint8_t'},
    {name:'byte17', size:1, cType:'uint8_t'},
    {name:'byte18', size:1, cType:'uint8_t'},
    {name:'byte19', size:1, cType:'uint8_t'},
    {name:'byte20', size:1, cType:'uint8_t'},
    {name:'byte21', size:1, cType:'uint8_t'},
    {name:'byte22', size:1, cType:'uint8_t'},
    {name:'byte23', size:1, cType:'uint8_t'},
    {name:RESERVED, size:3}
]

messages.append(SPIOperationRequest)

# ---------------------------------------------------------------------------------------
# Coprocessor2FSP Message
Coprocessor2FSPMessage = Message("Coprocessor2FSPMessage", encode=True, shouldGenerate=False)
Coprocessor2FSPMessage.Documentation = "\
The Coprocessor2FSP message is sent from the host to the COP2USB adapter. The length\n\t\
field is the size of the coprocessor message in bytes. The payload of this message is a\n\t\
complete coprocessor message, including coprocessor message length, coprocessor\n\t\
message ID and coprocessor message payload. The coprocessor message is sent out in its\n\t\
entirety over the coprocessor SPI interface. If the length of the payload is less than 16\n\t\
bytes then only length bytes are sent. Bytes in the payload field after length bytes are ignored."
Coprocessor2FSPMessage.ID = {
    ConstantID:15,
    SubMessageID:{size:1, id:3}
}
Coprocessor2FSPMessage.Fields = [
    {name:'length', size:1, cType:'uint8_t'},
    {name:'payload', size:16},
    {name:RESERVED, size:12}
]

messages.append(Coprocessor2FSPMessage)

# ---------------------------------------------------------------------------------------
# ---------------------------- CRS Message Reports --------------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# CRS Keepalive
CRSKeepalive = Message("CRSKeepalive", decode=True)
CRSKeepalive.Documentation = "The CRS Keepalive report is used to ensure that there is always traffic on the\n\t handheld/base station CRS link. This report is never sent over the USB interface."
CRSKeepalive.ID = {
    ConstantID:11
}
CRSKeepalive.Fields = []

messages.append(CRSKeepalive)

# ---------------------------------------------------------------------------------------
# Button State
ButtonState = Message("ButtonState", decode=True)
ButtonState.Documentation = "The button state report is sent from the handheld to the base station over the CRS link to indicate a change in the state of the buttons on the handheld."
ButtonState.ID = {
    ConstantID:128
}
ButtonState.Fields = [
    {name:'buttons', size:1, bits:[{name:'leftButton'}, {name:'rightButton'}, {name:'scrollButton'}, {name:'park'},
                                   {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]}
]

messages.append(ButtonState)

# ---------------------------------------------------------------------------------------
# Scroll Motion
ScrollMotion = Message("ScrollMotion", decode=True)
ScrollMotion.Documentation = "The button state report is sent from the handheld to the base station over the CRS link to indicate a change in the state of the buttons on the handheld."
ScrollMotion.ID = {
    ConstantID:129
}
ScrollMotion.Fields = [
    {name:'deltaWheel', size:1, cType:'int8_t'}
]

messages.append(ScrollMotion)

# ---------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------
# ---------------------------------------------------------------------------------------