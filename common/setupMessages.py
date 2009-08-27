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
# Coprocessor Pass-through Out Report
CoprocessorOutReport = Message("CoprocessorOutReport", encode=True)
CoprocessorOutReport.Documentation = "Reserved for passing message through from the Freespace coprocessor to the USB host."
CoprocessorOutReport.ID = {
    ConstantID:5
}
CoprocessorOutReport.Fields = [
    {name:"payloadLength", size:1, cType:'uint8_t'},
    {name:"payload",       size:14}
]

messages.append(CoprocessorOutReport)

# ---------------------------------------------------------------------------------------
# Coprocessor Pass-through In Report
CoprocessorInReport = Message("CoprocessorInReport", decode=True)
CoprocessorInReport.Documentation = "Used for passing messages through from the USB host to the Freespace coprocessor interface."
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
    {name:RESERVED,          size:2}
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
# User Frame Message
UserFrameMessage = Message("UserFrame", decode=True)
UserFrameMessage.Documentation = "Conveys the handheld device position and orientation with respect to a user frame of reference."
UserFrameMessage.ID = {
    ConstantID:33
}
UserFrameMessage.Fields = [
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
# Data Motion Control Message
DataMotion = Message("DataMotionControl", encode=True)
DataMotion.Documentation = "DEPRECATED: This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features."
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
# Pairing Message
PairingMessage = Message("PairingMessage", encode=True)
PairingMessage.Documentation = "Used by the host to put the dongle into pairing mode."
PairingMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:13}
}
PairingMessage.Fields = [
    {name:RESERVED,   size:6}
]

messages.append(PairingMessage)

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
# LED Set Request
LEDSetRequest = Message("LEDSetRequest", encode=True)
LEDSetRequest.Documentation = "This request causes the Loop or dongle to set a status LED to a particular value"
LEDSetRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:34}
}
LEDSetRequest.Fields = [
    {name:'onOff',     size:1, cType:'uint8_t', Documentation:"WP160: 0-Off, 1-On, 2-Release, FSAP160: 0-cause0, 1-cause1, 2-cause2"},
    {name:'selectLED', size:1, cType:'uint8_t', Documentation:"LED Select: 0-green(all devices)\n\t 1-red(all devices)\n\t 2-yellow(all devices)\n\t 3-blue(all devices)\n\t 4-FTA green\n\t 5-FTA red\n\t 6-S2U yellow\n\t 7-S2U blue\n\t 8-Dominion LED PWM\n\t 9-Dominion LED1\n\t 10-Dominion LED2\n\t 11-RFT LED A\n\t 12-RFT LED B"},
    {name:RESERVED,    size:4}
]

messages.append(LEDSetRequest)

# ---------------------------------------------------------------------------------------
# Link Quality Request Message
LinkQualityRequest = Message("LinkQualityRequest", encode=True)
LinkQualityRequest.Documentation = "Controls link quality status reporting"
LinkQualityRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:48}
}
LinkQualityRequest.Fields = [
    {name:'enable', size:1, cType:'uint8_t', Documentation:"0: disable status messages, 1: enable status messages"},
    {name:RESERVED, size:5}
]

messages.append(LinkQualityRequest)

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
FrequencyFixRequest.Documentation = "This message causes the RF frequencies of the selected device to be fixed at channels 0-4. The last byte selects the device.\n\t When the loop is selected it is put into a mode where it does not require the dongle to transmit and where it does not go to sleep."
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
SoftwareReset.Documentation = "This message causes the dongle to reset itself."
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
DongleRFDisableMessage.Documentation = "This message disables the RF on the dongle."
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
# FRS Loop Read Request Message
FRSLoopReadRequest = Message("FRSLoopReadRequest", encode=True)
FRSLoopReadRequest.Documentation = "This is sent from dongle towards the loop to request flash record to be sent.\n\tThe data sent starts from the word offset and continues through to the end of the record."
FRSLoopReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSLoopReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Request Message
FRSLoopWriteRequest = Message("FRSLoopWriteRequest", encode=True)
FRSLoopWriteRequest.Documentation = "This is sent from the host towards the loop to initiate a flash record write.\n\tA length of 0 will cause the record to be invalidated."
FRSLoopWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:61}
}
FRSLoopWriteRequest.Fields = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSLoopWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Data Message
FRSLoopWriteData = Message("FRSLoopWriteData", encode=True)
FRSLoopWriteData.Documentation = "This message is sent from the host towards the loop to write data to the record a previous write request indicated."
FRSLoopWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:63}
}
FRSLoopWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSLoopWriteData)

