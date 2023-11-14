/**
  @page HASH_SHA3384SHA3512_MDMA  HASH digest calculation using SHA3-384 and SHA3-512 example with MDMA transfer.

  @verbatim
  ******************************************************************************
  * @file    HASH/HASH_SHA3384SHA3512_MDMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the HASH digest calculation using SHA3-384 and SHA3-512 example with MDMA transfer.
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

This example provides a short description of how to use the HASH peripheral to
hash data using SHA3-384 and SHA3-512 Algorithms.

In this example, System clock is configured at 650 MHz.

In this example, MMDMA is used to transfer data from internal RAM to HASH peripheral.
The SHA3-384 message digest length is 48 bytes and the SHA3-512 message digest length
is 64 bytes.

The expected HASH digests (for SHA3-384 and SHA3-512) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH peripheral.
In case of digest computation mismatch or initialization issue the LED_GREEN is blinking (200 ms period).
In case the SHA3-384 and SHA3-512 digest are computed correctly the LED_GREEN is turned ON.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

  - HASH/HASH_SHA3384SHA3512_MDMA/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
  - HASH/HASH_SHA3384SHA3512_MDMA/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - HASH/HASH_SHA3384SHA3512_MDMA/Inc/main.h                    Header for main.c module
  - HASH/HASH_SHA3384SHA3512_MDMA/Src/stm32mp13xx_it.c          Interrupt handlers
  - HASH/HASH_SHA3384SHA3512_MDMA/Src/main.c                    Main program
  - HASH/HASH_SHA3384SHA3512_MDMA/Src/stm32mp13xx_hal_msp.c     HAL MSP module
  - HASH/HASH_SHA3384SHA3512_MDMA/Src/system_stm32mp13xx.c      STM32MP13xx system source file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with a STM32MP13xx-DK embedded on an
    STM32MP135F-DK board and can be easily tailored to any other supported
    device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
