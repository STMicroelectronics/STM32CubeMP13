/**
  ******************************************************************************
  * @file    stm32mp13xx_disco_stpmic1.h
  * @author  MCD Application Team
  * @brief   stpmu driver functions used for ST internal validation
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP13XX_DISCO_STPMIC_H
#define STM32MP13XX_DISCO_STPMIC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp13xx_hal.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32MP13XX_DISCO_STPMU
  * @{
  */

/** @defgroup STM32MP13XX_DISCO_STPMU_Exported_Types Exported Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STPMU1_BUCK1=1,
  STPMU1_BUCK2,
  STPMU1_BUCK3,
  STPMU1_BUCK4,
  STPMU1_LDO1,
  STPMU1_LDO2,
  STPMU1_LDO3,
  STPMU1_LDO4,
  STPMU1_LDO5,
  STPMU1_LDO6,
  STPMU1_VREFDDR,
}PMIC_RegulId_TypeDef;

/* IRQ definitions */
typedef enum {

/* Interrupt Register 1 (0x50 for latch) */
IT_SWOUT_R,
IT_SWOUT_F,
IT_VBUS_OTG_R,
IT_VBUS_OTG_F,
IT_WAKEUP_R,
IT_WAKEUP_F,
IT_PONKEY_R,
IT_PONKEY_F,

/* Interrupt Register 2 (0x51 for latch) */
IT_OVP_BOOST,
IT_OCP_BOOST,
IT_OCP_SWOUT,
IT_OCP_OTG,
IT_CURLIM_BUCK4,
IT_CURLIM_BUCK3,
IT_CURLIM_BUCK2,
IT_CURLIM_BUCK1,

/* Interrupt Register 3 (0x52 for latch) */
IT_SHORT_SWOUT,
IT_SHORT_SWOTG,
IT_CURLIM_LDO6,
IT_CURLIM_LDO5,
IT_CURLIM_LDO4,
IT_CURLIM_LDO3,
IT_CURLIM_LDO2,
IT_CURLIM_LDO1,

/* Interrupt Register 3 (0x52 for latch) */
IT_SWIN_R,
IT_SWIN_F,
IT_RESERVED_1,
IT_RESERVED_2,
IT_VINLOW_R,
IT_VINLOW_F,
IT_TWARN_R,
IT_TWARN_F,

IRQ_NR,
} PMIC_IRQn;

/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_STPMU_Exported_Constants Exported Constants
  * @{
  */

/* Exported constants --------------------------------------------------------*/
#define BIT_PMIC(_x)  (1U<<(_x))
#define STM32_PMIC_NUM_IRQ_REGS       4U

#define TURN_ON_REG                 0x1U
#define TURN_OFF_REG                0x2U
#define ICC_LDO_TURN_OFF_REG        0x3U
#define ICC_BUCK_TURN_OFF_REG       0x4U
#define RESET_STATUS_REG            0x5U
#define VERSION_STATUS_REG          0x6U
#define MAIN_CONTROL_REG            0x10U
#define PADS_PULL_REG               0x11U
#define BUCK_PULL_DOWN_REG          0x12U
#define LDO14_PULL_DOWN_REG         0x13U
#define LDO56_PULL_DOWN_REG         0x14U
#define VIN_CONTROL_REG             0x15U
#define PONKEY_TIMER_REG            0x16U
#define MASK_RANK_BUCK_REG          0x17U
#define MASK_RESET_BUCK_REG         0x18U
#define MASK_RANK_LDO_REG           0x19U
#define MASK_RESET_LDO_REG          0x1AU
#define WATCHDOG_CONTROL_REG        0x1BU
#define WATCHDOG_TIMER_REG          0x1CU
#define BUCK_ICC_TURNOFF_REG        0x1DU
#define LDO_ICC_TURNOFF_REG         0x1EU
#define BUCK_APM_CONTROL_REG        0x1FU
#define BUCK1_CONTROL_REG           0x20U
#define BUCK2_CONTROL_REG           0x21U
#define BUCK3_CONTROL_REG           0x22U
#define BUCK4_CONTROL_REG           0x23U
#define VREF_DDR_CONTROL_REG        0x24U
#define LDO1_CONTROL_REG            0x25U
#define LDO2_CONTROL_REG            0x26U
#define LDO3_CONTROL_REG            0x27U
#define LDO4_CONTROL_REG            0x28U
#define LDO5_CONTROL_REG            0x29U
#define LDO6_CONTROL_REG            0x2AU
#define BUCK1_PWRCTRL_REG           0x30U
#define BUCK2_PWRCTRL_REG           0x31U
#define BUCK3_PWRCTRL_REG           0x32U
#define BUCK4_PWRCTRL_REG           0x33U
#define VREF_DDR_PWRCTRL_REG        0x34U
#define LDO1_PWRCTRL_REG            0x35U
#define LDO2_PWRCTRL_REG            0x36U
#define LDO3_PWRCTRL_REG            0x37U
#define LDO4_PWRCTRL_REG            0x38U
#define LDO5_PWRCTRL_REG            0x39U
#define LDO6_PWRCTRL_REG            0x3AU
#define FREQUENCY_SPREADING_REG     0x3BU
#define USB_CONTROL_REG             0x40U
#define ITLATCH1_REG                0x50U
#define ITLATCH2_REG                0x51U
#define ITLATCH3_REG                0x52U
#define ITLATCH4_REG                0x53U
#define ITSETLATCH1_REG             0x60U
#define ITSETLATCH2_REG             0x61U
#define ITSETLATCH3_REG             0x62U
#define ITSETLATCH4_REG             0x63U
#define ITCLEARLATCH1_REG           0x70U
#define ITCLEARLATCH2_REG           0x71U
#define ITCLEARLATCH3_REG           0x72U
#define ITCLEARLATCH4_REG           0x73U
#define ITMASK1_REG                 0x80U
#define ITMASK2_REG                 0x81U
#define ITMASK3_REG                 0x82U
#define ITMASK4_REG                 0x83U
#define ITSETMASK1_REG              0x90U
#define ITSETMASK2_REG              0x91U
#define ITSETMASK3_REG              0x92U
#define ITSETMASK4_REG              0x93U
#define ITCLEARMASK1_REG            0xA0U
#define ITCLEARMASK2_REG            0xA1U
#define ITCLEARMASK3_REG            0xA2U
#define ITCLEARMASK4_REG            0xA3U
#define ITSOURCE1_REG               0xB0U
#define ITSOURCE2_REG               0xB1U
#define ITSOURCE3_REG               0xB2U
#define ITSOURCE4_REG               0xB3U
#define LDO_VOLTAGE_MASK            0x7CU
#define BUCK_VOLTAGE_MASK           0xFCU
#define LDO_BUCK_VOLTAGE_SHIFT      2U

#define LDO_ENABLE_MASK             0x01U
#define BUCK_ENABLE_MASK            0x01U
#define BUCK_HPLP_ENABLE_MASK       0x02U
#define LDO_HPLP_ENABLE_MASK        0x02U
#define LDO_BUCK_HPLP_SHIFT         1U

#define LDO_BUCK_RANK_MASK          0x01U
#define LDO_BUCK_RESET_MASK         0x01U
#define LDO_BUCK_PULL_DOWN_MASK     0x03U


/* Main PMIC Control Register
 * MAIN_CONTROL_REG
 * Address : 0x10
 * */
#define ICC_EVENT_ENABLED           BIT_PMIC(4U)
#define PWRCTRL_POLARITY_HIGH       BIT_PMIC(3U)
#define PWRCTRL_PIN_VALID           BIT_PMIC(2U)
#define RESTART_REQUEST_ENABLED     BIT_PMIC(1U)
#define SOFTWARE_SWITCH_OFF_ENABLED BIT_PMIC(0U)

/* Main PMIC PADS Control Register
 * PADS_PULL_REG
 * Address : 0x11
 * */
#define WAKEUP_DETECTOR_DISABLED    BIT_PMIC(4U)
#define PWRCTRL_PD_ACTIVE           BIT_PMIC(3U)
#define PWRCTRL_PU_ACTIVE           BIT_PMIC(2U)
#define WAKEUP_PD_ACTIVE            BIT_PMIC(1U)
#define PONKEY_PU_ACTIVE            BIT_PMIC(0U)


/* Main PMIC VINLOW Control Register
 * VIN_CONTROL_REGC DMSC
 * Address : 0x15
 * */
#define SWIN_DETECTOR_ENABLED       BIT_PMIC(7U)
#define SWOUT_DETECTOR_ENABLED      BIT_PMIC(6U)
#define VINLOW_HYST_MASK            0x3U
#define VINLOW_HYST_SHIFT           4U
#define VINLOW_THRESHOLD_MASK       0x7U
#define VINLOW_THRESHOLD_SHIFT      1U
#define VINLOW_ENABLED              0x01U
#define VINLOW_CTRL_REG_MASK        0xFFU


/* USB Control Register
 * Address : 0x40
 * */
#define BOOST_OVP_DISABLED          BIT_PMIC(7U)
#define VBUS_OTG_DETECTION_DISABLED BIT_PMIC(6U)
// Discharge not implemented
#define OCP_LIMIT_HIGH              BIT_PMIC(3U)
#define SWIN_SWOUT_ENABLED          BIT_PMIC(2U)
#define USBSW_OTG_SWITCH_ENABLED    BIT_PMIC(1U)


/* IRQ masks */
/* Interrupt Mask for Register 1 (0x50 for latch) */
#define IT_SWOUT_R_MASK             BIT_PMIC(7U)
#define IT_SWOUT_F_MASK             BIT_PMIC(6U)
#define IT_VBUS_OTG_R_MASK          BIT_PMIC(5U)
#define IT_VBUS_OTG_F_MASK          BIT_PMIC(4U)
#define IT_WAKEUP_R_MASK            BIT_PMIC(3U)
#define IT_WAKEUP_F_MASK            BIT_PMIC(2U)
#define IT_PONKEY_R_MASK            BIT_PMIC(1U)
#define IT_PONKEY_F_MASK            BIT_PMIC(0U)

/* Interrupt Mask for Register 2 (0x51 for latch) */
#define IT_OVP_BOOST_MASK           BIT_PMIC(7U)
#define IT_OCP_BOOST_MASK           BIT_PMIC(6U)
#define IT_OCP_SWOUT_MASK           BIT_PMIC(5U)
#define IT_OCP_OTG_MASK             BIT_PMIC(4U)
#define IT_CURLIM_BUCK4_MASK        BIT_PMIC(3U)
#define IT_CURLIM_BUCK3_MASK        BIT_PMIC(2U)
#define IT_CURLIM_BUCK2_MASK        BIT_PMIC(1U)
#define IT_CURLIM_BUCK1_MASK        BIT_PMIC(0U)

/* Interrupt Mask for Register 3 (0x52 for latch) */
#define IT_SHORT_SWOUT_MASK         BIT_PMIC(7U)
#define IT_SHORT_SWOTG_MASK         BIT_PMIC(6U)
#define IT_CURLIM_LDO6_MASK         BIT_PMIC(5U)
#define IT_CURLIM_LDO5_MASK         BIT_PMIC(4U)
#define IT_CURLIM_LDO4_MASK         BIT_PMIC(3U)
#define IT_CURLIM_LDO3_MASK         BIT_PMIC(2U)
#define IT_CURLIM_LDO2_MASK         BIT_PMIC(1U)
#define IT_CURLIM_LDO1_MASK         BIT_PMIC(0U)

/* Interrupt Mask for Register 4 (0x53 for latch) */
#define IT_SWIN_R_MASK              BIT_PMIC(7U)
#define IT_SWIN_F_MASK              BIT_PMIC(6U)
/*  Reserved 1 */
/*  Reserved 2 */
#define IT_VINLOW_R_MASK            BIT_PMIC(3U)
#define IT_VINLOW_F_MASK            BIT_PMIC(2U)
#define IT_TWARN_R_MASK             BIT_PMIC(1U)
#define IT_TWARN_F_MASK             BIT_PMIC(0U)

#define PMIC_VERSION_ID             0x10U

/**
  * @}
  */

/** @defgroup STM32MP13XX_DISCO_STPMU_Exported_Functions Exported Functions
  * @{
  */

/* Exported functions --------------------------------------------------------*/
uint8_t STPMU1_Register_Read(uint8_t register_id);
void STPMU1_Register_Write(uint8_t register_id, uint8_t value);
void STPMU1_Register_Update(uint8_t register_id, uint8_t value, uint8_t mask);
void STPMU1_Enable_Interrupt(PMIC_IRQn IRQn);
void STPMU1_Disable_Interrupt(PMIC_IRQn IRQn);
void STPMU1_Regulator_Enable(PMIC_RegulId_TypeDef id);
void STPMU1_Regulator_Disable(PMIC_RegulId_TypeDef id);
uint8_t STPMU1_Is_Regulator_Enabled(PMIC_RegulId_TypeDef id);
void STPMU1_Regulator_Voltage_Set(PMIC_RegulId_TypeDef id,uint16_t milivolts);
void STPMU1_Sw_Reset(void);
uint32_t BSP_PMIC_Init(void);
uint32_t BSP_PMIC_DeInit(void);
uint32_t BSP_PMIC_Is_Device_Ready(void);
uint32_t BSP_PMIC_InitRegulators (void);
uint32_t BSP_PMIC_PrepareLpStop(void);
uint32_t BSP_PMIC_PrepareLpLvStop(void);
uint32_t BSP_PMIC_PrepareLpLvStop2(void);
uint32_t BSP_PMIC_PrepareStandbySelfRefresh(void);
uint32_t BSP_PMIC_PrepareStandby(void);
uint32_t BSP_PMIC_SwitchOff(void);
__weak void BSP_PMIC_INTn_Callback(PMIC_IRQn IRQn);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* STM32MP13XX_DISCO_STPMIC_H */
