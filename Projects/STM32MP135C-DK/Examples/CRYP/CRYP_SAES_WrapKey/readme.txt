/**
  @page CRYP_SAES_WrapKey  Use the Secure AES co-processor (SAES) peripheral to wrap application keys
  using hardware secret key DHUK then use it to encrypt in polling mode.
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    CRYP/CRYP_SAES_WrapKey/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Secure AES co-processor (SAES) peripheral to wrap application
  * keys using hardware secret key DHUK then use it to encrypt in polling mode
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

The Secure AES instance is configured in wrap mode and hardware secret key DHUK is selected.
The goal here to not share application key with users.
First, application key is encrypted (the result is stored in Encryptedkey buffer).
Then, user decrypt Encryptedkey, now application key is loaded in SAES_KEYRx registers
(without knowing him), immediately usable by users for any SAES operation.
Finally, ECB encryption/decryption is performed using key already in SAES_KEYRx registers
and results are compared with expected ones.

The Secure AES mode requires:

1. Application keys (AESKey256) to be wrapped using DHUK(derived hardware unique key).
2. Plain text (Plaintext) which will be encrypted.
3. Cipher text (CiphertextAESECB256)  which is the encryption result of the Plaint text.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.
Then the System clock is configured at 650 MHz.

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

System, Security, SAES, AES,  TrustZone disabled

@par Directory contents 

  - CRYP/CRYP_SAES_WrapKey/Src/main.c                    Main program
  - CRYP/CRYP_SAES_WrapKey/Src/stm32mp13xx_it.c          Interrupt handlers
  - CRYP/CRYP_SAES_WrapKey/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_SAES_WrapKey/Inc/main.h                    Main program header file
  - CRYP/CRYP_SAES_WrapKey/Inc/stm32mp13xx_disco_conf.h  BSP Configuration file
  - CRYP/CRYP_SAES_WrapKey/Inc/stm32mp13xx_hal_conf.h    HAL Configuration file
  - CRYP/CRYP_SAES_WrapKey/Inc/stm32mp13xx_it.h          Interrupt handlers header file

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
