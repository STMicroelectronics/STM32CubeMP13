/**
  @page FreeRTOS_Semaphore FreeRTOS Semaphore application
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_Semaphore/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Semaphore application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to use semaphores with RTOS native API.

This application creates two threads that toggle LEDs through a shared semaphore,
as following:

The first thread which have the higher priority obtains the semaphore and 
toggle the LED7 each 200 ms. After 5 seconds it releases the semaphore and 
suspends itself.

The low priority thread can execute now, it obtains the semaphore and 
resume execution of the first thread, as it has the higher priority
the first thread will try to obtain the semaphore but it fails because
the semaphore is already taken by the low priority thread, which will
toggle the LED3 each 200 ms for 5 seconds before releasing the semaphore
to begin a new cycle

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

@par Keywords

RTOS, FreeRTOS, Threading, Semaphore, Priorities

@par Directory contents
    - FreeRTOS/FreeRTOS_Semaphore/Src/main.c                         Main program
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32mp13xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32mp13xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32mp13xx_hal_msp.c          MSP Initialization file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/main.h                         Main program header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32mp13xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32mp13xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/FreeRTOSConfig.h               FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32mp13xx_disco_conf.h       BSP configuration file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
  
  - This example has been tested with STMicroelectronics STM32MP135F-DK boards.
    and can be easily tailored to any other supported device and development board.

    

@par How to use it ?

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all FreeRTOS_Semaphore application files and load your image into target memory
 - Run the application
  

 */
