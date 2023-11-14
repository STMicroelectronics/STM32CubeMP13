#!/usr/bin/env python3
# ******************************************************************************
# @file      Stm32ImageAddHeader.py
# @author    MCD Application Team
# @brief     Python script to generate STM32 Image Header
#*****************************************************************************
# @attention
#
# Copyright (c) 2021 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
#*****************************************************************************
from __future__ import print_function
import time
import struct
import argparse


def _getsize(fileobject):
    fileobject.seek(0, 2)  # move the cursor to the end of the file
    size = fileobject.tell()
    return size


def _stm32image_checksum(image):
    checksum = 0
    for dat in image:
        checksum += dat
    return checksum & 0xffffffff


def display_array( name, offset, data ):
    newdata = bytearray(data)
    mystr = ""
    size = len(data)
    for x in range(0, len(newdata) ):
        mystr += ("%02X " % newdata[x])
    maxNbByte = 16
    maxlg = maxNbByte * 2 +(maxNbByte)
    if len(mystr) > maxlg:
        mystr = mystr[0:maxlg] + "..."
    print("| %-23s | @0x%02x %3d | sz %3d | %s " % (name, offset, offset, size, mystr) )


class Stm32Image:

    def __init__(self, header_major_ver=0, header_minor_ver=0, entry=0, loadaddr=0, _binary_type=0):
        self.magic_number = b'STM\x32'                             # Magic number
        self.image_signature = b'\x00' * 64                        # Image Signature
        self.checksum = 0                                          # Image Checksum
        self.header_major_ver = header_major_ver
        self.header_minor_ver = header_minor_ver
        self.image_length = 0                                      # Image Length
        self.image_entry_point = entry                             # Image Entry  Point
        self.load_address = loadaddr                               # Load address
        self.version_number = 0                                    # Version Number
        self.extension_flag = (1 << 31)                            # Extension flags
        self.post_headers_length = 512 - 128                       # Post headers length
        self.binary_type = _binary_type                            # Binary type : 0x00 U-Boot, 0x10 TF-A, 0x20..0x2F OPTEE, 0x30 CM33 
        self.ext_header_type = b'ST\xFF\xFF'                       # Extension header type
        self.ext_header_length = self.post_headers_length          # Extension header length
        self.ext_padding0 = b'\x00' * (self.ext_header_length - 8) # Extension PAD
        self.option_flags = 1                                      # V1: b0=1: no signature verification
        self.ecdsa_algorithm = 1                                   # V1: 1: P-256 NIST ; 2: brainpool 256
        self.ecdsa_pub_key = b'\x00' * 64                          # V1: public key to be used to verify the signature


    def print_header(self):
        print("Image Type  : ST Microelectronics STM32 V%d.%d" % (self.header_major_ver, self.header_minor_ver))
        print("Image Size  : %lu bytes" % self.image_length)
        print("Image Load  : 0x%08X" % self.load_address)
        print("Entry Point : 0x%08X" % self.image_entry_point)
        print("Checksum    : 0x%08X" % self.checksum)
        print("Ext flag    : 0x%08X" % self.extension_flag)
        print("Binary Type : 0x%08X" % self.binary_type)
        print("Version     : 0x%08X" % self.version_number)

    def generate(self, input_file, output_file):

        try:
            origin_file = open(input_file, "r+b")
            pass
        except Exception as e:
            print("%s" % e)
            return -1

        bin_data = origin_file.read()  # store original bin
        self.image_length = _getsize(origin_file)  # size calculation
        origin_file.close()

        self.checksum = _stm32image_checksum(bin_data)  # checksum calculation of the original bin

        nbfields=17
        field = ["" for x in range(nbfields)]
        names = ["" for x in range(nbfields)]
        names[0]  = "Magic number"
        names[1]  = "Image Signature"
        names[2]  = "Image Checksum"
        names[3]  = "Header version"
        names[4]  = "Image Length"
        names[5]  = "Image Entry  Point"
        names[6]  = "Reserved1"
        names[7]  = "Load address"
        names[8]  = "Reserved2"
        names[9]  = "Version Number"

        field[0]  = struct.pack('<4s', self.magic_number)       # Magic number
        field[1]  = struct.pack('64s', self.image_signature)    # Image Signature
        field[2]  = struct.pack('<I', self.checksum)            # Image Checksum
        field[3]  = struct.pack('<4B', 0x0, self.header_minor_ver, self.header_major_ver, 0x0)
        field[4]  = struct.pack('<I', self.image_length)        # Image Length
        field[5]  = struct.pack('<I', self.image_entry_point)   # Image Entry  Point
        field[6]  = struct.pack('4s', b'\x00' * 4)              # Reserved1
        field[7]  = struct.pack('<I', self.load_address)        # Load address
        field[8]  = struct.pack('4s', b'\x00' * 4)              # Reserved2
        field[9]  = struct.pack('<I', self.version_number)      # Version Number

        if self.header_major_ver == 1:
            nbfields=15
            field[10] = struct.pack('<I', self.option_flags)                # V1: option flags
            field[11] = struct.pack('<I', self.ecdsa_algorithm)             # V1: ecdsa algorithm
            field[12] = struct.pack('64s', self.ecdsa_pub_key)              # V1: pubic key
            field[13] = struct.pack('83s', b'\x00'*83)                      # V1: PAD
            field[14] = struct.pack('<B', self.binary_type)                 # V1: Binary type : 0x10-0x1F TF-A, 0x30 copro
            names[10] = "Option flags"
            names[11] = "ecdsa algo"
            names[12] = "ecdsa_public_key"
            names[13] = "padding"
            names[14] = "Binary type"
        else:
            nbfields=17
            field[10] = struct.pack('<I', self.extension_flag)              # V2: Extension flags
            field[11] = struct.pack('<I', self.post_headers_length)         # V2: Post headers length
            field[12] = struct.pack('<I', self.binary_type)                 # V2: Binary type : 0x00 U-Boot, 0x10 TF-A, 0x20..0x2F OPTEE, 0x30 CM33 image
            field[13] = struct.pack('16s', b'\x00' * 16)                    # V2: PAD
            field[14] = struct.pack('<4s', self.ext_header_type)            # V2: Extension header type
            field[15] = struct.pack('<I', self.ext_header_length)           # V2: Extension header length
            field[16] = struct.pack('376s', self.ext_padding0)              # V2: Extension PAD
            names[10] = "Extension flags"
            names[11] = "Post headers length"
            names[12] = "Binary type"
            names[13] = "PAD"
            names[14] = "Extension header type"
            names[15] = "Extension header length"
            names[16] = "Extension PAD"

        # creating the output file
        new_file = open(output_file, "w+b")
        # adding header
        for i in range(0, nbfields ):
            if field[i] != '':
                new_file.write(field[i])
        # adding firmware code
        new_file.write(bin_data)
        # close file
        new_file.close()

        # display header field
        offset = 0
        for i in range(0, nbfields ):
            if field[i] != '':
                display_array( names[i], offset, field[i] )
                offset += len(field[i])

        return 0


