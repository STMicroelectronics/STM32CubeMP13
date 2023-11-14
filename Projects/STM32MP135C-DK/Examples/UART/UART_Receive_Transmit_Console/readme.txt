/**
  @page UART_TwoBoards_ComIT UART Two Boards Communication IT example

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    UART/UART_Receive_Transmit_Console/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the UART Receive and Transmit on Console example.
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

UART transmission (printf/getchar) via console with user interaction.
The UART outputs a message on the HyperTerminal and receives some characters.

Board: STM32MP135F-DK (embeds a STM32MP135xx device)
Tx Pin: PD8
Rx Pin: PD6
   _________________________
  |           ______________|                      ________________
  |          |UART4         |                     | Hyperterminal  |
  |          |              |                     |                |
  |          |           TX |_____________________| TXD            |
  |          |              |                     |                |
  |          |              |    ST-Link Cable    |                |
  |          |              |                     |                |
  |          |           RX |_____________________| RXD            |
  |          |              |                     |                |
  |          |______________|                     |________________|
  |                         |
  |                         |
  |                         |
  |_STM32_Board 1___________|

The USART is configured as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals: not useful for this example)
    - Reception and transmission are enabled in the time

The user sees some printf messages on the Hyperterminal console.
He has to type "c" to continue with getchar test.
A menu is displayed, and the user can select:
- "Test1" to execute fast LED_GREEN blink (during 5s)
- "Test2" to execute slow LED_GREEN blink (during 5s)
- "Test3" to quit the example

STM32MP135F-DK board LED is used to monitor:
- LED_GREEN is blinking fastly or slowly during Test1 and Test2.
- LED_GREEN is on at the end of the test (if user selects "Test3")
- If there is an error, LED_GREEN is blinking (2 sec. period).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

In this example, System clock is configured at 650 MHz.

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - UART/UART_Receive_Transmit_Console/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - UART/UART_Receive_Transmit_Console/Inc/stm32mp13xx_it.h          IT interrupt handlers header file
  - UART/UART_Receive_Transmit_Console/Inc/main.h                    Header for main.c module
  - UART/UART_Receive_Transmit_Console/Src/stm32mp13xx_it.c          IT interrupt handlers
  - UART/UART_Receive_Transmit_Console/Src/main.c                    Main program
  - UART/UART_Receive_Transmit_Console/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - UART/UART_Receive_Transmit_Console/Src/system_stm32mp13xx.c      STM32MP13xx system source file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
  - This example has been tested with two STM32MP135F-DK boards embedding
    a STM32MP135xx device and can be easily tailored to any other supported device
    and development board.

  - STM32MP135F-DK set-up
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
      A virtual COM port will then appear in the HyperTerminal.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
