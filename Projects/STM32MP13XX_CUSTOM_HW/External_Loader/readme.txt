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
memories like SNOR flash and eMMC via STM32CubeProgrammer using UART or USB.

@par External Loader application package contents

  - EMMC_Ext_Loader      eMMC loader application. Refer -:
                         Projects\STM32MP13XX_CUSTOM_HW\External_Loader\EMMC_Ext_Loader\readme.txt for more details.

  - External_Mem_Loader  External Memory Loader application. Refer -:
                         Projects\STM32MP13XX_CUSTOM_HW\External_Loader\External_Mem_Loader\readme.txt for more details.

  - SNOR_Ext_Loader      SNOR loader application. Refer -:
                         Projects\STM32MP13XX_CUSTOM_HW\External_Loader\SNOR_Ext_Loader\readme.txt for more details.

  - FSBLA_SNOR           FSBLA SNOR application. Refer -:
                         Projects\STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\readme.txt for more details.

  - FSBLA_eMMC           FSBLA EMMC application. Refer -:
                         Projects\STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_eMMC\readme.txt

  - Custom Application   Any application that is to be programmed on external memory.

  - Prebuild_Binaries    Prebuild binaries from above applications, that can be directly used as is for demo purpose.

@par Hardware and Software environment

  - This package is for STM32MP135xx devices.

  - This package has been tested with ST internal boards and can be easily tailored to any other supported device and 
    development board.

@par How to use it ?

For programming eMMC follow below steps :
 - Build EMMC_Ext_Loader, External_Mem_Loader, FSBLA_eMMC and any Custom application that needs to be flashed using your
   preferred toolchain.
 - Copy above binaries into Prebuild_Binaries\EMMC_Ext_Loader folder.
 - Open Prebuild_Binaries\EMMC_Ext_Loader\FlashLayout_OpenBL_ExtLoaderEMMC_SerialBoot.tsv using latest 
   STM32Cubeprogrammer and select binaries path as Prebuild_Binaries\EMMC_Ext_Loader.
 - Select boot pins to boot from UART or USB.
 - Connect UART or USB cable and click on Download.
 - All binaries are flashed successfully.
 - Select boot pins to boot from eMMC.

For programming SNOR flash follow below steps :
 - Build SNOR_Ext_Loader, External_Mem_Loader, FSBLA_SNOR and any Custom application that needs to be flashed using your
   preferred toolchain.
 - Copy above binaries into Prebuild_Binaries\SNOR_Ext_Loader folder.
 - Open Prebuild_Binaries\SNOR_Ext_Loader\FlashLayout_OpenBL_ExtLoaderSNOR_SerialBoot.tsv using latest 
   STM32Cubeprogrammer and select binaries path as Prebuild_Binaries\SNOR_Ext_Loader.
 - Select boot pins to boot from UART or USB.
 - Connect UART or USB cable and click on Download.
 - All binaries are flashed successfully.
 - Select boot pins to boot from SNOR flash.

 */
