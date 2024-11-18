/**
  @page External_Loader External Loader application package

  @verbatim
  ******************************************************************************
  * @file    External_Loader/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the External Loader application package.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

External Loader application package contains multiple applications which can be collectively used to program external
memories like SDCard via STM32CubeProgrammer using UART or USB.

@par External Loader application package contents

  - External_Mem_Loader  External Memory Loader application. Refer -:
                         Projects\STM32MP135C-DK\External_Loader\External_Mem_Loader\readme.txt for more details.

  - SD_Ext_Loader        SDCard loader application. Refer -:
                         Projects\STM32MP135C-DK\External_Loader\SD_Ext_Loader\readme.txt for more details.

  - FSBLA_Sdmmc1         FSBLA SDMMC application. Refer -:
                         Projects\STM32MP135C-DK\Applications\FSBLA\FSBLA_Sdmmc1\readme.txt for more details.

  - Custom Application   Any application that is to be programmed on external memory.

  - Prebuild_Binaries    Prebuild binaries from above applications, that can be directly used as is for demo purpose.

@par Hardware and Software environment

  - This package is for STM32MP135xx devices.

  - This package has been tested on STM32MP135F-DK board and can be easily tailored to any other supported device and 
    development board.

@par How to use it ?

For programming SDCard follow below steps :
 - Build SD_Ext_Loader, External_Mem_Loader, FSBLA_Sdmmc1 and any Custom application that needs to be flashed using your
   preferred toolchain.
 - Copy above binaries into Prebuild_Binaries\SD_Ext_Loader folder.
 - Open Prebuild_Binaries\SD_Ext_Loader\FlashLayout_OpenBL_ExtLoaderSDMMC_SerialBoot.tsv using latest 
   STM32Cubeprogrammer and select binaries path as Prebuild_Binaries\SD_Ext_Loader.
 - Select boot pins to boot from UART or USB.
 - Connect UART or USB cable and click on Download.
 - All binaries are flashed successfully.
 - Select boot pins to boot from SDCard.

 */