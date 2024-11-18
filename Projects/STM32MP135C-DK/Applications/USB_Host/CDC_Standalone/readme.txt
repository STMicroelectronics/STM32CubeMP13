/**
  @page CDC_Standalone USB Host Communication (CDC) application

  @verbatim
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host CDC application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description

This application is a part of the USB Host Library package using STM32Cube firmware. It describes how to use
USB host application based on the Communication (CDC) on the STM32MP135xx devices.

This is a typical application on how to use the STM32MP135xx USB OTG Host peripheral where the STM32 MPU
behaves as a USB-to-UART bridge following the Virtual COM Port (VCP) implementation.
 - The STM32 exchanges data with a CDC device through USB interface in Host mode.

@note A STM32 MPU or MCU running as CDC Device can be connected to STM32MP135X-DK via C Port(CN7).

@parExpected success behavior
When the application is started, the connected USB CDC device is detected in CDC mode and gets
initialized. The STM32 MPU behaves as a CDC Host, it enumerates the device and extracts VID, PID,
manufacturer name, Serial no and product name information and displays it on the UART Hyperterminal.
Open two hyperterminals(USB STLink VCP for Host and Device) to send/receive data to/from host from/to device.

Debug messages are displayed along with each new step of the application :
 - "USB OTG HS in HS CDC Host" appears on VCP port once application starts.
 - "USB Device Connected" appears on VCP port device is connected and successfully enumerated.
 - Data from Device VCP can be seen in Host VCP port and "USB_STM32_Host_CDC" appears in Device VCP 
   each time data is recieved by HOST.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".


It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages to uart terminal, stm32mp13xx_hal_uart.c driver needs to be added to the application sources.
   Debug messages are displayed on the uart terminal using ST-Link.

@par Error behaviors
USB Host does not operate as designed (CDC Device enumeration failed, Host and Device
can not communicate over VCP ports).

@Assumptions if any
User is familiar with USB 2.0 “Universal Serial BUS” Specification and CDC class Specification.

@parKnown limitations
None.


@par Keywords

Connectivity, USB_Host, USB, CDC, Communication, Full Speed, High Speed, UART, VCP


@par Directory contents

   - USB_Host/CDC_Standalone/USB_Host/Target/usbh_conf.c           General low level driver configuration
   - USB_Host/CDC_Standalone/USB_Host/Target/usbh_conf.h           USB Host driver Configuration file
   - USB_Host/CDC_Standalone/USB_Host/App/cdc_send.c               Send Data to CDC Device
   - USB_Host/CDC_Standalone/USB_Host/App/cdc_receive.c            Receive Data to CDC Device
   - USB_Host/CDC_Standalone/USB_Host/App/usb_host.c               MSC State Machine
   - USB_Host/CDC_Standalone/Core/Src/main.c                       Main program
   - USB_Host/CDC_Standalone/Core/Src/syscalls.c                   STM32MP13xx system calls
   - USB_Host/CDC_Standalone/Core/Src/stm32mp13xx_it.c             Interrupt handlers
   - USB_Host/CDC_Standalone/Core/Src/stm32mp13xx_hal_msp.c        HAL MSP Module
   - USB_Host/CDC_Standalone/Startup/startup_stm32mp135c_ca7.c
   - USB_Host/CDC_Standalone/Core/Inc/main.h                       Main program header file
   - USB_Host/CDC_Standalone/Core/Inc/stm32mp13xx_it.h             Interrupt handlers header file
   - USB_Host/CDC_Standalone/Core/Inc/stm32mp13xx_hal_conf.h       HAL configuration file
   - USB_Host/CDC_Standalone/Core/Inc/stm32mp13xx_disco_conf.h     Hardware configuration File

@par Hardware and Software environment

  - This application runs on STM32MP135xx devices.

  - This application has been tested with STMicroelectronics STM32MP135F-DK
    boards and can be easily tailored to any other supported device
    and development board.

  - STM32MP135X-DK Set-up
    - Plug the USB key into the STM32MP13-DK board CN7 "TYPE-C" port (use Type-C to Type-A-Receptacle adapter to plug in type-A USB Cable)
    -Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:

     - Hyperterminal configuration:
      - Data Length = 8 Bits
      - One Stop Bit
      - No parity
      - BaudRate = 115200 baud
      - Flow control: None

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Open the configured uart hyperterminal in order to display debug messages.
 - Run the application
 - Open the configured uart hyperterminal in order to display debug messages.
@note
   The user has to check the list of the COM ports in Device Manager to find out the number of the
   COM ports that have been assigned (by OS) to the Stlink VCP .


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
