/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FSBLA_SNOR/Src/main.c
  * @author  MCD Application Team
  * @brief   main program
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
DDR_InitTypeDef hddr;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef struct
{
  uint32_t  id;
  char     name[40];
  uint32_t  offset;
} OPENBL_Flashlayout_Storage_TypeDef;

/* DDR define */
#define DDR_PATTERN 1234

/* DDR_MEM_ADD = By default base address of DRAM (DDR) over AXI  = DRAM_MEM_BASE */
/* Possible to select a specific address in DDR, but the cube example has to be linked with the same address */
#define DDR_MEM_ADD DRAM_MEM_BASE

/* XSPI-NOR define */
#if defined (USE_STM32MP13XX_CUSTOM_HW)
/* SECTOR size for MX25L512 = 4kBytes */
#define XSPI_NOR_SECTOR_SIZE MX25L512_SECTOR_SIZE
#endif
/**
 * Cube example : binary has been copied by XSPI_NOR_Programmer at XSPI-NOR @0x30000.
 * This address can be changed, but need to align this address with XSPI_NOR_Programmer implementation.
 */
#define FLASH_ADDRESS_SNOR_CUBE 0x30000

#define GPT_APPLICATION_PHASE_ID	 5
#define MAX_ENTRIES_IN_GPT_TABLE	 10
#define GPT_TABLE_OFFSET			 0x30000U
#define GPT_TABLE_SIZE				 512U
/* ST_HEADER define */
#define ST_HEADER_SIZE               512U  /* ST HEADER Size of CubeExample binary for the signed binary*/
#define IMAGE_LENGTH_POSITION         76U  /* First hexadecimal number position (aRxBuffer position) of cube example image length */
#define NB_HEXA_NB_FOR_IMAGE_LENGTH    4U  /* Number of hexadecimal number representing the cube example image length */
#define LITLE_ENDIAN 1 /* Cube example header endianness */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

void (*p_AppEntryPoint)(void); /* Entry point of the application*/

__IO uint8_t RxCplt, TxCplt;
__IO uint8_t CmdCplt, StatusMatch;

/* MAGIC Number defined in ST HEADER Section */
uint32_t MAGIC_NB[4] = {0x53,0x54,0x4D,0x32};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
void BoardCfg(void);
HAL_StatusTypeDef Init_DDR(void);
uint32_t GetApplicationOffsetFromGPTTable(uint32_t AddressCubeSNOR);
uint32_t Get_CubeSize(uint32_t AddressCubeSNOR);
HAL_StatusTypeDef Copy_CubeExampleToDDR(uint32_t AddressCubeSNOR, uint32_t nbSector);
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  uint32_t nbSector_CubeExample;
  uint32_t sizeCubExample = 0;
  uint32_t applicationOffset = 0;

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();

  /* USER CODE BEGIN SysInit */
  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the Board */
  BoardCfg();
  /* USER CODE END SysInit */

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Initialize DDR */
  if(Init_DDR() != HAL_OK)
  {
    Error_Handler();
  }
#if defined(USE_STM32MP13XX_CUSTOM_HW)
  /* LED_BLUE ON */
  BSP_LED_On(LED_BLUE);
#endif

  /* USER CODE BEGIN 2 */

  MX_GPIO_Init();

  /*##- Initialize XSPI in QuadSPI mode ####################*/
  Init_XSPI();

  applicationOffset = GetApplicationOffsetFromGPTTable(GPT_TABLE_OFFSET);

  /*##- Get image length from HEADER section ####################*/
  sizeCubExample = Get_CubeSize(applicationOffset);

  /*##- Convert image size to number of sector ####################*/
  nbSector_CubeExample = sizeCubExample / XSPI_NOR_SECTOR_SIZE;

  /*##- Copy CubeExample from XSPI-NOR into DDR ####################*/
  if (Copy_CubeExampleToDDR(applicationOffset, nbSector_CubeExample) != HAL_OK)
  {
    Error_Handler();
  }
#if defined(USE_STM32MP13XX_CUSTOM_HW)
  /* LED_BLUE OFF */
  BSP_LED_Off(LED_BLUE);
