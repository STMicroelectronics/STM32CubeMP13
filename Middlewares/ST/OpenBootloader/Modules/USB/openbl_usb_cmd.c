/**
  ******************************************************************************
  * @file    openbl_usb_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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

/* Includes ------------------------------------------------------------------*/
#include "openbl_usb_cmd.h"
#include "openbl_mem.h"
#include "openbootloader_conf.h"
#include "usb_interface.h"
#include "openbl_util.h"

/* External variables --------------------------------------------------------*/
extern OPENBL_Flashlayout_TypeDef FlashlayoutStruct;
uint8_t prog_USB;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t phase = PHASE_FLASHLAYOUT;
static uint32_t phase3_addr_offset = 0;
static uint32_t phase4_counter = 0, phase4_addr_offset = 0;
static uint32_t phase5_counter = 0, phase5_addr_offset = 0;
uint8_t count = 0;
static uint32_t cur_sector = 0;
static uint32_t last_sector = 0;
static uint8_t cur_part = PHASE_FLASHLAYOUT;
static bool is_start_operation = false;
uint32_t addr;

/* Private function prototypes -----------------------------------------------*/
uint32_t OPENBL_USB_GetAddress(uint8_t Phase);
uint8_t OPENBL_USB_GetPhase(uint32_t Alt);
void Write_Flash_layout_Ext_Mem(void);

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  Memory write routine.
  * @param  pSrc: Pointer to the source buffer. Address to be written to.
  * @param  Alt: USB Alternate.
  * @param  Length: Number of data to be written (in bytes).
  * @retval None.
  */
