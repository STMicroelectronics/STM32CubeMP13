/**
  @page DMA_FIFOMode_Secure DMA FIFO mode Example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    DMA/DMA_FIFOMode_Secure/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DMA FIFO mode example.
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

This example provides a description of how to use a DMA to transfer
a word data buffer from Flash memory to embedded SYSRAM configured secure
with FIFO mode enabled through the HAL API.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

In this example, System clock is configured at 650 MHz

DMA3 is configured to transfer the contents of a 32-word data
buffer stored in Flash memory to the reception buffer declared in RAM.

In this example the FIFO mode is enabled and the FIFO threshold is configured
as "FULL". Also an incremental burst transfer of 4 beats is requested.

The start of transfer is triggered by software. DMA3 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the stream enable bit for DMA3.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled and the callback function (customized by user) is called.


STM32MP135F-DK Board's LEDs can be used to monitor the transfer status:
 - LED_GREEN is ON when the transfer is complete (into the Transfer Complete interrupt
   routine).
 - LED_GREEN is Toggled with a period of 200 ms when there is a transfer error
 - LED_GREEN is Toggled with a period of 1000 ms when Error_Handler is called

It is possible to select a different stream for the DMA transfer example

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - DMA/DMA_FIFOMode_Secure/Src/system_stm32mp13xx.c    STM32MP13xx system clock configuration file
  - DMA/DMA_FIFOMode_Secure/Src/stm32mp13xx_it.c        Interrupt handlers
  - DMA/DMA_FIFOMode_Secure/Src/main.c                  Main program
  - DMA/DMA_FIFOMode_Secure/Inc/stm32mp13xx_hal_conf.h  HAL Configuration file
  - DMA/DMA_FIFOMode_Secure/Inc/stm32mp13xx_it.h        Interrupt handlers header file
  - DMA/DMA_FIFOMode_Secure/Inc/main.h                  Main program header file


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