#endif

  /* Reset_handler of the Application to jump on CubeExample*/
  p_AppEntryPoint = (void *)(DDR_MEM_ADD);
  p_AppEntryPoint();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if defined(USE_STM32MP13XX_CUSTOM_HW)
    /* Toggle LED_BLUE */
    BSP_LED_Toggle(LED_BLUE);
    HAL_Delay(1000);
#endif
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

/* USER CODE BEGIN 4 */

/**
  * @brief  Board Specific Configuration
  * @param  None
  * @retval None
  */
void BoardCfg(void)
{
  __HAL_RCC_TZC_CLK_ENABLE();
  __HAL_RCC_ETZPC_CLK_ENABLE();
  /* Set SYSRAM in Non Secure */
  LL_ETZPC_SetSecureSysRamSize(ETZPC, 0);

  /* Set all IPs in Non Secure */
  LL_ETZPC_Set_All_PeriphProtection(ETZPC, LL_ETZPC_PERIPH_PROTECTION_READ_WRITE_NONSECURE);

/* No STPMIC on STM32MP13XX_VALID BOARD*/
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* Configure LEDs */
#if defined(USE_STM32MP13XX_CUSTOM_HW)
/* Configure LED_BLUE and LED_RED */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  BSP_LED_Off(LED_BLUE);
  BSP_LED_Off(LED_RED);
#endif

}

/**
  * @brief  Initialize DDR and check DDR access (read / Write)
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef Init_DDR(void)
{
  HAL_StatusTypeDef status = HAL_OK;

/* DRAM_MEM_BASE = Base address of DRAM (DDR) over AXI */
  uint32_t *pddr = (uint32_t*)(DDR_MEM_ADD);

/*##- Enable MCE ####################*/
  __HAL_RCC_MCE_CLK_ENABLE();
/*##- TZC configuration ####################*/
  __HAL_RCC_TZC_CLK_ENABLE();

/* Configure TZC to allow  DDR Region0 R/W  non secure for all IDs */
  TZC->GATE_KEEPER =0;
  TZC->REG_ID_ACCESSO=0xFFFFFFFF;  //Allow DDR Region0 R/W  non secure for all IDs
  TZC->REG_ATTRIBUTESO = DDR_MEM_ADD + 1;
  TZC->GATE_KEEPER |=1;  //Enable the access in secure Mode  // filter 0 request close

/*##- Init DDR then Check ####################*/
  hddr.wakeup_from_standby = false;
  hddr.self_refresh = false;
  hddr.zdata = 0;
  hddr.clear_bkp = false;
  if (HAL_DDR_Init(&hddr) != HAL_OK)
  {
    status = HAL_ERROR;
  }

/*##- Check DDR access (Write/Read) ####################*/
  *pddr=DDR_PATTERN;
  if (*pddr!=DDR_PATTERN)
  {
    status = HAL_ERROR;
  }

  return status;
}

uint32_t GetApplicationOffsetFromGPTTable(uint32_t AddressCubeSNOR)
{
  uint8_t buffer[GPT_TABLE_SIZE];
  OPENBL_Flashlayout_Storage_TypeDef (*FlashlayoutStorageStruct)[MAX_ENTRIES_IN_GPT_TABLE] = (OPENBL_Flashlayout_Storage_TypeDef (*)[MAX_ENTRIES_IN_GPT_TABLE])&buffer;
  uint32_t index = 0;
  uint32_t exampleOffset = 0;

  /*##- Read HEADER from CubeExample binary ####################*/
  if (Read_XSPI(buffer, AddressCubeSNOR, GPT_TABLE_SIZE) != 0x1)
  {
    Error_Handler();
  }
  for (index = 0; index < MAX_ENTRIES_IN_GPT_TABLE; index++)
  {
	  if (GPT_APPLICATION_PHASE_ID == (*FlashlayoutStorageStruct)[index].id)
	  {
		  exampleOffset = (*FlashlayoutStorageStruct)[index].offset;
		  break;
	  }
  }
  if (MAX_ENTRIES_IN_GPT_TABLE == index)
  {
	  Error_Handler();
  }

  return exampleOffset;
}

/**
  * @brief  Get CubeExample Size from HEADER section (binary stored in XSPI_NOR)
  * @param   AddressCubeSNOR : XSPI-NOR Address about CubeExample
  * @retval sizeCubeExample Size of cubeExample (without HEADER)
  */
