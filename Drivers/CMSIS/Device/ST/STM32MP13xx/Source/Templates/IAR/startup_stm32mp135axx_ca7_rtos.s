/**
  ******************************************************************************
  * @file    startup_stm32mp135axx_ca7_rtos.s
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-A7 Device Peripheral Access Layer A7 Startup source file
  *          for GCC toolchain
  *   This file :
  *      - Sets up initial PC => Reset Handler
  *      - Puts all A7 cores except first one on "Wait for interrupt"
  *      - Configures cores
  *      - Sets up Exception vectors
  *      - Sets up stacks for all exception modes
  *      - Call SystemInit to initialize IRQs, caches, MMU, ...
  *
  ******************************************************************************
  * Copyright (c) 2009-2018 ARM Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024-2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  MODULE  ?cstartup

   /*SECTION ZI_DATA:CODE:NOROOT(2) */
   /*SECTION __BSS_END__:CODE:NOROOT(2) */

/*----------------------------------------------------------------------------
  Definitions
 *----------------------------------------------------------------------------*/
#define USR_MODE 0x10            /* User mode */
#define FIQ_MODE 0x11            /* Fast Interrupt Request mode */
#define IRQ_MODE 0x12            /* Interrupt Request mode */
#define SVC_MODE 0x13            /* Supervisor mode */
#define ABT_MODE 0x17            /* Abort mode */
#define UND_MODE 0x1B            /* Undefined Instruction mode */
#define SYS_MODE 0x1F            /* System mode */

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
     SECTION SVC_STACK:DATA:NOROOT(3)
     SECTION IRQ_STACK:DATA:NOROOT(3)
     SECTION FIQ_STACK:DATA:NOROOT(3)
     SECTION ABT_STACK:DATA:NOROOT(3)
     SECTION UND_STACK:DATA:NOROOT(3)
     SECTION CSTACK:DATA:NOROOT(3)

 /*----------------------------------------------------------------------------
  SoC External Interrupt Handler
 *----------------------------------------------------------------------------*/
   SECTION .text:CODE:NOROOT(4)

      PUBLIC  IRQ_Vector_Table
      DATA
