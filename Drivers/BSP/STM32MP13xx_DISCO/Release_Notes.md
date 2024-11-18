---
pagetitle: Release Notes for STM32MP13xx_DISCO BSP Drivers
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---
::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>STM32MP13xx_DISCO BSP Drivers</mark>
Copyright &copy; 2023 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Purpose

The BSP (Board Specific Package) drivers are parts of the STM32Cube package based on the HAL drivers and provide a set of high level APIs relative to the hardware components and features in the disco boards coming with the STM32Cube package for a given STM32 series.

The BSP drivers allow a quick access to the boards’ services using high level APIs and without any specific configuration as the link with the HAL and the external components is done in intrinsic within the drivers.

From project settings points of view, user has only to add the necessary driver’s files in the workspace and call the needed functions from examples. However some low level configuration functions are weak and can be overridden by the applications if user wants to change some BSP drivers default behavior.

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section0" checked aria-hidden="true">
<label for="collapse-section0" aria-hidden="true">__V1.2.0 / 30-Oct-2024__</label>
<div>			

## Main Changes

**Maintenance DV** of STMP32MP13xx_DISCO BSP

## Contents

- No update

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section9" aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">__V1.1.0 / 23-Feb-2024__</label>
<div>			

## Main Changes

**Major Maintenance DV** of STMP32MP13xx_DISCO BSP

## Contents

- TouchScreen support added with gt911 component

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">__V1.0.0 / 01-November-2023__</label>
<div>			

## Main Changes

**Mass Market DV** of STMP32MP13xx_DISCO BSP

## Contents

- Mirror, Flip and Special effect modes added in GC2145 Component

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::



::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">__V0.7.0 / 27-June-2023__</label>
<div>

**5th ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

- Support added for GC2145 and STMIPID02

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__V0.6.0 / 16-December-2022__</label>
<div>			

## Main Changes

**4th ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

- No Update

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::
		
::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V0.5.0 / 29-June-2022__</label>
<div>	

## Main Changes

**3rd ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

- Fix some MISRAC2012 issues

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::
		
::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__V0.4.0 / 04-March-2022__</label>
<div>	

## Main Changes

**2nd ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

- STPMIC file: VDDCPU nominal voltage is 1.25 V (and not 1.20 V)
- Add BSP_Error_Handler() to manage BSP Errors
- LCD file : Fix missing LCD clock

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V0.3.0 / 03-December-2021__</label>
<div>			

## Main Changes

**ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

- Fix build issue when PMIC flag not enabled
- Add BSP support for IO expander mcp23017
- Add TAMPER_BUTTON support
- Add BSP support for LCD Driver

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V0.2.0 / 09-June-2021__</label>
<div>			

## Main Changes

**PRE-ALPHA DV** of STMP32MP13xx_DISCO BSP

## Contents

Update the licenses declaration

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__V0.1.0 / 20-April-2021__</label>
<div>			

## Main Changes

**Bring Up DV** of STM32MP13xx_DISCO BSP

## Contents

This release is the Bring Up DV of STM32MP13xx_DISCO BSP drivers

## Notes

stm32mp13xx_disco_conf_template.h file must be copied in user application as >stm32mp13xx_disco_conf.h with optional configuration update

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [[www.st.com](http://www.st.com/STM32)]{style="font-color: blue;"}
</footer>
