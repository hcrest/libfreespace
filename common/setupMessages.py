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
        self.Fields = [[], [], []]  # keep a separate list for each version of the HID message protocol
        self.ID = [{}, {}, {}]      # keep a separate dictionary for each version of theHID message protocol. The Fields and ID entries must correspond.
        self.Documentation = "Undocumented Message"
        self.enumName = "FREESPACE_MESSAGE_" + self.name.upper()
        if self.decode:
            self.className = "FreespaceMsgIn" + self.name
        else:
            self.className = "FreespaceMsgOut" + self.name
        self.structName = self.name[0].lower() + self.name[1:]
        self.shouldGenerate = shouldGenerate
        # Information about firmware versions
        self.addedVersion = ""      # what is the first firmware version you can use this message on
        self.deprecatedVersion = "" # when you should stop using this message
        self.removedVersion = ""    # when the message is no longer in the firmware
        self.appliesTo = []         # what firmware (i.e. software part numbers) does this message apply to
    
    def getMessageSize(self):
        toRet = 1 # Add one for the opening message type byte
        if self.ID[1].has_key('subId'):
            toRet += self.ID[1]['subId']['size']
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
# Coprocessor Pass-through Out Report
CoprocessorOutReport = Message("CoprocessorOutReport", encode=True)
CoprocessorOutReport.Documentation = "Reserved for passing message through from the Freespace coprocessor to the USB host."
CoprocessorOutReport.addedVersion = "1.0.0"
CoprocessorOutReport.deprecatedVersion = ""
CoprocessorOutReport.removedVersion = ""
CoprocessorOutReport.appliesTo = [10001602]
CoprocessorOutReport.ID[1] = {
    ConstantID:5
}
CoprocessorOutReport.Fields[1] = [
    {name:"payloadLength", size:1, cType:'uint8_t'},
    {name:"payload",       size:14, cType:'uint8_t'}
]

messages.append(CoprocessorOutReport)

# ---------------------------------------------------------------------------------------
# Coprocessor Pass-through In Report
CoprocessorInReport = Message("CoprocessorInReport", decode=True)
CoprocessorInReport.Documentation = "Used for passing messages through from the USB host to the Freespace coprocessor interface."
CoprocessorInReport.addedVersion = "1.0.0"
CoprocessorInReport.deprecatedVersion = ""
CoprocessorInReport.removedVersion = ""
CoprocessorInReport.appliesTo = [10001602]
CoprocessorInReport.ID[1] = {
    ConstantID:6
}
CoprocessorInReport.Fields[1] = [
    {name:"payloadLength", size:1, cType:'uint8_t'},
    {name:"payload",       size:14, cType:'uint8_t'}
]

messages.append(CoprocessorInReport)

# ---------------------------------------------------------------------------------------
# Battery Level Request Message
BatteryLevelRequest = Message("BatteryLevelRequest", encode=True)
BatteryLevelRequest.Documentation = "Sent by the host to request the battery status of the handheld unit."
BatteryLevelRequest.addedVersion = "1.0.0"
BatteryLevelRequest.deprecatedVersion = ""
BatteryLevelRequest.removedVersion = ""
BatteryLevelRequest.appliesTo = [10001602]
BatteryLevelRequest.ID[1] = {
    ConstantID:9
}
BatteryLevelRequest.Fields[1] = [
    {name:RESERVED, size:1}
]

messages.append(BatteryLevelRequest)

# ---------------------------------------------------------------------------------------
# Battery Level Request V2 Message
BatteryLevelRequestV2 = Message("BatteryLevelRequestV2", encode=True)
BatteryLevelRequestV2.Documentation = "Sent by the host to request the battery status of the handheld unit."
BatteryLevelRequestV2.addedVersion = ""
BatteryLevelRequestV2.deprecatedVersion = ""
BatteryLevelRequestV2.removedVersion = ""
BatteryLevelRequestV2.appliesTo = []
BatteryLevelRequestV2.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:5}
}

messages.append(BatteryLevelRequestV2)

# ---------------------------------------------------------------------------------------
# Battery Level Message
BatteryLevel = Message("BatteryLevel", decode=True)
BatteryLevel.Documentation = "Indicates the battery strength of the handheld unit."
BatteryLevel.addedVersion = "1.0.0"
BatteryLevel.deprecatedVersion = ""
BatteryLevel.removedVersion = ""
BatteryLevel.appliesTo = [10001602]
BatteryLevel.ID[1] = {
    ConstantID:10
}
BatteryLevel.ID[2] = {
    ConstantID:5,
    SubMessageID:{size:1, id:5}
}
BatteryLevel.Fields[1] = [
    {name:"batteryStrength", size:1, cType:'uint8_t', Documentation:"A percentage of the operating voltage range (0-100%)"},
    {name:RESERVED,          size:2}
]
BatteryLevel.Fields[2] = [
    {name:"batteryStrength", size:1, cType:'uint8_t', Documentation:"A percentage of the operating voltage range (0-100%)"}
]

messages.append(BatteryLevel)

