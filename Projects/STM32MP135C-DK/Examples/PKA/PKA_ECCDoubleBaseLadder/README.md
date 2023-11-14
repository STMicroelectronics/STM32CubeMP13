## <b>PKA_ECCDoubleBaseLadder Example Description</b>

How to use the PKA to run ECC Double Base Ladder operation

This project is targeted to run on STM32MP135xx devices on STM32MP135F-DK board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the systick.
Then the System clock is configured at 650 MHz.

ECC double base ladder operation consists in the computation of k*P + m*Q, where (P,Q) are two points on an elliptic curve and (k,m) are two scalars.

The RNG initialization is mandatory for the PKA's operations.
To run ECC double base ladder operation with PKA we should fill the PKA_ECCDoubleBaseLadderInTypeDef with required input vectors.
The result of the operation is composed by 2 elements:

 - coordinate X of the result point which is stored in the attribute ptX of the structure out
 - coordinate Y of the result point which is stored in the attribute ptY of the structure out

In this example, LED_GREEN is turned on when the operation finish with success
and LED_ORANGE is blinking when the operation generate an error.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use  IRQ_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Security, PKA, NIST, CAVP, ECC double base ladder

### <b>Directory contents</b>

  - PKA/PKA_ECCDoubleBaseLadder/Src/main.c                   Main program
  - PKA/PKA_ECCDoubleBaseLadder/Src/stm32mp13xx_it.c         Interrupt handlers
  - PKA/PKA_ECCDoubleBaseLadder/Src/stm32mp13xx_hal_msp.c    HAL MSP module
  - PKA/PKA_ECCDoubleBaseLadder/Inc/main.h                   Main program header file
  - PKA/PKA_ECCDoubleBaseLadder/Inc/stm32mp13xx_disco_conf.h BSP Configuration file
  - PKA/PKA_ECCDoubleBaseLadder/Inc/stm32mp13xx_hal_conf.h   HAL Configuration file
  - PKA/PKA_ECCDoubleBaseLadder/Inc/stm32mp13xx_it.h         Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32MP135F-DK
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