IRQ_Vector_Table
         /* Software Generated Interrupts */
         DCD             SGI0_IRQHandler  ;                    /* 0 */
         DCD             SGI1_IRQHandler  ;
         DCD             SGI2_IRQHandler  ;
         DCD             SGI3_IRQHandler  ;
         DCD             SGI4_IRQHandler  ;
         DCD             SGI5_IRQHandler  ;
         DCD             SGI6_IRQHandler  ;
         DCD             SGI7_IRQHandler  ;
         DCD             SGI8_IRQHandler  ;
         DCD             SGI9_IRQHandler  ;
         DCD             SGI10_IRQHandler  ;                   /* 10 */
         DCD             SGI11_IRQHandler  ;
         DCD             SGI12_IRQHandler  ;
         DCD             SGI13_IRQHandler  ;
         DCD             SGI14_IRQHandler  ;
         DCD             SGI15_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;                /* 20 */
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
        /* Private Peripheral Interrupts */
         DCD             VirtualMaintenanceInterrupt_IRQHandler  ;
         DCD             HypervisorTimer_IRQHandler  ;
         DCD             VirtualTimer_IRQHandler  ;
         DCD             Legacy_nFIQ_IRQHandler  ;
         DCD             SecurePhysicalTimer_IRQHandler  ;
         DCD             NonSecurePhysicalTimer_IRQHandler  ;  /* 30 */
         DCD             Legacy_nIRQ_IRQHandler  ;
        /******  STM32 specific Interrupt Numbers **********************************************************************/
         DCD             RESERVED_IRQHandler  ;
         DCD             PVD_AVD_IRQHandler  ;
         DCD             TAMP_IRQHandler  ;
         DCD             RTC_WKUP_ALARM_IRQHandler  ;
         DCD             TZC_IT_IRQHandler  ;
         DCD             RCC_IRQHandler  ;
         DCD             EXTI0_IRQHandler  ;
         DCD             EXTI1_IRQHandler  ;
         DCD             EXTI2_IRQHandler  ;                   /* 40 */
         DCD             EXTI3_IRQHandler  ;
         DCD             EXTI4_IRQHandler  ;
         DCD             DMA1_Stream0_IRQHandler  ;
         DCD             DMA1_Stream1_IRQHandler  ;
         DCD             DMA1_Stream2_IRQHandler  ;
         DCD             DMA1_Stream3_IRQHandler  ;
         DCD             DMA1_Stream4_IRQHandler  ;
         DCD             DMA1_Stream5_IRQHandler  ;
         DCD             DMA1_Stream6_IRQHandler  ;
         DCD             ADC1_IRQHandler  ;                    /* 50 */
         DCD             ADC2_IRQHandler  ;
         DCD             FDCAN1_IT0_IRQHandler  ;
         DCD             FDCAN2_IT0_IRQHandler  ;
         DCD             FDCAN1_IT1_IRQHandler  ;
         DCD             FDCAN2_IT1_IRQHandler  ;
         DCD             EXTI5_IRQHandler  ;
         DCD             TIM1_BRK_IRQHandler  ;
         DCD             TIM1_UP_IRQHandler  ;
         DCD             TIM1_TRG_COM_IRQHandler  ;
         DCD             TIM1_CC_IRQHandler  ;                 /* 60 */
         DCD             TIM2_IRQHandler  ;
         DCD             TIM3_IRQHandler  ;
         DCD             TIM4_IRQHandler  ;
         DCD             I2C1_EV_IRQHandler  ;
         DCD             I2C1_ER_IRQHandler  ;
         DCD             I2C2_EV_IRQHandler  ;
         DCD             I2C2_ER_IRQHandler  ;
         DCD             SPI1_IRQHandler  ;
         DCD             SPI2_IRQHandler  ;
         DCD             USART1_IRQHandler  ;                  /* 70 */
         DCD             USART2_IRQHandler  ;
         DCD             USART3_IRQHandler  ;
         DCD             EXTI10_IRQHandler  ;
         DCD             RTC_TIMESTAMP_IRQHandler  ;
         DCD             EXTI11_IRQHandler  ;
         DCD             TIM8_BRK_IRQHandler  ;
         DCD             TIM8_UP_IRQHandler  ;
         DCD             TIM8_TRG_COM_IRQHandler  ;
         DCD             TIM8_CC_IRQHandler  ;
         DCD             DMA1_Stream7_IRQHandler  ;            /* 80 */
         DCD             FMC_IRQHandler  ;
         DCD             SDMMC1_IRQHandler  ;
         DCD             TIM5_IRQHandler  ;
         DCD             SPI3_IRQHandler  ;
         DCD             UART4_IRQHandler  ;
         DCD             UART5_IRQHandler  ;
         DCD             TIM6_IRQHandler  ;
         DCD             TIM7_IRQHandler  ;
         DCD             DMA2_Stream0_IRQHandler  ;
         DCD             DMA2_Stream1_IRQHandler  ;            /* 90 */
         DCD             DMA2_Stream2_IRQHandler  ;
         DCD             DMA2_Stream3_IRQHandler  ;
         DCD             DMA2_Stream4_IRQHandler  ;
         DCD             ETH1_IRQHandler  ;
         DCD             ETH1_WKUP_IRQHandler  ;
         DCD             FDCAN_CAL_IRQHandler  ;
         DCD             EXTI6_IRQHandler  ;
         DCD             EXTI7_IRQHandler  ;
         DCD             EXTI8_IRQHandler  ;
         DCD             EXTI9_IRQHandler  ;                   /* 100 */
         DCD             DMA2_Stream5_IRQHandler  ;
         DCD             DMA2_Stream6_IRQHandler  ;
         DCD             DMA2_Stream7_IRQHandler  ;
         DCD             USART6_IRQHandler  ;
         DCD             I2C3_EV_IRQHandler  ;
         DCD             I2C3_ER_IRQHandler  ;
         DCD             USBH_PORT1_IRQHandler  ;
         DCD             USBH_PORT2_IRQHandler  ;
         DCD             EXTI12_IRQHandler  ;
         DCD             EXTI13_IRQHandler  ;                  /* 110 */
         DCD             DCMIPP_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             HASH1_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             UART7_IRQHandler  ;
         DCD             UART8_IRQHandler  ;
         DCD             SPI4_IRQHandler  ;
         DCD             SPI5_IRQHandler  ;
         DCD             SAI1_IRQHandler  ;
         DCD             LTDC_IRQHandler  ;                    /* 120 */
         DCD             LTDC_ER_IRQHandler  ;
         DCD             SAI2_IRQHandler   ;
         DCD             QUADSPI_IRQHandler  ;
         DCD             LPTIM1_IRQHandler  ;
         DCD             I2C4_EV_IRQHandler  ;
         DCD             I2C4_ER_IRQHandler  ;
         DCD             SPDIF_RX_IRQHandler   ;
         DCD             OTG_IRQHandler  ;
         DCD             ETH2_IRQHandler  ;
         DCD             ETH2_WKUP_IRQHandler  ;               /* 130 */
         DCD             DMAMUX1_OVR_IRQHandler  ;
         DCD             DMAMUX2_OVR_IRQHandler  ;
         DCD             TIM15_IRQHandler  ;
         DCD             TIM16_IRQHandler  ;
         DCD             TIM17_IRQHandler  ;
         DCD             TIM12_IRQHandler  ;
         DCD             PKA_IRQHandler  ;
         DCD             EXTI14_IRQHandler  ;
         DCD             MDMA_IRQHandler  ;
         DCD             SDMMC2_IRQHandler  ;                  /* 140 */
         DCD             EXTI15_IRQHandler  ;
         DCD             MDMA_SEC_IT_IRQHandler  ;
         DCD             TIM13_IRQHandler  ;
         DCD             TIM14_IRQHandler  ;
         DCD             RNG1_IRQHandler  ;
         DCD             I2C5_EV_IRQHandler  ;
         DCD             I2C5_ER_IRQHandler  ;
         DCD             LPTIM2_IRQHandler  ;
         DCD             LPTIM3_IRQHandler  ;
         DCD             LPTIM4_IRQHandler  ;                  /* 150 */
         DCD             LPTIM5_IRQHandler  ;
         DCD             ETH1_LPI_IRQHandler  ;
         DCD             ETH2_LPI_IRQHandler  ;
         DCD             RCC_WAKEUP_IRQHandler  ;
         DCD             RESERVED_IRQHandler  ;
         DCD             DTS_IRQHandler  ;
         DCD             MPU_WAKEUP_PIN_IRQHandler  ;
         DCD             IWDG1_IRQHandler  ;
         DCD             IWDG2_IRQHandler  ;
         DCD             TAMP_SIRQHandler  ;                   /* 160 */	
         DCD             RTC_WKUP_ALARM_S_IRQHandler  ;
         DCD             RTC_TS_S_IRQHandler  ;
         DCD             LTDC_SEC_IRQHandler  ;
         DCD             LTDC_SEC_ER_IRQHandler  ;
         DCD             PMUIRQ0_IRQHandler  ;
         DCD             COMMRX0_IRQHandler  ;
         DCD             COMMTX0_IRQHandler  ;
         DCD             AXIERRIRQ_IRQHandler  ;
         DCD             DDRPERFM_IRQHandler  ;
         DCD             nCTIIRQ0_IRQHandler  ;                /* 170 */
         DCD             RESERVED_IRQHandler  ;
         DCD             DFSDM1_IRQHandler  ;
         DCD             DFSDM2_IRQHandler  ;
         DCD             DMA3_Stream0_IRQHandler  ;
         DCD             DMA3_Stream1_IRQHandler  ;
         DCD             DMA3_Stream2_IRQHandler  ;
         DCD             DMA3_Stream3_IRQHandler  ;
         DCD             DMA3_Stream4_IRQHandler  ;
         DCD             DMA3_Stream5_IRQHandler  ;
         DCD             DMA3_Stream6_IRQHandler  ;            /* 180 */
         DCD             DMA3_Stream7_IRQHandler  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
        /* THUMB */
        PUBWEAK RESERVED_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RESERVED_IRQHandler
        B RESERVED_IRQHandler

        PUBWEAK SGI0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI0_IRQHandler
        B SGI0_IRQHandler

        PUBWEAK SGI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI1_IRQHandler
        B SGI1_IRQHandler

        PUBWEAK SGI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI2_IRQHandler
        B SGI2_IRQHandler

        PUBWEAK SGI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI3_IRQHandler
        B SGI3_IRQHandler

        PUBWEAK SGI4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI4_IRQHandler
        B SGI4_IRQHandler
		
        PUBWEAK SGI5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI5_IRQHandler
        B SGI5_IRQHandler

        PUBWEAK SGI6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI6_IRQHandler
        B SGI6_IRQHandler

        PUBWEAK SGI7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI7_IRQHandler
        B SGI7_IRQHandler

        PUBWEAK SGI8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI8_IRQHandler
        B SGI8_IRQHandler		

        PUBWEAK SGI9_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI9_IRQHandler
        B SGI9_IRQHandler

        PUBWEAK SGI10_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI10_IRQHandler
        B SGI10_IRQHandler
		
        PUBWEAK SGI11_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI11_IRQHandler
        B SGI11_IRQHandler

        PUBWEAK SGI12_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI12_IRQHandler
        B SGI12_IRQHandler		

        PUBWEAK SGI13_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI13_IRQHandler
        B SGI13_IRQHandler

        PUBWEAK SGI14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI14_IRQHandler
        B SGI14_IRQHandler

        PUBWEAK SGI15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SGI15_IRQHandler
        B SGI15_IRQHandler

        PUBWEAK VirtualMaintenanceInterrupt_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
