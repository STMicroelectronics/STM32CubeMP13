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

This projects provides a reference template that can be used to build any firmware application.

This directory provides a reference template project that can be used to build any firmware application for
STM32MP13xx devices using STM32CubeMP13 HAL and running on STM32MP13XX_CUSTOM_HW board from STMicroelectronics. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Expected success behavior
- A Hello World message is displayed in Output console

@par Directory contents 

  - BSP_TemplatesA7/Src/main.c                   Main program
  - BSP_TemplatesA7/Src/system_stm32mp13xx.c     STM32MP13xx system clock configuration file
  - BSP_TemplatesA7/Src/stm32mp13xx_it.c         Interrupt handlers 
  - BSP_TemplatesA7/Src/stm32mp13xx_hal_msp.c    HAL MSP module
  - BSP_TemplatesA7/Inc/main.h                   Main program header file  
  - BSP_TemplatesA7/Inc/stm32mp13xx_hal_conf.h   HAL Configuration file
  - BSP_TemplatesA7/Inc/stm32mp13xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STMicroelectronics STM32MP13XX_CUSTOM_HW
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
   - 2 compilations are possible :
     . Debug_Custom : for execution on SYSRAM
     . Debug_Custom_DDR : for execution on DDR
 - At the end of the compilations a "Postbuild" script add a Header expected by BootROM or FSBLA application.
  ( a complete signature can be done by  CubeProgrammer/signing tool)
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