uint32_t Get_CubeSize(uint32_t AddressCubeSNOR)
{
  uint32_t i = 0;
  uint32_t var = 0;
  volatile uint32_t sizeCubeExample = 0;
  uint32_t actualHexadecimalNbPosition;
  uint32_t bitShift;
  uint8_t buffer[ST_HEADER_SIZE];

  for (var = 0; var < ST_HEADER_SIZE; var++)
  {
  buffer[var] = (var & 0xFF);
  }
  /*##- Read HEADER from CubeExample binary ####################*/
  if (Read_XSPI(buffer, AddressCubeSNOR, ST_HEADER_SIZE) != 0x1)
  {
    Error_Handler();
  }
  /*##- Check Magic Number from HEADER ####################*/
  for (i=0; i<4; i++)
  {
    if (buffer[i] != MAGIC_NB[i])
    {
      /* If MAGIC_NB is not correct, the size won't be available in HEADER */
      /* Need to verify the HEADER*/
      Error_Handler();
    }
  }

  /*##- Search for image length in the ST HEADER section ####################*/

  for (int i = 1;i <= NB_HEXA_NB_FOR_IMAGE_LENGTH; i++)
  {
    actualHexadecimalNbPosition = NB_HEXA_NB_FOR_IMAGE_LENGTH-i;

    if(LITLE_ENDIAN == 1)
    {
      /*Little Endian : The most significant byte of the data is placed at the byte with the lowest address */
      bitShift = 8 * (NB_HEXA_NB_FOR_IMAGE_LENGTH - i);
    }else if (LITLE_ENDIAN == 0)
    {
      /* Big Endian : The least significant byte of the data is placed at the byte with the lowest address */
      bitShift = 8 * (i - 1);
    }
    sizeCubeExample |= (buffer[IMAGE_LENGTH_POSITION + actualHexadecimalNbPosition] << bitShift);
  }

  return sizeCubeExample;
}

/**
  * @brief  Copy CubeExample binary from XSPI-NOR into DDR
  * @param  AddressCubeSNOR : XSPI-NOR Address about CubeExample
  * @param  nbSector : nb of sector to copy ( size of sector = XSPI_NOR_SECTOR_SIZE )
  * @retval HAL status
  */
HAL_StatusTypeDef Copy_CubeExampleToDDR(uint32_t AddressCubeSNOR, uint32_t nbSector)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t i, var;
  uint8_t RxBuffer[XSPI_NOR_SECTOR_SIZE];
/* DRAM_MEM_BASE = Base address of DRAM (DDR) over AXI */
  uint8_t *p_bintocopy = (uint8_t*)(DDR_MEM_ADD);

  for (i = 0; i <= nbSector; i++)
  {
    if (Read_XSPI(RxBuffer, AddressCubeSNOR + ST_HEADER_SIZE + (XSPI_NOR_SECTOR_SIZE * i), XSPI_NOR_SECTOR_SIZE) != 0x1)
    {
      status = HAL_ERROR;
    }
    for (var = 0;var < XSPI_NOR_SECTOR_SIZE; var++)
    {
      *p_bintocopy = RxBuffer[var];
      p_bintocopy++; //=p_bintocopy+1;
    }
  }
  return status;
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

}

/**
  * @brief  Command completed callbacks.
  * @param  hxspi: XSPI handle
  * @retval None
  */
void HAL_XSPI_CmdCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  CmdCplt++;
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  hxspi: XSPI handle
  * @retval None
  */
void HAL_XSPI_RxCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  RxCplt++;
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  hxspi: XSPI handle
  * @retval None
  */
void HAL_XSPI_TxCpltCallback(XSPI_HandleTypeDef *hxspi)
{
  TxCplt++;
}

/**
  * @brief  Status Match callbacks
  * @param  hxspi: XSPI handle
  * @retval None
  */
void HAL_XSPI_StatusMatchCallback(XSPI_HandleTypeDef *hxspi)
{
  StatusMatch++;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  /* Infinite loop */
  while(1)
  {
#if defined(USE_STM32MP13XX_CUSTOM_HW)
    /* Toggle LED_RED on */
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(1000);
#endif
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

