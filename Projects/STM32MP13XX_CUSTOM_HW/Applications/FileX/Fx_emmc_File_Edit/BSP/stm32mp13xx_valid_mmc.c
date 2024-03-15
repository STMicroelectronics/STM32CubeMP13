/**
  ******************************************************************************
  * @file    stm32h750b_discovery_mmc.c
  * @author  MCD Application Team
  * @brief   This file includes the EMMC driver mounted on STM32MP13-CUSTOM_HW
  *          board.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the EMMC mounted on STM32H750B-DK board.
   - This driver does not need a specific component driver for the EMMC device
     to be included with.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the external EMMC memory using the BSP_MMC_Init() function. This
       function includes the MSP layer hardware resources initialization and the
       SDIO interface configuration to interface with the external EMMC. It
       also includes the EMMC initialization sequence.
     o The function BSP_MMC_GetCardInfo() is used to get the MMC information
       which is stored in the structure "HAL_MMC_CardInfoTypedef".

  + Micro MMC card operations
     o The micro MMC card can be accessed with read/write block(s) operations once
       it is ready for access. The access can be performed whether using the polling
       mode by calling the functions BSP_MMC_ReadBlocks()/BSP_MMC_WriteBlocks(), or by DMA
       transfer using the functions BSP_MMC_ReadBlocks_DMA()/BSP_MMC_WriteBlocks_DMA()
     o The DMA transfer complete is used with interrupt mode. Once the MMC transfer
       is complete, the MMC interrupt is handled using the function BSP_MMC_IRQHandler(),
       the DMA Tx/Rx transfer complete are handled using the functions
       MMC_DMA_Tx_IRQHandler()/MMC_DMA_Rx_IRQHandler() that should be defined by user.
       The corresponding user callbacks are implemented by the user at application level.
     o The MMC erase block(s) is performed using the function BSP_MMC_Erase() with specifying
       the number of blocks to erase.
     o The MMC runtime status is returned when calling the function BSP_MMC_GetStatus().

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_valid_mmc.h"


/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H750B_DK
  * @{
  */

/** @defgroup STM32H750B_DK_MMC MMC
  * @{
  */


/** @defgroup STM32H750B_DK_MMC_Exported_Variables Exported Variables
  * @{
  */
MMC_HandleTypeDef hsd_sdmmc[MMC_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup STM32H750B_DK_MMC_Private_FunctionsPrototypes Private Functions Prototypes
  * @{
  */
static void MMC_MspInit(MMC_HandleTypeDef *hmmc);
static void MMC_MspDeInit(MMC_HandleTypeDef *hmmc);
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
static void MMC_AbortCallback(MMC_HandleTypeDef *hmmc);
static void MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc);
static void MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc);
#endif
/**
  * @}
  */

/** @defgroup STM32H750B_DK_MMC_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Initializes the MMC card device.
  * @param  Instance      SDMMC Instance
  * @retval BSP status
  */
int32_t BSP_MMC_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 0)
    /* Msp MMC initialization */
    MMC_MspInit(&hsd_sdmmc[Instance]);
#else
    /* Register the MMC MSP Callbacks */
    if(IsMspCallbacksValid[Instance] == 0UL)
    {
      if(BSP_MMC_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
    if(ret == BSP_ERROR_NONE)
    {
#endif
      /* HAL MMC initialization */
      if(MX_MMC_SD_Init(&hsd_sdmmc[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
      /* Register MMC TC, HT and Abort callbacks */
      else if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_TX_CPLT_CB_ID, MMC_TxCpltCallback) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_RX_CPLT_CB_ID, MMC_RxCpltCallback) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_ABORT_CB_ID, MMC_AbortCallback) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }
    }
#endif /* USE_HAL_MMC_REGISTER_CALLBACKS */
  }
  return  ret;
}

/**
  * @brief  DeInitializes the MMC card device.
  * @param  Instance      SDMMC Instance
  * @retval BSP status
  */
int32_t BSP_MMC_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }/* HAL MMC deinitialization */
  else if(HAL_MMC_DeInit(&hsd_sdmmc[Instance]) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    /* Msp MMC de-initialization */
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 0)
    MMC_MspDeInit(&hsd_sdmmc[Instance]);
#endif /* (USE_HAL_MMC_REGISTER_CALLBACKS == 0) */
  }

  return  ret;
}

