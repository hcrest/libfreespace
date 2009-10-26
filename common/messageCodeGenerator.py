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

import sys
import getopt
import os

def compareMessages(a, b):
    return cmp(a.ID['constID'], b.ID['constID'])

class MessageCodeGenerator:
    def writeMessages(self, messages):
        messages.sort(compareMessages)
        
        codecsHFile = open("freespace_codecs.h", "w")
        self.writeHFileHeader(codecsHFile, 'freespace_codecs')
        self.writeDoxygenModuleDef(codecsHFile)
        
        printersHFile = open("freespace_printers.h", "w")
        self.writeHFileHeader(printersHFile, 'freespace_printers')
        printersHFile.write('#include "freespace_codecs.h"\n')
        printersHFile.write('#include <stdio.h>\n\n')
        self.writePrintMessageHeader(printersHFile)
        
        codecsCFile = open("freespace_codecs.c", "w")
        self.writeCFileHeader(codecsCFile, 'freespace_codecs')
        codecsCFile.write('#include <stdio.h>\n')
        codecsCFile.write('\n#ifdef _WIN32\n')
        codecsCFile.write('#define STRICT_DECODE_LENGTH 0\n')
        codecsCFile.write('#else\n')
        codecsCFile.write('#define STRICT_DECODE_LENGTH -1\n')
        codecsCFile.write('#endif\n\n')
        self.writeBitHelper(codecsCFile)
        
        printersCFile = open("freespace_printers.c", "w")
        self.writeCFileHeader(printersCFile, 'freespace_printers')
        self.writePrintMessageBody(messages, printersCFile)
        
        for message in messages:
            writeCodecs(message, codecsHFile, codecsCFile)
            
        for message in messages:
            writePrinter(message, printersHFile, printersCFile)
            
        self.writeUnionStruct(codecsHFile, messages)
        self.writeUnionDecodeEncodeBodies(codecsCFile, messages)
            
        self.writeHFileTrailer(codecsHFile, 'freespace_codecs')
        self.writeHFileTrailer(printersHFile, 'freespace_printers')
        codecsHFile.close()
        codecsCFile.close()
        printersHFile.close()
        printersCFile.close()
    
    def writeBitHelper(self, outHeader):
        outHeader.write('''
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

''')

    def writeHFileHeader(self, outHeader, name):
        # Print the struct, and define the message functions
        writeCopyright(outHeader)
        outHeader.write("\n")
        writeIfndef(outHeader, name)
        outHeader.write("\n")
        outHeader.write('#include "freespace/freespace.h"\n')
        outHeader.write("\n")
        writeExternC(outHeader)
        outHeader.write('\n')
        
    def writeHFileTrailer(self, outHeader, name):
        writeCloseExternC(outHeader)
        outHeader.write("\n")
        writeCloseIfndef(outHeader, name)

    def writeCFileHeader(self, outFile, name):
        writeCopyright(outFile)
        outFile.write('''
#include "freespace/%s.h"
#include <string.h>
'''%name)
        
    def writePrintMessageBody(self, messages, outFile):
        outFile.write('''
static void printUnknown(const char* name, const uint8_t* buffer, int length) {
    int i;
    printf("%s(", name);
    for (i = 0; i < length; ++i) {
        printf("%02x ", (uint8_t) buffer[i]);
    }
    printf(")\\n");
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
    switch(s->messageType) {''')
        for message in messages:
            if (not message.decode) or not message.hasUnReservedFields():
                continue
            outFile.write('''
    case %(enumName)s:
        freespace_print%(messageName)s(fp, &(s->%(structName)s));
        break;'''%{'enumName':message.enumName, 
                                                                             'messageName':message.name, 
                                                                             'structName':message.structName})
        outFile.write('''
    default:
        return;
    }
}''')
    
    def writePrintMessageHeader(self, outFile):
        outFile.write('''
/**
 * Pretty print a Freespace message to the terminal.
 *
 * @param fp the file pointer to print into
 * @param message the HID message
 * @param length the length of the message
 */
LIBFREESPACE_API void freespace_printMessage(FILE* fp, const uint8_t* message, int length);

LIBFREESPACE_API void freespace_printMessageStruct(FILE* fp, struct freespace_message* s);

''')
    
    def writeDoxygenModuleDef(self, outFile):
        outFile.write('''
/**
 * @defgroup messages Freespace Messages
 *
 * This page describes the messages that can be sent to and from the Freespace Device.
 * They are represented as structs which can be encoded and decoded from strings.
 */
''')
            
    def writeUnionStruct(self, file, messages):
        file.write('''
/** @ingroup messages
 * An enumeration for all the types of messages that can exist. Used in freespace_message
 * to determine the type of message contained in the union
 */
enum MessageTypes {''')
        i = 0
        for message in messages:
            if message.hasUnReservedFields() and message.shouldGenerate:
                file.write('''
    %s = %d,'''%(message.enumName, i))
                i = i+1
        file.write('''
};
''')
    
        file.write('''
/** @ingroup messages
 * freespace_message has an enum which defines the type of the message contained
 * and a union of all the possible message structs. 
 */
struct freespace_message {
    int messageType;
    union {''')
        for message in messages:
            if message.hasUnReservedFields() and message.shouldGenerate:
                file.write("\n\t\tstruct freespace_%(name)s %(varName)s;"%{'name':message.name, 'varName':message.structName})
        file.write('''
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
LIBFREESPACE_API int freespace_decode_message(const uint8_t* message, int length, struct freespace_message* s);

/** @ingroup messages
 * Encode an arbitrary message.
 *
 * @param s the freespace_message struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @return the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encode_message(const struct freespace_message* s, uint8_t* message, int maxlength);

''')
        
    def writeUnionDecodeEncodeBodies(self, file, messages):
        file.write('''
LIBFREESPACE_API int freespace_decode_message(const uint8_t* message, int length, struct freespace_message* s) {
    if (length == 0) {
        return -1;
    }

    switch ((uint8_t) message[0]) {\n''')
        usedIDs = []
        for message in messages:
            if (not message.decode) or (message.ID['constID'] in usedIDs) or (not message.hasUnReservedFields()) or (not message.shouldGenerate):
                continue
            file.write("\tcase %d:"%message.ID['constID'])
            if message.ID.has_key('subId'):
                file.write('''
        switch ((uint8_t) message[1]) {''')
                for subMessage in messages:
                    if subMessage.hasUnReservedFields() and message.shouldGenerate:
                        if subMessage.ID['constID'] == message.ID['constID']:
                            file.write('''
        case %(subId)d:
            {
                s->messageType = %(messageType)s;
                return freespace_decode%(subName)s(message, length, &(s->%(unionStruct)s));
            }
            break;'''%{'subId':subMessage.ID['subId']['id'],
                       'subName':subMessage.name,
                       'unionStruct':subMessage.structName,
                       'messageType':subMessage.enumName})                
        
                file.write('''
        default:
            return -1;
''')
            else:
                file.write('''
        {
            s->messageType = %(messageType)s;
            return freespace_decode%(name)s(message, length, &(s->%(unionStruct)s));
            '''%{'messageType':message.enumName,
                 'name':message.name,
                 'unionStruct':message.structName})
                
            file.write('''
        }
        break;
''')
            usedIDs.append(message.ID['constID'])
            
        file.write('''
    default:
        return -1;
    }
}
''')
        
        file.write('''
LIBFREESPACE_API int freespace_encode_message(const struct freespace_message* s, uint8_t* message, int maxlength) {
    switch (s->messageType) {''')
        for message in messages:
            if (not message.encode) or (not message.hasUnReservedFields()) or (not message.shouldGenerate):
                continue
            file.write('''
    case %(enumName)s:
        return freespace_encode%(messageName)s(&(s->%(structName)s), message, maxlength);'''%{'enumName':message.enumName, 
                                                                                            'messageName':message.name, 
                                                                                            'structName':message.structName})
        file.write('''
    default:
        return -1;
    }
}''')



# --------------------------  Individual Message ------------------------------------
    
def writeCodecs(message, outHFile, outCFile):
    writeCodecHeader(message, outHFile)
    writeCodecCFile(message, outCFile)
            
def writePrinter(message, outHFile, outCFile):
    writePrinterHeader(message, outHFile)
    writePrinterBody(message, outCFile)
    
def writePrinterHeader(message, outHeader):
    if message.hasUnReservedFields() and message.shouldGenerate:
        writePrintDecl(message, outHeader)
    
def writePrinterBody(message, outFile):
    if message.hasUnReservedFields() and message.shouldGenerate:
        writePrintBody(message, outFile)
        outFile.write('\n')
    
def writeCodecHeader(message, outHeader):
    if message.hasUnReservedFields() and message.shouldGenerate:
        writeStruct(message, outHeader)
        outHeader.write('\n')
        
        if message.decode:
            writeDecodeDecl(message, outHeader)
            outHeader.write('\n')
    
    if message.encode and message.shouldGenerate:
        writeEncodeDecl(message, outHeader)
        outHeader.write('\n')

def writeCodecCFile(message, outFile):
    if message.hasUnReservedFields() and message.shouldGenerate:
        if message.decode:
            writeDecodeBody(message, outFile)
            outFile.write('\n')

    if message.encode and message.shouldGenerate:
        writeEncodeBody(message, outFile)
        outFile.write('\n')

