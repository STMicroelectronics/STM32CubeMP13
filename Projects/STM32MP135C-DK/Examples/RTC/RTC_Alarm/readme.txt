/**
  @page RTC_Alarm RTC Alarm Example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_Alarm/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC Tamper example.
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

How to configure and generate an RTC alarm using the RTC HAL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the systick.

The System clock is configured at 650 MHz.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default.
      The user can use also LSI or HSE as RTC clock source. The user uncomment the adequate
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSE
        /* #define RTC_CLOCK_SOURCE_LSI */
        /* #define RTC_CLOCK_SOURCE_HSE */
      @endcode
      LSI oscillator clock is delivered by a 32 kHz RC.
      LSE (when available on board) is delivered by a 32.768 kHz crystal.
      HSE (when available on board)

HAL_RTC_SetDate() and HAL_RTC_SetTime() functions are called to initialize the 
time and the date.
HAL_RTC_SetAlarm_IT() function is then called to initialize the Alarm feature with 
interrupt mode.

In this example, the Time is set to 02:20:00 and the Alarm must be generated after 
30 seconds on 02:20:30.

While the 30 seconds are not elapsed yet LED_BLUE is toggled with a period of 1 second.

LED_GREEN is turned ON when the RTC Alarm is generated correctly.
The current time is updated and displayed on the debugger in aShowTime variable.

In case of error, LED_BLUE is toggled with a period of 100 milliseconds.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - RTC/RTC_Alarm/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - RTC/RTC_Alarm/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - RTC/RTC_Alarm/Inc/main.h                    Header for main.c module
  - RTC/RTC_Alarm/Inc/stm32mp13xx_hal_conf.h    STM32MP13xx board configuration file
  - RTC/RTC_Alarm/Src/stm32mp13xx_it.c          Interrupt handlers
  - RTC/RTC_Alarm/Src/main.c                    Main program
  - RTC/RTC_Alarm/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - RTC/RTC_Alarm/Src/system_stm32mp13xx.c      STM32MP13xx system source file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
  - This example has been tested with STMicroelectronics STM32MP135F-DK
    board and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