# ---------------------------------------------------------------------------------------
# Body Frame Message
BodyFrameMessage = Message("BodyFrame", decode=True)
BodyFrameMessage.Documentation = "Conveys the motion relative to the body frame of the Freespace handheld device. \n The data have been processed to remove tremor and other unwanted side effects."
BodyFrameMessage.addedVersion = "1.0.0"
BodyFrameMessage.deprecatedVersion = ""
BodyFrameMessage.removedVersion = ""
BodyFrameMessage.appliesTo = [10001602, 10001853]
BodyFrameMessage.ID[1] = {
    ConstantID:32
}
BodyFrameMessage.Fields[1] = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:'button6'},{name:'button7'},{name:'button8'}]},
    {name:"deltaX",         size:1, cType:'int8_t', Documentation:"X pointer movement."},
    {name:"deltaY",         size:1, cType:'int8_t', Documentation:"Y pointer movement."},
    {name:"deltaWheel",     size:1, cType:'int8_t', Documentation:"Scroll wheel movement."},
    {name:"sequenceNumber", size:4, cType:'uint32_t', Documentation:"A monotonically increasing integer generated by the Freespace sensor board at a nominal rate of 125 Hz.\n\tCan be used to correlate body frame messages with the user frame messages"},
    {name:"linearAccelX",   size:2, cType:'int16_t', Documentation:"Linear Acceleration is reported in SI units (cm/s^2) with an exponent of -1. X is positive forward. Y is positive right. Z is positive down wrt handheld frame of reference."},
    {name:"linearAccelY",   size:2, cType:'int16_t'},
    {name:"linearAccelZ",   size:2, cType:'int16_t'},
    {name:"angularVelX",    size:2, cType:'int16_t', Documentation:"Angular Velocity is reported in units of rad/s with an exponent of -3. X positive is tilt right(roll). Y positive it tilt up(pitch). Z positive is turn right(yaw) wrt the handheld device frame of reference."},
    {name:"angularVelY",    size:2, cType:'int16_t'},
    {name:"angularVelZ",    size:2, cType:'int16_t'}
]

messages.append(BodyFrameMessage)

# ---------------------------------------------------------------------------------------
# Body Frame V2 Message
# From V1 to V2 the sequence number changes from uint32_t to uint16_t. That's why there are two messages
BodyFrameV2Message = Message("BodyFrameV2", decode=True)
BodyFrameV2Message.Documentation = "Conveys the motion relative to the body frame of the Freespace handheld device. \n The data have been processed to remove tremor and other unwanted side effects."
BodyFrameV2Message.addedVersion = ""
BodyFrameV2Message.deprecatedVersion = ""
BodyFrameV2Message.removedVersion = ""
BodyFrameV2Message.appliesTo = []
BodyFrameV2Message.ID[2] = {
    ConstantID:32
}
BodyFrameV2Message.Fields[2] = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:'button6'},{name:'button7'},{name:'button8'}]},
    {name:"deltaX",         size:1, cType:'int8_t', Documentation:"X pointer movement."},
    {name:"deltaY",         size:1, cType:'int8_t', Documentation:"Y pointer movement."},
    {name:"deltaWheel",     size:1, cType:'int8_t', Documentation:"Scroll wheel movement."},
    {name:"sequenceNumber", size:2, cType:'uint16_t', Documentation:"A monotonically increasing integer generated by the Freespace sensor board at a nominal rate of 125 Hz.\n\tCan be used to correlate body frame messages with the user frame messages"},
    {name:"linearAccelX",   size:2, cType:'int16_t', Documentation:"Linear Acceleration is reported in SI units (cm/s^2) with an exponent of -1. X is positive forward. Y is positive right. Z is positive down wrt handheld frame of reference."},
    {name:"linearAccelY",   size:2, cType:'int16_t'},
    {name:"linearAccelZ",   size:2, cType:'int16_t'},
    {name:"angularVelX",    size:2, cType:'int16_t', Documentation:"Angular Velocity is reported in units of rad/s with an exponent of -3. X positive is tilt right(roll). Y positive it tilt up(pitch). Z positive is turn right(yaw) wrt the handheld device frame of reference."},
    {name:"angularVelY",    size:2, cType:'int16_t'},
    {name:"angularVelZ",    size:2, cType:'int16_t'}
]

messages.append(BodyFrameV2Message)

# ---------------------------------------------------------------------------------------
# User Frame Message
UserFrameMessage = Message("UserFrame", decode=True)
UserFrameMessage.Documentation = "Conveys the handheld device position and orientation with respect to a user frame of reference."
UserFrameMessage.addedVersion = "1.0.0"
UserFrameMessage.deprecatedVersion = ""
UserFrameMessage.removedVersion = ""
UserFrameMessage.appliesTo = [10001602, 10001853]
UserFrameMessage.ID[1] = {
    ConstantID:33
}
UserFrameMessage.Fields[1] = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:'button6'},{name:'button7'},{name:'button8'}]},
    {name:"deltaX",         size:1, cType:'int8_t', Documentation:"X pointer movement."},
    {name:"deltaY",         size:1, cType:'int8_t', Documentation:"Y pointer movement."},
    {name:"deltaWheel",     size:1, cType:'int8_t', Documentation:"Scroll wheel movement."},
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
# User Frame V2 Message
# From V1 to V2 sequenceNumber uint32_t -> uint16_t and angularPosB eliminated
UserFrameV2Message = Message("UserFrameV2", decode=True)
UserFrameV2Message.Documentation = "Conveys the handheld device position and orientation with respect to a user frame of reference."
UserFrameV2Message.addedVersion = ""
UserFrameV2Message.deprecatedVersion = ""
UserFrameV2Message.removedVersion = ""
UserFrameV2Message.appliesTo = []
UserFrameV2Message.ID[2] = {
    ConstantID:33
}
UserFrameV2Message.Fields[2] = [
    {name:"buttons",        size:1, bits:[{name:'button1'},{name:'button2'},{name:'button3'},{name:'button4'},{name:'button5'},{name:'button6'},{name:'button7'},{name:'button8'}]},
    {name:"deltaX",         size:1, cType:'int8_t', Documentation:"X pointer movement."},
    {name:"deltaY",         size:1, cType:'int8_t', Documentation:"Y pointer movement."},
    {name:"deltaWheel",     size:1, cType:'int8_t', Documentation:"Scroll wheel movement."},
    {name:"sequenceNumber", size:2, cType:'uint16_t', Documentation:"Correlates the position report with the Body Frame Motion Report"},
    {name:"linearPosX",     size:2, cType:'int16_t', Documentation:"Linear Offset is in units of meters. X positive is right. Y positive is near. Z positive is down wrt the user frame of reference."},
    {name:"linearPosY",     size:2, cType:'int16_t'},
    {name:"linearPosZ",     size:2, cType:'int16_t'},
    {name:"angularPosB",    size:2, cType:'int16_t', Documentation:"Angular Position is in dimensionless units. The axes are given in quaternion form where A, B, C, D represent the real, i, j, and k coefficients."},
    {name:"angularPosC",    size:2, cType:'int16_t'},
    {name:"angularPosD",    size:2, cType:'int16_t'}
]

