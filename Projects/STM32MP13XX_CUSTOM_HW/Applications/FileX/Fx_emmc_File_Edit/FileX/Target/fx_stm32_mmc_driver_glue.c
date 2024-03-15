/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#include "fx_stm32_mmc_driver.h"
#include "stm32mp13xx_valid_mmc.h"

TX_SEMAPHORE transfer_semaphore;
/* USER CODE BEGIN  0 */

/* USER CODE END  0 */

/**
* @brief Initializes the MMC IP instance
* @param UINT Instance MMC instance to initialize
* @retval 0 on success error value otherwise
*/
INT fx_stm32_mmc_init(UINT instance)
{
  INT ret = 0;
/* USER CODE BEGIN  FX_MMC_INIT */
  UNUSED(instance);
/* USER CODE END  FX_MMC_INIT */
#if (FX_STM32_MMC_INIT == 1)
  if (BSP_MMC_Init(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }
#endif

  return ret;
}

/**
* @brief Deinitializes the MMC IP instance
* @param UINT Instance MMC instance to deinitialize
* @retval 0 on success error value otherwise
*/
INT fx_stm32_mmc_deinit(UINT instance)
{
  INT ret = 0;
/* USER CODE BEGIN  FX_MMC_DEINIT */
  UNUSED(instance);
/* USER CODE END  FX_MMC_DEINIT */

  if(BSP_MMC_DeInit(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  return ret;
}

/**
* @brief Check the MMC IP status.
* @param UINT Instance MMC instance to check
* @retval 0 when ready 1 when busy
*/
INT fx_stm32_mmc_get_status(UINT instance)
{
  INT ret = 0;
/* USER CODE BEGIN  GET_STATUS */
  UNUSED(instance);
/* USER CODE END  GET_STATUS */
  if (BSP_MMC_GetCardState(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  return ret;
}

/**
* @brief Read Data from the MMC device into a buffer.
* @param UINT *Buffer buffer into which the data is to be read.
* @param UINT StartBlock the first block to start reading from.
* @param UINT NbrOfBlocks total number of blocks to read.
*/
INT fx_stm32_mmc_read_blocks(UINT instance, UINT *buffer, UINT start_block, UINT total_blocks)
{
  INT ret = 0;
/* USER CODE BEGIN  READ_BLOCKS */
  UNUSED(instance);
/* USER CODE END  READ_BLOCKS */

  if (BSP_MMC_ReadBlocks_DMA(instance, (uint32_t *)buffer, start_block, total_blocks) != BSP_ERROR_NONE)
  {
     ret = 1;
  }

  return ret;
}
/**
* @brief Write data buffer into the MMC device.
* @param UINT *Buffer buffer .to write into the MMC device.
* @param UINT StartBlock the first block to start writing from.
* @param UINT NbrOfBlocks total number of blocks to write.
* @retval 0 on success error code otherwise
*/

INT fx_stm32_mmc_write_blocks(UINT instance, UINT *buffer, UINT start_block, UINT total_blocks)
{
  INT ret = 0;
/* USER CODE BEGIN  WRITE_BLOCKS */
  UNUSED(instance);
/* USER CODE END  WRITE_BLOCKS */
  if (BSP_MMC_WriteBlocks_DMA(instance, (uint32_t *)buffer, start_block, total_blocks) != BSP_ERROR_NONE)
  {
     ret = 1;
  }

  return ret;
}

/**
* @brief MMC DMA Write Transfer completed callbacks
* @param Instance the mmc instance
* @retval None
*/
void BSP_MMC_WriteCpltCallback(uint32_t instance)
{
  /* USER CODE BEGIN PRE_TX_CMPLT */

  /* USER CODE END PRE_TX_CMPLT */

  tx_semaphore_put(&transfer_semaphore);

  /* USER CODE BEGIN POST_TX_CMPLT */

  /* USER CODE END POST_TX_CMPLT */
}

/**
* @brief MMC DMA Read Transfer completed callbacks
* @param Instance the mmc instance
* @retval None
*/
void BSP_MMC_ReadCpltCallback(uint32_t instance)
{
  /* USER CODE BEGIN PRE_RX_CMPLT */

  /* USER CODE END PRE_RX_CMPLT */

  tx_semaphore_put(&transfer_semaphore);

  /* USER CODE BEGIN POST_RX_CMPLT */

  /* USER CODE END POST_RX_CMPLT */
}

/* USER CODE BEGIN  1 */

/* USER CODE END  1 */
