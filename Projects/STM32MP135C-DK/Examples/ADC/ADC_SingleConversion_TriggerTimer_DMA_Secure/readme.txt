/* USER CODE BEGIN Header */
/**
  @page ADC_SingleConversion_TriggerTimer_DMA_Secure_Secure ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    Examples/ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerTimer_DMA_Secure example.
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
Use ADC to convert a single channel at each trig from timer, 
conversion data are transferred by secure DMA into an array, indefinitely (circular mode).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from HW trigger: timer peripheral.
DMA is configured to transfer conversion data in an array, in circular mode.
A timer is configured in time base and to generate TRGO events.

Example execution:
From the start, the ADC converts the selected channel at each trig from timer.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).
LED_GREEN is turned on when the DMA transfer is completed (results array full)
and turned off at next DMA half-transfer (result array first half updated).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data (array of data)
 - "aADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values (array of data)

Connection needed:
Connect an external voltage supply on ADC channel (PF11 CN8 pin 35).

Other peripherals used:
  1 GPIO for LED
  1 GPIO for analog input: PF11 (CN8 pin 35)
  DMA
  Timer

Board settings:
 - ADC is configured to convert ADC2_CHANNEL_8 (PF11 CN8 pin 35).
 - The voltage input on ADC channel is provided by the external voltage supply.
 - Voltage value should be between 0 and 3.3V.

STM32MP135F-DK board LED is be used to monitor the program execution status:
 - Normal operation: LED_GREEN is turned-on/off in function of ADC conversion
   result.
    - Toggling: "On" upon conversion completion (full DMA buffer filled)
                "Off" upon half conversion completion (half DMA buffer filled)
 - Error: In case of error, LED_GREEN is toggling twice at a frequency of 1Hz.

@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/Inc/stm32mp13xx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/Inc/main.h                    Header for main.c module  
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/Src/stm32mp13xx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/Src/main.c                    Main program
  - ADC/ADC_SingleConversion_TriggerTimer_DMA_Secure/Src/system_stm32mp13xx.c      stm32mp13xx system source file


@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.
    
  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 */
