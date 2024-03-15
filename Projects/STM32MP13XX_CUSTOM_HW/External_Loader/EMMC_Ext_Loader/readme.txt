/**
  @page EMMC External Loader Firmware utility

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    MMC_Ext_Loader/readme.txt
  * @author  MCD Application Team
  * @brief   Description of 'EMMC External Loader Firmware utility'.
  ******************************************************************************
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

@par EMMC External Loader Firmware utility Description

EMMC External Loader Firmware utility is used to manage external EMMC
  - This utility provides embedded SW applications to program and manage
    external EMMC on VALID Board.
  - This package includes only binaries and supports ONLY UART protocol without more 
    complex software on VALID board. USB DFU protocol is not supported.
  -	EMMC External Loader Firmware utility source code should be requested from STMicroelectronics and easily
    adapted and improved based on your own needs.
  - OTP Programming is not supported in this package.

This package contain CP_Serial_Boot mode that you can use with **Windows® PC**.
Please find below detail about CP_Serial_Boot mode described in this document.

Setup
  - VALID Board with UART serial interface.
  - STM32CubeProgrammer PC tool installed (v2.14.0 minimum)
    . (https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer_release_note)                                                                                                                     |                                |                                                            |                                                                                                      |      X      |

How to Use CP_Serial_Boot

   - In this section, you will use 
     "Projects\STM32MP135C-VALID\External_Loader\EMMC_Ext_Loader" directory 
     containing EMMC External Loader binaries and a tsv file used by STM32CubeProgrammer.

EMMC External Loader contents
   - FlashLayout_OpenBL_ExtLoaderEMMC_SerialBoot.tsv
     . It contains details about binaries, their type etc and is used by STM32CubeProgrammer 
	   for programming the binaries.
	   
   - FSBLA_Sdmmc2_A7_Signed.bin
     . This is the First Stage BootLoader which will be programmed via STM32PRGFW_UTIL_MP13xx_CP_Serial_Boot.stm32
	   in EMMC and will be used for loading test application from EMMC to DDR.
	   
   - MP13_BSP_TemplatesA7_DISCO_Signed.bin
     . This is the test application, which will be programmed in EMMC and then loaded in DDR
       for execution. This can be replaced with any test application used by customer and has to built
       and properly signed with CubeProgrammer/signing tool ( or via Postbuild script).
	   This user app must be generated with ddr linker(not sysram linker) for compatibility.
	   DDR linker is kept at Drivers\CMSIS\Device\ST\STM32MP13xx\Source\Templates\gcc\linker\stm32mp13xx_a7_ddr.ld
	   can be easily tailored for any customer needs.
	   
   
   - MMC_Ext_Loader.bin
     . This is used for programming EMMC.
	 
   - STM32PRGFW_UTIL_MP13xx_CP_Serial_Boot.stm32
     . This utilises MMC_Ext_Loader.bin and FSBLA_Sdmmc2_A7_Signed.bin for  
	   programming EMMC, loading and execution from DDR respectively.

Hardware prerequisites

  - Set boot boot pins to b000 (BOOT0/1/2 to OFF) position to select 
    [serial boot]
	(https://wiki.st.com/stm32mpu/wiki/STM32_MPU_ROM_code_overview#Boot_device_selection)
  - Connect micro-USB cable between PC and the board. PC should detect UART interface.
    NOTE: could be STLink connector if Virtual COM port is connected on the board 
	like ST VALID boards.
    NOTE: USB cable should not be connected other than STLink USB port on the board, if you want 
	to use UART !!! - ROM CODE will give preference to USB DFU if USB cable connected at other places 
	than STLink USB port.

STM32CubeProgrammer GUI interface

Please Read STM32CubeProgrammer user manual for further details if needed

  - Select UART port.
  - Click on Connect button - STM32CubeProgrammer is now connected to ROM Code
  - Select TSV 
    . Firmware\Projects\STM32MP135C-VALID\External_Loader\EMMC_Ext_Loader\FlashLayout_OpenBL_ExtLoaderEMMC_SerialBoot.tsv
  - Select Binary Path with Browse Button: Firmware\Projects\STM32MP135C-VALID\External_Loader\EMMC_Ext_Loader
  - Click on Download - STM32CubeProgrammer is now connected to EMMC External Loader firmware
    and started downloading the binaries.

STM32CubeProgrammer CLI interface

Please Read STM32CubeProgrammer user manual for further details if needed

  - Go to the STM32CubeProgrammer binary directory (i.e. STM32CubeProgrammer\bin)
  - Open command prompt inside this bin directory
  - Run below command to load the EMMC External Loader binaries in the embedded RAM:
    $STM32_Programmer_CLI.exe -c port=COM <num> -w <Your Directory Path>\Projects\<STM32 device>\External_Loader\EMMC_Ext_Loader\FlashLayout_OpenBL_ExtLoaderEMMC_SerialBoot.tsv
  - Do not reset the board till the command has completed.
  
Test Application Output
  - After all the binaries are loaded successfully, set boot pins to b010 (BOOT0 and BOOT2 to OFF and BOOT1 to ON) position to select 
    [EMMC boot]https://wiki.st.com/stm32mpu/wiki/STM32_MPU_ROM_code_overview#Boot_device_selection) and reset.
  - Blue LED toggles at 200ms rate, indicating that test application loading and execution is successful.

For any other question related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the ST Community
on the STM32 MPUs related [page](https://community.st.com/s/topic/0TO0X0000003u2AWAQ/stm32-mpus).
