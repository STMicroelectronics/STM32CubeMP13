/**
  @page LwIP_TCP_Echo_Server LwIP TCP Echo Server Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2024 STMicroelectronics *******************
  * @file    LwIP/LwIP_TCP_Echo_Server/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP TCP Echo server Application.
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

This application guides STM32Cube HAL API users to run TCP Echo Server application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
  C:\>echotool IP_address /p tcp /r 7 /n 15 /t 2 /d Testing LwIP TCP echo server

where:
    – IP_address is the actual board’s IP address. If DHCP is disabled, 
    the following static IP address is used: 192.168.0.10 
    – /p transport layer protocol used for communication (TCP)
    – /r is the actual remote port on the echo server (echo port)
    – /n is the number of echo requests (for application, 15)
    – /t is the connection timeout in seconds (for application, 2)
    – /d is the message to be sent for echo 

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
 + LED1: ethernet cable is connected.
 + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h)

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@par Keywords

Connectivity, LwIP, Ethernet, TCP/IP, DHCP, echo server, 

@par Directory contents

  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/app_ethernet.h          		header of app_ethernet.c file
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/ethernetif.h            		header for ethernetif.c file
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/stm32mp13xx_hal_conf.h    	HAL configuration file
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/stm32mp13xx_it.h          	STM32 interrupt handlers header file
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/main.h                  		Main program header file
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/lwipopts.h              		LwIP stack configuration options
  - LwIP/LwIP_TCP_Echo_Server/Core/Inc/tcp_echoserver.h                 Header for tcp echo server application
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/app_ethernet.c          		Ethernet specific module
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/main.c                  		Main program
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/stm32mp13xx_it.c                 Interrupt handlers
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/stm32mp13xx_hal_msp.c            MSP Initialization file
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/ethernetif.c            		Interfacing LwIP to ETH driver
  - LwIP/LwIP_TCP_Echo_Server/Core/Src/tcp_echoserver.c               	tcp echo server application

@par Hardware and Software environment

  - This application runs on STM32MP135xx devices.
    
  - This application has been tested with the following environments:
     - STM32MP135F-DK board
     - echotool: (http://bansky.net/echotool/) is used as echo client that sends
       data to the server and checking whether they came back 
     - DHCP server:  PC utility TFTPD32 (http://tftpd32.jounin.net/) is used as a DHCP server    
      
  - STM32MP135F-DK Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - PC must share the same LAN network configuration with the DK board

@par How to use it ?

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all LwIP_TCP_Echo_Server application files and load your image into target memory
 - Run the application
  

 */
