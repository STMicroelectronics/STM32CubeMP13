/**
  @page RTC_Timestamp RTC Timestamp Example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_Timestamp/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC Timestamp example.
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

Configuration of the RTC HAL API to demonstrate the timestamp feature.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The System clock is configured at 650 MHz.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit() function which core is implementing
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

HAL_RTCEx_SetTamper_IT() function is then called to initialize the Tamper with
interrupt mode. It configures the tamper pin to be falling edge and enables
the time stamp event on Tamper detection.
HAL_RTC_SetTime() and HAL_RTC_SetDate() functions are then called to initialize the 
time and the date.

@note Debugger will go OFF on tamper detection.
      Then, this test has been made with UART configuration to print useful variables on the console.
      Please, open a serial console.

The associated firmware performs the following:
1. It configures the Tamper pin to be falling edge, and enables the Tamper interrupt.

2. It display each second, via serial console, the current time and date respectively as aShowTime and aShowDate.

2. When applying a low level on the Tamper (Tamper2) pin (PA.6) by pressing the Tamper push-button,
   a time stamp event is detected and the calendar is saved in the time stamp structures.
   Each time the user presses on the User Button, the current time and date stamp are updated and displayed
   via serial console respectively as aShowTimeStamp and aShowDateStamp.

- LED_GREEN is toggling : This indicates that the system generates an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - RTC/RTC_Timestamp/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - RTC/RTC_Timestamp/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - RTC/RTC_Timestamp/Inc/main.h                    Header for main.c module
  - RTC/RTC_Timestamp/Inc/stm32mp13xx_hal_conf.h    STM32MP13xx board configuration file
  - RTC/RTC_Timestamp/Src/stm32mp13xx_it.c          Interrupt handlers
  - RTC/RTC_Timestamp/Src/main.c                    Main program
  - RTC/RTC_Timestamp/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - RTC/RTC_Timestamp/Src/system_stm32mp13xx.c      STM32MP13xx system source file


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