# ---------------------------------------------------------------------------------------
# FRS Dongle Read Request Message
FRSDongleReadRequest = Message("FRSDongleReadRequest", encode=True)
FRSDongleReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSDongleReadRequest)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Request Message
FRSDongleWriteRequest = Message("FRSDongleWriteRequest", encode=True)
FRSDongleWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteRequest.Fields = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSDongleWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Data Message
FRSDongleWriteData = Message("FRSDongleWriteData", encode=True)
FRSDongleWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:64}
}
FRSDongleWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSDongleWriteData)

# ---------------------------------------------------------------------------------------
# FRS EFlash Read Request Message
FRSEFlashReadRequest = Message("FRSEFlashReadRequest", encode=True)
FRSEFlashReadRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadRequest.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to begin reading.'},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:"BlockSize",  size:2, cType:'uint16_t', Documentation:'Number of 32-bit words to read.'}
]

messages.append(FRSEFlashReadRequest)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Request Message
FRSEFlashWriteRequest = Message("FRSEFlashWriteRequest", encode=True)
FRSEFlashWriteRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteRequest.Fields = [
    {name:"length",  size:2, cType:'uint16_t', Documentation:'Length in 32-bit words of record to be written.'},
    {name:"FRStype", size:2, cType:'uint16_t', Documentation:'FRS record type to read.'},
    {name:RESERVED,  size:2}
]

messages.append(FRSEFlashWriteRequest)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Data Message
FRSEFlashWriteData = Message("FRSEFlashWriteData", encode=True)
FRSEFlashWriteData.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:67}
}
FRSEFlashWriteData.Fields = [
    {name:"wordOffset", size:2, cType:'uint16_t', Documentation:'Offset from start of record to write data.'},
    {name:"data",       size:4, cType:'uint32_t', Documentation:'32-bit word to write.'}
]

messages.append(FRSEFlashWriteData)

# ---------------------------------------------------------------------------------------
# Dongle RF Enable Message
DongleRFEnableMessage = Message("DongleRFEnableMessage", encode=True)
DongleRFEnableMessage.Documentation = "This message enables the RF on the dongle."
DongleRFEnableMessage.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:71}
}
DongleRFEnableMessage.Fields = [
    {name:RESERVED, size:6}
]

messages.append(DongleRFEnableMessage)

# ---------------------------------------------------------------------------------------
# Data Mode Request Message
DataModeRequest = Message("DataModeRequest", encode=True)
DataModeRequest.Documentation = "This report controls the behavior of the Freespace motion reports. The unused bits are reserved for future features."
DataModeRequest.ID = {
    ConstantID:7,
    SubMessageID:{size:1, id:73}
}
DataModeRequest.Fields = [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 enables Body Frame Motion reports."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 enables User Frame Position reports"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 disables the power management feature that automatically stops sending motion reports after a period of no motion."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 enables Mouse Movement reports."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 disables the Freespace motion sensing system to conserve power. No pointer or motion reports are sent regardless of the value of the other bits."},
                                 {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]}
]

messages.append(DataModeRequest)

# ---------------------------------------------------------------------------------------
# ------------------------- Generic In Reports (ID 8) -----------------------------------
# ---------------------------------------------------------------------------------------

