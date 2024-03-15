#!/bin/bash -
# ******************************************************************************
# @file      postbuild_STM32MP15.sh
# @author    MCD Application Team
# @brief     postbuild command script
#*****************************************************************************
# @attention
#
# Copyright (c) 2022 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
#*****************************************************************************

# arg1 : toolchain bin directory path
# arg2 : elf file base name (without .elf extension)
CompilerPath="${1}"
elf_file_basename="${2}"

debug=0

local_script_path=$(dirname $0)
local_script_path=$(readlink -f ${local_script_path})

case "$(uname -s)" in
  Linux)
    #line for python
    echo Postbuild with python script
    imgtool="${local_script_path}/Python3/Stm32ImageAddHeader.py"
    cmd="python"
    ;;
  *)
    #line for window executable
    echo Postbuild with windows executable
    imgtool="${local_script_path}/exe.win-amd64-2.7/Stm32ImageAddHeader.exe"
    cmd=""
    ;;
esac

readelf_path=$(find ${CompilerPath} -name "*readelf*" -print)
objcopy_path=$(find ${CompilerPath} -name "*objcopy*" -print)

if [ ${debug} -ne 0 ] ; then
  echo "<D> readelf_path                =<${readelf_path}>"
  echo "<D> objcopy_path                =<${objcopy_path}>"
fi

elf_entry_point=$(${readelf_path} -h ${elf_file_basename}.elf | grep "Entry point address" | sed 's/.*0x\([^ ]*\)/0x\1/')
if [ ${debug} -ne 0 ] ; then
  echo "<D> elf_entry_point             =<$elf_entry_point>"
fi

# elf_entry_point is in format 0x%x so Reformat to 0x%08X and process entry-point address
formatted_ep_addr=$(printf "%08x" $((elf_entry_point)))
if [ ${debug} -ne 0 ] ; then
  echo "<D> formatted_ep_addr           =<$formatted_ep_addr>"
fi

${objcopy_path} -O binary ${elf_file_basename}.elf ${elf_file_basename}_postbuild.bin


command="${cmd} ${imgtool} ${elf_file_basename}_postbuild.bin ${elf_file_basename}.stm32 -hv 1.0 -bt 10 -ep ${formatted_ep_addr}"
${command}
ret=$?

if [ ${debug} -eq 0 ] ; then
  rm -f ${elf_file_basename}_postbuild.bin
fi

if [ ${ret} -eq 0 ] ; then
  echo "${elf_file_basename}.elf stm32 image header added. Output file: ${elf_file_basename}.stm32"
else
  echo "postbuild_STM32MP15.sh failed"
fi
