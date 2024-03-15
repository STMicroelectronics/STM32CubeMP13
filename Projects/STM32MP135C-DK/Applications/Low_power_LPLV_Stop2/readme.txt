/**
  @page Low_power_LPLV_Stop2

  @verbatim
  ******************** (C) COPYRIGHT 2021 STMicroelectronics *******************
  * @file    Low_power_LPLV_Stop2/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the LPLV-Stop2 application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application shows a LPLV-Stop2 entry with DDR in self refresh to keep its content.
The Cortex A7 is off and the VDDCORE is put in retention (0.9V).
At wakeup the bootrom directly jumps into sysram to allow the FSBL-A to execute the wakeup treatments.
The DDR application is still present and is not reloaded. When the FSBL-A has treated the wakeup, it
jumps back in the DDR application.
The wakeup can be triggered by pressing B3 (wake_up button)

This application relies on two signed firmwares.
The FSBL-A is located in sysram and is loaded from SD card by the bootrom at power on.
The user application is located in DDR by the FSBL-A at power on.

The FSBL-A needs to be flashed at sector 128 on SD card.
The DDR application needs to be flashed at sector 640 on SD card.

The bootrom pins have to be set to SD card boot.

Board: STM32MP135F-DK (embeds a STM32MP135xx device and a PMIC)

   _________________________
  |           ______________|                      ________________
  |          |Connector CN14|                     | Oscilloscope   |
  |          |              |                     |                |
  |          |      VDDCORE |_____________________|                |
  |          |              |                     |                |
  |          |      VDDCPU  |_____________________|                |
  |          |              |                     |                |
  |          |      GND     |_____________________|                |
  |          |              |                     |                |
  |          |______________|                     |________________|
  |                         |
  |                         |
  |                         |
  |_STM32_Board 1___________|


The user sees some printf messages on the Hyperterminal console.
These messages come from the user application running in DDR.

STM32MP135F-DK board LEDs are used to monitor:
- LED_BLUE and LED_RED are slowly blinking together at power on. It corresponds to
the FSBL-A signalling that is has finished to load the DRR application. This happens only once.
- The DDR application activity is shown by a fast alternate toggle of LED_BLUE and LED_RED.
- Both LEDs are off in LPLV-Stop2 meaning there is no software running anymore.
- If there is an error, LED_RED is blinking (1 sec. period).

It is possible to check the Cortex A7 power supply and the VDDCORE value on CN14.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      This app can be build without GPT Table also by deleting GPT_TABLE_PRESENT in preprocessor build
      setting. This will be useful in case of application is put into the SD card using HxD
      software.

@par Directory contents

  - Low_power_LPLV_Stop2/BSP_BasicTemplates_low_power                The files that are needed to build the DDR application
  - Low_power_LPLV_Stop2/FSBLA_LPLV_Stop2                            The files that are needed to build the FSBL-A


@par Hardware and Software environment

  - This application runs on STM32MP135xx DK board.

  - STM32MP135F-DK set-up
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
      A virtual COM port will then appear in the HyperTerminal.
    - Configure the bootrom pins in SD boot mode

@par How to use it ?

In order to make the program work, you must do the following :
 - Import the Low_power_LPLV_Stop2 folder, it will open both projects
 - Rebuild both firmwares and flash them on SD Card starting at the right sectors
 - Power up the board, the global application starts immediately

 */