# ---------------------------------------------------------------------------------------
# Pairing Response Message
PairingResponse = Message("PairingResponse", decode=True)
PairingResponse.Documentation = "Pairing response is used to either respond to pairing requests from the host or to send pairing status updates to the host that describe events during the pairing process."
PairingResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:13}
}
PairingResponse.Fields = [
    {name:'flags',    size:1, bits:[{name:'pairing',     size:1, Documentation:"0: not pairing.\n\t 1: pairing"},
                                    {name:'autoPairing', size:1, Documentation:"0: dongle is not in auto-pairing\n\t1: dongle is in auto-pairing"},
                                    {name:'success',     size:1, Documentation:"0: not successful or still in progress\n\t1: successful"},
                                    {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]},
    {name:RESERVED,         size:24}
]

messages.append(PairingResponse)

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
    {name:'swVersionPatch', size:2, cType:'uint16_t'},
    {name:'swVersionMinor', size:1, cType:'uint8_t'},
    {name:'swVersionMajor', size:1, cType:'uint8_t'},
    {name:RESERVED,         size:2},
    {name:'serialNumber',   size:4, cType:'uint32_t'},
    {name:'deviceClass',    size:1, bits:[{name:'deviceClass', size:7, Documentation:"The device class represents the characteristics of the device providing the product ID. \n\t 0: device type not known.\n\t 1: non-data-generating device.\n\t 2: data-generating device."},
                                          {name:'invalidNS',           Documentation:"0: read serial number is valid, 1 read serial number is invalid; retry read until valid."}]},
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
    {name:"status",     size:1, cType:'uint8_t', Documentation:"0: bad\n\t1: good"},
    {name:"mode",       size:1, cType:'uint8_t', Documentation:"0: normal operation\n\t1: fixed frequency operation\n\t2: RF disabled"},
    {name:"resetStatus",size:1, cType:'uint8_t', Documentation:"0: did not occur\n\t1: occurred. Self clears."},
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
# FRS Loop Read Response Message
FRSLoopReadResponse = Message("FRSLoopReadResponse", decode=True)
FRSLoopReadResponse.Documentation = "This is sent from the loop to the host to convey an FRS record."
FRSLoopReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:58}
}
FRSLoopReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}
]

messages.append(FRSLoopReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Loop Write Response Message
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
# FRS Dongle Read Response Message
FRSDongleReadResponse = Message("FRSDongleReadResponse", decode=True)
FRSDongleReadResponse.Documentation = "This is sent from the dongle to the host to convey an FRS record."
FRSDongleReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:59}
}
FRSDongleReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}    
]

messages.append(FRSDongleReadResponse)

# ---------------------------------------------------------------------------------------
# FRS Dongle Write Response Message
FRSDongleWriteResponse = Message("FRSDongleWriteResponse", decode=True)
FRSDongleWriteResponse.Documentation = "This is sent from the dongle to the host to indicate status of the write operation."
FRSDongleWriteResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:62}
}
FRSDongleWriteResponse.Fields = [
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
FRSEFlashReadResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:65}
}
FRSEFlashReadResponse.Fields = [
    {name:"wordOffset", size:2,  cType:'uint16_t', Documentation:"Word Offset indicates the number of words the data is offset from the beginning of the record"},
    {name:"data",       size:20},
    {name:"status",     size:1, nibbles:[{name:'status',     Documentation:"Status:\n\t0: no error\n\t1: unrecognized FRS type\n\t2: busy\n\t3: read completed\n\t4: offset out of range\n\t5: record empty\n\t6: read block completed\n\t7: read block completed and read reacord completed"},
                                         {name:'dataLength', Documentation:"Data Length indicates the number of data words contained within the message, typically 5 words"}]},
    {name:"FRStype",    size:2, cType:'uint16_t', Documentation:"FRS record type"}    
]

messages.append(FRSEFlashReadResponse)

# ---------------------------------------------------------------------------------------
# FRS EFlash Write Response Message
FRSEFlashWriteResponse = Message("FRSEFlashWriteResponse", decode=True)
FRSLoopWriteResponse.Documentation = "This is sent from the loop to the host to indicate status of the write operation."
FRSEFlashWriteResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:66}
}
FRSEFlashWriteResponse.Fields = [
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
DataModeResponse = Message("DataModeResponse", encode=True)
DataModeResponse.Documentation = "This report acknowledges the last DataModeRequest received by the dongle."
DataModeResponse.ID = {
    ConstantID:8,
    SubMessageID:{size:1, id:73}
}
DataModeResponse.Fields = [
    {name:'flags', size:1, bits:[{name:'enableBodyMotion',    Documentation:"Enable Body Motion: when set to 1 Body Frame Motion reports are enabled."},
                                 {name:'enableUserPosition',  Documentation:"Enable User Position: when set to 1 User Frame Position reports are enabled"},
                                 {name:'inhibitPowerManager', Documentation:"Inhibit Power Manager: when set to 1 the power management feature isinhibited."},
                                 {name:'enableMouseMovement', Documentation:"Enable Mouse Movement: when set to 1 Mouse Movement reports are enabled."},
                                 {name:'disableFreespace',    Documentation:"Disable Freespace: when set to 1 the Freespace motion sensing system disabled."},
                                 {name:RESERVED}, {name:RESERVED}, {name:RESERVED}]}
]

messages.append(DataModeResponse)
