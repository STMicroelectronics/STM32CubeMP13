/**
  @page MDMA_Mem2MemTransfer MDMA Memory to Memory Transfer

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    MDMA/MDMA_Mem2MemTransfer/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MDMA Memory to Memory Transfer.
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

This example provides a description of how to use a MDMA (configured non-secure) to transfer a data buffer from SYSRAM to SRAM3 
through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

In this example, System clock is configured at 650 MHz

MDMA is configured to transfer the contents of a 32-word data
buffer stored in SYSRAM to the reception buffer declared in SRAM3.

In this example the MDMA is configured as Non-Secure and Buffer Transfer mode is enabled.
Also an incremental burst transfer of 16 beats is requested.

The start of transfer is triggered by software. 
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.


STM32MP135F-DK Board's LEDs can be used to monitor the transfer status:
 - LED_GREEN is ON when the transfer is complete (into the Transfer Complete interrupt
   routine).
 - LED_GREEN is Toggled with a period of 200 ms when there is a transfer error
 - LED_GREEN is Toggled with a period of 500 ms when Error_Handler is called

It is possible to select a different stream for the MDMA transfer example

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - MDMA/MDMA_Mem2MemTransfer/Src/system_stm32mp13xx.c    STM32MP13xx system clock configuration file
  - MDMA/MDMA_Mem2MemTransfer/Src/stm32mp13xx_it.c        Interrupt handlers
  - MDMA/MDMA_Mem2MemTransfer/Src/main.c                  Main program
  - MDMA/MDMA_Mem2MemTransfer/Inc/stm32mp13xx_hal_conf.h  HAL Configuration file
  - MDMA/MDMA_Mem2MemTransfer/Inc/stm32mp13xx_it.h        Interrupt handlers header file
  - MDMA/MDMA_Mem2MemTransfer/Inc/main.h                  Main program header file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STMicroelectronics STM32MP135F-DK
    evaluation boards and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
