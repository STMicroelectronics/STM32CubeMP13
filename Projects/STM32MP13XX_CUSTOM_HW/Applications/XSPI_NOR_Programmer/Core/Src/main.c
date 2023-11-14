/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @brief   This utility is used to copy A7 signed binary in XSPI-NOR.
 ******************************************************************************
 *
 * @attention
 *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32MP13xx_HAL_Examples
 * @{
 */

/** @addtogroup ... _Example
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USE_FULL_ASSERT 1

/**
 * Binary files to load in XSPI NOR are stored in folder C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13:
 * (extension .bin or .stm32)
 */
#define FILE_PATH_FSBLA "C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13\\FSBLA_SNOR_A7_Signed.bin"
//#define FILE_PATH_CUBEFW "C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13\\Fx_uSD_File_Edit_A7_Signed.bin"
#define FILE_PATH_CUBEFW "C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13\\MP13_BSP_TemplatesA7_CUSTOM_Signed.bin"
//#define FILE_PATH_CUBEFW "C:\\Temp\\XSPI_NOR_PROGRAMMER_STM32MP13\\CubeExample1_Signed.stm32"

/**
 *  For FSBLA  : binary will be copied at XSPI-NOR @0x0.
 *  This address can not be changed (used by bootrom to boot from NOR).
 */
#define FLASH_ADDRESS_SNOR 0x0

/**
 * For CubeFW : binary will be copied at XSPI-NOR @0x30000 : this address can be changed.
 * But need to align this address with FSBLA_SNOR application.
 * Indeed this address is used by FSBLA_SNOR to know where FWCube is located before copy it into DDR
 */
#define FLASH_ADDRESS_SNOR_CUBE 0x30000

#if defined (USE_STM32MP13XX_CUSTOM_HW)
  #define XSPI_NOR_SECTOR_SIZE MX25L512_SECTOR_SIZE
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t buffer_load[XSPI_NOR_SECTOR_SIZE];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

static void MX_GPIO_Init(void);

void Select_Load_Option(void);

HAL_StatusTypeDef load_firmware(char *file, uint32_t StartAddress);

void BoardCfg(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{

  HAL_Init();

/* Configure the system clock */
  SystemClock_Config();

/* Configure the Board */
  BoardCfg();

/* Initialize Display destination */
/* Activate semihosting to get printf traces output from console tab */
  initialise_monitor_handles();

  printf("\n\r Start XSPI-NOR Programmer application tool\n\r");
  MX_GPIO_Init();

  Init_XSPI();
  printf("XSPI initialized \n\r");

  /* Binary loaded depending on the user selection */
  Select_Load_Option();

  while (1) {
  };

  return 0;
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = HSI
 *            SYSCLK(Hz)                     = 48000000
 *            HCLK(Hz)                       = 48000000
 *            APB1 Prescaler                 = 1
 *            APB2 Prescaler                 = 1
 * @param  None
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
  RCC_OscInitStructure.PLL2.PLLM = 2;
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
#endif /* USE_DDR */
}

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

/* No STPMIC on STM32MP13XX_CUSTOM_HW BOARD*/
#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* Configure LEDs */
#if defined(USE_STM32MP13XX_CUSTOM_HW)
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);
#endif

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
  * @brief  Select Binary to load (FSBLA_SNOR or CubeExample)
  * @param  None
  * @retval None
  */

void Select_Load_Option(void)
{
  uint8_t stop_test = 0;

  char menu_select[] = "0";
  char menu1[] = "1";
  char menu2[] = "2";
  char menu3[] = "Q";

  do
  {
    printf("Select the option 1,2 or Q: \r\n");
    printf("    - 1 : Load the FSBLA_SNOR firmware \r\n");
    printf("    - 2 : Load a CubeExample firmware \r\n");
    printf("    - Q : Quit the application \r\n");

    scanf("%s",menu_select);    /*semihosting*/
    printf("        ** your choice : %s \r\n", menu_select);

/* CASE '1' selected */
    if(strncmp(menu_select, menu1, 1U) == 0)
    {
      printf("        ** Start FSBLA_SNOR firmware loading\r\n");
#if defined(USE_STM32MP13XX_CUSTOM_HW)
      BSP_LED_Off(LED_BLUE);
#endif
      if (load_firmware(FILE_PATH_FSBLA, FLASH_ADDRESS_SNOR) != HAL_OK)
      {
        Error_Handler();
      }
#if defined(USE_STM32MP13XX_CUSTOM_HW)
      BSP_LED_On(LED_BLUE);
#endif
      printf("        ** FSBLA_SNOR firmware is loaded!\r\n");
    }

/* CASE '2' selected */
    else if(strncmp(menu_select, menu2, 1U) == 0)
    {
      printf("        ** Start CubeExample firmware loading\r\n");
#if defined(USE_STM32MP13XX_CUSTOM_HW)
      BSP_LED_Off(LED_RED);
#endif
      if (load_firmware(FILE_PATH_CUBEFW, FLASH_ADDRESS_SNOR_CUBE) != HAL_OK)
      {
        Error_Handler();
      }
#if defined(USE_STM32MP13XX_CUSTOM_HW)
      BSP_LED_On(LED_RED);
#endif
      printf("        ** CubeExample firmware is loaded!\r\n");
    }

/* CASE 'Q' selected */
    else if(strncmp(menu_select, menu3, 1U) == 0)
    {
      printf("        >> Quit the application. ** \n\r");
      stop_test=1;
    }

/* CASE wrong selection */
    else
    {
      printf("        !!!! INVALID CHOICE. Please Select 1, 2 or Q \r\n");
    }

  } while (stop_test != 1);
}

/**
  * @brief  Load binary from your PC into XSPI NOR
  * @param  file : binary path to load
  * @param  StartAddress : XSPI NOR Address to load the binary
  * @retval HAL status
  */
HAL_StatusTypeDef load_firmware(char *file, uint32_t StartAddress)
{
  HAL_StatusTypeDef status = HAL_OK;
  volatile uint32_t nb_fread;
  volatile uint32_t nbblocks;

  printf("          . Binary file to load:%s \n\r", file);
  FILE *fd;
  fd = fopen(file, "rb");
  if (!fd)
  {
    printf("          !! Can't open file %s\n", file);
    status = HAL_ERROR;
  }
  else
  {
    printf("          . Load at @NOR :0x%lx \n\r", StartAddress);
    nbblocks = 0;
    while (nb_fread != 0)
    {
      nb_fread = fread(&buffer_load, XSPI_NOR_SECTOR_SIZE, 1 , fd);
      if (SectorErase_XSPI((StartAddress + nbblocks * XSPI_NOR_SECTOR_SIZE), (StartAddress + (nbblocks + 1) * XSPI_NOR_SECTOR_SIZE - 1)) != 0x1)
      {
        status = HAL_ERROR;
      }
      if (Write_XSPI((uint8_t*) buffer_load, (StartAddress + nbblocks * XSPI_NOR_SECTOR_SIZE), XSPI_NOR_SECTOR_SIZE) != 0x1)
      {
        status = HAL_ERROR;
      }
      nbblocks++;
      printf("            . load sector %ld (sector of 4kBytes) \n",nbblocks);
    }
  }
  return status;
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
  /* Error if LED_RED is slowly blinking (1/2 sec. period) */

  while(1)
  {
#if defined(USE_STM32MP13XX_CUSTOM_HW)
	  BSP_LED_Toggle(LED_RED);
      HAL_Delay(500);
#endif
  }
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  Error_Handler();
}
#endif




/**
 * @}
 */

/**
 * @}
 */