VirtualMaintenanceInterrupt_IRQHandler
        B VirtualMaintenanceInterrupt_IRQHandler		

        PUBWEAK HypervisorTimer_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HypervisorTimer_IRQHandler
        B HypervisorTimer_IRQHandler
		
        PUBWEAK VirtualTimer_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
VirtualTimer_IRQHandler
        B VirtualTimer_IRQHandler	

        PUBWEAK Legacy_nFIQ_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
Legacy_nFIQ_IRQHandler
        B Legacy_nFIQ_IRQHandler

        PUBWEAK SecurePhysicalTimer_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SecurePhysicalTimer_IRQHandler
        B SecurePhysicalTimer_IRQHandler

        PUBWEAK NonSecurePhysicalTimer_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
NonSecurePhysicalTimer_IRQHandler
        B NonSecurePhysicalTimer_IRQHandler

        PUBWEAK Legacy_nIRQ_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
Legacy_nIRQ_IRQHandler
        B Legacy_nIRQ_IRQHandler

        PUBWEAK PVD_AVD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PVD_AVD_IRQHandler
        B PVD_AVD_IRQHandler

        PUBWEAK TAMP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TAMP_IRQHandler
        B TAMP_IRQHandler
		
        PUBWEAK RTC_WKUP_ALARM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_WKUP_ALARM_IRQHandler
        B RTC_WKUP_ALARM_IRQHandler
		
        PUBWEAK TZC_IT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TZC_IT_IRQHandler
        B TZC_IT_IRQHandler
		
        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_IRQHandler
        B RCC_IRQHandler
		
        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler
		
        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler
		
        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler
		
        PUBWEAK DMA1_Stream0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream0_IRQHandler
        B DMA1_Stream0_IRQHandler
		
        PUBWEAK DMA1_Stream1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream1_IRQHandler
        B DMA1_Stream1_IRQHandler
		
        PUBWEAK DMA1_Stream2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream2_IRQHandler
        B DMA1_Stream2_IRQHandler
		
        PUBWEAK DMA1_Stream3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream3_IRQHandler
        B DMA1_Stream3_IRQHandler
		
        PUBWEAK DMA1_Stream4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream4_IRQHandler
        B DMA1_Stream4_IRQHandler

        PUBWEAK DMA1_Stream5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream5_IRQHandler
        B DMA1_Stream5_IRQHandler

        PUBWEAK DMA1_Stream6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream6_IRQHandler
        B DMA1_Stream6_IRQHandler

        PUBWEAK ADC1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC1_IRQHandler
        B ADC1_IRQHandler

        PUBWEAK ADC2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC2_IRQHandler
        B ADC2_IRQHandler

        PUBWEAK FDCAN1_IT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDCAN1_IT0_IRQHandler
        B FDCAN1_IT0_IRQHandler

        PUBWEAK FDCAN2_IT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDCAN2_IT0_IRQHandler
        B FDCAN2_IT0_IRQHandler

        PUBWEAK FDCAN1_IT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDCAN1_IT1_IRQHandler
        B FDCAN1_IT1_IRQHandler
		
        PUBWEAK FDCAN2_IT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDCAN2_IT1_IRQHandler
        B FDCAN2_IT1_IRQHandler
		
        PUBWEAK EXTI5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI5_IRQHandler
        B EXTI5_IRQHandler
		
        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_BRK_IRQHandler
        B TIM1_BRK_IRQHandler
		
        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_UP_IRQHandler
        B TIM1_UP_IRQHandler
		
        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_TRG_COM_IRQHandler
        B TIM1_TRG_COM_IRQHandler
		
        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM1_CC_IRQHandler
        B TIM1_CC_IRQHandler
		
        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM2_IRQHandler
        B TIM2_IRQHandler
		
        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM3_IRQHandler
        B TIM3_IRQHandler
		
        PUBWEAK TIM4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM4_IRQHandler
        B TIM4_IRQHandler
		
        PUBWEAK I2C1_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_EV_IRQHandler
        B I2C1_EV_IRQHandler
		
        PUBWEAK I2C1_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_ER_IRQHandler
        B I2C1_ER_IRQHandler
		
        PUBWEAK I2C2_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_EV_IRQHandler
        B I2C2_EV_IRQHandler

        PUBWEAK I2C2_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_ER_IRQHandler
        B I2C2_ER_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART1_IRQHandler
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART2_IRQHandler
        B USART2_IRQHandler

        PUBWEAK USART3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART3_IRQHandler
        B USART3_IRQHandler

        PUBWEAK EXTI10_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI10_IRQHandler
        B EXTI10_IRQHandler

        PUBWEAK RTC_TIMESTAMP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_TIMESTAMP_IRQHandler
        B RTC_TIMESTAMP_IRQHandler

        PUBWEAK EXTI11_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI11_IRQHandler
        B EXTI11_IRQHandler

        PUBWEAK TIM8_BRK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_BRK_IRQHandler
        B TIM8_BRK_IRQHandler

        PUBWEAK TIM8_UP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_UP_IRQHandler
        B TIM8_UP_IRQHandler

        PUBWEAK TIM8_TRG_COM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_TRG_COM_IRQHandler
        B TIM8_TRG_COM_IRQHandler

        PUBWEAK TIM8_CC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM8_CC_IRQHandler
        B TIM8_CC_IRQHandler	

        PUBWEAK DMA1_Stream7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_Stream7_IRQHandler
        B DMA1_Stream7_IRQHandler

        PUBWEAK FMC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FMC_IRQHandler
        B FMC_IRQHandler

        PUBWEAK SDMMC1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SDMMC1_IRQHandler
        B SDMMC1_IRQHandler

        PUBWEAK TIM5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM5_IRQHandler
        B TIM5_IRQHandler

        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI3_IRQHandler
        B SPI3_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART4_IRQHandler
        B UART4_IRQHandler

        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART5_IRQHandler
        B UART5_IRQHandler

        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM6_IRQHandler
        B TIM6_IRQHandler

        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM7_IRQHandler
        B TIM7_IRQHandler

        PUBWEAK DMA2_Stream0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream0_IRQHandler
        B DMA2_Stream0_IRQHandler

        PUBWEAK DMA2_Stream1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream1_IRQHandler
        B DMA2_Stream1_IRQHandler

        PUBWEAK DMA2_Stream2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream2_IRQHandler
        B DMA2_Stream2_IRQHandler

        PUBWEAK DMA2_Stream3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream3_IRQHandler
        B DMA2_Stream3_IRQHandler

        PUBWEAK DMA2_Stream4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream4_IRQHandler
        B DMA2_Stream4_IRQHandler

        PUBWEAK ETH1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH1_IRQHandler
        B ETH1_IRQHandler

        PUBWEAK ETH1_WKUP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH1_WKUP_IRQHandler
        B ETH1_WKUP_IRQHandler

        PUBWEAK FDCAN_CAL_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FDCAN_CAL_IRQHandler
        B FDCAN_CAL_IRQHandler

        PUBWEAK EXTI6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI6_IRQHandler
        B EXTI6_IRQHandler

        PUBWEAK EXTI7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI7_IRQHandler
        B EXTI7_IRQHandler

        PUBWEAK EXTI8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI8_IRQHandler
        B EXTI8_IRQHandler

        PUBWEAK EXTI9_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI9_IRQHandler
        B EXTI9_IRQHandler

        PUBWEAK DMA2_Stream5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream5_IRQHandler
        B DMA2_Stream5_IRQHandler

        PUBWEAK DMA2_Stream6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream6_IRQHandler
        B DMA2_Stream6_IRQHandler

        PUBWEAK DMA2_Stream7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_Stream7_IRQHandler
        B DMA2_Stream7_IRQHandler	

        PUBWEAK USART6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USART6_IRQHandler
        B USART6_IRQHandler	

        PUBWEAK I2C3_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C3_EV_IRQHandler
        B I2C3_EV_IRQHandler	

        PUBWEAK I2C3_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C3_ER_IRQHandler
        B I2C3_ER_IRQHandler	

        PUBWEAK USBH_PORT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USBH_PORT1_IRQHandler
        B USBH_PORT1_IRQHandler	

        PUBWEAK USBH_PORT2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USBH_PORT2_IRQHandler
        B USBH_PORT2_IRQHandler	

        PUBWEAK EXTI12_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI12_IRQHandler
        B EXTI12_IRQHandler	

        PUBWEAK EXTI13_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI13_IRQHandler
        B EXTI13_IRQHandler	

        PUBWEAK DCMIPP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DCMIPP_IRQHandler
        B DCMIPP_IRQHandler	
		
        PUBWEAK HASH1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HASH1_IRQHandler
        B HASH1_IRQHandler

        PUBWEAK UART7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART7_IRQHandler
        B UART7_IRQHandler	

        PUBWEAK UART8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART8_IRQHandler
        B UART8_IRQHandler	

        PUBWEAK SPI4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI4_IRQHandler
        B SPI4_IRQHandler	

        PUBWEAK SPI5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI5_IRQHandler
        B SPI5_IRQHandler	

        PUBWEAK SAI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SAI1_IRQHandler
        B SAI1_IRQHandler

        PUBWEAK LTDC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LTDC_IRQHandler
        B LTDC_IRQHandler	

        PUBWEAK LTDC_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LTDC_ER_IRQHandler
        B LTDC_ER_IRQHandler	

        PUBWEAK SAI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SAI2_IRQHandler
        B SAI2_IRQHandler	

        PUBWEAK QUADSPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
