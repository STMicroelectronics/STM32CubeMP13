/**
  @page LTDC_Display_2Layers LTDC Display 2 layers example

  @verbatim
  ******************************************************************************
  * @file    LTDC/LTDC_Display_2Layers/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LTDC Display 2 layers example.
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

  How to configure the LTDC peripheral to display two layers at the same time.
  The goal of this example is to explain how to blend two layers.

  The DDR memory need to be initialized with DDR_Init project.
  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices :
  The CPU at 650 MHz.
  The HCLK for D1 Domain AXI and AHB3 peripherals , D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4  peripherals at 650 MHz/2.
  The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 650 MHz/4.

  After LCD initialization, the LCD layer 1 and Layer 2 are configured as following :
  - Layer 1 is configured to display an image loaded from DDR with direct
    color (RGB565) as pixel format and 320x240 (QVGA) size.
  - Layer 2 is configured to display an image loaded from DDR with direct
    color (RGB565) as pixel format and 320x240 (QVGA) size.

  The blending is always active and the two layers can be blended following
  the configured blending factors and the constant alpha.
  In this example the constant alpha for layer 2 is decreased to see the layer 1
  in the intersection zone.

  Example :
  ------------------------------------------------------------------------------
  |Image data   |  position   CLUT(RGB Value)  |   Alpha  | Output (ARGB value)|
  |-------------|------------------------------|----------|------------------- |
  |00 10 FF 25  |  0   -----> 0xFD10EA         | 0xFF     | 0xFFFD10EA         |
  |             |  .                           |          | 0xFF1548AD         |
  |             |  .                           |          | 0xFFAE6547         |
  |             |  .                           |          | 0xFFDA14EA         |
  |             |  .                           |          | .                  |
  |             |  16  -----> 0x1548AD         |          | .                  |
  |             |  .                           |          | .                  |
  |             |  .                           |          | .                  |
  |             |  37  -----> 0xDA14EA         |          | .                  |
  |             |  .                           |          |                    |
  |             |  .                           |          |                    |
  |             |  255 -----> 0xAE6547         |          |                    |
  ------------------------------------------------------------------------------

  The images change unlike their positions on LCD in an infinite loop.

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

Display, LTDC, Display Controller, TFT, LCD, Graphics, RGB888, 2 Layers, RGB,

@par Directory contents

    - LTDC/LTDC_Display_2Layers/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32mp13xx_disco_conf.h  STM32MP135F-DK board configuration file
    - LTDC/LTDC_Display_2Layers/Inc/stm32mp13xx_it.h          Interrupt handlers header file
    - LTDC/LTDC_Display_2Layers/Inc/main.h                    Main configuration file
    - LTDC/LTDC_Display_2Layers/Src/main.c                    Main program
    - LTDC/LTDC_Display_2Layers/Src/stm32mp13xx_it.c          Interrupt handlers
    - LTDC/LTDC_Display_2Layers/Src/stm32mp13xx_hal_msp.c     HAL MSP module
    - LTDC/LTDC_Display_2Layers/Inc/st_logo_320x240.h         Image layer 1 to be displayed on LCD : QVGA (320x240) in RGB565

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all files of LTDC example and load your image into target memory
 - Run the example

 */