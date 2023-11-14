/**
  @page DTS_GetTemperature DTS Get Temperature example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    DTS/DTS_GetTemperature/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DTS Get Temperature example.
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

How to configure and use the DTS to get the temperature of the die.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices :
The CPU at 650 MHz.
The HCLK for D1 Domain AXI and AHB3 peripherals, D2 Domain AHB1/AHB2 peripherals and D3 Domain AHB4 peripherals at 650 MHz/2.
The APB clock dividers for D1 Domain APB3 peripherals, D2 Domain APB1 and APB2 peripherals and D3 Domain APB4 peripherals to run at 650 MHz/4.

The DTS is configured to continuously get the temperature.
The Temperature variable will hold the temperature value in °C unit.
The Temperature value can be displayed on the debugger in "Temperature" variable.

STM32 board LED can be used to monitor the error detection:
  - LED BLUE is ON when the program is completed.
  - LED RED is Toggling when there is an error in initialization.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

DTS, Sensor, Temperature, Die

@par Directory contents

  - DTS/DTS_GetTemperature/Inc/stm32mp13xx_hal_conf.h  HAL configuration file
  - DTS/DTS_GetTemperature/Inc/stm32mp13xx_it.h        HAL interrupt handlers header file
  - DTS/DTS_GetTemperature/Inc/stm32mp13_disco_conf.h  STM32MP135F-DK board configuration file
  - DTS/DTS_GetTemperature/Inc/main.h                  Header for main.c module
  - DTS/DTS_GetTemperature/Src/main.c                  Main program
  - DTS/DTS_GetTemperature/Src/stm32mp13xx_it.c        HAL interrupt handlers
  - DTS/DTS_GetTemperature/Src/stm32mp13xx_hal_msp.c   HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

  - To get the temperature information, put the "Temperature" variable in the debugger watch.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
