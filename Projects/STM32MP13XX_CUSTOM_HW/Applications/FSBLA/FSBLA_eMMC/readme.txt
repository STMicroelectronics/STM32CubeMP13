/**
  @page FSBLA_eMMC example

  @verbatim
  ******************** (C) COPYRIGHT 2023 STMicroelectronics *******************
  * @file    FSBLA_eMMC/readme.txt
  * @author  MCD Application Team
  * @brief   Description of 'FSBLA eMMC" application.
  ******************************************************************************
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

  This application describes how to load and execute a cubeExample binary from E-MMC into DDR on STM32MP13XX-CUSTOM_HW (DDR3). 

@par The process of FSBLA_eMMC is : 

  This FSBLA_eMMC application:
  - Initialize the DDR on STM32MP13XX-CUSTOM_HW (DDR3).
  - Initialize eMMC.
  - Read from eMMC the cubeExample image length and load cubeExample into DDR.
    (Rq : cubeExample binary has to be prealably signed and loaded in E-MMC.)
  - Jump into DDR to execute this cubeExample 
    (Rq : by default at the beginning of the DDR : DDR_MEM_ADD = 0xC0000000)

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FSBLA_eMMC/Inc/stm32mp13xx_hal_conf.h   	HAL Configuration file for Cortex-A7
  - FSBLA_eMMC/Inc/stm32mp13xx_valid_conf.h  	STM32MP135F-CUSTOM_HW board configuration file
  - FSBLA_eMMC/Inc/stm32mp13xx_it.h         	Interrupt handlers header file for Cortex-A7
  - FSBLA_eMMC/Inc/main.h                 	    Main program header file for Cortex-A7
  - FSBLA_eMMC/Src/main.c                 	    Main program header file for Cortex-A7
  - FSBLA_eMMC/Src/stm32mp13xx_it.c         	Interrupt handlers for Cortex-A7
  - FSBLA_eMMC/Src/stm32mp13xx_hal_msp.c    	HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STM32MP13XX-CUSTOM_HW board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

  In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Sign a cubeExample binary thanks to the signing tool.
  - Flash this signed cubeExample binary into E-MMC at this specific E-MMC Offset in User Partition : 0 LBA (0x00000000).
    (Rq: It is possible to change this value by changing the value of the variable OFFSET_HEADER in the file main.c.)
  - Run the FSBLA_eMMC.
  - This example can also be used as a part of External Loader application package for programming EMMC via
   STM32CubeProgrammer. Refer Projects/STM32MP13XX_CUSTOM_HW/External_Loader/readme.txt for more details.
  
If you want to BOOT from E-MMC : 
  - Open your preferred toolchain 
  - Sign a cubeExample binary thanks to the signing tool.
  - Flash this signed cubeExample binary into E-MMC at this specific E-MMC Offset in User Partition : 0 LBA (0x00000000).
    (Rq: It is possible to change this value by changing the value of the variable OFFSET_HEADER in the file main.c.)
  - Rebuild all files and flash signed FSBLA_eMMC binary into E-MMC at this specific E-MMC Offset  in User Partition : 0 LBA (0x00000000).
  - Reset to BOOT from E-MMC.

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */