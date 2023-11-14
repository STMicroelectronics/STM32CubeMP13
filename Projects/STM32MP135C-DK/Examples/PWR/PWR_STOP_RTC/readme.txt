/**
  @page PWR_STOP_RTC Power Stop Mode Example 
  
  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_STOP_RTC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode using RTC example.
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

How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event connected to an interrupt.

In the associated software
  - the system clock is set to 650 MHz.
  - the Low Speed Internal (LSI) clock is used as RTC clock source by default.
  - the EXTI_Line19 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 20s.
  - the SysTick is programmed to generate an interrupt each 1 ms. 

After wake-up from Stop mode, program execution is resumed.

Two leds LED_BLUE and LED_GREEN are used to monitor the system state as following:
 - LED_BLUE ON: configuration failed (system will go to an infinite loop)
 - LED_GREEN ON: system in Run mode
 - LED_GREEN OFF: system in Stop mode

These steps are repeated in an infinite loop

The above scenario is repeated in an infinite loop.
 
@note This example can not be used in DEBUG mode, this is due to the fact 
      that the system core is no longer clocked during low power mode 
       so debugging features are disabled.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
      the RTC clock source; in this case the Backup domain will be reset in  
      order to modify the RTC Clock source, as consequence RTC registers (including 
      the backup registers) and RCC_CSR register are set to their reset values.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI, 
#endif

@par Directory contents 

  - PWR/PWR_STOP_RTC/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - PWR/PWR_STOP_RTC/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - PWR/PWR_STOP_RTC/Inc/main.h                    Header for main.c module
  - PWR/PWR_STOP_RTC/Src/stm32mp13xx_it.c          Interrupt handlers
  - PWR/PWR_STOP_RTC/Src/main.c                    Main program
  - PWR/PWR_STOP_RTC/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - PWR/PWR_STOP_RTC/Src/system_stm32mp13xx.c      STM32MP13xx system source file

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
 