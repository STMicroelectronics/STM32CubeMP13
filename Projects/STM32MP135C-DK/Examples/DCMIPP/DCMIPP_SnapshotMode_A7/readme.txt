/**
  @page DCMIPP_SnapshotMode_example DCMIPP Snapshot Mode example

  @verbatim
  ******************************************************************************
  * @file    DCMIPP/DCMIPP_SnapshotMode_A7/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the DCMIPP Snapshot Mode Example.
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

@par Example Description

  How to configure the DCMIPP peripheral to display a single Captured frame.
  The goal of this example is to explain how to capture single frame.

  The DDR memory need to be initialized with DDR_Init project.
  At the beginning of the main program the HAL_Init() function is called to reset
  all the peripherals, initialize the systick.
  The SystemClock_Config() function is used to configure the system clock for STM32MP13xx Devices.
  LCD and Camera are initialized in LCD_Config and Camera_Config api respectively. Camera is configured in SNAPSHOT mode. 
  Capture is started when USER2 BUtton is pressed. Frame is captured and displayed. Blue led toggle means Image capture is success. 

STM32MP135F-DK board's LED can be used to monitor the transfer status:
 - LED_RED is blinking when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, LTDC, DCMIPP, Display Controller, CAMERA, Capture, Snapshot

@par Directory contents

    - DCMIPP/DCMIPP_SnapshotMode_A7/Inc/stm32mp13xx_hal_conf.h    HAL configuration file
    - DCMIPP/DCMIPP_SnapshotMode_A7/Inc/stm32mp13xx_disco_conf.h  STM32MP135F-DK board configuration file
    - DCMIPP/DCMIPP_SnapshotMode_A7/Inc/stm32mp13xx_it.h          Interrupt handlers header file
    - DCMIPP/DCMIPP_SnapshotMode_A7/Inc/main.h                    Main configuration file
    - DCMIPP/DCMIPP_SnapshotMode_A7/Src/main.c                    Main program
    - DCMIPP/DCMIPP_SnapshotMode_A7/Src/stm32mp13xx_it.c          Interrupt handlers
    - DCMIPP/DCMIPP_SnapshotMode_A7/Src/stm32mp13xx_hal_msp.c     HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with STM32MP135F-DK board and can be
    easily tailored to any other supported device and development board.
    Camera used is GC2145(2MP) with max image size 1600x1200.
    A DeSerializer (STMIPID02) converts the Camera serial interface (CSI) to Parallel interface.
    An onboard IO_EXPANDER is used to expand the IO capability of the STM32MP135xx to control camera
    reset, enable/disable, STMIPID02 shutdown etc.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all files of DCMIPP example and load your image into target memory
 - Run the example

 */