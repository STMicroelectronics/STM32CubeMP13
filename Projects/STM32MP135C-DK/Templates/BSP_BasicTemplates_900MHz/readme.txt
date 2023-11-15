/**
  @page Templates  Description of the Templates example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    Templates/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
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
This project provides a reference template that can be used to build any firmware application.
The goal of this template is to :
	- increase MPU frequency from 650MHz to 900 MHz with function UpdateSystemClock_config_to_900() and,
	- display an "Hello World" in a console.

In this example, if WFI interrupt requested, it is mandatory to be back @ 650MHz before (with function SystemClock_config_back_to_650()) 

The BSP STM32MP13xx_DISCO is used to configure the UART.

This directory provides a reference template project that can be used to build any firmware application for
STM32MP135CACx devices using STM32CubeMP13 HAL and running on STM32MP135F Disco Board from STMicroelectronics. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - Templates/Src/main.c                     Main program
  - Templates/Src/stm32mp13xx_it.c           Interrupt handlers 
  - Templates/Src/stm32mp13xx_hal_msp.c      HAL MSP module
  - Templates/Inc/main.h                     Main program header file  
  - Templates/Inc/stm32mp13xx_hal_conf.h     HAL Configuration file
  - Templates/Inc/stm32mp13xx_disco_conf.h   BSP configuration for Disco board
  - Templates/Inc/stm32mp13xx_it.h           Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STMicroelectronics STM32MP135F-DK
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
