/**
  @page PWR_STANDBY_RTC PWR Standby RTC example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY_RTC/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Standby RTC example.
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
 
How to enter the Standby mode and wake-up from this mode by using an external 
reset or the RTC wakeup timer.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

The SysTick is programmed to generate an interrupt each 1 ms.

In this example, System clock is configured at 650 MHz.

The Low Speed Internal (LSI) clock is used as RTC clock source by default.
EXTI_Line19 is internally connected to the RTC Wakeup event.

The system automatically enters Standby mode 5 seconds after start-up. The RTC wake-up
is configured to generate an interrupt on rising edge about 33 seconds afterwards.
Current consumption in Standby mode with RTC feature enabled can be measured during that time.
These 33 seconds are chosen to ensure current convergence to its lowest operating point.

After wake-up from Standby mode, the program execution restarts in the same way as after
a software RESET.

Two leds LED_GREEN and LED_RED (Boot led) are used to monitor the system state as following:
 - LED_GREEN ON: configuration failed (system will go to an infinite loop)
 - LED_RED ON: system in Reset mode or wake-up from Standby mode
 - LED_RED OFF, Debugger loss: system in Standby mode
 
@note This example can not be used in DEBUG mode due to the fact 
      that the system core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset,


@par Directory contents

  - PWR/PWR_STANDBY_RTC/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - PWR/PWR_STANDBY_RTC/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - PWR/PWR_STANDBY_RTC/Inc/main.h                    Header for main.c module
  - PWR/PWR_STANDBY_RTC/Src/stm32mp13xx_it.c          Interrupt handlers
  - PWR/PWR_STANDBY_RTC/Src/main.c                    Main program
  - PWR/PWR_STANDBY_RTC/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - PWR/PWR_STANDBY_RTC/Src/system_stm32mp13xx.c      STM32MP13xx system source file


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