uint32_t OPENBL_USB_Download(uint8_t *pSrc, uint32_t Alt, uint32_t Length, uint32_t BlockNumber)
{
  uint32_t res;
  switch (phase)
  {
  case PHASE_0x3:
	/* As there will be multiple packet download request, addr needs to be incremented with amount of data already written in the memory */
    addr += phase3_addr_offset;
    /* Write memory */
    OPENBL_MEM_Write(addr, pSrc, Length);
    phase3_addr_offset = Length;
    break;

  case PHASE_0x4:
	/* External memory initialization is done only for the first download packet request and for 0x4 phase/partition only */
    if(phase4_counter == 0)
	{
		/* Init the external memories */
		if (1 != OPENBL_MEM_Init(addr))
		{
			return 1;
		}
#if defined SDCARD_FLASH
		/* Sector Erase is being to erase first 2MB so that GPT table gets erased in case SD Card
		 * is previously programmed with Linux Image */
		OPENBL_MEM_SectorErase(EXT_MEMORY_START_ADDRESS, EXT_MEMORY_START_ADDRESS, (2 * 1024 * 1024));
#endif /* SDCARD_FLASH */
		uint32_t command = CONFIGURE_ENABLE_BOOT1_PARTITION;
		OPENBL_MEM_Configure(addr, (void *)&command);
	}
	/* As there will be multiple packet download request, addr needs to be incremented with amount of data already written in the memory */
	addr += phase4_addr_offset;

	/* Get the current sector */
#if defined SNOR_FLASH
    cur_sector = ((addr - EXT_MEMORY_START_ADDRESS) / SECTOR_SIZE) + 1;
#elif defined SDCARD_FLASH
    cur_sector = addr - EXT_MEMORY_START_ADDRESS;
#endif
    if (cur_sector > last_sector)
    {
      /* Erase sector */
#if defined SDCARD_FLASH
      /* For SDcard, erase 2 sector/blocks as USB packet size is 1024 bytes */
      OPENBL_MEM_SectorErase(addr, addr, (addr + 2));
#elif defined SNOR_FLASH
      OPENBL_MEM_SectorErase(addr, addr, (addr + Length));
#endif
      /* Update last sector */
      last_sector = cur_sector;
    }

    /* Write memory */
#if defined SNOR_FLASH
    OPENBL_MEM_Write(addr, pSrc, Length);
    phase4_addr_offset = 0;
    phase4_addr_offset += Length;
#elif defined SDCARD_FLASH
    prog_USB = 1;
    /* For SDcard write 2 blocks of 512 bytes as USB packet size is 1024 bytes */
    OPENBL_MEM_Write(addr, pSrc, 2);
    phase4_addr_offset = 2;
#endif
    /* Verify data write to memory */
#if defined SNOR_FLASH
    res = OPENBL_MEM_Verify(addr, (uint32_t)pSrc, Length, 0);
    if ((res != 0) && (res < (addr + Length)))
    {
      /* Error */
    	return 1;
    }
#elif defined SDCARD_FLASH
    /* Verify data write to SDCard memory */
    res = OPENBL_MEM_Verify((addr * BLOCKSIZE_SD), (uint32_t)pSrc, Length, 0);
    if (res != 0)
    {
        /* Error */
        while(1){};
    }
#endif
    phase4_counter++;
    break;

  case PHASE_0x5:
	addr += phase5_addr_offset;

	if(phase5_counter == 0)
	{
		uint32_t command = CONFIGURE_DISABLE_BOOT1_PARTITION;
		OPENBL_MEM_Configure(addr, (void *)&command);
		Write_Flash_layout_Ext_Mem();
	}
	/* Get the current sector */
#if defined SNOR_FLASH
    cur_sector = ((addr - EXT_MEMORY_START_ADDRESS) / SECTOR_SIZE) + 1;
#elif defined SDCARD_FLASH
    cur_sector = addr - EXT_MEMORY_START_ADDRESS;
#endif
    if (cur_sector > last_sector)
    {
      /* Erase sector */
#if defined SDCARD_FLASH
      /* For SDcard, erase 2 sector/blocks as USB packet size is 1024 bytes */
      OPENBL_MEM_SectorErase(addr, addr, (addr + 2));
#elif defined SNOR_FLASH
      OPENBL_MEM_SectorErase(addr, addr, (addr + Length));
#endif
      /* Update last sector */
      last_sector = cur_sector;
    }
    /* Write memory */
#if defined SNOR_FLASH
    OPENBL_MEM_Write(addr, pSrc, Length);
    phase5_addr_offset = 0;
    phase5_addr_offset += Length;
#elif defined SDCARD_FLASH
    /* For SDcard write 2 blocks of 512 bytes as USB packet size is 1024 bytes */
    OPENBL_MEM_Write(addr, pSrc, 2);
    phase5_addr_offset = 2;
#endif
    /* Verify data write to memory */
#if defined SNOR_FLASH
    res = OPENBL_MEM_Verify(addr, (uint32_t)pSrc, Length, 0);
    if ((res != 0) && (res < (addr + Length)))
    {
      /* Error */
    	return 1;
    }
#elif defined SDCARD_FLASH
    /* Verify data write to SDCard memory */
    res = OPENBL_MEM_Verify((addr * BLOCKSIZE_SD), (uint32_t)pSrc, Length, 0);
    if (res != 0)
    {
        /* Error */
        while(1){};
    }
#endif
    phase5_counter++;
    break;

  case PHASE_FLASHLAYOUT:
    /* first 256 is reserved for binary signature info */
    addr = addr + 256;
    /* Parse the flashlayout */
    if (parse_flash_layout((uint32_t)pSrc + 256, (Length - 256)) == PARSE_ERROR)
    {
      /* Error */
    	return 1;
    }
    break;

  default:
    break;
  }
  return 0;
}

/**
  * @brief  Memory read routine.
  * @param  USB Alternate
  * @param  pDest: Pointer to the destination buffer.
  * @param  Length: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *OPENBL_USB_ReadMemory(uint32_t Alt, uint8_t *pDest, uint32_t Length, uint32_t BlockNumber)
{
  phase = OPENBL_USB_GetPhase(Alt);

  switch (phase)
  {
  case PHASE_CMD:
    if (cur_part == PHASE_FLASHLAYOUT) /* Phase flashlayout */
    {
      phase = PHASE_FLASHLAYOUT;
    }
#if !defined(__CP_SERIAL_BOOT__) /* For Dev boot, first partition is partition 0x3 not 0x1 */
    else if (cur_part <= FlashlayoutStruct.partsize)
    {
      phase = FlashlayoutStruct.id[cur_part-1];
    }
#else
    else if (cur_part < FlashlayoutStruct.partsize) /* Phase external memories 0x3, 0x4 and 0x5 */
    {
      phase = FlashlayoutStruct.id[cur_part];
    }
#endif
    else /* Phase END */
    {
      phase = PHASE_END;
    }

    /* Get the phase address */
    addr = OPENBL_USB_GetAddress(phase);
    /* Get phase command response */
    pDest[0] = phase;
    pDest[1] = (uint8_t)(addr >> 0);
    pDest[2] = (uint8_t)(addr >> 8);
    pDest[3] = (uint8_t)(addr >> 16);
    pDest[4] = (uint8_t)(addr >> 24);
    pDest[5] = 0x00;
    pDest[6] = 0x00;
    pDest[7] = 0x00;
    pDest[8] = 0x00;

    /* Start operation call after each phase operation */
    /* If current operation is phase operation, next operation is start operation */
    if (is_start_operation) /* Start operation */
    {
      /* Next operation is phase operation */
      is_start_operation = false;

      /* Update current partition */
      cur_part++;
    }
    else /* Phase operation */
    {
      /* Next operation is start operation */
      is_start_operation = true;
    }
    break;
  default:

    break;
  }

  /* Return a valid address to avoid HardFault */
  return pDest;
}

/**
  * @brief  Link between USB Alternate and STM32CubeProgrammer phase
  * @param  Alt: USB Alternate.
  * @retval STM32CubeProgramer Phase.
  */
uint8_t OPENBL_USB_GetPhase(uint32_t Alt)
{
  uint8_t ret;

  switch (Alt)
  {
  case 0:
    ret = PHASE_FLASHLAYOUT;
    break;

  case 1:
    ret = PHASE_0x3;
    break;

  case 2:
    ret = PHASE_0x4;
    break;

  case 3:
    ret = PHASE_CMD;
    break;

  case 5:
    ret = PHASE_0x5;
    break;

  default:
    ret = PHASE_END;
    break;
  }

  return ret;
}

/**
  * @brief  Get Address of STM32CubeProgrammer phase
  * @param  Phase: USB Alternate.
  * @retval STM32CubeProgrammer Phase.
  */
uint32_t OPENBL_USB_GetAddress(uint8_t Phase)
{
  uint32_t ret;
  uint8_t i = 0;

  switch (Phase)
  {
  case 0x00:
    ret = FLASHLAYOUT_RAM_ADDRESS;
    break;

  case 0x03:
    ret = RAM_WRITE_ADDRESS;
    break;

#if defined SNOR_FLASH
//  case 0x04:
//    ret = EXT_MEMORY_START_ADDRESS;
//    break;
//
//  case 0x05:
//    ret = EXT_MEMORY_START_ADDRESS + TEST_APP_OFFSET;
//    break;
#endif

#if defined(SDCARD_FLASH) ||defined(SNOR_FLASH)
  case 0x04:
  case 0x05:
	for (i = 0; i < FlashlayoutStruct.partsize; i++)
	{
		if (FlashlayoutStruct.id[i] == Phase)
		{
			break;
		}
	}
	if (i == FlashlayoutStruct.partsize)
	{
		ret = UNDEF_ADDRESS;
	}
	else
	{
		ret = EXT_MEMORY_START_ADDRESS + FlashlayoutStruct.offset[i];
	}
    break;
#endif

  default:
    ret = UNDEF_ADDRESS;
    break;
  }

  return ret;
}
