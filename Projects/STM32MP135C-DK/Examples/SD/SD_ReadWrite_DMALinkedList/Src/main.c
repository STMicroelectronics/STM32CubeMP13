/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    SD/SD_ReadWrite_DMALinkedList/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use SD through
  *          the STM32MP13xx HAL API.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_SIZE              ((uint32_t)0x01800000U) /* Data Size 24MB */

#define BUFFER_SIZE            ((uint32_t)0x00000400U) /* 1KB */
#define BUFFER_WORD_SIZE       (BUFFER_SIZE>>2)        /* Buffer size in Word */
#define NB_BLOCK_BUFFER        BUFFER_SIZE / BLOCKSIZE /* Number of Block (512B) by Buffer */
#define BLOCK_WORD_SIZE        (BLOCKSIZE>>2) /* Block size in words */


#define DATA_ADDRESS           ((uint32_t)0x00000000U) /* SD Card Address to write/read data */
#define DATA_PATTERN0          ((uint32_t)0x01000000U) /* Data pattern for buffer0*/
#define DATA_PATTERN1          ((uint32_t)0x02000000U) /* Data pattern for buffer1 */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
SD_HandleTypeDef hsd1;

/* USER CODE BEGIN PV */
uint32_t Buffer0[BUFFER_WORD_SIZE];
uint32_t Buffer1[BUFFER_WORD_SIZE];


static SDMMC_DMALinkNodeTypeDef pLinkNode[4];
static SDMMC_DMALinkedListTypeDef Read_LinkedList;
static SDMMC_DMALinkedListTypeDef Write_LinkedList;
SD_DMALinkNodeConfTypeDef LinkNodeConf;
__IO uint8_t TxLnkLstBufCplt, RxLnkLstBufCplt,TxNodeBufCplt,RxNodeBufCplt;

