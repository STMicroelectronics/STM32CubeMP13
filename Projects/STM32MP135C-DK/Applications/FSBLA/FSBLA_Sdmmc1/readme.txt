/**
  @page FSBLA_Sdmmc1 example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    FSBLA_Sdmmc1/readme.txt
  * @author  MCD Application Team
  * @brief   Description of 'FSBLA SDMMC1" application.
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

@par Example Description

  This application describes how to load and execute a cubeExample binary from SDCARD into DDR on STM32MP135F-DK (DDR3). 

@par The process of FSBLA_Sdmmc1 is : 

  This FSBLA_Sdmmc1 application:
  - Initialize the DDR on STM32MP135F-DK (DDR3).
  - Initialize SDMMC1.
  - Read from SDMMC1 the cubeExample image length and load cubeExample into DDR.
    (Rq : cubeExample binary has to be prealably signed and loaded in SDCard.)
  - Jump into DDR to execute this cubeExample 
    (Rq : by default at the beginning of the DDR : DDR_MEM_ADD = 0xC0000000)

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FSBLA_Sdmmc1/Inc/stm32mp13xx_hal_conf.h   	HAL Configuration file for Cortex-A7
  - FSBLA_Sdmmc1/Inc/stm32mp13xx_disco_conf.h  	STM32MP135F-DK board configuration file
  - FSBLA_Sdmmc1/Inc/stm32mp13xx_it.h         	Interrupt handlers header file for Cortex-A7
  - FSBLA_Sdmmc1/Inc/main.h                 	Main program header file for Cortex-A7
  - FSBLA_Sdmmc1/Src/main.c                 	Main program header file for Cortex-A7
  - FSBLA_Sdmmc1/Src/stm32mp13xx_it.c         	Interrupt handlers for Cortex-A7
  - FSBLA_Sdmmc1/Src/stm32mp13xx_hal_msp.c    	HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

  In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Sign a cubeExample binary thanks to the signing tool.
  - Flash this signed cubeExample binary into SD CARD at this specific SD Offset : 640 LBA (0x00050000).
    (Rq: It is possible to change this value by changing the value of the variable OFFSET_HEADER in the file main.c.)
  - Run the FSBLA_Sdmmc1.
  
If you want to BOOT from micro SD : 
  - Open your preferred toolchain 
  - Sign a cubeExample binary thanks to the signing tool.
  - Flash this signed cubeExample binary into SD CARD at this specific SD Offset : 640 LBA (0x00050000).
    (Rq: It is possible to change this value by changing the value of the variable OFFSET_HEADER in the file main.c.)
  - Rebuild all files and flash signed FSBLA_Sdmmc1 binary into SD CARD at this specific SD Offset : 128 LBA(0x00010000).
  - Reset to BOOT from micro SD.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */