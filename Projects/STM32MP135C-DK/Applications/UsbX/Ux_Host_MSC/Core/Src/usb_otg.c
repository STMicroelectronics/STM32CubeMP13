/**
  ******************************************************************************
  * @file    usb_otg.c
  * @brief   This file provides code for the configuration
  *          of the USB_OTG instances.
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
#include "usb_otg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

HCD_HandleTypeDef hhcd_USB_OTG_HS;

/* USB_OTG_HS init function */

void MX_USB_OTG_HS_HCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_HS_Init 0 */

  /* USER CODE END USB_OTG_HS_Init 0 */

  /* USER CODE BEGIN USB_OTG_HS_Init 1 */

  /* USER CODE END USB_OTG_HS_Init 1 */
  hhcd_USB_OTG_HS.Instance = USB_OTG_HS;
  hhcd_USB_OTG_HS.Init.Host_channels = 16;
  hhcd_USB_OTG_HS.Init.speed = HCD_SPEED_HIGH;
  hhcd_USB_OTG_HS.Init.dma_enable = DISABLE;
  hhcd_USB_OTG_HS.Init.phy_itface = USB_OTG_HS_EMBEDDED_PHY;
  hhcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
  hhcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
  hhcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
  hhcd_USB_OTG_HS.Init.lpm_enable = DISABLE;
  if (HAL_HCD_Init(&hhcd_USB_OTG_HS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_HS_Init 2 */

  /* USER CODE END USB_OTG_HS_Init 2 */

}

void HAL_HCD_MspInit(HCD_HandleTypeDef* hcdHandle)
{
  /* USER CODE BEGIN USB_MspInit 1 */
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(hcdHandle->Instance==USB_OTG_HS)
  {
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USBPHY;
	PeriphClkInit.UsbphyClockSelection = RCC_USBPHYCLKSOURCE_HSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
	  Error_Handler();
	}

    /* Enable USBPHY Clock */
    __HAL_RCC_USBPHY_CLK_ENABLE();

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USBO;
	PeriphClkInit.UsboClockSelection = RCC_USBOCLKSOURCE_PHY;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
	  Error_Handler();
	}

    /* Peripheral clock enable */
    __HAL_RCC_USBO_CLK_ENABLE();
    __HAL_RCC_USBO_FORCE_RESET();
    __HAL_RCC_USBO_RELEASE_RESET();

    /* OTG interrupt init */
    IRQ_SetPriority(OTG_IRQn, 6);
    IRQ_Enable(OTG_IRQn);
  }

  /* USER CODE END USB_MspInit 1 */
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hcdHandle)
{

  if(hcdHandle->Instance==USB_OTG_HS)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USBO_CLK_DISABLE();
    __HAL_RCC_USBO_FORCE_RESET();

    /* Peripheral interrupt Deinit*/
    IRQ_Disable(OTG_IRQn);
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
