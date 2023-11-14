/**
  @page CRYP_SAES_MDMA_ECB_CBC Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    CRYP/CRYP_SAES_MDMA_ECB_CBC/readme.txt 
  * @author  MCD Application Team
  * @brief   How to use the SecureAES co-processor (SAES) peripheral to encrypt and decrypt data
  * using AES ECB and CBC Algorithms with MDMA
  ******************************************************************************
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

How to use the SecureAES co-processor (SAES) peripheral to encrypt and decrypt data
using AES ECB and CBC Algorithms with MDMA.

The Secure AES mode requires:

1. Plain text which will be encrypted. Its size must be a multiple of 16 bytes.
   So if the original plain text size is not a multiple of 16 bytes, it must be padded.
2. Cipher text which is the encryption result of the Plaint text. In this
   example, the cipher text is available in "EncryptedText".
3. Initialization vector used only for CBC Algorithm.
4. Key is the parameter which determines the cipher text. In this example the
   key size is 128 bits =16 bytes for ECB and 256 bits for CBC.

After startup , The Secure AES  is configured to encrypt/decrypt using ECB algorithm
and 128bit key length, then change the configuration to use CBC algorithm and
256bit key length  then perform the encrypt/decrypt.
All encrypt text are compared to a known results in CiphertextAESECB128 and CiphertextAESCBC256 buffers.

In this example, LED_GREEN is turned on when the operation finish with success
and LED_ORANGE is turned on when the operation generate an error.

@note

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Security, SAES, AES, MDMA, TrustZone disabled

@par Directory contents 

  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Src/main.c                    Main program
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Src/stm32mp13xx_it.c          Interrupt handlers
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Inc/main.h                    Main program header file
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Inc/stm32mp13xx_disco_conf.h  BSP Configuration file
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Inc/stm32mp13xx_hal_conf.h    HAL Configuration file
  - CRYP/CRYP_SAES_MDMA_ECB_CBC/Inc/stm32mp13xx_it.h          Interrupt handlers header file

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
  
  - This example has been tested with an STMicroelectronics STM32MP135F-DK
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
