/**
  @page SPI_FullDuplex_ComDMA_Slave SPI Full Duplex DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2022 STMicroelectronics *******************
  * @file    SPI/SPI_FullDuplex_ComDMA_Slave/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex DMA example.
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

Data buffer transmission/reception between two boards via SPI using Interrupt mode.

Board: STM32MP135F-DK (embeds a STM32MP135xx device)
GND Pin:  (CN8, pin D25)
CLK Pin: PH7 (CN8, pin D23)
MISO Pin: PA8 (CN8, pin D21)
MOSI Pin: PH3 (CN8, pin D19)
   _________________________                       __________________________
  |           ______________|                      |______________           |
  |          |SPI5          |                      |          SPI5|          |
  |          |              |                      |              |          |
  |          |    CLK(PH.7) |______________________|(PH.7)CLK     |          |
  |          |              |                      |              |          |
  |          |   MISO(PA.8) |______________________|(PA.8)MISO    |          |
  |          |              |                      |              |          |
  |          |   MOSI(PH.3) |______________________|(PH.3)MOSI    |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |    User push-button     |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32MP13xx Master______|                      |_STM32MP13xx Slave_______|

HAL architecture allows user to easily change code to move to DMA or Polling
mode. To see others communication modes please check following examples:
SPI\SPI_FullDuplex_ComIT_Master and SPI\SPI_FullDuplex_ComIT_Slave

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK,
GPIO and interrupt). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_DMA() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode).
If the Master board is used, the project SPI_FullDuplex_ComDMA_Master must be used.
If the Slave board is used, the project SPI_FullDuplex_ComDMA_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the USER1 push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA().
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED_GREEN toggles quickly on master board waiting USER1 push-button to be pressed.
 - LED_GREEN turns ON if transmission/reception is complete and OK.
 - LED_GREEN toggles slowly when there is a timeout or an error in transmission/reception process.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Inc/main.h                    Header for main.c module
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/stm32mp13xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/main.c                    Main program
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/system_stm32mp13xx.c      stm32mp13xx system source file
  - SPI/SPI_FullDuplex_ComDMA_Slave/Src/stm32mp13xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.

  - STM32MP135F-DK Set-up
    - Connect Master board PH7 (CN8, pin D23) to Slave Board PH7 (CN8, pin D23)
    - Connect Master board PA8 (CN8, pin D21) to Slave Board PA8 (CN8, pin D21)
    - Connect Master board PH3 (CN8, pin D19) to Slave Board PH3 (CN8, pin D19)
    - Connect Master board GND  to Slave Board GND

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files (master project) and load your image into target memory
    o Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
    o Load the project in Slave Board
 - Run the example

 */
