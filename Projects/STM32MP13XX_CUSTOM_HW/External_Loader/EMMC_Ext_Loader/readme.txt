/**
  @page MMC_Ext_Loader Description of the MMC_Ext_Loader example
  
  @verbatim
  ******************************************************************************
  * @file    External_Loader/MMC_Ext_Loader/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MMC read and write example.
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
This example performs some write and read transfers to MMC Card with MMC IP feature.

@note This example works with Normal MMC Card.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the MMC interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices.

The transfer clock is set at (MMC_CLK / Clock_DIV * 2) and configured within the
MMC Handle init parameters.
Next, prepare Buffer0 and start the writing transfer to MMC Card.
Then prepare Buffer1 and start reading transfer from MMC Card.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Storage, MMC, Read, Write, Block, Sector, DMA, Linked List

@par Directory contents

  - External_Loader/EMMC_Ext_Loader/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - External_Loader/EMMC_Ext_Loader/Inc/RTE_Components.h          Run-Time-Environment Component Configuration file
  - External_Loader/EMMC_Ext_Loader/Inc/main.h                    Header for main.c module
  - External_Loader/EMMC_Ext_Loader/Src/main.c                    Main program
  - External_Loader/EMMC_Ext_Loader/Src/stm32mp13xx_hal_msp.c     HAL MSP module

        
@par Hardware and Software environment  

  - This example runs on STM32MP135xx devices.

  - This example has been tested with ST internal board and can be
    easily tailored to any other supported device and development board.
    
  ST internal board Set-up :	
  - You can display test message on the HyperTerminal,
      * Connect the board to the PC serial port.
 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - This example can also be used as a part of External Loader application package for programming EMMC via
   STM32CubeProgrammer. Refer Projects/STM32MP13XX_CUSTOM_HW/External_Loader/readme.txt for more details.

 */