QUADSPI_IRQHandler
        B QUADSPI_IRQHandler	

        PUBWEAK LPTIM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM1_IRQHandler
        B LPTIM1_IRQHandler	

        PUBWEAK I2C4_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C4_EV_IRQHandler
        B I2C4_EV_IRQHandler	

        PUBWEAK I2C4_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C4_ER_IRQHandler
        B I2C4_ER_IRQHandler	

        PUBWEAK SPDIF_RX_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPDIF_RX_IRQHandler
        B SPDIF_RX_IRQHandler

        PUBWEAK OTG_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
OTG_IRQHandler
        B OTG_IRQHandler	

        PUBWEAK ETH2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH2_IRQHandler
        B ETH2_IRQHandler	

        PUBWEAK ETH2_WKUP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH2_WKUP_IRQHandler
        B ETH2_WKUP_IRQHandler	

        PUBWEAK DMAMUX1_OVR_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMAMUX1_OVR_IRQHandler
        B DMAMUX1_OVR_IRQHandler	

        PUBWEAK DMAMUX2_OVR_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMAMUX2_OVR_IRQHandler
        B DMAMUX2_OVR_IRQHandler

        PUBWEAK TIM15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM15_IRQHandler
        B TIM15_IRQHandler	

        PUBWEAK TIM16_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM16_IRQHandler
        B TIM16_IRQHandler	

        PUBWEAK TIM17_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM17_IRQHandler
        B TIM17_IRQHandler	

        PUBWEAK TIM12_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM12_IRQHandler
        B TIM12_IRQHandler	

        PUBWEAK PKA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PKA_IRQHandler
        B PKA_IRQHandler	

        PUBWEAK EXTI14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI14_IRQHandler
        B EXTI14_IRQHandler	

        PUBWEAK MDMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MDMA_IRQHandler
        B MDMA_IRQHandler	

        PUBWEAK SDMMC2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SDMMC2_IRQHandler
        B SDMMC2_IRQHandler	

        PUBWEAK EXTI15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EXTI15_IRQHandler
        B EXTI15_IRQHandler	

        PUBWEAK MDMA_SEC_IT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MDMA_SEC_IT_IRQHandler
        B MDMA_SEC_IT_IRQHandler	

        PUBWEAK TIM13_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM13_IRQHandler
        B TIM13_IRQHandler	

        PUBWEAK TIM14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIM14_IRQHandler
        B TIM14_IRQHandler	

        PUBWEAK RNG1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RNG1_IRQHandler
        B RNG1_IRQHandler	

        PUBWEAK I2C5_EV_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C5_EV_IRQHandler
        B I2C5_EV_IRQHandler	

        PUBWEAK I2C5_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C5_ER_IRQHandler
        B I2C5_ER_IRQHandler	

        PUBWEAK LPTIM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM2_IRQHandler
        B LPTIM2_IRQHandler	

        PUBWEAK LPTIM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM3_IRQHandler
        B LPTIM3_IRQHandler	

        PUBWEAK LPTIM4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM4_IRQHandler
        B LPTIM4_IRQHandler	

        PUBWEAK LPTIM5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPTIM5_IRQHandler
        B LPTIM5_IRQHandler	

        PUBWEAK ETH1_LPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH1_LPI_IRQHandler
        B ETH1_LPI_IRQHandler	

        PUBWEAK ETH2_LPI_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ETH2_LPI_IRQHandler
        B ETH2_LPI_IRQHandler	

        PUBWEAK RCC_WAKEUP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RCC_WAKEUP_IRQHandler
        B RCC_WAKEUP_IRQHandler	

        PUBWEAK DTS_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DTS_IRQHandler
        B DTS_IRQHandler	

        PUBWEAK MPU_WAKEUP_PIN_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MPU_WAKEUP_PIN_IRQHandler
        B MPU_WAKEUP_PIN_IRQHandler	

        PUBWEAK IWDG1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IWDG1_IRQHandler
        B IWDG1_IRQHandler	

        PUBWEAK IWDG2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IWDG2_IRQHandler
        B IWDG2_IRQHandler	

        PUBWEAK TAMP_SIRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TAMP_SIRQHandler
        B TAMP_SIRQHandler	

        PUBWEAK RTC_WKUP_ALARM_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_WKUP_ALARM_S_IRQHandler
        B RTC_WKUP_ALARM_S_IRQHandler	

        PUBWEAK RTC_TS_S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_TS_S_IRQHandler
        B RTC_TS_S_IRQHandler	

        PUBWEAK LTDC_SEC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LTDC_SEC_IRQHandler
        B LTDC_SEC_IRQHandler	
		
        PUBWEAK LTDC_SEC_ER_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LTDC_SEC_ER_IRQHandler
        B LTDC_SEC_ER_IRQHandler	
		
	PUBWEAK PMUIRQ0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PMUIRQ0_IRQHandler
        B PMUIRQ0_IRQHandler	
		
	PUBWEAK COMMRX0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