def writeStruct(message, outHeader):
    if message.Documentation != None:
        outHeader.write("/**   @ingroup messages \n * " + message.Documentation + "\n */\n")
    outHeader.write("struct freespace_" + message.name + " {\n")
    outHeader.write("\t// ID = %d\n"%message.ID['constID'])
    if message.ID.has_key('subId'):
        outHeader.write('\t// subID = 0x%02X\n'%((message.ID['subId'])['id']))
    for field in message.Fields:
        if field.has_key('comment'):
            outHeader.write("\n\t/** " + field['comment'] + " */\n")
        if (field['name'] == 'RESERVED'):
            continue
        if (field.has_key('cType')):
            outHeader.write("\t")
            outHeader.write(field['cType'])
            outHeader.write(" " + field['name'] + ";\n")
        elif (field.has_key('bits')):
            for bitField in field['bits']:
                if bitField['name'] != 'RESERVED':
                    if bitField.has_key('comment'):
                        outHeader.write("\n\t/** " + bitField['comment'] + " */\n")
                    outHeader.write("\t")
                    outHeader.write("uint8_t")
                    outHeader.write(" " + bitField['name'] + ";\n")
        elif (field.has_key('nibbles')):
            for nibble in field['nibbles']:
                if nibble['name'] != 'RESERVED':
                    if nibble.has_key('comment'):
                        outHeader.write("\n\t/** " + nibble['comment'] + " */\n")
                    outHeader.write("\t")
                    outHeader.write("uint8_t")
                    outHeader.write(" " + nibble['name'] + ";\n")
        else:
            # Assume it's just some arbitrary amount of data, use the size for allocation
            outHeader.write('\tuint8_t %s[%d];\n'%(field['name'], field['size']))
    outHeader.write("};\n")
    
def writeEncodeDecl(message, outHeader):
    if not message.hasUnReservedFields():
        outHeader.write('''
/** @ingroup messages
 * Encode a %(name)s message.
 *
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encode%(name)s(uint8_t* message, int maxlength);
'''%{'name':message.name})
    else:
        outHeader.write('''
/** @ingroup messages
 * Encode a %(name)s message.
 *
 * @param s the freespace_%(name)s struct
 * @param message the string to put the encoded message into
 * @param maxlength the maximum length of the message
 * @returns the actual size of the encoded message or an error code
 */
LIBFREESPACE_API int freespace_encode%(name)s(const struct freespace_%(name)s* s, uint8_t* message, int maxlength);
'''%{'name':message.name})
    
def writeDecodeDecl(message, outHeader):
    outHeader.write('''
/** @ingroup messages
 * Decode a %(name)s message. Fill out the corresponding values in struct s.
 *
 * @param message the message to decode that was received from the Freespace device
 * @param length the length of the received message
 * @param s the preallocated freespace_%(name)s struct to decode into
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_decode%(name)s(const uint8_t* message, int length, struct freespace_%(name)s* s);
'''%{'name':message.name})

def writePrintDecl(message, outHeader):
    outHeader.write('''
/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_print%(name)sStr(char* dest, int maxlen, const struct freespace_%(name)s* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_print%(name)s(FILE* fp, const struct freespace_%(name)s* s);

'''%{'name':message.name})
    
def writeEncodeBody(message, outFile):
    if not message.hasUnReservedFields():
        outFile.write('''
LIBFREESPACE_API int freespace_encode%(name)s(uint8_t* message, int maxlength) {
    if (maxlength < %(size)d) {
        printf("freespace_%(name)s encode(<INVALID LENGTH>)\\n");
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    message[0] = (uint8_t) %(id)d;'''%{'name':message.name, 'size':message.getMessageSize(), 'id':message.ID['constID']})
        if message.ID.has_key('subId'):
            outFile.write('''
    message[1] = (uint8_t) %d;'''%message.ID['subId']['id'])
        outFile.write('''
    return %(size)d;
}
'''%{'size':message.getMessageSize()})
    else:
        outFile.write("LIBFREESPACE_API int freespace_encode%s(const struct freespace_%s* s, uint8_t* message, int maxlength) {\n"%(message.name, message.name))
        outFile.write("\tif (maxlength < %d) {\n"%message.getMessageSize())
        outFile.write('\t\tprintf("freespace_%s encode(<INVALID LENGTH>)\\n");\n'%message.name)
        outFile.write('\t\treturn -1;\n')
        outFile.write("\t}\n")
        outFile.write('\tmessage[0] = (uint8_t) %d;\n'%message.ID['constID'])
        byteCounter = 1
        if (message.ID.has_key('subId')):
            outFile.write('\tmessage[%d] = (uint8_t) %d;\n'%(byteCounter,message.ID['subId']['id']))
            byteCounter += 1
        for field in message.Fields:
            elementSize = field['size']
            if field['name'] == 'RESERVED':
                byteCounter += elementSize
                continue
            if (elementSize == 1):
                if field.has_key('bits'):
                    outFile.write('\tmessage[%d] = byteFromBits('%byteCounter)
                    firstLoop = True
                    for bit in field['bits']:
                        if not firstLoop:
                            outFile.write(', ')
                        else:
                            firstLoop = False
                        if bit['name'] == 'RESERVED':
                            outFile.write('0')
                        else:
                            outFile.write('s->%s'%bit['name']);
                            if bit.has_key('size'):
                                for i in range(1, bit['size']):
                                    outFile.write(', s->%s >> %d'%(bit['name'],i))
                    outFile.write(');\n')
                elif field.has_key('nibbles'):
                    outFile.write('\tmessage[%d] = byteFromNibbles('%byteCounter)
                    firstLoop = True
                    for nibble in field['nibbles']:
                        if not firstLoop:
                            outFile.write(', ')
                        else:
                            firstLoop = False
                        if nibble['name'] == 'RESERVED':
                            outFile.write('0')
                        else:
                            outFile.write('s->%s'%nibble['name']);
                    outFile.write(');\n')
                else:
                    outFile.write('\tmessage[%d] = (uint8_t) s->%s;\n'%(byteCounter,field['name']))
            else:
                if field.has_key('cType'):
                    for i in range(0,elementSize):
                        outFile.write('\tmessage[%d] = (uint8_t) (s->%s >> %d);\n'%(byteCounter+i,field['name'], 8*i))
                else:
                    outFile.write('\tmemcpy(message + %d, s->%s, %d);\n'%(byteCounter, field['name'], elementSize))
            byteCounter += elementSize
        outFile.write('\treturn %d;\n' % message.getMessageSize())
        outFile.write('}\n')

def writeDecodeBody(message, outFile):
    outFile.write('''
LIBFREESPACE_API int freespace_decode%(name)s(const uint8_t* message, int length, struct freespace_%(name)s* s) {
    if ((STRICT_DECODE_LENGTH && length != %(size)d) || (!STRICT_DECODE_LENGTH && length < %(size)d)) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    if ((uint8_t) message[0] != %(id)d) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
'''%{'name':message.name, 'size':message.getMessageSize(), 'id':message.ID['constID']})
    byteCounter = 1
    if message.ID.has_key('subId'):
        outFile.write('''
    if ((uint8_t) message[1] != %(subId)d) {
        return FREESPACE_ERROR_MALFORMED_MESSAGE;
    }
'''%{'subId':message.ID['subId']['id']})
        byteCounter += 1
    for field in message.Fields:
        elementSize = field['size']
        if field['name'] == 'RESERVED':
            byteCounter += elementSize
            continue
        if elementSize == 1:
            if field.has_key('cType'):
                #straight cast
                outFile.write('    s->%s = (%s) message[%d];\n'%(field['name'],field['cType'], byteCounter))
            elif field.has_key('bits'):
                bitCounter = 0
                for bit in field['bits']:
                    if bit['name'] != 'RESERVED':
                        if bit.has_key('size'):
                            outFile.write('    s->%s = (uint8_t) ((message[%d] >> %d) & 0x%02X);\n'%(bit['name'], byteCounter, bitCounter, 2**bit['size']-1))
                            bitCounter += bit['size']-1
                        else:
                            outFile.write('    s->%s = getBit(message[%d], %d);\n'%(bit['name'], byteCounter, bitCounter))
                    bitCounter += 1
            elif field.has_key('nibbles'):
                nibbleCounter = 0
                for nibble in field['nibbles']:
                    if nibble['name'] != 'RESERVED':
                        outFile.write('    s->%s = getNibble(message[%d], %d);\n'%(nibble['name'], byteCounter, nibbleCounter))
                    nibbleCounter += 1
        else:
            if field.has_key('cType'):
                outFile.write('\ts->%s = %s('%(field['name'], IntConversionHelper(elementSize)))
                outFile.write('message[%d]'%byteCounter)
                for i in range(1,elementSize):
                    outFile.write(', message[%d]'%(byteCounter+i))
                outFile.write(');\n')
            else:
                outFile.write('\tmemcpy(s->%s, &message[%d], %d);\n'%(field['name'],byteCounter,field['size']))
        byteCounter += elementSize
    outFile.write('\treturn FREESPACE_SUCCESS;\n')
    outFile.write('}\n')

def printStrHelper(message, outFile):
    fieldNames = []
    for field in message.Fields:
        if field['name'] == 'RESERVED':
            continue
        if field.has_key('cType'):
            fieldNames.append(field['name'])
        elif field.has_key('bits'):
            for bit in field['bits']:
                if bit['name'] != 'RESERVED':
                    fieldNames.append(bit['name'])
        elif field.has_key('nibbles'):
            for nibble in field['nibbles']:
                if nibble['name'] != 'RESERVED':
                    fieldNames.append(nibble['name'])
    first = True
    for field in fieldNames:
        if not first:
            outFile.write(' ')
        else:
            first = False
        outFile.write("%s=%%d"%field)
    outFile.write(')"')
    for field in fieldNames:
        outFile.write(', s->%s'%field)
    outFile.write(');')

def writePrintBody(message, outFile):
    outFile.write('''
LIBFREESPACE_API int freespace_print%(name)sStr(char* dest, int maxlen, const struct freespace_%(name)s* s) {
    int n;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
#ifdef _WIN32
    n = sprintf_s(dest, maxlen, "%(name)s('''%{'name':message.name})
    printStrHelper(message, outFile)
    outFile.write('''
#else
    n = sprintf(dest, "%(name)s('''%{'name':message.name})
    printStrHelper(message, outFile)
    
    outFile.write('''
#endif
    if (n < 0) {
        return FREESPACE_ERROR_BUFFER_TOO_SMALL;
    }
    return n;
}
''')
    
    
    outFile.write('''
LIBFREESPACE_API int freespace_print%(name)s(FILE* fp, const struct freespace_%(name)s* s) {
    char str[1024];
    int rc;
    if (s == NULL) {
        return FREESPACE_ERROR_UNEXPECTED;
    }
    rc = freespace_print%(name)sStr(str, sizeof(str), s);
    if (rc < 0) {
        return rc;
    }
    return fprintf(fp, "%%s\\n", str);
}
'''%{'name':message.name})
        
# --------------------------  Syntax Helpers ------------------------------------

def fieldToPrintFormat(field):
    return '%d'

def IntConversionHelper(elementSize):
    if elementSize == 2:
        return "toInt"
    if elementSize == 4:
        return "toUInt32"
    raise "Unknown toIntXX size"
    
# ---------------------- Output printing helpers --------------------------------

def writeCopyright(outHeader):
    outHeader.write('''/*
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
''')

def writeIfndef(outHeader, name):
    outHeader.write("#ifndef " + name.upper() + "_H_\n")
    outHeader.write("#define " + name.upper() + "_H_\n")
    
def writeCloseIfndef(outHeader, name):
    outHeader.write("#endif /* " + name.upper() + "_H_ */\n")
    
def writeExternC(outHeader):
    outHeader.write("#ifdef __cplusplus\n")
    outHeader.write('extern "C" {\n')
    outHeader.write('#endif\n')
    
def writeCloseExternC(outHeader):
    outHeader.write("#ifdef __cplusplus\n")
    outHeader.write('}\n')
    outHeader.write('#endif\n')

# ---------------------- Main function --------------------------------
# Courtesy of Guido: http://www.artima.com/weblogs/viewpost.jsp?thread=4829
class Usage(Exception):
    def __init__(self, msg):
        self.msg = msg

def main(argv=None):
    if argv is None:
        argv = sys.argv
    try:
        try:
            opts, args = getopt.getopt(argv[1:], "h", ["help"])
        except getopt.error, msg:
             raise Usage(msg)
        # process options
        for o, a in opts:
            if o in ("-h", "--help"):
                print __doc__
                return 0
        # process arguments
        for arg in args:
            g = {}
            d = {}
            execfile(arg, g, d)
            messages = d['messages']
            mcg = MessageCodeGenerator()
            mcg.writeMessages(messages)
    except Usage, err:
        print >>sys.stderr, err.msg
        print >>sys.stderr, "for help use --help"
        return 2
        
        
if __name__ == "__main__":
    sys.exit(main())