messages.append(UserFrameV2Message)

# ---------------------------------------------------------------------------------------
# Data Motion Control Message
DataMotion = Message("DataMotionControl", encode=True)
DataMotion.Documentation = "DEPRECATED: This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features."
DataMotion.addedVersion = "1.0.0"
DataMotion.deprecatedVersion = "1.0.5"
DataMotion.removedVersion = ""
DataMotion.appliesTo = [10001602, 10001853]
DataMotion.ID[1] = {
    ConstantID:34
}
DataMotion.Fields[1] = [
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
# Pairing Message
PairingMessage = Message("PairingMessage", encode=True)
PairingMessage.Documentation = "Used by the host to put the dongle into pairing mode."
PairingMessage.addedVersion = "1.0.0"
PairingMessage.deprecatedVersion = ""
PairingMessage.removedVersion = ""
PairingMessage.appliesTo = [10001853]
PairingMessage.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:13}
}
PairingMessage.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:2}
}
PairingMessage.Fields[1] = [
    {name:RESERVED,   size:6}
]

messages.append(PairingMessage)

# ---------------------------------------------------------------------------------------
# Product ID Request Message
ProductIDRequest = Message("ProductIDRequest", encode=True)
ProductIDRequest.Documentation = "This is sent from the host to the attached device(dongle) to request the product ID information. The dongle will forward this request to the Loop."
ProductIDRequest.addedVersion = "1.0.0"
ProductIDRequest.deprecatedVersion = ""
ProductIDRequest.removedVersion = ""
ProductIDRequest.appliesTo = [10001602, 10001853]
ProductIDRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:32}
}
ProductIDRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:9}
}
ProductIDRequest.Fields[1] = [
    {name:RESERVED, size:6}
]

messages.append(ProductIDRequest)

# ---------------------------------------------------------------------------------------
# LED Set Request
LEDSetRequest = Message("LEDSetRequest", encode=True)
LEDSetRequest.Documentation = "This request causes the Loop or dongle to set a status LED to a particular value"
LEDSetRequest.addedVersion = "1.0.0"
LEDSetRequest.deprecatedVersion = ""
LEDSetRequest.removedVersion = ""
LEDSetRequest.appliesTo = [10001602, 10001853]
LEDSetRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:34}
}
LEDSetRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:10}
}
LEDSetRequest.Fields[1] = [
    {name:'onOff',     size:1, cType:'uint8_t', Documentation:"WP160: 0-Off, 1-On, 2-Release, FSAP160: 0-cause0, 1-cause1, 2-cause2"},
    {name:'selectLED', size:1, cType:'uint8_t', Documentation:"LED Select: 0-green(all devices)\n\t 1-red(all devices)\n\t 2-yellow(all devices)\n\t 3-blue(all devices)\n\t 4-FTA green\n\t 5-FTA red\n\t 6-S2U yellow\n\t 7-S2U blue\n\t 8-Dominion LED PWM\n\t 9-Dominion LED1\n\t 10-Dominion LED2\n\t 11-RFT LED A\n\t 12-RFT LED B"},
    {name:RESERVED,    size:4}
]
LEDSetRequest.Fields[2] = LEDSetRequest.Fields[1]

messages.append(LEDSetRequest)

# ---------------------------------------------------------------------------------------
# Link Quality Request Message
LinkQualityRequest = Message("LinkQualityRequest", encode=True)
LinkQualityRequest.Documentation = "Controls link quality status reporting"
LinkQualityRequest.addedVersion = "1.0.0"
LinkQualityRequest.deprecatedVersion = ""
LinkQualityRequest.removedVersion = ""
LinkQualityRequest.appliesTo = [10001853]
LinkQualityRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:48}
}
LinkQualityRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:3}
}
LinkQualityRequest.Fields[1] = [
    {name:'enable', size:1, cType:'uint8_t', Documentation:"0: disable status messages, 1: enable status messages"},
    {name:RESERVED, size:5}
]
LinkQualityRequest.Fields[2] = LinkQualityRequest.Fields[1]