/**
  * @brief  Initializes the SDMMC1 peripheral.
  * @param  hmmc SD handle
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_MMC_SD_Init(MMC_HandleTypeDef *hmmc)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hmmc->Instance                 = SDMMC2;
  hmmc->Init.ClockDiv            = 2;
  hmmc->Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hmmc->Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
  hmmc->Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;
  hmmc->Init.BusWide             = SDMMC_BUS_WIDE_8B;

  /* HAL SD initialization */
  if(HAL_MMC_Init(hmmc) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP MMC Msp Callbacks
  * @param  Instance      SDMMC Instance
  * @retval BSP status
  */
int32_t BSP_MMC_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_MSP_INIT_CB_ID, MMC_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_MSP_DEINIT_CB_ID, MMC_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsMspCallbacksValid[Instance] = 1U;
    }
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP MMC Msp Callback registering
  * @param  Instance   SDMMC Instance
  * @param  CallBacks  pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_MMC_RegisterMspCallbacks(uint32_t Instance, BSP_MMC_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if(HAL_MMC_RegisterCallback(&hsd_sdmmc[Instance], HAL_MMC_MSP_DEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsMspCallbacksValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_MMC_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  Instance   MMC Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be read
  * @param  BlocksNbr  Number of MMC blocks to read
  * @retval BSP status
  */
int32_t BSP_MMC_ReadBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  uint32_t timeout = MMC_READ_TIMEOUT*BlocksNbr;
  int32_t ret;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(HAL_MMC_ReadBlocks(&hsd_sdmmc[Instance], (uint8_t *)pData, BlockIdx, BlocksNbr, timeout) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
  * @param  Instance   MMC Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be written
  * @param  BlocksNbr  Number of MMC blocks to write
  * @retval BSP status
  */
int32_t BSP_MMC_WriteBlocks(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  uint32_t timeout = MMC_READ_TIMEOUT*BlocksNbr;
  int32_t ret;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(HAL_MMC_WriteBlocks(&hsd_sdmmc[Instance], (uint8_t *)pData, BlockIdx, BlocksNbr, timeout) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  Instance   MMC Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be read
  * @param  BlocksNbr  Number of MMC blocks to read
  * @retval BSP status
  */
int32_t BSP_MMC_ReadBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t ret;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(HAL_MMC_ReadBlocks_DMA(&hsd_sdmmc[Instance], (uint8_t *)pData, BlockIdx, BlocksNbr) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  Instance   MMC Instance
  * @param  pData      Pointer to the buffer that will contain the data to transmit
  * @param  BlockIdx   Block index from where data is to be written
  * @param  BlocksNbr  Number of MMC blocks to write
  * @retval BSP status
  */
int32_t BSP_MMC_WriteBlocks_DMA(uint32_t Instance, uint32_t *pData, uint32_t BlockIdx, uint32_t BlocksNbr)
{
  int32_t ret;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(HAL_MMC_WriteBlocks_DMA(&hsd_sdmmc[Instance], (uint8_t *)pData, BlockIdx, BlocksNbr) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the specified memory area of the given MMC card.
  * @param  Instance   MMC Instance
  * @param  StartAddr : Start byte address
  * @param  EndAddr : End byte address
  * @retval BSP status
  */
int32_t BSP_MMC_Erase(uint32_t Instance, uint32_t StartAddr, uint32_t EndAddr)
{
//  int32_t ret;

//  if(Instance >= MMC_INSTANCES_NBR)
//  {
//    ret = BSP_ERROR_WRONG_PARAM;
//  }
//  else if( HAL_MMC_Erase(&hsd_sdmmc[Instance], StartAddr, EndAddr) != HAL_OK)
//  {
//    ret = BSP_ERROR_PERIPH_FAILURE;
//  }
//  else
//  {
//    ret = BSP_ERROR_NONE;
//  }

  return BSP_ERROR_NONE ;
}

/**
  * @brief  Handles MMC card interrupt request.
  * @param  Instance   MMC Instance
  * @retval None
  */
void BSP_MMC_IRQHandler(uint32_t Instance)
{
  HAL_MMC_IRQHandler(&hsd_sdmmc[Instance]);
}

/**
  * @brief  Gets the current MMC card data status.
  * @param  Instance   MMC Instance
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  MMC_TRANSFER_OK: No data transfer is acting
  *            @arg  MMC_TRANSFER_BUSY: Data transfer is acting
  *            @arg  MMC_TRANSFER_ERROR: Data transfer error
  */
int32_t BSP_MMC_GetCardState(uint32_t Instance)
{
//  return MMC_TRANSFER_OK;
  return((HAL_MMC_GetCardState(&hsd_sdmmc[Instance]) == HAL_MMC_CARD_TRANSFER ) ? MMC_TRANSFER_OK : MMC_TRANSFER_BUSY);
}

/**
  * @brief  Get MMC information about specific MMC card.
  * @param  Instance   MMC Instance
  * @param  CardInfo : Pointer to HAL_MMC_CardInfoTypedef structure
  * @retval None
  */
int32_t BSP_MMC_GetCardInfo(uint32_t Instance, BSP_MMC_CardInfo *CardInfo)
{
  int32_t ret;

  if(Instance >= MMC_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(HAL_MMC_GetCardInfo(&hsd_sdmmc[Instance], CardInfo) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP MMC Abort callbacks
  * @param  Instance   MMC Instance
  * @retval None
  */
__weak void BSP_MMC_AbortCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief BSP Tx Transfer completed callbacks
  * @param  Instance   MMC Instance
  * @retval None
  */
__weak void BSP_MMC_WriteCpltCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

/**
  * @brief BSP Rx Transfer completed callbacks
  * @param  Instance   MMC Instance
  * @retval None
  */
__weak void BSP_MMC_ReadCpltCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);
}

#if (USE_HAL_MMC_REGISTER_CALLBACKS == 0)
/**
  * @brief MMC Abort callbacks
  * @param hmmc : MMC handle
  * @retval None
  */
void HAL_MMC_AbortCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_AbortCallback(0);
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc: MMC handle
  * @retval None
  */
void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_WriteCpltCallback(0);
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc: MMC handle
  * @retval None
  */
void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_ReadCpltCallback(0);
}
#endif
/**
  * @}
  */

/** @defgroup STM32H750B_DK_MMC_Private_Functions Private Functions
  * @{
  */

void MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
  GPIO_InitTypeDef GPIO_Init_Structure = {0};

  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */

    /* Enable SDMMC Clock */
    __HAL_RCC_SDMMC2_CLK_ENABLE();
    /* Force the SDMMC Periheral Clock Reset */
    __HAL_RCC_SDMMC2_FORCE_RESET();
    /* Release the SDMMC Periheral Clock Reset */
    __HAL_RCC_SDMMC2_RELEASE_RESET();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;

    /* D0 D1 D2 D3 CK on PC8 PC9 PC10 PC11 PC12 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

    /* CMD on PD2 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

    /* D5 on PB12 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

    /* D6 on PC6 - AF8 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

    /* D7 on PC7 - AF12 NOPULL*/
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Alternate = GPIO_AF10_SDIO2;
    GPIO_Init_Structure.Pin       = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

    /* SDMMC1 interrupt Init */
  /* GVIC configuration for SDIO interrupts */
  IRQ_SetPriority(SDMMC2_IRQn, 0x00);
  IRQ_Enable(SDMMC2_IRQn);
  /* USER CODE BEGIN SDMMC1_MspInit 1 */

  /* USER CODE END SDMMC1_MspInit 1 */
}

/**
  * @brief SD MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and interrupt configuration to their default state
  * @param hsd: SD handle pointer
  * @retval None
  */
void MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{

  if(hmmc->Instance==SDMMC2)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC2_CLK_DISABLE();

    /**SDMMC1 GPIO Configuration
    PC11     ------> SDMMC1_D3
    PC10     ------> SDMMC1_D2
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
    PC9     ------> SDMMC1_D1
    PC8     ------> SDMMC1_D0
    */
    HAL_GPIO_DeInit(GPIOB, (GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_14 | GPIO_PIN_15));

    HAL_GPIO_DeInit(GPIOC, (GPIO_PIN_6 | GPIO_PIN_7));

    HAL_GPIO_DeInit(GPIOE, (GPIO_PIN_3));

    HAL_GPIO_DeInit(GPIOF, (GPIO_PIN_0));

    HAL_GPIO_DeInit(GPIOG, (GPIO_PIN_6));

    /* SDMMC1 interrupt DeInit */
    IRQ_Disable(SDMMC2_IRQn);
  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
}

#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
/**
  * @brief MMC Abort callbacks
  * @param hmmc : MMC handle
  * @retval None
  */
static void MMC_AbortCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_AbortCallback(0);
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc : MMC handle
  * @retval None
  */
static void MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_WriteCpltCallback(0);
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc : MMC handle
  * @retval None
  */
static void MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)
{
  BSP_MMC_ReadCpltCallback(0);
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
