## <b>ETZPC_NonSecure_Access Example Description</b>

This example describes non-secure accesses of memory using DMA.

The different memories are configured as follows:
- The first 124K of SYSRAM are configured as secure, the last 4K of this area is used for transfers
- The last 4K of SYSRAM are configured as non-secure and used for transfers
- The SYSRAM secure size is then locked, it can thus only be changed after a reset
- SRAM1 is read and write secure
- SRAM2 is read non-secure and write secure
- SRAM3 is read and write non-secure

DMA1 is used for the transfers. DMA1 is always non-secure.

Transfers of 4K memory blocks in both directions are then made between the different memory areas:
- SYSRAM non-secure area and SYSRAM secure area 
- SYSRAM non-secure area and SRAM1, SRAM2, SRAM3

Memories are filled with specific values before each transfer to check if the transfer has been done or not.

When either the source or the destination is secure, the hardware cancels the transfer.

Board: STM32MP135F-DK (embeds a STM32MP135xx device)

STM32MP135F-DK board LEDs are used to monitor the example status:
- When the test passes, LED_GREEN is turned on, otherwise the test has failed.
- If there is an error, LED_GREEN is blinking slowly (500 ms period).

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

### <b>Keywords</b>

ETZPC, secure, non-secure

### <b>Directory contents</b>

  - ETZPC/ETZPC_NonSecure_Access/Inc/main.h                  Main program header file
  - ETZPC/ETZPC_NonSecure_Access/Inc/stm32mp13xx_hal_conf.h  HAL configuration file
  - ETZPC/ETZPC_NonSecure_Access/Inc/stm32mp13xx_it.h        Interrupt handlers header file
  - ETZPC/ETZPC_NonSecure_Access/Src/main.c                  Main program source file
  - ETZPC/ETZPC_NonSecure_Access/Src/stm32mp13xx_hal_msp.c   HAL MSP module
  - ETZPC/ETZPC_NonSecure_Access/Src/stm32mp13xx_it.c        Interrupt handlers source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135F-DK devices.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the example