messages.append(LinkQualityRequest)

# ---------------------------------------------------------------------------------------
# Always On Request Message
AlwaysOnRequest = Message("AlwaysOnRequest", encode=True)
AlwaysOnRequest.Documentation = "This message forces the Loop into an always on state. It is relayed to the Loop from the dongle."
AlwaysOnRequest.addedVersion = "1.0.0"
AlwaysOnRequest.deprecatedVersion = ""
AlwaysOnRequest.removedVersion = ""
AlwaysOnRequest.appliesTo = [10001602, 10001853]
AlwaysOnRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:49}
}
AlwaysOnRequest.Fields[1] = [
    {name:RESERVED,    size:6}
]

messages.append(AlwaysOnRequest)

# ---------------------------------------------------------------------------------------
# Frequency Fix Request Message
FrequencyFixRequest = Message("FrequencyFixRequest", encode=True)
FrequencyFixRequest.Documentation = "This message causes the RF frequencies of the selected device to be fixed at channels 0-4. The last byte selects the device.\n\t When the loop is selected it is put into a mode where it does not require the dongle to transmit and where it does not go to sleep."
FrequencyFixRequest.addedVersion = "1.0.0"
FrequencyFixRequest.deprecatedVersion = ""
FrequencyFixRequest.removedVersion = ""
FrequencyFixRequest.appliesTo = [10001602, 10001853]
FrequencyFixRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:50}
}
FrequencyFixRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:11}
}
FrequencyFixRequest.Fields[1] = [
    {name:'channel0', size:1, cType:'uint8_t'},
    {name:'channel1', size:1, cType:'uint8_t'},
    {name:'channel2', size:1, cType:'uint8_t'},
    {name:'channel3', size:1, cType:'uint8_t'},
    {name:'channel4', size:1, cType:'uint8_t'},
    {name:'device',   size:1, cType:'uint8_t', Documentation:"1 for dongle, 2 for loop"}
]
FrequencyFixRequest.Fields[2] = [
    {name:'channel0', size:1, cType:'uint8_t'},
    {name:'channel1', size:1, cType:'uint8_t'},
    {name:'channel2', size:1, cType:'uint8_t'},
    {name:'channel3', size:1, cType:'uint8_t'},
    {name:'channel4', size:1, cType:'uint8_t'}
]

messages.append(FrequencyFixRequest)

# ---------------------------------------------------------------------------------------
# Software Reset Message
SoftwareReset = Message("SoftwareResetMessage", encode=True)
SoftwareReset.Documentation = "This message causes the dongle to reset itself."
SoftwareReset.addedVersion = "1.0.0"
SoftwareReset.deprecatedVersion = ""
SoftwareReset.removedVersion = ""
SoftwareReset.appliesTo = [10001853]
SoftwareReset.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:51}
}
SoftwareReset.Fields[1] = [
    {name:"device", size:1, cType:'uint8_t', Documentation:"1 for dongle"},
    {name:RESERVED, size:5}
]

messages.append(SoftwareReset)

# ---------------------------------------------------------------------------------------
# Dongle RF Disable Message
DongleRFDisableMessage = Message("DongleRFDisableMessage", encode=True)
DongleRFDisableMessage.Documentation = "This message disables the RF on the dongle."
DongleRFDisableMessage.addedVersion = "1.0.0"
DongleRFDisableMessage.deprecatedVersion = ""
DongleRFDisableMessage.removedVersion = ""
DongleRFDisableMessage.appliesTo = [10001602, 10001853]
DongleRFDisableMessage.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:52}
}
DongleRFDisableMessage.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:13}
}
DongleRFDisableMessage.Fields[1] = [
    {name:RESERVED, size:6}
]

messages.append(DongleRFDisableMessage)

# ---------------------------------------------------------------------------------------
# TX Disable Message
TxDisableMessage = Message("TxDisableMessage", encode=True)
TxDisableMessage.Documentation = "This message disables the RF transmission on the dongle."
TxDisableMessage.addedVersion = ""
TxDisableMessage.deprecatedVersion = ""
TxDisableMessage.removedVersion = ""
TxDisableMessage.appliesTo = []
TxDisableMessage.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:14}
}

messages.append(TxDisableMessage)

# ---------------------------------------------------------------------------------------
# Dongle RF Supress Home Frequency Message
RFSupressMessage = Message("DongleRFSupressHomeFrequencyMessage", encode=True)
RFSupressMessage.Documentation = "This message is for RF Home frequency supression on the dongle.\n\n\t\
802.11 defines the peak of a channel to cover the range +-11MHz from the center frequency of the channel.\n\t\
Hillcrest adds an extra 1MHz to this boundary, so Low and High should be -12MHz from the center channel of the 802.11\n\t\
and +12MHz from the center channel of 802.11 respectively. These values must be in the range [1,82].\n\t\
To disable home frequency suppression, set either Low or High to be out-of-range. 0xFF is the preferred value for disabling suppression."
RFSupressMessage.addedVersion = "1.0.0"
RFSupressMessage.deprecatedVersion = ""
RFSupressMessage.removedVersion = ""
RFSupressMessage.appliesTo = [10001853]
RFSupressMessage.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:53}
}
RFSupressMessage.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:15}
}
RFSupressMessage.Fields[1] = [
    {name:'low',  size:1, cType:'uint8_t'},
    {name:'high', size:1, cType:'uint8_t'},                        
    {name:RESERVED, size:4}
]
RFSupressMessage.Fields[2] = RFSupressMessage.Fields[1]