COMMRX0_IRQHandler
        B COMMRX0_IRQHandler	
		
	PUBWEAK COMMTX0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
COMMTX0_IRQHandler
        B COMMTX0_IRQHandler	
		
	PUBWEAK AXIERRIRQ_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
AXIERRIRQ_IRQHandler
        B AXIERRIRQ_IRQHandler
		
	PUBWEAK DDRPERFM_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DDRPERFM_IRQHandler
        B DDRPERFM_IRQHandler
		
	PUBWEAK nCTIIRQ0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
nCTIIRQ0_IRQHandler
        B nCTIIRQ0_IRQHandler
		
	PUBWEAK DFSDM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DFSDM1_IRQHandler
        B DFSDM1_IRQHandler
		
	PUBWEAK DFSDM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DFSDM2_IRQHandler
        B DFSDM2_IRQHandler
				
	PUBWEAK DMA3_Stream0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream0_IRQHandler
        B DMA3_Stream0_IRQHandler
		
	PUBWEAK DMA3_Stream1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream1_IRQHandler
        B DMA3_Stream1_IRQHandler
		
	PUBWEAK DMA3_Stream2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream2_IRQHandler
        B DMA3_Stream2_IRQHandler
		
	PUBWEAK DMA3_Stream3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream3_IRQHandler
        B DMA3_Stream3_IRQHandler
		
	PUBWEAK DMA3_Stream4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream4_IRQHandler
        B DMA3_Stream4_IRQHandler
		
	PUBWEAK DMA3_Stream5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream5_IRQHandler
        B DMA3_Stream5_IRQHandler
		
	PUBWEAK DMA3_Stream6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream6_IRQHandler
        B DMA3_Stream6_IRQHandler
		
	PUBWEAK DMA3_Stream7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_Stream7_IRQHandler
        B DMA3_Stream7_IRQHandler
