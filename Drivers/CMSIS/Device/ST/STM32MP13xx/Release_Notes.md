---
pagetitle: Release Notes for STM32MP13xx CMSIS
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>\ STM32MP13xx CMSIS </mark>
Copyright &copy; 2024\ STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose
		
This driver provides the **CMSIS device for the STM32MP13xx** products:

- **STM32MP131xx** devices
- **STM32MP133xx** devices
- **STM32MP135xx** devices

This driver is composed of the descriptions of the registers under “Include” directory.
Various template file are provided to easily build an application. 
They can be adapted to fit applications requirements:

- Templates/system_stm32mp13xx.c contains the initialization code referred as SystemInit.
- Startup files are provided as example for STM32CubeIDE©.
- Linker files are provided as example for STM32CubeIDE©.
  
:::

::: {.col-sm-12 .col-lg-8}

# __Update History__

::: {.collapse}
<input type="checkbox" id="collapse-section0" checked aria-hidden="true">
<label for="collapse-section0" checked aria-hidden="true">__V1.1.0 / 23-Feb-2024__</label>
<div>

## Main Changes

This release is the **Major Maintenance DV of Bit and registers definition** for STM32MP13xx

## Contents
- Support added for IAR Embedded Workbench 9.50

## Known Limitations

- None

## Dependencies

None

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" checked aria-hidden="true">__V1.0.0 / 01-Nov-2023__</label>
<div>

## Main Changes

This release is the **Mass Market DV of Bit and registers definition** for STM32MP13xx

## Contents

- Support added for the MMU and CACHE initialization
- System, Startup, ThreadX initialization and linker script files for AzureRTOS MW updated to support thumb mode
- Support added for part numbers 

## Known Limitations

- IAR support is not implemented

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">__V0.7.0 / 27-June-2023__</label>
<div>

## Main Changes

This release is the **5th ALPHA DV of Bit and registers definition** for STM32MP13xx

## Contents

- FMC NAND defines updated for the HAL NAND sequencer
- Fixes for the MMU initialization
- Update bit definition in header files for ETZPC
- System, startup, ThreadX initialization and linker script files for AzureRTOS MW updated to support thumb mode
  

## Known Limitations

- IAR Templates not yet available

## Dependencies

None

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section6"  aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__V0.6.0 / 16-December-2022__</label>
<div>

## Main Changes

This release is the **4th ALPHA DV of Bit and registers definition** for STM32MP13xx

## Contents

- Update bit definition in header files:
  - TIM, SPDIFRX, ETH
  - Update RTC_TAMP_NB to match with External Tamper
  - Rename TZPC to ETZPC
- System files for AzureRTOS MW
  - Timer should be started later in azurertos application init

## Known Limitations

- IAR Templates not yet available

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V0.5.0 / 29-June-2022__</label>
<div>			

## Main Changes

This release is the **2nd ALPHA DV of Bit and registers definition** for STM32MP13xx

## Contents

- Update bit definition in header files:
  - for PWR, ETH, LPTIM
  - add internal TAMPER 12 and 13 definitions
  - update USART structure with uint32_t only
  - Fix typo for SYSCFG_HSLVEN4R_HSLVEN_SDMMC1
- Implement startup and system files for AzureRTOS MW
- Fix some MISRAC2012 rules

## Known Limitations

- IAR Templates not yet available

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__V0.4.0 / 04-March-2022__</label>
<div>			

## Main Changes

This release is the **2nd ALPHA DV of Bit and registers definition** for STM32MP13xx

## Contents

- Update bit definition in header files:
  - DMAMUX: Fix SYNC_ID definition
- MMU : rename file and update the license
- Linker : Clean linker and add stm32mp13xx_a7_ddr.ld

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V0.3.0 / 03-December-2021__</label>
<div>			

## Main Changes

This release is the **ALPHA DV of Bit and registers definition** for STM32MP13xx

## Contents

  - MMU : Clean mmu configuration file
  - SAES : Update SAES CR and SR registers
  - RCC : Remove deprecated register MP_AHB4ENSETR
  - TIMER : Update macros according to IP specifications
  - USB : update register definitions for USB OTG and USBPHYC
  - RTC : RTC_CFGR register is renamed RTC_OR ( to be aligned with specifications)
  - [MISRAC2012-Rule-10.6] Use 'UL' postfix for _Msk definitions and memory/peripheral base addresses

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V0.2.0 / 09-June-2021__</label>
<div>			

## Main Changes

This release is the **PRE-ALPHA DV of Bit and registers definition** aligned with the STM32MP13 reference manual

## Contents

- Update bit definition in header files:
  - PWR : add bit register PWR_CR1_MPU_RAM_LOWSPEED
  - SYSCFG : clean useless Flag
- Linker script : update size and offset for template file
- Update the licenses declaration

## Known Limitations

None

## Dependencies

None

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__V0.1.0 / 20-April-2021__</label>
<div>			

## Main Changes

This release is the **Bring Up DV of Bit and registers definition** aligned with the STM32MP13 reference manual

## Contents

- Support of stm32mp13xx:
  - Add “stm32mp13xx.h”file
  - Add part numbers list to stm32mp13xx.h header file:
    - STM32MP131Axx, STM32MP131Cxx, STM32MP131Dxx, STM32MP131Fxx
    - STM32MP133Axx, STM32MP133Cxx, STM32MP133Dxx, STM32MP133Fxx
    - STM32MP135Axx, STM32MP135Cxx, STM32MP135Dxx, STM32MP135Fxx
  - Add startup file “startup_stm32mp135c_ca7.c” for STM32CubeIDE toolchain
  - Add STM32CubeIDE linker file for all devices for legacy and for TrustZone based application

## Known Limitations

None

## Dependencies

None

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32 Microcontrollers </mark> ,
visit: [[www.st.com/stm32](http://www.st.com/stm32)]{style="font-color: blue;"}
</footer>