messages.append(RFSupressMessage)

# ---------------------------------------------------------------------------------------
# FRS Loop Read Request Message
FRSLoopReadRequest = Message("FRSLoopReadRequest", encode=True)
FRSLoopReadRequest.Documentation = "This is sent from dongle towards the loop to request flash record to be sent.\n\tThe data sent starts from the word offset and continues through to the end of the record."
FRSLoopReadRequest.addedVersion = "1.0.0"
FRSLoopReadRequest.deprecatedVersion = ""
FRSLoopReadRequest.removedVersion = ""
FRSLoopReadRequest.appliesTo = [10001602, 10001853]
FRSLoopReadRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadRequest.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSLoopReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Request Message
FRSLoopWriteRequest = Message("FRSLoopWriteRequest", encode=True)
FRSLoopWriteRequest.Documentation = "This is sent from the host towards the loop to initiate a flash record write.\n\tA length of 0 will cause the record to be invalidated."
FRSLoopWriteRequest.addedVersion = "1.0.0"
FRSLoopWriteRequest.deprecatedVersion = ""
FRSLoopWriteRequest.removedVersion = ""
FRSLoopWriteRequest.appliesTo = [10001602, 10001853]
FRSLoopWriteRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:61}
}
FRSLoopWriteRequest.Fields[1] = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSLoopWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Write Request Message
FRSWriteRequest = Message("FRSWriteRequest", encode=True)
FRSWriteRequest.Documentation = "This is sent from the host towards the device to initiate a flash record write.\n\tA length of 0 will cause the record to be invalidated."
FRSWriteRequest.addedVersion = ""
FRSWriteRequest.deprecatedVersion = ""
FRSWriteRequest.removedVersion = ""
FRSWriteRequest.appliesTo = []
FRSWriteRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:6}
}
FRSWriteRequest.Fields[2] = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'}
]

messages.append(FRSWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Data Message
FRSLoopWriteData = Message("FRSLoopWriteData", encode=True)
FRSLoopWriteData.Documentation = "This message is sent from the host towards the loop to write data to the record a previous write request indicated."
FRSLoopWriteData.addedVersion = "1.0.0"
FRSLoopWriteData.deprecatedVersion = ""
FRSLoopWriteData.removedVersion = ""
FRSLoopWriteData.appliesTo = [10001602, 10001853]
FRSLoopWriteData.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:63}
}
FRSLoopWriteData.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSLoopWriteData)

# ---------------------------------------------------------------------------------------
# FRS Write Data Message
FRSWriteData = Message("FRSWriteData", encode=True)
FRSWriteData.Documentation = "This message is sent from the host towards the device to write data to the record a previous write request indicated."
FRSWriteData.addedVersion = ""
FRSWriteData.deprecatedVersion = ""
FRSWriteData.removedVersion = ""
FRSWriteData.appliesTo = []
FRSWriteData.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:7}
}
FRSWriteData.Fields[2] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSWriteData)

# ---------------------------------------------------------------------------------------
# FRS Dongle Read Request Message
FRSDongleReadRequest = Message("FRSDongleReadRequest", encode=True)
FRSDongleReadRequest.addedVersion = "1.0.0"
FRSDongleReadRequest.deprecatedVersion = ""
FRSDongleReadRequest.removedVersion = ""
FRSDongleReadRequest.appliesTo = [10001602, 10001853]
FRSDongleReadRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadRequest.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSDongleReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Read Request Message
FRSReadRequest = Message("FRSReadRequest", encode=True)
FRSReadRequest.addedVersion = ""
FRSReadRequest.deprecatedVersion = ""
FRSReadRequest.removedVersion = ""
FRSReadRequest.appliesTo = []
FRSReadRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:8}
}
FRSReadRequest.Fields[2] = [
    {name:"readOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Request Message
FRSDongleWriteRequest = Message("FRSDongleWriteRequest", encode=True)
FRSDongleWriteRequest.addedVersion = "1.0.0"
FRSDongleWriteRequest.deprecatedVersion = ""
FRSDongleWriteRequest.removedVersion = ""
FRSDongleWriteRequest.appliesTo = [10001602, 10001853]
FRSDongleWriteRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteRequest.Fields[1] = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSDongleWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Data Message
FRSDongleWriteData = Message("FRSDongleWriteData", encode=True)
FRSDongleWriteData.addedVersion = "1.0.0"
FRSDongleWriteData.deprecatedVersion = ""
FRSDongleWriteData.removedVersion = ""
FRSDongleWriteData.appliesTo = [10001602, 10001853]
FRSDongleWriteData.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:64}
}
FRSDongleWriteData.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSDongleWriteData)

