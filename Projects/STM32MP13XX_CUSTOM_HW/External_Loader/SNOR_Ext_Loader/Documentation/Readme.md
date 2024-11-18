
# Description of the SNOR_Ext_Loader project.

### Directory contents
the SNOR_Ext_Loader project contains the following folders:

**Project:**
Contains preconfigured STM32CubeIDE project and its associated files.
**Drivers:**
Contains Source and header files providing STM32 drivers for required peripherals.
**Flash_loader:**
Contains Source and header files required by the flashloaders and containing specific information on used  memory.

### Hardware and Software Environment:
- STM32MP135C CUSTOM_HW boards

### How to use it ?
In order to make the program work, you must do the following :
 - Open the project under Projects\STM32MP13XX_CUSTOM_HW\External_Loader\SNOR_Ext_Loader\STM32CubeIDE.
 - Rebuild the project.
 - Connect the debugger and execute the project.
 - This project erases the sectors of SNOR flash, then writes data, verfies it and finally does a mass erase.
 - This project can also be used with External Loader application for programming SNOR flash via External loader application using STM32CubeProgrammer.
 - Follow Projects\STM32MP13XX_CUSTOM_HW\External_Loader\readme.txt for more details.  
 - This project has been tested with ST internal boards interfaced with MX25LM51245G SNOR flash and can be
   easily tailored to any other supported device and development board.

## License
Copyright (c) 2021 STMicroelectronics. All rights reserved.
This software is licensed under terms that can be found in the LICENSE file, in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.