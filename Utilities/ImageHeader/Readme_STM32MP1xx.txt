/**
  ******************************************************************************
  * @file    Readme_STM32MP1xx.txt
  * @author  MCD Application Team
  * @brief   Description of ImageHeader
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

**** Introduction

ImageHeader allows to add a stm32 header needed by bootrom
You find the stm32 image header description in wiki `STM32_header_for_binary_files <https://wiki.st.com/stm32mpu/wiki/STM32_header_for_binary_files>`

**** ImageHeader directory content :

exe.win-amd64-2.7
-- bz2.pyd
-- _hashlib.pyd
-- library.zip
-- python27.dll
-- Stm32ImageAddHeader.exe  -> Windows executable to generate STM32 Image Header on Windows OS
-- unicodedata.pyd
Python27
-- Stm32ImageAddHeader.py   -> Python script to generate STM32 Image Header on Linux OS
Python3
-- Stm32ImageAddHeader.py   -> Python script to generate STM32 Image Header on Linux OS

**** How to use ImageHeader:

In STM32CubeIDE project you need to add the following line in the "Post-build steps" command in project Properties -> C/C++ Build -> Settings -> Build Steps :

  ../[../]ImageHeader/postbuild_STM32MP15.sh  "${gnu_tools_for_stm32_compiler_path}" "${BuildArtifactFileBaseName}"
or
  ../[../]ImageHeader/postbuild_STM32MP13.sh  "${gnu_tools_for_stm32_compiler_path}" "${BuildArtifactFileBaseName}"

About arguments:
* "../[../]ImageHeader/"                  -> Relative path from STM32CubeIDE build config build directory to Image generation tools
                                               That's one "../" more than from folder containing .cproject
* "postbuild_STM32MP15.sh" or "postbuild_STM32MP13.sh"  -> bash script file to generate the STM32MP15 or STM32MP13 Image Header
* "${gnu_tools_for_stm32_compiler_path}"  -> STM32CubeIDE variable containing path to GCC toolchain
                                               used by postbuild_STM32MPxx.sh to extract information from application .elf file
* "${BuildArtifactFileBaseName}"          -> STM32CubeIDE variable containing base name of application .elf file
                                               Used by postbuild_STM32MPxx.sh to get .elf file and generate Images names

To generate the Image Header, the "postbuild_STM32MP15.sh" or  "postbuild_STM32MP13.sh" script will automatically select either the Python script "Stm32ImageAddHeader.py" on Linux OS
or the windows executable "Stm32ImageAddHeader.exe" on Windows OS

**** Generated files
  ${BuildArtifactFileBaseName}.stm32                        -> Default A7 image to by written to SDCard in selected slot


**** How to regenerate the Windows executable:

If you made some changes in the Python script "Stm32ImageAddHeader.py", you need to update the Windows executable accordingly.
This has to be done on Windows OS only.

- Install Python 2.7
- Install "cx_Freeze-4.2.3.win-amd64-py2.7.msi" (this version is compatible with Python 2.7)
- in folder Python27 run command: "python setup.py install"
- New files are installed in folder "exe.win-amd64-2.7"
