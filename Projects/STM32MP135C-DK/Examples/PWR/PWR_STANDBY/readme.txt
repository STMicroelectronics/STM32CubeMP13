/**
  @page PWR_STANDBY PWR standby example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Standby example.
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
 
This example show how to enter the Standby mode and wake up from this mode by using an external 
reset or the WKUP pin.

In the associated software, the system clock is set to 650 MHz, an EXTI line
is connected to the BUTTON_WAKEUP through (PF.8) pin and configured to generate an 
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler.

After a reset (or wake-up from STANDBY mode), LED_RED is ON and MPU is in RUN mode.
The system enters STANDBY mode after 5s, LED_RED turns OFF.
A falling edge on WAKEUP pin WKUP1 (PF.8) by pressing BUTTON_WAKEUP will wake-up the system from STANDBY.
Alternatively, an external RESET of the board will lead to a system wake-up as well.
After wake-up from Standby mode, system resets and LED_RED turns ON again. 

In case of error, LED_GREEN turns ON.

User can also connect a scope on VDD_CORE pin (CN14 pin 9/10) to look at the VDD_CORE signal. 
VDD_CORE signal position:
- HIGH: reset state / wake-up from STANDBY mode
- LOW: system in Standby mode

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

  - PWR/PWR_STANDBY/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - PWR/PWR_STANDBY/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - PWR/PWR_STANDBY/Inc/main.h                    Header for main.c module
  - PWR/PWR_STANDBY/Src/stm32mp13xx_it.c          Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                    Main program
  - PWR/PWR_STANDBY/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - PWR/PWR_STANDBY/Src/system_stm32mp13xx.c      STM32MP13xx system source file


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
