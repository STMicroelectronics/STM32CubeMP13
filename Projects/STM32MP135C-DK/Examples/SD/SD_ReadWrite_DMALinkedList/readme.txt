/**
  @page SD_ReadWrite_DMALinkedList Description of the SD_ReadWrite_DMALinkedList example
  
  @verbatim
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMALinkedList/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SD read and write linked list buffers with internal DMA example.
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
This example performs some write and read transfers to SD Card with SDMMC IP internal
DMA mode based on Linked list feature.

@note This example works with Normal SD Card.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the SD interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices.

The transfer clock is set at (SDMMC_CLK / Clock_DIV * 2) and configured within the
SD Handle init parameters.
Next, prepare linked list nodes for write buffers and start the writing transfer to SD Card.
Then prepare linked list nodes for read buffers to write and start the reading transfer from SD Card.

STM32MP135F-DK board's LEDs can be used to monitor the transfer status:

 - LED_BLUE  is ON when write/read transfer is ongoing.
 - LED_GREEN is Toggling  when write and read transfer is completed and check buffers are OK.
 - LED_RED is Toggling  when there is an error in transfer process or initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Storage, SD, SDMMC, Read Write, Block, Sector, DMA, Linked List

@par Directory contents

  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - SD/SD_ReadWrite_DMALinkedList/Inc/main.h                    Header for main.c module
  - SD/SD_ReadWrite_DMALinkedList/Inc/stm32mp13xx_disco_conf.h  STM32MP135F-DK board configuration file
  - SD/SD_ReadWrite_DMALinkedList/Src/stm32mp13xx_it.c          Interrupt handlers
  - SD/SD_ReadWrite_DMALinkedList/Src/main.c                    Main program
  - SD/SD_ReadWrite_DMALinkedList/Src/stm32mp13xx_hal_msp.c     HAL MSP module

        
@par Hardware and Software environment  

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

  STM32MP135F-DK Set-up :	
  - Connect the uSD Card to the Micro SD connector (CN29).
 
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Plug the SD Card in the STM32MP135F-DK
 - Run the example

 */