uint32_t Node,RBuff0,RBuff1,WBuff0,WBuff1;
uint32_t ReadError = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
/* USER CODE BEGIN PFP */
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset);
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint32_t BufferLength, uint32_t Offset);
void Dynamic_buffer_fill(void);
void Dynamic_buffer_check(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* Configure the system clock */
  /* Configure the system clock */
  SystemClock_Config();
 
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);

  /* Set SYSRAM in Non Secure */
  __HAL_RCC_TZC_CLK_ENABLE();
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  /*##-1- Initialize LEDs mounted on STM32MP13XX-DISCO board #####################*/
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /*##-2- Initialize SD instance #####################*/
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDMMC1_SD_Init();
  
  /* USER CODE BEGIN 2 */
  /* Try to switch to High Speed Mode , if supported by the card */
  HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH);

  if(HAL_SD_Erase(&hsd1,DATA_ADDRESS,DATA_SIZE/BLOCKSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Wait for the Card Transfer status */
  while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
  {
  }
  /* Fill the buffer to send */
  Fill_Buffer(Buffer0, BUFFER_WORD_SIZE, DATA_PATTERN0);
  Fill_Buffer(Buffer1, BUFFER_WORD_SIZE, DATA_PATTERN1);
  /*  Write steps */
  /*Node 0 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer0;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Write_LinkedList, NULL, &pLinkNode[0] );

  /*Node 1 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer1;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Write_LinkedList ,&pLinkNode[0], &pLinkNode[1] );


  /*Enable Circular Linked list mode */
  HAL_SDEx_DMALinkedList_EnableCircularMode(&Write_LinkedList );

  TxLnkLstBufCplt=0;
  TxNodeBufCplt=0;
  BSP_LED_On(LED_BLUE);
  /* Write 24MB  on uSD card using DMA transfer */

  HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Write_LinkedList, DATA_ADDRESS, DATA_SIZE/BLOCKSIZE);

  /* Dynamic buffer filling Process*/
  Dynamic_buffer_fill();

  /* Remove write nodes*/
  HAL_SDEx_DMALinkedList_RemoveNode(&Write_LinkedList,&pLinkNode[1] );
  HAL_SDEx_DMALinkedList_RemoveNode(&Write_LinkedList,&pLinkNode[0] );

  /* Wait for the Card Transfer status */
  while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
  {
  }

  /*  Read steps */
  /*Node 0 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer0;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Read_LinkedList, NULL, &pLinkNode[0] );

  /*Node 1 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer1;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Read_LinkedList, &pLinkNode[0], &pLinkNode[1]);

  /*Enable Circular Linked list mode */
  HAL_SDEx_DMALinkedList_EnableCircularMode(&Read_LinkedList );

  /* Read 24MB  from uSD card using DMA transfer */

  HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Read_LinkedList,DATA_ADDRESS,DATA_SIZE/BLOCKSIZE);

  /* Dynamic buffer Check Process*/
  Dynamic_buffer_check();

  BSP_LED_Off(LED_BLUE);

  /* Check the correctness of written dada */

  if (ReadError == 0)
  {

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(200);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  }
  else
  {
    while(1)
    {
      BSP_LED_Toggle(LED_RED);
      HAL_Delay(200);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if !defined(USE_DDR)
  HAL_RCC_DeInit();
  RCC_ClkInitTypeDef RCC_ClkInitStructure;
  RCC_OscInitTypeDef RCC_OscInitStructure;

  /* Enable all available oscillators*/
  RCC_OscInitStructure.OscillatorType = (RCC_OSCILLATORTYPE_HSI |
                                         RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_CSI |
                                         RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE);

  RCC_OscInitStructure.HSIState = RCC_HSI_ON;
  RCC_OscInitStructure.HSEState = RCC_HSE_ON;
  RCC_OscInitStructure.LSEState = RCC_LSE_ON;
  RCC_OscInitStructure.LSIState = RCC_LSI_ON;
  RCC_OscInitStructure.CSIState = RCC_CSI_ON;

  RCC_OscInitStructure.HSICalibrationValue = 0x00; //Default reset value
  RCC_OscInitStructure.CSICalibrationValue = 0x10; //Default reset value
  RCC_OscInitStructure.HSIDivValue = RCC_HSI_DIV1; //Default value

  /* PLL configuration */
  RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStructure.PLL.PLLM = 3;
  RCC_OscInitStructure.PLL.PLLN = 81;
  RCC_OscInitStructure.PLL.PLLP = 1;
  RCC_OscInitStructure.PLL.PLLQ = 2;
  RCC_OscInitStructure.PLL.PLLR = 2;
  RCC_OscInitStructure.PLL.PLLFRACV = 0x800;
  RCC_OscInitStructure.PLL.PLLMODE = RCC_PLL_FRACTIONAL;

  RCC_OscInitStructure.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL2.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStructure.PLL2.PLLM = 3;
  RCC_OscInitStructure.PLL2.PLLN = 66;
  RCC_OscInitStructure.PLL2.PLLP = 2;
  RCC_OscInitStructure.PLL2.PLLQ = 2;
  RCC_OscInitStructure.PLL2.PLLR = 1;
  RCC_OscInitStructure.PLL2.PLLFRACV = 0x1400;
  RCC_OscInitStructure.PLL2.PLLMODE = RCC_PLL_FRACTIONAL;

  RCC_OscInitStructure.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL3.PLLSource = RCC_PLL3SOURCE_HSE;
  RCC_OscInitStructure.PLL3.PLLM = 2;
  RCC_OscInitStructure.PLL3.PLLN = 34;
  RCC_OscInitStructure.PLL3.PLLP = 2;
  RCC_OscInitStructure.PLL3.PLLQ = 17;
  RCC_OscInitStructure.PLL3.PLLR = 2;
  RCC_OscInitStructure.PLL3.PLLRGE = RCC_PLL3IFRANGE_1;
  RCC_OscInitStructure.PLL3.PLLFRACV = 0x1a04;
  RCC_OscInitStructure.PLL3.PLLMODE = RCC_PLL_FRACTIONAL;

  RCC_OscInitStructure.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStructure.PLL4.PLLSource = RCC_PLL4SOURCE_HSE;
  RCC_OscInitStructure.PLL4.PLLM = 2;
  RCC_OscInitStructure.PLL4.PLLN = 50;
  RCC_OscInitStructure.PLL4.PLLP = 12;
  RCC_OscInitStructure.PLL4.PLLQ = 60;
  RCC_OscInitStructure.PLL4.PLLR = 6;
  RCC_OscInitStructure.PLL4.PLLRGE = RCC_PLL4IFRANGE_1;
  RCC_OscInitStructure.PLL4.PLLFRACV = 0;
  RCC_OscInitStructure.PLL4.PLLMODE = RCC_PLL_INTEGER;

  /* Enable access to RTC and backup registers */
  SET_BIT(PWR->CR1, PWR_CR1_DBP);
  /* Configure LSEDRIVE value */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK) {
    /* HAL RCC configuration error */
    Error_Handler();
    }

  /* Select PLLx as MPU, AXI and MCU clock sources */
  RCC_ClkInitStructure.ClockType = (RCC_CLOCKTYPE_MPU   | RCC_CLOCKTYPE_ACLK  |
                                    RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK4 |
                                    RCC_CLOCKTYPE_PCLK5 | RCC_CLOCKTYPE_PCLK1 |
                                    RCC_CLOCKTYPE_PCLK6 |
                                    RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);

  RCC_ClkInitStructure.MPUInit.MPU_Clock = RCC_MPUSOURCE_PLL1;
  RCC_ClkInitStructure.MPUInit.MPU_Div = RCC_MPU_DIV2;
  RCC_ClkInitStructure.AXISSInit.AXI_Clock = RCC_AXISSOURCE_PLL2;
  RCC_ClkInitStructure.AXISSInit.AXI_Div = RCC_AXI_DIV1;
  RCC_ClkInitStructure.MLAHBInit.MLAHB_Clock = RCC_MLAHBSSOURCE_PLL3;
  RCC_ClkInitStructure.MLAHBInit.MLAHB_Div = RCC_MLAHB_DIV1;
  RCC_ClkInitStructure.APB1_Div = RCC_APB1_DIV2;
  RCC_ClkInitStructure.APB2_Div = RCC_APB2_DIV2;
  RCC_ClkInitStructure.APB3_Div = RCC_APB3_DIV2;
  RCC_ClkInitStructure.APB4_Div = RCC_APB4_DIV2;
  RCC_ClkInitStructure.APB5_Div = RCC_APB5_DIV4;
  RCC_ClkInitStructure.APB6_Div = RCC_APB6_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure) != HAL_OK) {
    /* HAL RCC configuration error */
    Error_Handler();
    }