#if defined (AZURE_RTOS)
	PUBWEAK __tx_irq_handler
        SECTION .text:CODE:NOROOT:REORDER(1)
__tx_irq_handler
        B __tx_irq_handler
#endif

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector Table
 *----------------------------------------------------------------------------*/

  SECTION .intvec:CODE:NOROOT(2)
  PUBLIC  __vector
  PUBLIC  Reset_Handler
#ifdef AZURE_RTOS
  EXTERN __tx_undefined
  EXTERN PUBWEAK __tx_swi_interrupt
  EXTERN __tx_prefetch_handler
  EXTERN __tx_abort_handler
  EXTERN __tx_irq_handler
  EXTERN __tx_fiq_handler
#else
  //EXTERN  Undef_Handler
  EXTERN PUBWEAK  FreeRTOS_SWI_Handler
  //EXTERN PAbt_Handler
  //EXTERN DAbt_Handler
  EXTERN FreeRTOS_IRQ_Handler
  EXTERN FIQ_Handler
#endif
  /* PUBLIC  Vectors */

  DATA

__vector:                       ; Make this a DATA label, so that stack usage
                                ; analysis doesn't consider it an uncalled fun

  ARM

//Vectors:
  LDR    PC, =Reset_Handler
#ifdef AZURE_RTOS
  LDR    PC, =__tx_undefined
  LDR    PC, =__tx_swi_interrupt
  LDR    PC, =__tx_prefetch_handler
  LDR    PC, =__tx_abort_handler
  NOP
  LDR    PC, =__tx_irq_handler
  LDR    PC, =__tx_fiq_handler
