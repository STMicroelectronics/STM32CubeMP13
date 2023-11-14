/**
  @page DDR Init example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    DDR_Init/readme.txt
  * @author  MCD Application Team
  * @brief   Description of 'DDR Init" example.
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

This example provides a reference to initialize the DDR on STM32MP135F-DK (DDR3)

STM32MP135F-DK board's LED can be used to monitor the initialization status:
 - LED_RED is blinking when there is an error in DDR Init process.
 - LED_BLUE is blinking when the DDR is correctly initialized.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - DDR_Init/Inc/stm32mp13xx_hal_conf.h   	HAL Configuration file for Cortex-A7
  - DDR_Init/Inc/stm32mp13xx_disco_conf.h  	STM32MP135F-DK board configuration file
  - DDR_Init/Inc/stm32mp13xx_it.h         	Interrupt handlers header file for Cortex-A7
  - DDR_Init/Inc/main.h                 	Main program header file for Cortex-A7
  - DDR_Init/Src/main.c                 	Main program header file for Cortex-A7
  - DDR_Init/Src/stm32mp13xx_it.c         	Interrupt handlers for Cortex-A7
  - DDR_Init/Src/stm32mp13xx_hal_msp.c    	HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */