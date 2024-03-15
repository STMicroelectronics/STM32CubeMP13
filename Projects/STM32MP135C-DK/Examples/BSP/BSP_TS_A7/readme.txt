/**
  @page BSP BSP_TS_A7 example

  @verbatim
  ******************************************************************************
  * @file    BSP/BSP_TS_A7/readme.txt
  * @author  MCD Application Team
  * @brief   Description of Touchscreen example.
  ******************************************************************************
  * @attention
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

  This example provides a description of how to use the different BSP drivers on the board.
  
  The DDR memory need to be initialized with DDR_Init project.
  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices :
  LCD is initialized in LCD_Config api. 

STM32MP135F-DK board's LED can be used to monitor the transfer status:
 - LED_RED is blinking when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, TouchScreen, Demo, DDR, Button, Board Support Package

@par Directory contents

    
  - BSP/BSP_TS_A7/Inc/gt911_conf.h                GT911 configuration file
  - BSP/BSP_TS_A7/Inc/main.h                      Main program header file for Cortex-A7
  - BSP/BSP_TS_A7/Inc/stlogo.h                    Image used for BSP example
  - BSP/BSP_TS_A7/Inc/stm32mp13xx_hal_conf.h      HAL Configuration file for Cortex-A7
  - BSP/BSP_TS_A7/Inc/stm32mp13xx_it.h            Interrupt handlers header file for Cortex-A7
  - BSP/BSP_TS_A7/Inc/stm32mp13xx_disco_conf.h    STM32MP135F-DK board configuration file
  - BSP/BSP_TS_A7/Src/main.c                      Main program for Cortex-A7
  - BSP/BSP_TS_A7/Src/stm32mp13xx_hal_msp.c       HAL MSP module for Cortex-A7
  - BSP/BSP_TS_A7/Src/atm32mp13xx_it.c            Interrupt handlers for Cortex-A7
  - BSP//BSP_TS_A7/Src/touchscreen.c              Touchscreen features
 


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initilize the DDR memory
 - Rebuild all files of BSP_TS_A7 example and load your image into target memory
 - Remove "monitor reset" from the debugger settings of BSP_TS_A7 example
 - Run the example

 */