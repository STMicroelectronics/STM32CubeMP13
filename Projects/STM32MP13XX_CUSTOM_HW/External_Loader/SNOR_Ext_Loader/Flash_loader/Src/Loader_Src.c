/**
  ******************************************************************************
  * @file    Loader_Src.c
  * @author  MCD Application Team
  * @brief   This file defines the operations of the external loader for
  *          mx25lm51245g OSPI memory of STM32H7B3I-EVAL.
  *           
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
#include "main.h"

#define LOADER_OK   0x1
#define LOADER_FAIL 0x0

/**
 * @brief  System initialization.
 * @param  None
 * @retval  LOADER_OK = 1   : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int Init(void)
{
  HAL_Init();
  BSP_LED_Init(LED_BLUE);
//  SystemClock_Config(); /* Uncomment this to test the application,  Keep in comment for obenbl usage */

  MX_GPIO_Init();
  BSP_LED_On(LED_BLUE);
  __HAL_RCC_QSPI_FORCE_RESET();  //completely reset peripheral
  __HAL_RCC_QSPI_RELEASE_RESET();

  MX_XSPI_Init();

  if (BSP_XSPI_Init() != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }
  HAL_SuspendTick();
  return LOADER_OK;
}

/**
 * @brief  SNOR Configuration.
 * @param  void * pointer to the configuration structure
 */
void configuration(void *ptr)
{
	return;
}

/**
 * @brief   Program memory.
 * @param   Address: page address
 * @param   Size   : size of data
 * @param   buffer : pointer to data buffer
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int Write(uint32_t Address, uint32_t Size, uint8_t* buffer)
{
  HAL_ResumeTick();
  if (HAL_XSPI_Abort(&hxspi) != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }

  if (BSP_XSPI_WriteMemory((uint8_t*) buffer, (Address & (0x0fffffff)), Size) != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }

  HAL_SuspendTick();
  return LOADER_OK;
}

/**
 * @brief   Sector erase.
 * @param   EraseStartAddress :  erase start address
 * @param   EraseEndAddress   :  erase end address
 * @retval  LOADER_OK = 1       : Operation succeeded
 * @retval  LOADER_FAIL = 0 : Operation failed
 */
int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
  HAL_ResumeTick();

  if (HAL_XSPI_Abort(&hxspi) != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }


  if (BSP_XSPI_EraseSector(EraseStartAddress, EraseEndAddress) != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }

  HAL_SuspendTick();
  return LOADER_OK;
}

/**
 * Description :
 * Mass erase of external flash area
 * Optional command - delete in case usage of mass erase is not planed
 * Inputs    :
 *      none
 * outputs   :
 *     none
 * Note: Optional for all types of device
 */
int MassErase(void) {
  HAL_ResumeTick();

  if (HAL_XSPI_Abort(&hxspi) != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }


  if (BSP_XSPI_Erase_Chip() != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }

  HAL_SuspendTick();
  return LOADER_OK;
}

/**
 * Description :
 * Calculates checksum value of the memory zone
 * Inputs    :
 *      StartAddress  : Flash start address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Checksum value
 * Note: Optional for all types of device
 */
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
  uint8_t missalignementAddress = StartAddress % 4;
  uint8_t missalignementSize = Size;
  int cnt;
  uint32_t Val;

  StartAddress -= StartAddress % 4;
  Size += (Size % 4 == 0) ? 0 : 4 - (Size % 4);

  for (cnt = 0; cnt < Size; cnt += 4) {
      Val = *(uint32_t*) StartAddress;
      if (missalignementAddress) {
          switch (missalignementAddress) {
              case 1:
                  InitVal += (uint8_t) (Val >> 8 & 0xff);
                  InitVal += (uint8_t) (Val >> 16 & 0xff);
                  InitVal += (uint8_t) (Val >> 24 & 0xff);
                  missalignementAddress -= 1;
                  break;
              case 2:
                  InitVal += (uint8_t) (Val >> 16 & 0xff);
                  InitVal += (uint8_t) (Val >> 24 & 0xff);
                  missalignementAddress -= 2;
                  break;
              case 3:
                  InitVal += (uint8_t) (Val >> 24 & 0xff);
                  missalignementAddress -= 3;
                  break;
          }
      } else if ((Size - missalignementSize) % 4 && (Size - cnt) <= 4) {
          switch (Size - missalignementSize) {
              case 1:
                  InitVal += (uint8_t) Val;
                  InitVal += (uint8_t) (Val >> 8 & 0xff);
                  InitVal += (uint8_t) (Val >> 16 & 0xff);
                  missalignementSize -= 1;
                  break;
              case 2:
                  InitVal += (uint8_t) Val;
                  InitVal += (uint8_t) (Val >> 8 & 0xff);
                  missalignementSize -= 2;
                  break;
              case 3:
                  InitVal += (uint8_t) Val;
                  missalignementSize -= 3;
                  break;
          }
      } else {
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val >> 8 & 0xff);
          InitVal += (uint8_t) (Val >> 16 & 0xff);
          InitVal += (uint8_t) (Val >> 24 & 0xff);
      }
      StartAddress += 4;
  }

  return (InitVal);
}

/**
 * Description :
 * Verify flash memory with RAM buffer and calculates checksum value of
 * the programmed memory
 * Inputs    :
 *      FlashAddr     : Flash address
 *      RAMBufferAddr : RAM buffer address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Operation failed (address of failure)
 *     R1             : Checksum value
 * Note: Optional for all types of device
 */
uint64_t Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement) {
  HAL_ResumeTick();

  uint32_t VerifiedData = 0, InitVal = 0;
  uint64_t checksum;
  Size *= 4;

  if (BSP_XSPI_EnableMemoryMappedMode() != HAL_OK) {
      HAL_SuspendTick();
      return LOADER_FAIL;
  }

  checksum = CheckSum((uint32_t) MemoryAddr + (missalignement & 0xf),
                      Size - ((missalignement >> 16) & 0xF), InitVal);
  while (Size > VerifiedData) {
      if (*(uint8_t*) MemoryAddr++
          != *((uint8_t*) RAMBufferAddr + VerifiedData)) {
          HAL_SuspendTick();
          return ((checksum << 32) + (MemoryAddr + VerifiedData));
      }
      VerifiedData++;
  }

  HAL_SuspendTick();

  return (checksum << 32);
}

