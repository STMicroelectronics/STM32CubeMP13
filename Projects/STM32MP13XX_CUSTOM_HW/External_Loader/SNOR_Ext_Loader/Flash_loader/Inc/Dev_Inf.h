/**
  ******************************************************************************
  * @file    Dev_Inf.h
  * @author  MCD Application Team
  * @brief   header file of Dev_Inf.c
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
*/

#define 	MCU_FLASH 	    1
#define 	NAND_FLASH      2
#define 	NOR_FLASH       3
#define 	SRAM            4
#define 	PSRAM           5
#define 	PC_CARD         6
#define 	SPI_FLASH       7
#define 	I2C_FLASH       8
#define 	SDRAM           9
#define 	I2C_EEPROM      10


#define   SECTOR_NUM      10				                // Max Number of Sector types

struct DeviceSectors  
{
  unsigned long		SectorNum;                                              // Number of Sectors
  unsigned long		SectorSize;                                             // Sector Size in Bytes
};

struct StorageInfo  
{
   char                 DeviceName[100];			                // Device Name and Description
   unsigned short       DeviceType;					        // Device Type: ONCHIP, EXT8BIT, EXT16BIT, ...
   unsigned long        DeviceStartAddress;		                        // Default Device Start Address
   unsigned long        DeviceSize;					        // Total Size of Device
   unsigned long        PageSize;					        // Programming Page Size
   unsigned char        EraseValue;					        // Content of Erased Memory
   struct 	        DeviceSectors	 sectors[SECTOR_NUM];
};

