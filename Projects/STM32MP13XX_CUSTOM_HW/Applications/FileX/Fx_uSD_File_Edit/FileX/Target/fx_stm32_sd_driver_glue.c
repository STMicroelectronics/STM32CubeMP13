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

#include "fx_stm32_sd_driver.h"
#include "stm32mp13xx_valid_sd.h"

TX_SEMAPHORE transfer_semaphore;

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/**
* @brief Initializes the SD IP instance
* @param UINT instance SD instance to initialize
* @retval 0 on success error value otherwise
*/
INT fx_stm32_sd_init(UINT instance)
{
  INT ret = 0;

  /* USER CODE BEGIN PRE_FX_SD_INIT */
  UNUSED(instance);
  /* USER CODE END PRE_FX_SD_INIT */

#if (FX_STM32_SD_INIT == 1)
  if (BSP_SD_Init(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }
#endif

  /* USER CODE BEGIN POST_FX_SD_INIT */

  /* USER CODE END POST_FX_SD_INIT */

  return ret;
}


/**
* @brief Deinitializes the SD IP instance
* @param UINT instance SD instance to deinitialize
* @retval 0 on success error value otherwise
*/
INT fx_stm32_sd_deinit(UINT instance)
{
  INT ret = 0;
 /* USER CODE BEGIN PRE_FX_SD_DEINIT */
  UNUSED(instance);
  /* USER CODE END PRE_FX_SD_DEINIT */

  if(BSP_SD_DeInit(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  /* USER CODE BEGIN POST_FX_SD_DEINIT */

  /* USER CODE END POST_FX_SD_DEINIT */

  return ret;
}


/**
* @brief Check the SD IP status.
* @param UINT instance SD instance to check
* @retval 0 when ready 1 when busy
*/
INT fx_stm32_sd_get_status(UINT instance)
{
  INT ret = 0;

  /* USER CODE BEGIN PRE_GET_STATUS */
  UNUSED(instance);
  /* USER CODE END PRE_GET_STATUS */

  if (BSP_SD_GetCardState(instance) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  /* USER CODE BEGIN POST_GET_STATUS */

  /* USER CODE END POST_GET_STATUS */

  return ret;
}

/**
* @brief Read Data from the SD device into a buffer.
* @param UINT instance SD IP instance to read from.
* @param UINT *buffer buffer into which the data is to be read.
* @param UINT start_block the first block to start reading from.
* @param UINT total_blocks total number of blocks to read.
* @retval 0 on success error code otherwise
*/
INT fx_stm32_sd_read_blocks(UINT instance, UINT *buffer, UINT start_block, UINT total_blocks)
{
  INT ret = 0;

  /* USER CODE BEGIN PRE_READ_BLOCKS */
  UNUSED(instance);
  /* USER CODE END PRE_READ_BLOCKS */

 if (BSP_SD_ReadBlocks_DMA(instance, (uint32_t *)buffer, start_block, total_blocks) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  /* USER CODE BEGIN POST_READ_BLOCKS */

  /* USER CODE END POST_READ_BLOCKS */

  return ret;
}


/**
* @brief Write data buffer into the SD device.
* @param UINT instance SD IP instance to write into.
* @param UINT *buffer buffer to write into the SD device.
* @param UINT start_block the first block to start writing into.
* @param UINT total_blocks total number of blocks to write.
* @retval 0 on success error code otherwise
*/
INT fx_stm32_sd_write_blocks(UINT instance, UINT *buffer, UINT start_block, UINT total_blocks)
{
  INT ret = 0;

  /* USER CODE BEGIN PRE_WRITE_BLOCKS */
  UNUSED(instance);
  /* USER CODE END PRE_WRITE_BLOCKS */
 if (BSP_SD_WriteBlocks_DMA(instance, (uint32_t *)buffer, start_block, total_blocks) != BSP_ERROR_NONE)
  {
    ret = 1;
  }

  /* USER CODE BEGIN POST_WRITE_BLOCKS */

  /* USER CODE END POST_WRITE_BLOCKS */

  return ret;
}

/**
* @brief SD DMA Write Transfer completed callbacks
* @param Instance the sd instance
* @retval None
*/
void BSP_SD_WriteCpltCallback(uint32_t instance)
{
  /* USER CODE BEGIN PRE_TX_CMPLT */

  /* USER CODE END PRE_TX_CMPLT */

  tx_semaphore_put(&transfer_semaphore);

  /* USER CODE BEGIN POST_TX_CMPLT */

  /* USER CODE END POST_TX_CMPLT */
}

/**
* @brief SD DMA Read Transfer completed callbacks
* @param Instance the sd instance
* @retval None
*/
void BSP_SD_ReadCpltCallback(uint32_t instance)
{
  /* USER CODE BEGIN PRE_RX_CMPLT */

  /* USER CODE END PRE_RX_CMPLT */

  tx_semaphore_put(&transfer_semaphore);

  /* USER CODE BEGIN POST_RX_CMPLT */

  /* USER CODE END POST_RX_CMPLT */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