/*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
  (GPIO, SPI, FMC, XSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
  The I/O Compensation Cell activation  procedure requires :
  - The activation of the CSI clock
  - The activation of the SYSCFG clock
  - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR

  To do this please uncomment the following code
  */

  /*
  __HAL_RCC_CSI_ENABLE() ;

  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  HAL_EnableCompensationCell();
*/
#endif
}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_FALLING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = SDMMC_NSPEED_CLK_DIV;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/**
  * @brief Dynamic buffer fill
  * @param None
  * @retval None
  */
void Dynamic_buffer_fill(void)
{

  while ((WBuff0+WBuff1) <= (DATA_SIZE/BUFFER_SIZE))
  {
    /* Wait for Node transfer completed or the whole transfer completed */
    while ((TxNodeBufCplt == 0) && (TxLnkLstBufCplt ==0));

    TxNodeBufCplt = 0;

    if(Node == 0)
    {
      WBuff0++;
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]);

      Fill_Buffer(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (WBuff0 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]);

      /*Set Next Node*/
      Node = 1;
    }
    else if(Node == 1)
    {
      WBuff1++;
      /*Start processing of Node1 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[1]);

      Fill_Buffer(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (WBuff1 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[1]);

      /*Set Next Node*/
      Node = 0;
    }
  }
  TxLnkLstBufCplt = 0;
  Node = 0;
}



/**
  * @brief Dynamic buffer check
  * @param None
  * @retval None
  */
void Dynamic_buffer_check(void)
{

  while ((RBuff0+RBuff1) < (DATA_SIZE/BUFFER_SIZE))
  {
    /* Wait for Node transfer completed or the whole transfer completed */
    while ((RxNodeBufCplt == 0) && (RxLnkLstBufCplt ==0));

    RxNodeBufCplt = 0;

    if(Node == 0)
    {
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]);

      ReadError += Buffercmp(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (RBuff0 * (uint32_t)0x00010000));

      /*Finish processing of Node0 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]);

      RBuff0++;
      /*Next Node*/
      Node = 1;
    }
    else if(Node == 1)
    {
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[1]);

      ReadError += Buffercmp(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (RBuff1 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[1]);

      RBuff1++;
      /*Next Node*/
      Node = 0;
    }
  }
  RxLnkLstBufCplt = 0;
  Node = 0;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd)
{
  RxLnkLstBufCplt = 1;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd)
{
  TxLnkLstBufCplt = 1;
}

/**
  * @brief Read DMA Linked list node Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
  RxNodeBufCplt = 1;
}

/**
  * @brief Read DMA Linked list node Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
  TxNodeBufCplt = 1;
}

/**
  * @brief SD error callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}



/**
  * @brief  Fills the goal 32-bit length buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: length of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset)
{
  uint16_t IndexTmp;

  /* Put in global buffer same values */
  for ( IndexTmp = 0; IndexTmp < BufferLength; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }

}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint32_t counter = 0;
  while(BufferLength--)
  {
    if(pBuffer[counter] != counter + Offset)
    {
      return HAL_ERROR;
    }
    counter++;
  }
  return HAL_OK;
}
/* USER CODE END 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(200);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

 /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
