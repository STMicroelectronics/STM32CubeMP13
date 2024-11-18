/**
  @page External_Mem_Loader External Memory Loader application

  @verbatim
  ******************************************************************************
  * @file    External_Mem_Loader/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the External Memory Loader application.
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

SDCARD_FLASH: This MACRO should be added in C/C++ build preprocessor settings to build the application 
			  for writing to the SD/EMMC Card.
			  
SNOR_FLASH:	  This MACRO should be added in C/C++ build preprocessor settings to build the application 
			  for writing to the SNOR FLASH.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use IRQ_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, GPIO, EXTI, Output, Alternate function, Push-pull, Toggle

@par Directory contents

  - External_Mem_Loader/Inc/stm32mp13xx_hal_conf.h    							HAL configuration file
  - External_Mem_Loader/Inc/stm32mp13xx_it.h          							Interrupt handlers header file
  - External_Mem_Loader/Inc/main.h                    							Header for main.c module
  - External_Mem_Loader/Src/stm32mp13xx_it.c          							Interrupt handlers
  - External_Mem_Loader/Src/stm32mp13xx_hal_msp.c     							HAL MSP file
  - External_Mem_Loader/Src/main.c                    							Main program
  - External_Mem_Loader/OpenBootloader/App/app_openbootloader.c                 OpenBootloader application
  - External_Mem_Loader/OpenBootloader/App/app_openbootloader.h					Header for app_openbootloader.c module
  - External_Mem_Loader/OpenBootloader/Target/common_interface.c         		Contains common functions used by different interfaces
  - External_Mem_Loader/OpenBootloader/Target/common_interface.h         		Header for common_interface.c module
  - External_Mem_Loader/OpenBootloader/Target/external_memory_interface.c		Contains External memory access functions
  - External_Mem_Loader/OpenBootloader/Target/external_memory_interface.h		Header for external_memory_interface.c module
  - External_Mem_Loader/OpenBootloader/Target/interfaces_conf.h          		Contains Interfaces configuration
  - External_Mem_Loader/OpenBootloader/Target/iwdg_interface.c           		Contains IWDG HW configuration
  - External_Mem_Loader/OpenBootloader/Target/iwdg_interface.h           		Header for iwdg_interface.c module
  - External_Mem_Loader/OpenBootloader/Target/openbootloader_conf.h             Contains Open Bootloader configuration
  - External_Mem_Loader/OpenBootloader/Target/platform.h 		          		Header for patterns
  - External_Mem_Loader/OpenBootloader/Target/ram_interface.c             		Contains RAM access functions
  - External_Mem_Loader/OpenBootloader/Target/ram_interface.h             		Header for ram_interface.c module
  - External_Mem_Loader/OpenBootloader/Target/usart_interface.c            		Contains USART HW configuration
  - External_Mem_Loader/OpenBootloader/Target/usart_interface.h            		Header for usart_interface.c module
  - External_Mem_Loader/OpenBootloader/Target/usb_interface.c             		Contains USB protocol commands
  - External_Mem_Loader/OpenBootloader/Target/usb_interface.h             		Header for usb_interface.c module
  - External_Mem_Loader/USB_Device/Target/usbd_conf.c             		        board support package for the USB device library
  - External_Mem_Loader/USB_Device/Target/usbd_conf.h             		        Header for usbd_conf.c module
  - External_Mem_Loader/USB_Device/App/usb_device.c            					implements the USB Device
  - External_Mem_Loader/USB_Device/App/usb_device.h            					Header for usb_device.c module
  - External_Mem_Loader/USB_Device/App/usbd_desc.c             					USB device descriptors
  - External_Mem_Loader/USB_Device/App/usbd_desc.h             					Header for usbd_desc.c module
  - External_Mem_Loader/USB_Device/App/usbd_dfu_media.c             			implements the USB Device
  - External_Mem_Loader/USB_Device/App/usbd_dfu_media.h             			Header for usbd_dfu_media.c module

@par Hardware and Software environment

  - This example runs on STM32MP135xx devices.

  - This example has been tested with ST internal board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred and build the project.
 - This example is used as a part of External Loader application package for programming EMMC or SNOR flash via
   STM32CubeProgrammer. Refer Projects/STM32MP13XX_CUSTOM_HW/External_Loader/readme.txt for more details.

 */