# ---------------------------------------------------------------------------------------
# FRS EFlash Read Request Message
FRSEFlashReadRequest = Message("FRSEFlashReadRequest", encode=True)
FRSEFlashReadRequest.addedVersion = "1.0.0"
FRSEFlashReadRequest.deprecatedVersion = ""
FRSEFlashReadRequest.removedVersion = ""
FRSEFlashReadRequest.appliesTo = [10001602, 10001853]
FRSEFlashReadRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadRequest.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSEFlashReadRequest)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Request Message
FRSEFlashWriteRequest = Message("FRSEFlashWriteRequest", encode=True)
FRSEFlashWriteRequest.addedVersion = "1.0.0"
FRSEFlashWriteRequest.deprecatedVersion = ""
FRSEFlashWriteRequest.removedVersion = ""
FRSEFlashWriteRequest.appliesTo = [10001602, 10001853]
FRSEFlashWriteRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteRequest.Fields[1] = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSEFlashWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Data Message
FRSEFlashWriteData = Message("FRSEFlashWriteData", encode=True)
FRSEFlashWriteData.addedVersion = "1.0.0"
FRSEFlashWriteData.deprecatedVersion = ""
FRSEFlashWriteData.removedVersion = ""
FRSEFlashWriteData.appliesTo = [10001602, 10001853]
FRSEFlashWriteData.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:67}
}
FRSEFlashWriteData.Fields[1] = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSEFlashWriteData)

# ---------------------------------------------------------------------------------------
# Dongle RF Enable Message
DongleRFEnableMessage = Message("DongleRFEnableMessage", encode=True)
DongleRFEnableMessage.Documentation = "This message enables the RF on the dongle."
DongleRFEnableMessage.addedVersion = "1.0.2"
DongleRFEnableMessage.deprecatedVersion = ""
DongleRFEnableMessage.removedVersion = ""
DongleRFEnableMessage.appliesTo = [10001602, 10001853]
DongleRFEnableMessage.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:71}
}
DongleRFEnableMessage.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:12}
}
DongleRFEnableMessage.Fields[1] = [
    {name:RESERVED, size:6}
]

messages.append(DongleRFEnableMessage)

# ---------------------------------------------------------------------------------------
# Data Mode Request Message
DataModeRequest = Message("DataModeRequest", encode=True)
DataModeRequest.Documentation = "This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features."
DataModeRequest.addedVersion = "1.0.5"
DataModeRequest.deprecatedVersion = ""
DataModeRequest.removedVersion = ""
DataModeRequest.appliesTo = [10001602, 10001853]
DataModeRequest.ID[1] = {
    ConstantID:7,
    SubMessageID:{size:1, id:73}
}
DataModeRequest.ID[2] = {
    ConstantID:7,
    SubMessageID:{size:1, id:4}
}
DataModeRequest.Fields[1] = [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 enables Body Frame Motion reports."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 enables User Frame Position reports"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 disables the power management feature that automatically stops sending motion reports after a period of no motion."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 enables Mouse Movement reports."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 disables the Freespace motion sensing system to conserve power. No pointer or motion reports are sent regardless of the value of the other bits."},
                                 {name:RESERVED},
                                 {name:RESERVED},
                                 {name:'status',              Documentation:"Report current data mode: when set to causes a doata mode repsones message to be generated but does not update data mode."}]}
]
DataModeRequest.Fields[2] =  [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 enables Body Frame Motion reports."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 enables User Frame Position reports"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 disables the power management feature that automatically stops sending motion reports after a period of no motion."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 enables Mouse Movement reports."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 disables the Freespace motion sensing system to conserve power. No pointer or motion reports are sent regardless of the value of the other bits."},
                                 {name:RESERVED},
                                 {name:'aggregate',           Documentation:"Aggregate: when set, if both Body Frame and User frame are enabled, send them as a BodyUser message, which combines the two. "},
                                 {name:'status',              Documentation:"Status: Report current data mode: when set to causes a doata mode repsones message to be generated but does not update data mode."}]}
]

messages.append(DataModeRequest)

