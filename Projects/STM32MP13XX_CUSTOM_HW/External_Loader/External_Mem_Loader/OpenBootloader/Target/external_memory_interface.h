/**
  ******************************************************************************
  * @file    external_memory_interface.h
  * @author  MCD Application Team
  * @brief   Header for external_memory_interface.c module
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

#ifndef EXTERNAL_MEMORY_INTERFACE_H
#define EXTERNAL_MEMORY_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
struct DeviceSectors  
{
  unsigned long		SectorNum;                        // Number of Sectors
  unsigned long		SectorSize;                       // Sector Size in Bytes
};

struct StorageInfo  
{
   char                  DeviceName[100];	// Device Name and Description
   unsigned short        DeviceType;		// Device Type:Flash, ...
   unsigned long         DeviceStartAddress;	// Default Device Start Address
   unsigned long         DeviceSize;		// Total Size of Device
   unsigned long         PageSize;		// Programming Page Size
   unsigned char         EraseValue;		// Content of Erased Memory
   struct DeviceSectors	 sectors[10];
};

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
int32_t OPENBL_ExtMem_Init(uint32_t Address);
void OPENBL_ExtMem_Conf(void *ConfPtr);
uint8_t OPENBL_ExtMem_Read(uint32_t Address);
void OPENBL_ExtMem_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);
uint64_t OPENBL_ExtMem_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement);
void OPENBL_ExtMem_JumpToAddress(uint32_t Address);
void OPENBL_ExtMem_MassErase(uint32_t Address);
void OPENBL_ExtMem_SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress);

ErrorStatus OPENBL_ExtMem_Erase(uint16_t sectors_number);

#endif /* EXTERNAL_MEMORY_INTERFACE_H */
