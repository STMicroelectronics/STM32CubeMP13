  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @author  MCD Application Team
  * @brief   This file describes STM32MP13XX_CUSTOM_HW board.
  ******************************************************************************
  */
### V1.2.0 (25-10-2024) ###
============================
- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to validate some features not applicable on STM32MP135F-DK board:

/** HW information **/
  - DDR TYPE : LPDDR3 4Gb  (versus DDR3 4Gb on STM32MP135F-DK)
    . Switch in CubeIDE project = DDR_TYPE_LPDDR3_4Gb
	. Driver DDR:
		. Switch also used in stm32mp13xx_hal_ddr.c
		. Header file used : stm32mp13xx-lpddr3-4Gb.h
	For other DDR: 
	  . Use CubeMX for MP13 to generate your settings file
        . Generate your header file
	  . Use STM32DDRFW-UTIL to validate your DDR setup
		. Copy the header file as template in STM32DDRFW-UTIL\utilities-ddr\DDR_Tool\your_board project
		. Create a new DDR_Switch  related to your specific DDR Type and density
		. Add this DDR_switch in stm32mp_cubemx_ddr_conf.h
	    . Build your project, and launch tests to ensure expected behavior
		. Use save command to print the final header file related to your DDR 
	  . Copy this final header file content in stm32cube_fw_mp13\Drivers\STM32MP13xx_HAL_Driver\Inc for your next applications
	  . Finally, add the DDR_Switch in HAL driver stm32cube_fw_mp13\Drivers\STM32MP13xx_HAL_Driver\Src\hal_ddr.c
	(For more information refer to README.md from STM32DDRFW-UTIL)

  - QSPI-NOR : Macronix MX25L51245G (versus no QSPI-NOR on STM32MP135F-DK)
    . Files related to this QSPI-NOR : 
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\mx25l512.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\stm32mp13xx_valid_qspi.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\stm32mp13xx_valid_qspi.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSP\mx25l512.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSP\stm32mp13xx_valid_qspi.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSPstm32mp13xx_valid_qspi.h,
	. For other QSPI-NOR, need to adapt these BSP files or create new ones
	
  - FMC-NAND : Micron MT29F8G08ABACAH4 (versus no FMC-NAND on STM32MP135F-DK)
    . Files related to this FMC_NAND : 
	  - STM32MP13XX_CUSTOM_HW\Applications\FileX\Fx_Nand_Write_Read_File\BSP\stm32mp13xx_valid_fmc_nand.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\FileX\Fx_Nand_Write_Read_File\BSP\stm32mp13xx_valid_fmc_nand.h
	. For other FMC_NAND, need to adapt these BSP files or create new ones
	
  - MicroSD card slot available on this board ( as STM32MP135F-DK)

  - No PMIC on this board
  
  - No IOEXPANDER on this board
  
/** Firmware information **/
  - Some applications are implemented to demonstrate features not applicable on STM32MP135F-DK board:
	. Templates : Reference template for STM32MP13XX_CUSTOM_HW - see readme.txt for more details
	. Examples - DDR_Init : provide a reference to initialize the DDR (LPDDR3 4Gb) on STM32MP13XX_CUSTOM_HW - see readme.txt for more details
    . Applications - XSPI_NOR_Programmer : load a signed binary to XSPI-NOR (MX25L51245G)  - see readme.txt for more details
	. Applications - FSBLA\FSBLA_eMMC : First Stage Boot Loader to boot from eMMC - see readme.txt for more details
                     (This application describes how to load a cubeExample binary from eMMC into DDR, and execute it on STM32MP13XX_CUSTOM_HW Board)
	. Applications - FSBLA\FSBLA_SNOR : First Stage Boot Loader to boot from XSPI-NOR (MX25L51245G) - see readme.txt for more details
                     (This application describes how to load a cubeExample binary from XSPI-NOR into DDR, and execute it on STM32MP13XX_CUSTOM_HW Board)					 
	. Applications - FileX\Fx_emmc_File_Edit : This application provides an example of Azure RTOS FileX stack usage.
	                 It shows how to develop a basic eMMC file operations application - see README.md for more details
	. Applications - FileX\Fx_uSD_File_Edit : This application provides an example of Azure RTOS FileX stack usage.
	                 It shows how to develop a basic SD card file operations application - see README.md for more details
	. Applications - FileX\Fx_NAND_Write_Read_File : This application provides an example of Azure RTOS FileX and LevelX stacks usage.
                     It demonstrates how to create a Fat File system on the NAND flash using FileX alongside LevelX .
					 The application is designed to execute file operations on the Micron MT29F8G08ABACAH4 NAND flash device,
					 the code provides all required software code for properly managing it. - see README.md for more details
	. Applications - FileX\Fx_Nand_Write_Read_File_Sequencer : This application provides an example of Azure RTOS FileX and LevelX stacks usage on custom
					 board STM32MP13XX_CUSTOM_HW (doesn't run on STM32MP135F-DK board unless a custom board is added).
					 It used the hardware NAND sequencer instead of direct CPU accesses.It demonstrates how to create a Fat File system
					 on the NAND flash using FileX alongside LevelX. The application is designed to execute file operations on the Micron MT29F8G08ABACAH4
					 NAND flash device, the code provides all required software code for properly managing it. - see README.md for more details	

### V0.7.0 (27-06-2023) ###
============================
- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to validate some features not applicable on STM32MP135F-DK board:

/** HW information **/
  - DDR TYPE : LPDDR3 4Gb  (versus DDR3 4Gb on STM32MP135F-DK)
    . Switch in CubeIDE project = DDR_TYPE_LPDDR3_4Gb
	. Driver DDR:
		. Switch also used in stm32mp13xx_hal_ddr.c
		. Header file used : stm32mp13xx-lpddr3-4Gb.h
	For other DDR: 
	  . Use CubeMX for MP13 to generate your settings file
        . Generate your header file
	  . Use STM32DDRFW-UTIL to validate your DDR setup
		. Copy the header file as template in STM32DDRFW-UTIL\utilities-ddr\DDR_Tool\your_board project
		. Create a new DDR_Switch  related to your specific DDR Type and density
		. Add this DDR_switch in stm32mp_cubemx_ddr_conf.h
	    . Build your project, and launch tests to ensure expected behavior
		. Use save command to print the final header file related to your DDR 
	  . Copy this final header file content in stm32cube_fw_mp13\Drivers\STM32MP13xx_HAL_Driver\Inc for your next applications
	  . Finally, add the DDR_Switch in HAL driver stm32cube_fw_mp13\Drivers\STM32MP13xx_HAL_Driver\Src\hal_ddr.c
	(For more information refer to README.md from STM32DDRFW-UTIL)

  - QSPI-NOR : Macronix MX25L51245G (versus no QSPI-NOR on STM32MP135F-DK)
    . Files related to this QSPI-NOR : 
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\mx25l512.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\stm32mp13xx_valid_qspi.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\FSBLA\FSBLA_SNOR\BSP\stm32mp13xx_valid_qspi.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSP\mx25l512.h,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSP\stm32mp13xx_valid_qspi.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\QSPI_NOR_Programmer\BSPstm32mp13xx_valid_qspi.h,
	. For other QSPI-NOR, need to adapt these BSP files or create new ones
	
  - FMC-NAND : Micron MT29F8G08ABACAH4 (versus no FMC-NAND on STM32MP135F-DK)
    . Files related to this FMC_NAND : 
	  - STM32MP13XX_CUSTOM_HW\Applications\FileX\Fx_Nand_Write_Read_File\BSP\stm32mp13xx_valid_fmc_nand.c,
	  - STM32MP13XX_CUSTOM_HW\Applications\FileX\Fx_Nand_Write_Read_File\BSP\stm32mp13xx_valid_fmc_nand.h
	. For other FMC_NAND, need to adapt these BSP files or create new ones
	
  - MicroSD card slot available on this board ( as STM32MP135F-DK)

  - No PMIC on this board
  
  - No IOEXPANDER on this board
  
/** Firmware information **/
  - Some applications are implemented to demonstrate features not applicable on STM32MP135F-DK board:
	. Templates : Reference template for STM32MP13XX_CUSTOM_HW - see readme.txt for more details
	. Examples - DDR_Init : provide a reference to initialize the DDR (LPDDR3 4Gb) on STM32MP13XX_CUSTOM_HW - see readme.txt for more details
    . Applications - QSPI_NOR_Programmer : load a signed binary to QSPI-NOR (MX25L51245G)  - see readme.txt for more details
	. Applications - FSBLA_SNOR : First Stage Boot Loader to boot from QSPI-NOR (MX25L51245G) - see readme.txt for more details
                     (This application describes how to load a cubeExample binary from QSPI-NOR into DDR, and execute it on STM32MP13XX_CUSTOM_HW Board)
	. Applications - FileX\Fx_uSD_File_Edit : This application provides an example of Azure RTOS FileX stack usage.
	                 It shows how to develop a basic SD card file operations application - see README.md for more details
	. Applications - FileX\Fx_NAND_Write_Read_File : This application provides an example of Azure RTOS FileX and LevelX stacks usage.
                     It demonstrates how to create a Fat File system on the NAND flash using FileX alongside LevelX .
					 The application is designed to execute file operations on the Micron MT29F8G08ABACAH4 NAND flash device,
					 the code provides all required software code for properly managing it. - see README.md for more details
	. Applications - FileX\Fx_Nand_Write_Read_File_Sequencer : This application provides an example of Azure RTOS FileX and LevelX stacks usage on custom
					 board STM32MP13XX_CUSTOM_HW (doesn't run on STM32MP135F-DK board unless a custom board is added).
					 It used the hardware NAND sequencer instead of direct CPU accesses.It demonstrates how to create a Fat File system
					 on the NAND flash using FileX alongside LevelX. The application is designed to execute file operations on the Micron MT29F8G08ABACAH4
					 NAND flash device, the code provides all required software code for properly managing it. - see README.md for more details
	