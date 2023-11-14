/**
  @page RNG_MultiRNG Multiple Random Numbers Generator example
  
  @verbatim
  ******************************************************************************
  * @file    RNG/RNG_MultiRNG/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of multiple random numbers generation example.
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

Configuration of the RNG using the HAL API. This example uses the RNG to generate 32-bit long random numbers.

In this example, System clock is configured at 650 MHz.

The RNG peripheral configuration is ensured by the HAL_RNG_Init() function.
The latter is calling the HAL_RNG_MspInit() function which implements
the configuration of the needed RNG resources according to the used hardware (CLOCK, 
GPIO, DMA and interrupt). You may update this function to change RNG configuration.

After startup, user is asked to press User Button.
The 8-entry array aRandom32bit[] is filled up by 32-bit long random numbers 
at each User Button press.

 
The random numbers can be displayed on the debugger in aRandom32bit variable.

In case of error, LED_GREEN is toggling at a frequency of 1Hz, otherwise LED_GREEN turns ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period

@par Directory contents 

  - RNG/RNG_MultiRNG/Src/system_stm32mp13xx.c    STM32MP13xx system clock configuration file
  - RNG/RNG_MultiRNG/Src/stm32mp13xx_it.c        Interrupt handlers
  - RNG/RNG_MultiRNG/Src/main.c                  Main program
  - RNG/RNG_MultiRNG/Inc/stm32mp13xx_hal_conf.h  HAL Configuration file
  - RNG/RNG_MultiRNG/Inc/stm32mp13xx_it.h        Interrupt handlers header file
  - RNG/RNG_MultiRNG/Inc/main.h                  Main program header file

     
@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 */