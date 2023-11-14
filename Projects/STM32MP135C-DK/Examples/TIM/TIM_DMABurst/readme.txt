/**
  @page TIM_DMABurst TIM_DMABurst example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    TIM/TIM_DMABurst/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TIM DMA Burst example.
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

This example shows how to update the TIMER TIM4_CH1 period and the duty cycle
using the TIMER DMA burst feature.

In this example, System clock is configured at 650 MHz.

Every update DMA request, the DMA will do 4 transfers of half words into Timer
registers beginning from ARR register.
On the DMA update request, 0x0FFF will be transferred into ARR, 0x0000
will be transferred into RCR (if supported), 0x0000 will be transferred into CCR1 and 0x0555 will be transferred into CCR2.

The TIM4CLK frequency is set to SystemCoreClock, to get TIM4 counter
clock at 16 MHz the Prescaler is computed as following:
- Prescaler = (TIM4CLK / TIM4 counter clock) - 1

The TIM4 Frequency = TIM4 counter clock/(ARR + 1)
                   = 16 MHz / 4096 = 3.9 KHz

The TIM4 CCR2 register value is equal to 0x555, so the TIM4 Channel 1 generates a
PWM signal with a frequency equal to 3.9 KHz and a duty cycle equal to 33.33%:
TIM4 Channel1 duty cycle = (TIM4_CCR2/ TIM4_ARR + 1)* 100 = 33.33%.

In case of error, LED_GREEN is toggling with a period of 100 ms, otherwise LED_GREEN turns ON.

The PWM waveform can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - TIM/TIM_DMABurst/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - TIM/TIM_DMABurst/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - TIM/TIM_DMABurst/Inc/main.h                    Header for main.c module
  - TIM/TIM_DMABurst/Src/stm32mp13xx_it.c          Interrupt handlers
  - TIM/TIM_DMABurst/Src/main.c                    Main program
  - TIM/TIM_DMABurst/Src/stm32mp13xx_hal_msp.c     HAL MSP file
  - TIM/TIM_DMABurst/Src/system_stm32mp13xx.c      STM32MP13xx system source file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STMicroelectronics STM32MP135F-DK
    board and can be easily tailored to any other supported device
    and development board.

  - STM32MP135F-DK Set-up
    - Connect the TIM4 output channel to an oscilloscope to monitor the different waveforms:
    - TIM4 TIM_CHANNEL_2 (PD.13 (pin 31 in CN8 connector))

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
