/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "string.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "stdio.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif
#include "ethernetif.h"
#include "main.h"
#include "app_ethernet.h"
#include "udp_echoclient.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
extern ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT];   /* Ethernet Tx DMA Descriptors */

/* ETH_TxPacketConfig_t TxConfig; */
extern ETH_TxPacketConfigTypeDef TxConfig;

/* ETH_HandleTypeDef heth; */
extern ETH_HandleTypeDef EthHandle;
struct netif gnetif;

EXTI_HandleTypeDef hexti;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
static void Netif_Config(void);

void SystemClock_Config(void);
static void MX_UART4_Init(void);
void BSP_PB_Callback(Button_TypeDef Button);
static void MX_ETH_Init(void);


/* USER CODE BEGIN PFP */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar (void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE * f)
#endif /* __GNUC__ */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Success_Handler(void)
{
   BSP_LED_Off(LED_RED);
   while(1)
   {
     BSP_LED_Toggle(LED_GREEN);
     HAL_Delay(200);
   }
}

/**
  * @brief  Setup the network interface
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif /* LWIP_DHCP */

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /*  Registers the default network interface */
  netif_set_default(&gnetif);

  ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif /* LWIP_NETIF_LINK_CALLBACK */
}

/**
  * @brief  This function handles EXTI_LINE_13 interrupt request.
  * @retval None
  */
void EXTI14_IRQHandler(void)
{
	BSP_PB_USER_EXTI_LINE_14_IRQHandler();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

#if (USE_STPMIC1x) && !defined(USE_DDR)
  /* Configure PMIC */
  BSP_PMIC_Init();
  BSP_PMIC_InitRegulators();
#endif

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */
  /* Initialize all configured peripherals */

  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize Peripherals */
  MX_UART4_Init();
  MX_ETH_Init();

  /* Initialize the LwIP stack */
  lwip_init();

  /* Configure the Network interface */
  Netif_Config();

  /* UDP client connect */
  udp_echoclient_connect();

  /* Infinite loop */
  while (1)
  {
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);

	/* Handle timeouts */
    sys_check_timeouts();

#if LWIP_NETIF_LINK_CALLBACK
    Ethernet_Link_Periodic_Handle(&gnetif);
#endif

#if LWIP_DHCP
    DHCP_Periodic_Handle(&gnetif);
#endif
  }
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
  RCC_ClkInitStructure.APB6_Div = RCC_APB6_DIV1;

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
#endif /* !USE_DDR */
}

/**
 * @brief ETH Initialization Function
 * @param None
 * @retval None
 */
void MX_ETH_Init(void)
{
  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

  static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  EthHandle.Instance = ETH;
  MACAddr[0] = ETH_MAC_ADDR0; //0x00;
  MACAddr[1] = ETH_MAC_ADDR1; //0x80;
  MACAddr[2] = ETH_MAC_ADDR2; //0xE1;
  MACAddr[3] = ETH_MAC_ADDR3; //0x00;
  MACAddr[4] = ETH_MAC_ADDR4; //0x00;
  MACAddr[5] = ETH_MAC_ADDR5; //0x00;
  EthHandle.Init.MACAddr = &MACAddr[0];
  EthHandle.Init.MediaInterface = HAL_ETH_RMII_MODE;
  EthHandle.Init.TxDesc = DMATxDscrTab;
  EthHandle.Init.RxDesc = DMARxDscrTab;
  EthHandle.Init.RxBuffLen = 1536;
  EthHandle.Init.ClockSelection = HAL_ETH1_REF_CLK_RX_CLK_PIN;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&EthHandle) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfigTypeDef));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_8;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */
}
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

int __write(int file, char *buf, int size)
{
  (void)file;
  (void)size;
  HAL_UART_Transmit(&huart4, (uint8_t *)buf, 1, 0xFFFF);
  return 1;
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART4 and Loop until the end of transmission */
  __write(1, (char *)&ch, 1);
  return ch;
}


GETCHAR_PROTOTYPE
{
  uint8_t ch = 0;
  /* Clear the Overrun flag just before receiving the first character */
  __HAL_UART_CLEAR_OREFLAG(&huart4);

  HAL_UART_Receive(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
  HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    /* send udp echo */
    udp_echoclient_send();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();

  BSP_LED_Off(LED_GREEN);

  while (1)
  {
    BSP_LED_Toggle(LED_RED);
    //tx_thread_sleep(20);
    HAL_Delay(50);
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



