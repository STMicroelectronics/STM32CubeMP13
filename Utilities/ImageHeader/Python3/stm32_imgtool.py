#!/usr/bin/env python3
#         
#  ******************************************************************************
#  * @attention
#  *
#  * Copyright (c) 2021 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file
#  * in the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *
#  ******************************************************************************

import os
import re
import argparse
from elftools.elf.elffile import ELFFile
import Stm32ImageAddHeader

def main():
    parser = argparse.ArgumentParser(description="Extract info from elf")
    # Add the arguments
    parser.add_argument('-e', '--elf_file', help='elf file', required=True)
    parser.add_argument('-b', '--bin_file', help='binary file', required=True)
    parser.add_argument('-o', '--out_file', help='output file', required=True)
    parser.add_argument('-bt', '--binary_type', help='binary file', required=True)

    args = parser.parse_args()

    if os.path.isfile(args.elf_file) is False:
        raise Exception("No such file:{}".format(args.elf_file))

    if os.path.isfile(args.bin_file) is False:
        raise Exception("No such file:{}".format(args.bin_file))

    with open(args.elf_file, "rb") as file:
        elf_file = ELFFile(file)
        text_section = elf_file.get_section_by_name(".text")

        load_address = text_section.header["sh_addr"]
        entry_point = elf_file.header.e_entry

    binary_type = int(args.binary_type, 16)

    print("elf file     :{}".format(args.elf_file))
    print("bin file     :{}".format(args.bin_file))
    print("load address :0x{0:X}".format(load_address))
    print("entry point  :0x{0:X}".format(entry_point))
    print("binary type  :0x{0:X}".format(binary_type))

    stm32im = Stm32ImageAddHeader.Stm32Image(entry_point, load_address, binary_type)
    ret = stm32im.generate(args.bin_file, args.out_file)
    if ret != 0:
      print("Aborted")
      return ret

    stm32im.print_header()
    print("%s generated" % args.out_file)

if __name__ == "__main__":
    main()