# ---------------------------------------------------------------------------------------
# ------------------------- Generic In Reports ------------------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# Pairing Response Message
PairingResponse = Message("PairingResponse", decode=True)
PairingResponse.Documentation = "Pairing response is used to either respond to pairing requests from the host or to send pairing status updates to the host that describe events during the pairing process."
PairingResponse.addedVersion = "1.0.0"
PairingResponse.deprecatedVersion = ""
PairingResponse.removedVersion = ""
PairingResponse.appliesTo = [10001853]
PairingResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:13}
}
PairingResponse.ID[2] = {
    ConstantID:5,
    SubMessageID:{size:1, id:2}
}
PairingResponse.Fields[1] = [
    {name:'flags',    size:1, bits:[{name:'pairing',     size:1, Documentation:"0: not pairing.\n\t 1: pairing"},
                                    {name:'autoPairing', size:1, Documentation:"0: dongle is not in auto-pairing\n\t1: dongle is in auto-pairing"},
                                    {name:'success',     size:1, Documentation:"0: not successful or still in progress\n\t1: successful"},
                                    {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]},
    {name:RESERVED,         size:24}
]
PairingResponse.Fields[2] = [
    {name:'flags',    size:1, bits:[{name:'pairing',     size:1, Documentation:"0: not pairing.\n\t 1: pairing"},
                                    {name:'autoPairing', size:1, Documentation:"0: dongle is not in auto-pairing\n\t1: dongle is in auto-pairing"},
                                    {name:'success',     size:1, Documentation:"0: not successful or still in progress\n\t1: successful"},
                                    {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]},
    {name:RESERVED,         size:6}
]

messages.append(PairingResponse)

# ---------------------------------------------------------------------------------------
# Product ID Response Message
ProductIDResponse = Message("ProductIDResponse", decode=True)
ProductIDResponse.Documentation = "This is sent from the polled device towards the host to convey the product ID information."
ProductIDResponse.addedVersion = "1.0.0"
ProductIDResponse.deprecatedVersion = ""
ProductIDResponse.removedVersion = ""
ProductIDResponse.appliesTo = [10001602, 10001853]
ProductIDResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:32}
}
ProductIDResponse.ID[2] = {
    ConstantID:6,
    SubMessageID:{size:1, id:9}
}
ProductIDResponse.Fields[1] = [
    {name:'swPartNumber',   size:4, cType:'uint32_t'},
    {name:'swBuildNumber',  size:4, cType:'uint32_t'},
    {name:'swicn',          size:4, cType:'uint32_t'},
    {name:'swVersionPatch', size:2, cType:'uint16_t'},
    {name:'swVersionMinor', size:1, cType:'uint8_t'},
    {name:'swVersionMajor', size:1, cType:'uint8_t'},
    {name:RESERVED,         size:2},
    {name:'serialNumber',   size:4, cType:'uint32_t'},
    {name:'deviceClass',    size:1, bits:[{name:'deviceClass', size:7, Documentation:"The device class represents the characteristics of the device providing the product ID. \n\t 0: device type not known.\n\t 1: non-data-generating device.\n\t 2: data-generating device."},
                                          {name:'invalidNS',           Documentation:"0: read serial number is valid, 1 read serial number is invalid; retry read until valid."}]},
    {name:RESERVED,         size:2}
]
ProductIDResponse.Fields[2] = [
    {name:'swPartNumber',   size:4, cType:'uint32_t'},
    {name:'swBuildNumber',  size:4, cType:'uint32_t'},
    {name:'swVersionPatch', size:2, cType:'uint16_t'},
    {name:'swVersionMinor', size:1, cType:'uint8_t'},
    {name:'swVersionMajor', size:1, cType:'uint8_t'},
    {name:'serialNumber',   size:4, cType:'uint32_t'},
    {name:'deviceClass',    size:1, bits:[{name:'deviceClass', size:6, Documentation:"The device class represents the characteristics of the device providing the product ID. \n\t 0: device type not known.\n\t 1: non-data-generating device.\n\t 2: data-generating device."},
                                          {name:'startup',             Documentation:"The device has just started up. This bit self clears after the first message is sent."},
                                          {name:'invalidNS',           Documentation:"0: read serial number is valid, 1 read serial number is invalid; retry read until valid."}]},
]

messages.append(ProductIDResponse)

# ---------------------------------------------------------------------------------------
# Link Quality Status Message
LinkStatusMessage = Message("LinkStatus", decode=True)
LinkStatusMessage.Documentation = "This message is sent from a compliance test-ready dongle to indicate the dongle's current status."
LinkStatusMessage.addedVersion = "1.0.0"
LinkStatusMessage.deprecatedVersion = ""
LinkStatusMessage.removedVersion = ""
LinkStatusMessage.appliesTo = [10001853]
LinkStatusMessage.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:48}
}
LinkStatusMessage.ID[2] = {
    ConstantID:5,
    SubMessageID:{size:1, id:3}
}
LinkStatusMessage.Fields[1] = [
    {name:"status",     size:1, cType:'uint8_t', Documentation:"0: bad\n\t1: good"},
    {name:"mode",       size:1, cType:'uint8_t', Documentation:"0: normal operation\n\t1: fixed frequency operation\n\t2: RF disabled"},
    {name:"resetStatus",size:1, cType:'uint8_t', Documentation:"0: did not occur\n\t1: occurred. Self clears."},
    {name:RESERVED,     size:22}
]
LinkStatusMessage.Fields[2] = LinkStatusMessage.Fields[1]
messages.append(LinkStatusMessage)

# ---------------------------------------------------------------------------------------
# Always On Response Message
AlwaysOnResponse = Message("AlwaysOnResponse", decode=True)
AlwaysOnResponse.Documentation = "This message is sent from a the loop to acknowledge an always on mode request message."
AlwaysOnResponse.addedVersion = "1.0.0"
AlwaysOnResponse.deprecatedVersion = ""
AlwaysOnResponse.removedVersion = ""
AlwaysOnResponse.appliesTo = [10001602, 10001853]
AlwaysOnResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:49}
}
AlwaysOnResponse.Fields[1] = [
    {name:RESERVED,     size:25}
]

messages.append(AlwaysOnResponse)

# ---------------------------------------------------------------------------------------
# FRS Read Response Message
FRSReadResponse = Message("FRSReadResponse", decode=True)
FRSReadResponse.Documentation = "This is sent from the device to the host to convey an FRS record."
FRSReadResponse.addedVersion = ""
FRSReadResponse.deprecatedVersion = ""
FRSReadResponse.removedVersion = ""
FRSReadResponse.appliesTo = []
FRSReadResponse.ID[2] = {
    ConstantID:6,
    SubMessageID:{size:1, id:8}
}
FRSReadResponse.Fields[2] = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:12, cType:'uint32_t'},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}
]

messages.append(FRSReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Loop Read Response Message
FRSLoopReadResponse = Message("FRSLoopReadResponse", decode=True)
FRSLoopReadResponse.Documentation = "This is sent from the loop to the host to convey an FRS record."
FRSLoopReadResponse.addedVersion = "1.0.0"
FRSLoopReadResponse.deprecatedVersion = ""
FRSLoopReadResponse.removedVersion = ""
FRSLoopReadResponse.appliesTo = [10001602, 10001853]
FRSLoopReadResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadResponse.Fields[1] = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20, cType:'uint32_t'},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}
]

