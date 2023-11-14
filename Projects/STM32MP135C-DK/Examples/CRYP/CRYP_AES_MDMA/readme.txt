/**
  @page CRYP_AES_MDMA  Encrypt and Decrypt data using AES Algo in ECB chaining
   modes using MDMA
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    CRYP/CRYP_AES_MDMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES-128 Algo in ECB mode with MDMA Example
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

This example provides a short description of how to use the CRYPTO peripheral to 
encrypt and decrypt data using AES-128 Algorithm with ECB chaining mode.

For this example, MDMA is used to transfer data from memory to the CRYPTO
processor IN FIFO and also to transfer data from CRYPTO processor OUT FIFO
to memory.

The data to be encrypted is a 256-bit data (8 words), which correspond to
2x AES-128 Blocks.

This example behaves as below: 
- AES Encryption (Plain Data --> Encrypted Data)
- AES Key preparation for decryption (Encryption Key --> Decryption Key)
- AES Decryption (Encrypted Data --> Decrypted Data)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.
Then the System clock is configured at 650 MHz.

STM32MP135F-DK LEDs can be used to monitor the transfer status:
 - LED_GREEN is ON when encryption/decryption and TAG generation is right.
 - LED_ORANGE is blinking when encryption or decryption or TAG generation is wrong.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, Cryptography, CRYPT, AES, MDMA, MAC, Cypher, ECB, Key

@par Directory contents 

  - CRYP/CRYP_AES_MDMA/Inc/main.h                    Header for main.c module  
  - CRYP/CRYP_AES_MDMA/Inc/stm32mp13xx_disco_conf.h  Interrupt handlers
  - CRYP/CRYP_AES_MDMA/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AES_MDMA/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AES_MDMA/Src/main.c                    Main program
  - CRYP/CRYP_AES_MDMA/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AES_MDMA/Src/stm32mp13xx_it.c          Interrupt handlers

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
  
  - This example has been tested with an STMicroelectronics STM32MP135F-DK
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
