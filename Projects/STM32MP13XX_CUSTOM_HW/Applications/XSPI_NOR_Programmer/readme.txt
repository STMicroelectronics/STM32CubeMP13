/**
  @page XSPI_NOR_Programmer  Description of the XSPI_NOR_Programmer example
  
  @verbatim
  ******************** (C) COPYRIGHT 2022 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the example used to load A7 signed binary onto XSPI-NOR (MX25L51245G)
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
This application shows how to load a signed binary to an XSPI-NOR (QSPI-NOR MX25L51245G).
This example run on Internal STM32MP13XX_CUSTOM_HW Board including MX25L51245G Flash ( not possible on STM32MP135C-DK board)

@par  The user should select one option in a menu:
 1- binary 'FSBLA_SNOR" > This binary MUST:
   . be made for A7 and compiled for SYSRAM,
   . not exceed SYSRAM size ( 128KB),
   . be properly signed with CubeProgrammer/signing tool ( or at least Header for debug - via Postbuild script),
   . be loaded at offset @0x0 of the XSPI-NOR. This address can not be modified by user.
    (In case of boot from NOR, this binary will be loaded by the bootrom on SYSRAM and executed.)
 2- binary 'Cube_Example" > This binary MUST:
   . be made for A7 and compiled for DDR (LPDDR3_4Gb for Internal STM32MP13XX_CUSTOM_HW Board)
   . be properly signed with  CubeProgrammer/signing tool ( or at least Header for debug - via Postbuild script),
   . be loaded at offset @0x30000 of the XSPI-NOR. This address can be modified by the user.
    (This binary will be loaded by FSBLA_SNOR on DDR and executed)
 Q- Quit the application

@par When the user do his choice, the selected binary is manipulated via semihosting function and loaded in XSPI-NOR.
The binaries to flash are stored on C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13 (This path can be modified by the user).
If FSBLA_SNOR binary is correctly loaded, a message is displayed on STM32CubeIDE console, and LED_BLUE is ON.
If Cube_Example binary is correctly loaded, a message is dispolayed on STM32CubeIDE console, and LED_RED is ON.
If a problem occurred, the LED_RED is blinking.

@par Directory contents 

  - /Inc/stm32mp13xx_hal_conf.h       HAL configuration file
  - /Inc/stm32mp13xx_it.h             Interrupt handlers header file
  - /Inc/main.h                       Header for main.c module  
  - /Inc/loader_XSPI_NOR.c            Header for loader_XSPI_NOR.c file
  - /Src/stm32mp13xx_it.c             Interrupt handlers
  - /Src/main.c                       Main program
  - /Inc/loader_XSPI_NOR.c            Loader operations for external XSPI-NOR memory (MX25L51245G)
  - /Src/stm32mp13xx_hal_msp.c        HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32MP135Fx devices (tested with an internal STM32MP13XX_CUSTOM_HW board including MX25L51245G XSPI-NOR memory).
    
  - You can display message on STM32CubeIDE console thanks to semihosting. 
  For Debug Configuration, "mon arm semihosting enable" should be added in <startup/Initialisation commands> window ( after "monitor halt")


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files for your board and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