def main():
    parser = argparse.ArgumentParser(description="Script that sign binaries with ST header")
    parser.add_argument("input_file", help="the input filename")
    parser.add_argument("output_file", help="the output filename")
    parser.add_argument("-hv", "--header_version", action="store", help=" Header version : 1.0 (STM32MP15xx), 2.0 (STM32MP13xx and after)")
    parser.add_argument("-ep", "--entry_point", action="store", help="entry point address in hexadecimal")
    parser.add_argument("-la", "--load_addr", action="store", help="load address in hexadecimal")
    parser.add_argument("-bt", "--binary_type", action="store", help="binary type : U-boot, tf-a, CM33 in hexadecimal")
    args = parser.parse_args()

    t = time.asctime()
    print("\n%s" % t)

    print("Input file  : %s" % args.input_file)
    print("Output file : %s" % args.output_file)

    if args.header_version:
        header_version_split = args.header_version.split(".")
        header_major_ver = int(header_version_split[0], 8)
        header_minor_ver = int(header_version_split[1], 8)
    else:
        header_major_ver = 2
        header_minor_ver = 0

    if args.entry_point:
        entry_point = int(args.entry_point, 16)
    else:
        entry_point = int("2ffe0000", 16)

    if args.load_addr:
        load_addr = int(args.load_addr, 16)
    else:
        load_addr = 0

    if args.binary_type:
        binary_type = int(args.binary_type, 16)
    else:
        binary_type = 0


    # Class object creation:
    stm32im = Stm32Image(header_major_ver, header_minor_ver, entry_point, load_addr, binary_type)

    ret = stm32im.generate(args.input_file, args.output_file)
    if ret != 0:
      print("Aborted")
      return ret

    stm32im.print_header()
    print("%s generated" % args.output_file)


if __name__ == '__main__':
    main()