#else
  LDR    PC, =Undef_Handler
  LDR    PC, =FreeRTOS_SWI_Handler
  LDR    PC, =PAbt_Handler
  LDR    PC, =DAbt_Handler
  NOP
  LDR    PC, =FreeRTOS_IRQ_Handler
  LDR    PC, =FIQ_Handler
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
  SECTION .text:CODE:NOROOT(2)

  EXTERN  SystemInit
  EXTERN  __iar_init_vfp
  EXTERN  __iar_init_core
  EXTERN  __cmain

Reset_Handler:
  ARM
  /* Mask interrupts */
  CPSID   if

  /* Put any cores other than 0 to sleep */
  MRC     p15, 0, R0, c0, c0, 5
  ANDS    R0, R0, #3
goToSleep:
  ITT NE
  WFINE
  BNE     goToSleep

  /* Reset SCTLR Settings */
  MRC     p15, 0, R0, c1, c0, 0 /* Read CP15 System Control register */
  BIC     R0, R0, #(0x1 << 12) /* Clear I bit 12 to disable I Cache */
  BIC     R0, R0, #(0x1 <<  2) /* Clear C bit  2 to disable D Cache */
  BIC     R0, R0, #0x1       /* Clear M bit  0 to disable MMU */
  BIC     R0, R0, #(0x1 << 11) /* Clear Z bit 11 to disable branch prediction */
  BIC     R0, R0, #(0x1 << 13) /* Clear V bit 13 to disable hivecs */
  BIC     R0, R0, #(0x1 << 29) /* Clear AFE bit 29 to enable the full range of access permissions */
  /* Removed for IAR, not needed for ARM Mode */
/*ORR     R0, R0, #(0x1 << 30) */ /* Set TE bit to take exceptions in Thumb mode */
  MCR     p15, 0, R0, c1, c0, 0 /* Write value back to CP15 System Control register */
  ISB

  /* Configure ACTLR */
  MRC     p15, 0, r0, c1, c0, 1 /* Read CP15 Auxiliary Control Register */
  ORR     r0, r0, #(1 <<  1) /* Enable L2 prefetch hint (UNK/WI since r4p1) */
  MCR     p15, 0, r0, c1, c0, 1 /* Write CP15 Auxiliary Control Register */

  /* Set Vector Base Address Register (VBAR) to point to this application's vector table */
  LDR    R0, =__vector
  MCR    p15, 0, R0, c12, c0, 0
  ISB
  
  /* Setup Stack for each exception mode */
  CPS    #0x11
  LDR    SP, =SFE(FIQ_STACK)
  CPS    #0x12
  LDR    SP, =SFE(IRQ_STACK)
  CPS    #0x13
  LDR    SP, =SFE(SVC_STACK)
  CPS    #0x17
  LDR    SP, =SFE(ABT_STACK)
  CPS    #0x1B
  LDR    SP, =SFE(UND_STACK)
  CPS    #0x1F
  LDR    SP, =SFE(CSTACK)
 

  /* Init VFP */
  /* Should be removed if already implemented in SystemInit() */
  BL      __iar_init_vfp

  /*  SystemInit Moved to __iar_data_init_done to ensure TTB Table is correctly initialized */

  /* Call __cmain */
  BL __cmain

#ifdef FreeRTOS
 /*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
Undef_Handler:
PAbt_Handler:
DAbt_Handler:
Default_Handler:
  B .
#endif

  SECTION .text:CODE:NOROOT(2)
  PUBLIC __iar_data_init_done
  ARM

__iar_data_init_done:
  PUSH   {R7, LR}
  /* *** Do not use/rely on initialized (or zero initialized) global variables in SystemInit() */
  /* *** since their initilzation will be invoked within __cmain */
  BLX    SystemInit
  /* Unmask interrupts */
  CPSIE  if
  POP    {R7, PC}

  /*PUBLIC ZeroBss
  EXTERN ZI_DATA
  EXTERN __BSS_END__

ZeroBss
     PUSH {R4-R11}
     LDR r2, = ZI_DATA
     b LoopFillZerobss
    /* Zero fill the bss segment. */
   /*  FillZerobss:
     MOVS r3, #0
     STR  r3, [r2]
     adds r2, r2, #4

     LoopFillZerobss:
     LDR r3, = __BSS_END__
     CMP r2, r3
     BCC FillZerobss
     DSB
     POP    {R4-R11}    */

/*----------------------------------------------------------------------------
  Reset_Handler is declared as a 32 bit function  - target("arm") instead of Thumb.
  Any function added after Reset_Handler() will conserve this mode declaration.
  To keep Thumb mode, new function should be added before Reset_Handler().
 *----------------------------------------------------------------------------*/
   END