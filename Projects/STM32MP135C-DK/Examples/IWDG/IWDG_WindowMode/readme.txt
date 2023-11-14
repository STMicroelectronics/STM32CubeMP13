/**
  @page IWDG_WindowMode IWDG Reset with window mode

  @verbatim
  ********************* COPYRIGHT(c) 2021 STMicroelectronics *******************
  * @file    IWDG/IWDG_WindowMode/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the IWDG Reset with window mode.
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

How to periodically update the IWDG reload counter and simulate a software fault that generates an MCU IWDG reset after a preset laps of time.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick. 
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 650 MHz.

The IWDG time-out is set to 762 ms (the time-out may varies due to LSI frequency dispersion).

The Window option is enabled with a window register value set to 400 ms. 
To prevent a reset, the down-counter must be reloaded when its value is:

-lower than the window register value (400ms)
-greater than 0x0
The IWDG counter is therefore refreshed each 450 ms in the main program infinite loop to prevent a IWDG reset (762 - 450 = 312 within the interval).
LED_BLUE is also toggled each 450 ms indicating that the program is running. LED_RED will turn on if any error occurs.

An EXTI Line is connected to a GPIO pin, configured to generate an interrupt when the User push-button (PC.13) is pressed.

Once the EXTI Line event occurs by pressing the User push-button (PC.13), the refresh period is set to 200 ms. 
That will make refresh being outside window value. As a result, when the IWDG counter is reloaded, the IWDG reset occurs.

In the ISR, a write to invalid address generates a Hard fault exception containing an infinite loop and preventing to return to main program (the IWDG counter is not refreshed). 
As a result, when the IWDG counter falls to 0, the IWDG reset occurs. If the IWDG reset is generated, after the system resumes from reset, LED_BLUE turns on for 4 seconds.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed in the main program infinite loop, and there is no IWDG reset

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The IWDG reset can only be checked in "SD Boot" mode (boot pins). It means that the example has to be signed by KeyGen tool then flashed with STM32CubeProgrammer on SD Card.

@par Keywords

System, IWDG, reload counter, MPU Reset, Window mode, Timeout, Software fault

@par Directory contents

  - IWDG/IWDG_WindowMode/Inc/main.h                        Main program header file for Cortex-A7
  - IWDG/IWDG_WindowMode/Inc/stm32mp13xx_hal_conf.h        HAL Configuration file for Cortex-A7
  - IWDG/IWDG_WindowMode/Inc/stm32mp13xx_disco_conf.h      STM32MP135F-DK board configuration file
  - IWDG/IWDG_WindowMode/Inc/stm32mp13xx_it.h              Interrupt handlers header file for Cortex-A7
  - IWDG/IWDG_WindowMode/Src/main.c                        Main program for Cortex-A7
  - IWDG/IWDG_WindowMode/Src/stm32mp13xx_it.c              Interrupt handlers for Cortex-A7

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STMicroelectronics STM32MP135F-DK
    boards and can be easily tailored to any other supported device
    and development board..

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Plug the SD Card in the STM32MP135F-DK
 - Run the example


 */
