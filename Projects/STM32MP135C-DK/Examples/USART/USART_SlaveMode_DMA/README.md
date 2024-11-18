/**
  @page USART_SlaveMode_DMA USART SlaveMode DMA example

  @verbatim
  ******************** (C) COPYRIGHT 2024 STMicroelectronics *******************
  * @file    USART/USART_SlaveMode_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USART SlaveMode DMA example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim
  
This example describes an USART-SPI communication (transmit/receive) with DMA between two
boards where the USART is configured as a slave.

Board: STM32MP135F-DK (embeds a STM32MP135xx device)

Two identical boards are connected:
- Board 1: SPI Master board, the associated project is SPI_FullDuplex_comDMA_Master_A7
- Board 2: USART Slave board, the associated project is USART_SlaveMode_DMA_A7

In a first step, after the user press on the User push-button, Master (SPI) starts
the communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive(), at the same time Slave (USART) transmits aTxBuffer
and receives aRxBuffer through HAL_USART_TransmitReceive().
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32MP135F-DK board LEDs are used to monitor the transfer status:

- While master board is waiting for the user to press the User push-button, its
  LED_GREEN is blinking rapidly (100 ms period).
- While slave board is waiting for the reception of the buffer, its
  LED_GREEN is blinking rapidly (100 ms period).
- While master board is transmitting/receiving,
  LED_GREEN is off.
- When the test passes, LED_GREEN on both boards is turned on, otherwise the
  test has failed.
- If there is an initialization or transfer error, LED_GREEN is blinking slowly (500 ms period).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

In this example, System clock is configured at 650 MHz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then
      the SysTick interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use
      IRQ_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to
      1 millisecond to have correct HAL operation.

 3. It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
      at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
      Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
      control on output state even if it is disabled.

### <b>Keywords</b>

Connectivity, UART/USART, SPI interface, full-duplex, Polling, Receiver, Transmitter, Synchronous, Slave, DMA

### <b>Directory contents</b>

  - USART/USART_SlaveMode_DMA/Inc/main.h                  Main program header file
  - USART/USART_SlaveMode_DMA/Inc/stm32mp13xx_hal_conf.h  HAL configuration file
  - USART/USART_SlaveMode_DMA/Inc/stm32mp13xx_it.h        Interrupt handlers header file
  - USART/USART_SlaveMode_DMA/Src/main.c                  Main program source file
  - USART/USART_SlaveMode_DMA/Src/stm32mp13xx_hal_msp.c   HAL MSP module
  - USART/USART_SlaveMode_DMA/Src/stm32mp13xx_it.c        Interrupt handlers source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135F-DK devices.

  - This example has been tested with two STM32MP135F-DK boards embedding
    a STM32MP135xx device and can be easily tailored to any other supported device
    and development board.

  - STM32MP135F-DK set-up
    - Connect Master board PH7 - (CN8, pin D23) (EXP_GPIO11 header pin, SPI5_SCK  signal) to Slave Board PB0  - (CN8, pin D10) (EXP_GPIO15 header pin, USART1_CK signal)
    - Connect Master board PH3 - (CN8, pin D19) (EXP_GPIO10 header pin, SPI5_MOSI signal) to Slave Board PD13 - (CN8, pin D31) (EXP_GPIO6  header pin, USART1_RX signal)
    - Connect Master board PA8 - (CN8, pin D21) (EXP_GPIO09 header pin, SPI5_MISO signal) to Slave Board PC0  - (CN8, pin D08) (EXP_GPIO14 header pin, USART1_TX signal)
    - Connect Master board GND - (CN8, pin D25)  to  Slave Board GND - (CN8, pin D25).

   _________________________                       _______________________________
  |           ______________|                      |___________________           |
  |          |SPI5          |                      |        USART1     |          |
  |          |              |                      |                   |          |
  |          |    CLK(PH.7) |______________________|CLK       (PB.0)   |          |
  |          |              |                      |                   |          |
  |          |   MISO(PA.8) |______________________|USART1_TX (PC.0)   |          |
  |          |              |                      |                   |          |
  |          |   MOSI(PH.3) |______________________|USART1_RX (PD.13)  |          |
  |          |              |                      |                   |          |
  |          |______________|                      |___________________|          |
  |      __                 |                      |                              |
  |     |__|                |                      |                              |
  |    User push-button     |                      |                              |
  |                      GND|______________________|GND                           |
  |                         |                      |                              |
  |_STM32MP13xx Master______|                      |_STM32MP13xx Slave____________|
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory :
  - Run the example
