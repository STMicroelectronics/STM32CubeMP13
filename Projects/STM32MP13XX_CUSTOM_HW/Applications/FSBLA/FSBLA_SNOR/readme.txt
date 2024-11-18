/**
  @page FSBLA_SNOR example

  @verbatim
  ******************** (C) COPYRIGHT 2022 STMicroelectronics *******************
  * @file    FSBLA_SNOR/readme.txt
  * @author  MCD Application Team
  * @brief   Description of 'FSBLA SNOR" application.
  ******************************************************************************
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

  This application describes how to load a cubeExample binary from XSPI-NOR into DDR, and execute it on STM32MP13XX_CUSTOM_HW Board.
  This application is needed to boot from NOR ( this is the First Stage Boot Loader)

@par This FSBLA_SNOR application:
  - Initialize the DDR on STM32MP13XX_CUSTOM_HW (LPDDR3).
  - Initialize XSPI (in QuadSPI mode).
  - Read from XSPI-NOR the cubeExample image length and load cubeExample into DDR.
    (Prerequisite : cubeExample binary has to be prealably signed and loaded in XSPI-NOR.)
  - Jump into DDR to execute this cubeExample 
    (Rq : by default at the beginning of the DDR : DDR_MEM_ADD = 0xC0000000)

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FSBLA_SNOR/Inc/stm32mp13xx_hal_conf.h       HAL configuration file
  - FSBLA_SNOR/Inc/stm32mp13xx_it.h             Interrupt handlers header file
  - FSBLA_SNOR/Inc/main.h                       Header for main.c module  
  - FSBLA_SNOR/Inc/loader_XSPI_NOR.c            Header for loader_XSPI_NOR.c file
  - FSBLA_SNOR/Src/stm32mp13xx_it.c             Interrupt handlers
  - FSBLA_SNOR/Src/main.c                       Main program
  - FSBLA_SNOR/Inc/loader_XSPI_NOR.c            Loader operations for external XSPI-NOR memory (MX25L51245G)
  - FSBLA_SNOR/Src/stm32mp13xx_hal_msp.c        HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STM32MP13XX_CUSTOM_HW board and can be
    easily tailored to any other supported device and development board.
    (not possible to execute on STM32MP135C-DK board as no XSPI-NOR available on this board)

@par How to use it ?

  In order to make the program work, you must do the following :
  - Open your preferred toolchain
  - Sign a cubeExample binary thanks to the CubeProgrammer/signing tool (or at least add Header for debug - via Postbuild script),
  - Flash this signed cubeExample binary into XSPI-NOR at this specific Offset (0x30000).(XSPI_NOR_PROGRAMMER application can be used)
    (Rq: It is possible to change this value by changing the value of the variable FLASH_ADDRESS_SNOR_CUBE in the file main.c)
  - Run the FSBLA_SNOR.
  - This example can also be used as a part of External Loader application package for programming SNOR flash via
   STM32CubeProgrammer. Refer Projects/STM32MP13XX_CUSTOM_HW/External_Loader/readme.txt for more details.
  
If you want to BOOT from NOR : 
  - Open your preferred toolchain 
  - Sign a cubeExample binary thanks to the CubeProgrammer/signing tool (or at least add Header for debug - via Postbuild script),
  - Sign the FSBLA_SNOR binary thanks to the CubeProgrammer/signing tool (or at least add Header for debug - via Postbuild script),
  - Flash these binaries into XSPI-NOR (with XSPI_NOR_PROGRAMMER application or other)
    (Rq: address by default : 
        . FSBLA_SNOR flash @0x0 of XSPI NOR (value can't be modify, related to bootrom)
        . cubeExample flashed at @0x30000 of XSPI NOR (value can be modified : FLASH_ADDRESS_SNOR_CUBE in the file main.c)
  - Reset to BOOT from NOR ( don't forget the Boot Pin).

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */