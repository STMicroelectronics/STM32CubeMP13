/**
  ******************************************************************************
* @file    stm32mp13xx_valid_fmc_nand.c
* @author  MCD Application Team
* @brief   This file includes a standard driver for the MT29F8G08ABACA FMC
*          memory mounted on STM32MP13XX-VALID board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_fmc_nand.h"

/** @addtogroup BSP
* @{
*/

/** @addtogroup STM32MP13XX_VALID
* @{
*/

/** @defgroup STM32MP13XX_VALID_FMC STM32MP13XX_VALID_FMC
* @{
*/
/* Exported variables --------------------------------------------------------*/
/** @addtogroup STM32MP13XX_VALID_FMC_NAND_Exported_Variables FMC_NAND Exported Variables
* @{
*/
static NAND_HandleTypeDef hnand[NAND_INSTANCES_NBR];

#ifdef HAL_MDMA_MODULE_ENABLED

static MDMA_HandleTypeDef hdma_dataRead[NAND_INSTANCES_NBR];
static MDMA_HandleTypeDef hdma_dataWrite[NAND_INSTANCES_NBR];
static MDMA_HandleTypeDef hdma_eccRead[NAND_INSTANCES_NBR];
static uint32_t eccBuffer[NAND_INSTANCES_NBR][5  * (BSP_NAND_PAGE_SIZE / BSP_NAND_ECC_SECTOR_SIZE)];

#endif /* HAL_MDMA_MODULE_ENABLED  */

/** @defgroup STM32MP13XX_VALID_NOR_Private_Functions_Prototypes NOR Private Functions Prototypes
* @{
*/

/**
* @brief  Initializes the NAND device.
* @param  Instance  NAND Instance      
* @retval BSP status
*/
int32_t BSP_FMC_NAND_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hnand[Instance].Instance  = FMC_NAND_DEVICE;
    
    /* __weak function can be rewritten by the application */
    if(MX_NAND_Init(&hnand[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }
  
  return ret;
}

#ifdef HAL_MDMA_MODULE_ENABLED

/**
* @brief  Initializes the NAND device for use with the sequencer.
* @param  Instance  NAND Instance
* @retval BSP status
*/
int32_t BSP_FMC_NAND_InitWithSequencer(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    hnand[Instance].Instance  = FMC_NAND_DEVICE;

    /* __weak function can be rewritten by the application */
    if (MX_NAND_InitWithSequencer(
        &hnand[Instance],
        &hdma_dataRead[Instance],
        &hdma_dataWrite[Instance],
        &hdma_eccRead[Instance],
        eccBuffer[Instance]
        ) != HAL_OK)
    {
      ret = BSP_ERROR_NO_INIT;
    }
  }

  return ret;
}

#endif /* HAL_MDMA_MODULE_ENABLED  */

/**
* @brief  DeInitializes the NAND memory.
* @retval BSP status
*/
int32_t BSP_FMC_NAND_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;;
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* NAND memory de-initialization */
    hnand[Instance].Instance = FMC_NAND_DEVICE;
    
#ifdef HAL_MDMA_MODULE_ENABLED
    if (HAL_NAND_Sequencer_DeInit(&hnand[Instance]) != HAL_OK)
    {
      return  HAL_ERROR;
    }
#endif /* HAL_MDMA_MODULE_ENABLED  */

    if(HAL_NAND_DeInit(&hnand[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Reads NAND flash IDs.
* @param  Instance  NAND Instance
* @param  pNAND_ID   Pointer to NAND ID structure
* @retval BSP status
*/
int32_t BSP_FMC_NAND_ReadID(uint32_t Instance, NAND_IDTypeDef *pNAND_ID)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if(HAL_NAND_Read_ID(&hnand[Instance], pNAND_ID) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Reads current status of the NAND memory.
* @param  Instance  NAND Instance
* @retval NAND memory status: whether busy or not
*/
int32_t BSP_FMC_NAND_GetStatus(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  HAL_StatusTypeDef status;
  uint32_t nandStatus;
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    status = HAL_NAND_Read_Status(&hnand[Instance], &nandStatus);
    if (status != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      switch (nandStatus)
      {
      case NAND_BUSY:
        ret = BSP_ERROR_BUSY;
        break;

      case NAND_ERROR:
        ret = BSP_ERROR_PERIPH_FAILURE;
        break;

      case NAND_READY:
        ret = BSP_ERROR_NONE;
        break;
      }
    }
  }
  
  return ret;
}

/**
* @brief  Erases the entire NAND chip.
* @param  Instance  NAND Instance
* @retval BSP status
*/
int32_t BSP_FMC_NAND_EraseChip(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t i;
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else /*Erase all blocks*/
  {
    for (i = 0; i < hnand[Instance].Config.BlockNbr; i++)
    {
      Address.Block = i;
      
      if (HAL_NAND_Erase_Block(&hnand[Instance], &Address) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }
  
  return ret;
}

/**
* @brief  Erases the specified block of the NAND device.
* @param  Instance  NAND Instance
* @param  pAddress  pointer to BSP_NAND address structure
* @retval BSP status
*/

int32_t BSP_FMC_NAND_Erase_Block(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress)
{
  int32_t ret = BSP_ERROR_NONE;
  
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;
    
    if (HAL_NAND_Erase_Block(&hnand[Instance], &Address) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Read pages from the NAND device.
* @param  Instance       NAND Instance
* @param  pBuffer        pointer to destination read buffer
* @param  pAddress       pointer to BSP_NAND address structure
* @param  NumPageToRead  number of pages to read from block
* @retval BSP status
*/
int32_t BSP_FMC_NAND_Read(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToRead)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;
    
    if (HAL_NAND_Read_Page_8b(&hnand[Instance], &Address, pBuffer, NumPageToRead) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Write pages to the NAND device.
* @param  Instance         NAND Instance
* @param  pAddress         pointer to BSP_NAND address structure
* @param  pBuffer          pointer to source buffer to write.
* @param  NumPageToWrite   number of pages to write to block
* @retval BSP status
*/
int32_t BSP_FMC_NAND_Write(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToWrite)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;
    
    if (HAL_NAND_Write_Page_8b(&hnand[Instance], &Address, pBuffer, NumPageToWrite) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Read spare areas from the NAND device.
* @param  Instance             NAND Instance
* @param  pAddress             pointer to BSP_NAND address structure
* @param  pBuffer              pointer to source buffer to write. pBuffer should be 16bits aligned.
* @param  NumSpareAreaToRead   Number of spare area to read
* @retval HAL status
*/
int32_t  BSP_FMC_NAND_Read_SpareArea(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaToRead)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;
    
    if (HAL_NAND_Read_SpareArea_8b(&hnand[Instance], &Address, pBuffer, NumSpareAreaToRead) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Write spare areas to the NAND device.
* @param  Instance              NAND Instance
* @param  pAddress              pointer to BSP_NAND address structure
* @param  pBuffer               pointer to source buffer to write
* @param  NumSpareAreaTowrite   number of spare areas to write to block
* @retval HAL status
*/
int32_t  BSP_FMC_NAND_Write_SpareArea(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumSpareAreaTowrite)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};
  
  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;
    
    if (HAL_NAND_Write_SpareArea_8b(&hnand[Instance], &Address, pBuffer, NumSpareAreaTowrite) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
* @brief  Read pages from the NAND device.
* @param  Instance       NAND Instance
* @param  pBuffer        pointer to destination read buffer
* @param  pAddress       pointer to BSP_NAND address structure
* @param  NumPageToRead  number of pages to read from block
* @param  NumPageRead    number of pages effectively read from block
* @retval BSP status
*/
int32_t BSP_FMC_NAND_ReadWithEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToRead, uint32_t *NumPageRead)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_ECC_Read_Page_8b(&hnand[Instance], &Address, pBuffer, NumPageToRead, NumPageRead) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
* @brief  Write pages to the NAND device.
* @param  Instance         NAND Instance
* @param  pAddress         pointer to BSP_NAND address structure
* @param  pBuffer          pointer to source buffer to write.
* @param  NumPageToWrite   number of pages to write to block
* @param  NumPageWritten   number of pages effectively written to block
* @retval BSP status
*/
int32_t BSP_FMC_NAND_WriteWithEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer, uint32_t NumPageToWrite, uint32_t *NumPageWritten)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_ECC_Write_Page_8b(&hnand[Instance], &Address, pBuffer, NumPageToWrite, NumPageWritten) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

#ifdef HAL_MDMA_MODULE_ENABLED

/**
* @brief  Read one page from the NAND device using the sequencer.
* @param  Instance       NAND Instance
* @param  pBuffer        pointer to destination read buffer
* @param  pAddress       pointer to BSP_NAND address structure
* @retval BSP status
*/
int32_t BSP_FMC_NAND_ReadWithSequencer(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    #if !defined(NO_CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
    #endif

    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_Sequencer_Read_Page_8b(&hnand[Instance], &Address, pBuffer) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
* @brief  Write one page to the NAND device using the sequencer.
* @param  Instance         NAND Instance
* @param  pAddress         pointer to BSP_NAND address structure
* @param  pBuffer          pointer to source buffer to write.
* @retval BSP status
*/
int32_t BSP_FMC_NAND_WriteWithSequencer(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    #if !defined(NO_CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
    #endif

    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_Sequencer_Write_Page_8b(&hnand[Instance], &Address, pBuffer) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
* @brief  Read one page from the NAND device using the sequencer.
* @param  Instance       NAND Instance
* @param  pBuffer        pointer to destination read buffer
* @param  pAddress       pointer to BSP_NAND address structure
* @retval BSP status
*/
int32_t BSP_FMC_NAND_ReadWithSequencerAndEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    #if !defined(NO_CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
    #endif

    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_Sequencer_ECC_Read_Page_8b(&hnand[Instance], &Address, pBuffer) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
* @brief  Write one page to the NAND device using the sequencer.
* @param  Instance         NAND Instance
* @param  pAddress         pointer to BSP_NAND address structure
* @param  pBuffer          pointer to source buffer to write.
* @retval BSP status
*/
int32_t BSP_FMC_NAND_WriteWithSequencerAndEcc(uint32_t Instance, BSP_NAND_AddressTypeDef_t *pAddress, uint8_t *pBuffer)
{
  int32_t ret = BSP_ERROR_NONE;
  NAND_AddressTypeDef Address = {0};

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    #if !defined(NO_CACHE_USE)
    L1C_CleanInvalidateDCacheAll();
    #endif

    Address.Block = pAddress->Block;
    Address.Page = pAddress->Page;

    if (HAL_NAND_Sequencer_ECC_Write_Page_8b(&hnand[Instance], &Address, pBuffer) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
* @brief  Wait for the end of the last sequencer page read or write.
* @param  Instance         NAND Instance
* @retval BSP status
*/
int32_t BSP_FMC_NAND_WaitSequencerCompletion(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HAL_NAND_Sequencer_WaitCompletion(&hnand[Instance], HAL_NAND_DEFAULT_SEQUENCER_TIMEOUT) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

int32_t BSP_FMC_NAND_GetEccStatistics(uint32_t Instance, NAND_EccStatisticsTypeDef *statistics)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= NAND_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    HAL_NAND_ECC_GetStatistics(&hnand[Instance], statistics);
  }

  return ret;
}

#endif /* HAL_MDMA_MODULE_ENABLED  */

static HAL_StatusTypeDef MX_NAND_InitCommon(NAND_HandleTypeDef *hNand)
{
  FMC_NAND_PCC_TimingTypeDef ComSpaceTiming = {0};
  FMC_NAND_PCC_TimingTypeDef AttSpaceTiming = {0};
  NAND_EccConfigTypeDef EccConfig = {0};
  
  /* Setup for MT29F8G08ABACA NAND device */

  /* hNand Init */
  hNand->Init.NandBank        = FMC_NAND_BANK3; /* Bank 3 is the only available with STM32MP135 */
  hNand->Init.Waitfeature     = FMC_NAND_WAIT_FEATURE_ENABLE; /* Waiting enabled when communicating with the NAND */
  hNand->Init.MemoryDataWidth = FMC_NAND_MEM_BUS_WIDTH_8; /* An 8-bit NAND is used */
  hNand->Init.EccComputation  = FMC_NAND_ECC_DISABLE; /* The HAL enable ECC computation when needed, keep it disabled at initialization */
  hNand->Init.EccAlgorithm    = BSP_NAND_ECC_ALGO; /* Hamming or BCH algorithm */
  hNand->Init.BCHMode         = BSP_NAND_ECC_BCH_MODE; /* BCH4 or BCH8 if BCH algorithm is used */
#if BSP_NAND_ECC_SECTOR_SIZE == 256
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_256BYTE;
#elif BSP_NAND_ECC_SECTOR_SIZE == 512
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_512BYTE; /* BCH works only with 512-byte sectors */
#elif BSP_NAND_ECC_SECTOR_SIZE == 1024
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_1024BYTE;
#elif BSP_NAND_ECC_SECTOR_SIZE == 2048
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_2048BYTE;
#elif BSP_NAND_ECC_SECTOR_SIZE == 4096
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_4096BYTE;
#elif BSP_NAND_ECC_SECTOR_SIZE == 8192
  hNand->Init.EccSectorSize   = FMC_NAND_ECC_SECTOR_SIZE_8192BYTE;
#else
#error "Unsupported ECC sector size"
#endif
  hNand->Init.TCLRSetupTime   = 2;
  hNand->Init.TARSetupTime    = 2;
  /* hNand Config */
  hNand->Config.PageSize = BSP_NAND_PAGE_SIZE;
  hNand->Config.SpareAreaSize = BSP_NAND_SPARE_AREA_SIZE;
  hNand->Config.BlockSize = BSP_NAND_BLOCK_SIZE;
  hNand->Config.BlockNbr = BSP_NAND_BLOCK_NBR;
  hNand->Config.PlaneSize = BSP_NAND_PLANE_SIZE;
  hNand->Config.PlaneNbr = BSP_NAND_PLANE_NBR;
  hNand->Config.ExtraCommandEnable = ENABLE;
  
  /* The following timings have been taken from UBOOT driver */

  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime = 0x1;
  ComSpaceTiming.WaitSetupTime = 0x7;
  ComSpaceTiming.HoldSetupTime = 0x2;
  ComSpaceTiming.HiZSetupTime = 0x1;
  
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime = 0x1A;
  AttSpaceTiming.WaitSetupTime = 0x7;
  AttSpaceTiming.HoldSetupTime = 0x6A;
  AttSpaceTiming.HiZSetupTime = 0x1;
  
  /* Initialize NAND HAL */
  if (HAL_NAND_Init(hNand, &ComSpaceTiming, &AttSpaceTiming) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  /* Initialize NAND HAL ECC computations */
  EccConfig.Offset = BSP_NAND_ECC_OFFSET;
  if (HAL_NAND_ECC_Init(hNand, &EccConfig) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  return HAL_OK;
}

#ifdef HAL_MDMA_MODULE_ENABLED

/**
* @brief  Initializes the NAND peripheral.
* @param  hNand NAND handle
* @param  hDmaReadData DMA handle for reading data
* @param  hDmaWriteData DMA handle for writing data
* @param  hDmaReadEcc DMA handle for reading ECC
* @param  eccBuffer Buffer for reading the ECC
* @retval HAL status
*/
HAL_StatusTypeDef MX_NAND_InitWithSequencer(NAND_HandleTypeDef *hNand, MDMA_HandleTypeDef *hDmaReadData, MDMA_HandleTypeDef *hDmaWriteData, MDMA_HandleTypeDef *hDmaReadEcc, uint32_t *eccBuffer)
{
  NAND_SequencerConfigTypeDef SequencerConfig = {0};

  if (MX_NAND_InitCommon(hNand) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  /* Initialize NAND HAL sequencer */
  SequencerConfig.HdmaRead = hDmaReadData;
  SequencerConfig.HdmaWrite = hDmaWriteData;
  SequencerConfig.HdmaReadEcc = hDmaReadEcc;
  SequencerConfig.EccBuffer = eccBuffer;
  if (HAL_NAND_Sequencer_Init(hNand, &SequencerConfig) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  /* Reset NAND device */
  if (HAL_NAND_Reset(hNand) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  return HAL_OK;
}

#endif /* HAL_MDMA_MODULE_ENABLED  */

/**
* @brief  Initializes the NAND peripheral.
* @param  hNand NAND handle
* @retval HAL status
*/
__weak HAL_StatusTypeDef MX_NAND_Init(NAND_HandleTypeDef *hNand)
{
  if (MX_NAND_InitCommon(hNand) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  /* Reset NAND device */
  if (HAL_NAND_Reset(hNand) != HAL_OK)
  {
    return  HAL_ERROR;
  }

  return HAL_OK;
}

/** @defgroup STM32MP13XX_VALID_NAND_Private_Functions NAND Private Functions
* @{
*/

static inline void SetupGPIO(GPIO_TypeDef *Gpio, GPIO_InitTypeDef *Init, uint32_t Pin)
{
  /* Setup GPIO options */
  Init->Pin = Pin;
  HAL_GPIO_Init(Gpio, Init);
  /* Unsecure GPIO */
  Gpio->SECCFGR &= ~Pin;
}

/**
* @brief  Initializes the NAND MSP.
* @param  hnand NAND handle
* @retval None
*/
void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hnand);

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  __HAL_RCC_FMC_FORCE_RESET();
  __HAL_RCC_FMC_RELEASE_RESET();

  /* Enable MDMA controller clock */
  __HAL_RCC_MDMA_CLK_ENABLE();

  /* Enable GPIOs clock */
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();
__HAL_RCC_GPIOD_CLK_ENABLE();
__HAL_RCC_GPIOE_CLK_ENABLE();
__HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  /* STM32MP135 pins: */
  GPIO_Init_Structure.Alternate = GPIO_AF10_QUADSPI; // FIXME: Should be GPIO_AF10_FMC but not defined and same value anyway
  SetupGPIO(GPIOA, &GPIO_Init_Structure, GPIO_PIN_9); /* FMC_NWAIT: PA9 */
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;
  SetupGPIO(GPIOG, &GPIO_Init_Structure, GPIO_PIN_9); /* FMC_NCE: PG9 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_4); /* FMC_NOE: PD4 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_5); /* FMC_NWE: PD5 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_12); /* FMC_ALE: PD12 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_11); /* FMC_CLE: PD11 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_14); /* FMC_D0: PD14 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_15); /* FMC_D1: PD15 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_0); /* FMC_D2: PD0 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_1); /* FMC_D3: PD1 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_7); /* FMC_D4: PE7 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_8); /* FMC_D5: PE8 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_9); /* FMC_D6: PE9 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_10); /* FMC_D7: PE10 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_11); /* FMC_D8: PE11 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_12); /* FMC_D9: PE12 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_13); /* FMC_D10: PE13 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_14); /* FMC_D11: PE14 */
  SetupGPIO(GPIOE, &GPIO_Init_Structure, GPIO_PIN_15); /* FMC_D12: PE15 */
  SetupGPIO(GPIOB, &GPIO_Init_Structure, GPIO_PIN_8); /* FMC_D13: PB8 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_9); /* FMC_D14: PD9 */
  SetupGPIO(GPIOD, &GPIO_Init_Structure, GPIO_PIN_10); /* FMC_D15: PD10 */

  /* Enable and set interrupt to the lowest priority */
  IRQ_SetPriority(FMC_IRQn, 0x0F);
  IRQ_Enable(FMC_IRQn);
}

/**
* @brief  DeInitializes NAND MSP.
* @param  hnand NAND handle
* @retval None
*/
void HAL_NAND_MspDeInit(NAND_HandleTypeDef *hnand)
{
  IRQ_Disable(FMC_IRQn);

  __HAL_RCC_FMC_FORCE_RESET();
  __HAL_RCC_FMC_RELEASE_RESET();

  __HAL_RCC_MDMA_CLK_DISABLE();

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_1);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_10);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_11);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_12);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_13);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_14);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10);

  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
}

#ifdef HAL_MDMA_MODULE_ENABLED

#define MDMA_BUFFER_TRANSFER_LENGTH_MAX 128

HAL_StatusTypeDef HAL_NAND_Sequencer_MspInit(NAND_HandleTypeDef *hnand)
{
  HAL_StatusTypeDef status;
  MDMA_HandleTypeDef *hdma;
  uint32_t eccSizePerSector = HAL_NAND_Sequencer_GetEccWordsPerSector(hnand) * 4;

  hdma = hnand->SequencerConfig.HdmaRead;
  hdma->Instance = MDMA_Channel0;
  hdma->Init.Request = MDMA_REQUEST_FMC_DATA;
  hdma->Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
  hdma->Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hdma->Init.SecureMode = MDMA_SECURE_MODE_DISABLE;
  hdma->Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hdma->Init.SourceInc = MDMA_SRC_INC_DISABLE;
  hdma->Init.DestinationInc = MDMA_DEST_INC_WORD;
  hdma->Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
  hdma->Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
  hdma->Init.DataAlignment = MDMA_DATAALIGN_RIGHT;
  hdma->Init.BufferTransferLength = MDMA_BUFFER_TRANSFER_LENGTH_MAX;
  hdma->Init.SourceBurst = MDMA_SOURCE_BURST_32BEATS;
  hdma->Init.DestBurst = MDMA_DEST_BURST_32BEATS;
  hdma->Init.SourceBlockAddressOffset = 0;
  hdma->Init.DestBlockAddressOffset = 0;
  status = HAL_MDMA_Init(hdma);
  if (status != HAL_OK)
  {
    return status;
  }

  hdma = hnand->SequencerConfig.HdmaWrite;
  hdma->Instance = MDMA_Channel1;
  hdma->Init.Request = MDMA_REQUEST_FMC_DATA;
  hdma->Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
  hdma->Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hdma->Init.SecureMode = MDMA_SECURE_MODE_DISABLE;
  hdma->Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hdma->Init.SourceInc = MDMA_SRC_INC_WORD;
  hdma->Init.DestinationInc = MDMA_DEST_INC_DISABLE;
  hdma->Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
  hdma->Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
  hdma->Init.DataAlignment = MDMA_DATAALIGN_RIGHT;
  hdma->Init.BufferTransferLength = MDMA_BUFFER_TRANSFER_LENGTH_MAX;
  hdma->Init.SourceBurst = MDMA_SOURCE_BURST_32BEATS;
  hdma->Init.DestBurst = MDMA_DEST_BURST_32BEATS;
  hdma->Init.SourceBlockAddressOffset = 0;
  hdma->Init.DestBlockAddressOffset = 0;
  status = HAL_MDMA_Init(hdma);
  if (status != HAL_OK)
  {
    return status;
  }

  hdma = hnand->SequencerConfig.HdmaReadEcc;
  hdma->Instance = MDMA_Channel2;
  hdma->Init.Request = MDMA_REQUEST_FMC_ERROR;
  hdma->Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
  hdma->Init.Priority = MDMA_PRIORITY_VERY_HIGH;
  hdma->Init.SecureMode = MDMA_SECURE_MODE_DISABLE;
  hdma->Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  hdma->Init.SourceInc = MDMA_SRC_INC_WORD;
  hdma->Init.DestinationInc = MDMA_DEST_INC_WORD;
  hdma->Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
  hdma->Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
  hdma->Init.DataAlignment = MDMA_DATAALIGN_RIGHT;
  hdma->Init.BufferTransferLength = eccSizePerSector;
  hdma->Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
  hdma->Init.DestBurst = MDMA_DEST_BURST_SINGLE;
  hdma->Init.SourceBlockAddressOffset = -eccSizePerSector;
  hdma->Init.DestBlockAddressOffset = 0;
  status = HAL_MDMA_Init(hdma);
  if (status != HAL_OK)
  {
    return status;
  }

  return HAL_OK;
}

void HAL_NAND_Sequencer_MspDeInit(NAND_HandleTypeDef *hnand)
{
  HAL_MDMA_DeInit(hnand->SequencerConfig.HdmaRead);
  HAL_MDMA_DeInit(hnand->SequencerConfig.HdmaWrite);
  HAL_MDMA_DeInit(hnand->SequencerConfig.HdmaReadEcc);
}

#endif /* HAL_MDMA_MODULE_ENABLED  */