messages.append(FRSLoopReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Write Response Message
FRSWriteResponse = Message("FRSWriteResponse", decode=True)
FRSWriteResponse.Documentation = "This is sent from the device to the host to indicate status of the write operation."
FRSWriteResponse.addedVersion = ""
FRSWriteResponse.deprecatedVersion = ""
FRSWriteResponse.removedVersion = ""
FRSWriteResponse.appliesTo = []
FRSWriteResponse.ID[2] = {
    ConstantID:5,
    SubMessageID:{size:1, id:6}
}
FRSWriteResponse.Fields[2] = [
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
9:record invalid (the complete record failed internal validation checks)"}
]

messages.append(FRSWriteResponse)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Response Message
FRSLoopWriteResponse = Message("FRSLoopWriteResponse", decode=True)
FRSLoopWriteResponse.Documentation = "This is sent from the loop to the host to indicate status of the write operation."
FRSLoopWriteResponse.addedVersion = "1.0.0"
FRSLoopWriteResponse.deprecatedVersion = ""
FRSLoopWriteResponse.removedVersion = ""
FRSLoopWriteResponse.appliesTo = [10001602, 10001853]
FRSLoopWriteResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:61}
}
FRSLoopWriteResponse.Fields[1] = [
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
# FRS Dongle Read Response Message
FRSDongleReadResponse = Message("FRSDongleReadResponse", decode=True)
FRSDongleReadResponse.Documentation = "This is sent from the dongle to the host to convey an FRS record."
FRSDongleReadResponse.addedVersion = "1.0.0"
FRSDongleReadResponse.deprecatedVersion = ""
FRSDongleReadResponse.removedVersion = ""
FRSDongleReadResponse.appliesTo = [10001602, 10001853]
FRSDongleReadResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadResponse.Fields[1] = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20, cType:'uint32_t'},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}    
]

messages.append(FRSDongleReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Response Message
FRSDongleWriteResponse = Message("FRSDongleWriteResponse", decode=True)
FRSDongleWriteResponse.Documentation = "This is sent from the dongle to the host to indicate status of the write operation."
FRSDongleWriteResponse.addedVersion = "1.0.0"
FRSDongleWriteResponse.deprecatedVersion = ""
FRSDongleWriteResponse.removedVersion = ""
FRSDongleWriteResponse.appliesTo = [10001602, 10001853]
FRSDongleWriteResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteResponse.Fields[1] = [
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

messages.append(FRSDongleWriteResponse)

# ---------------------------------------------------------------------------------------
# FRS EFlash Read Response Message
FRSEFlashReadResponse = Message("FRSEFlashReadResponse", decode=True)
FRSEFlashReadResponse.Documentation = "This is sent from the loop to the host to convey an FRS record."
FRSEFlashReadResponse.addedVersion = "1.0.0"
FRSEFlashReadResponse.deprecatedVersion = ""
FRSEFlashReadResponse.removedVersion = ""
FRSEFlashReadResponse.appliesTo = [10001602, 10001853]
FRSEFlashReadResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadResponse.Fields[1] = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20, cType:'uint32_t'},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}    
]

messages.append(FRSEFlashReadResponse)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Response Message
FRSEFlashWriteResponse = Message("FRSEFlashWriteResponse", decode=True)
FRSLoopWriteResponse.Documentation = "This is sent from the loop to the host to indicate status of the write operation."
FRSLoopWriteResponse.addedVersion = "1.0.0"
FRSLoopWriteResponse.deprecatedVersion = ""
FRSLoopWriteResponse.removedVersion = ""
FRSLoopWriteResponse.appliesTo = [10001602, 10001853]
FRSEFlashWriteResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteResponse.Fields[1] = [
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

messages.append(FRSEFlashWriteResponse)

# ---------------------------------------------------------------------------------------
# Data Mode Response Message
DataModeResponse = Message("DataModeResponse", decode=True)
DataModeResponse.Documentation = "This report acknowledges the last DataModeRequest received by the dongle."
DataModeResponse.addedVersion = "1.0.5"
DataModeResponse.deprecatedVersion = ""
DataModeResponse.removedVersion = ""
DataModeResponse.appliesTo = [10001602, 10001853]
DataModeResponse.ID[1] = {
    ConstantID:8,
    SubMessageID:{size:1, id:73}
}
DataModeResponse.ID[2] = {
    ConstantID:5,
    SubMessageID:{size:1, id:4}
}
DataModeResponse.Fields[1] = [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 Body Frame Motion reports are enabled."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 User Frame Position reports are enabled"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 the power management feature isinhibited."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 Mouse Movement reports are enabled."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 the Freespace motion sensing system disabled."},
                                 {name:RESERVED},
                                 {name:RESERVED},
                                 {name:RESERVED}]}
]
DataModeResponse.Fields[2] = [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 Body Frame Motion reports are enabled."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 User Frame Position reports are enabled"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 the power management feature isinhibited."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 Mouse Movement reports are enabled."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 the Freespace motion sensing system disabled."},
                                 {name:RESERVED},
                                 {name:'aggregate',           Documentation:"Aggregate: when set, if both Body Frame and User frame are enabled, send them as a BodyUser message, which combines the two. "},
                                 {name:RESERVED}]}
]
messages.append(DataModeResponse)

