---
pagetitle: Release Notes for LCD Utility Drivers
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>LCD Utility Drivers</mark>
Copyright &copy; 2019 STMicroelectronics\

[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>


# Purpose

This driver provides a set of basic 2D display functions common for all LCDs. It can be used on top of BSP LCD drivers offering the following basic APIs:

-	BSP_LCD_DrawBitmap
-	BSP_LCD_FillRGBRect
-	BSP_LCD_DrawHLine
-	BSP_LCD_DrawVLine
-	BSP_LCD_FillRect
-	BSP_LCD_ReadPixel
-	BSP_LCD_WritePixel
-	BSP_LCD_GetXSize
-	BSP_LCD_GetYSize
-	BSP_LCD_SetActiveLayer
-	BSP_LCD_GetPixelFormat

\

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section8" checked aria-hidden="true">
<label for="collapse-section8" checked aria-hidden="true">V2.1.0 / 12-February-2024</label>
<div>

## Main Changes

-	Add support for RGB888 and ARGB4444 color space
-   Fix multi-layer support with different PixelFormat and size

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" checked aria-hidden="true">V2.0.3 / 28-June-2023</label>
<div>

## Main Changes

-	Update license

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" checked aria-hidden="true">V2.0.2 / 09-February-2022</label>
<div>

## Main Changes

-	Update license

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" checked aria-hidden="true">V2.0.1 / 08-September-2020</label>
<div>

## Main Changes

-	Fix typo errors

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" checked aria-hidden="true">V2.0.0 / 25-February-2020</label>
<div>
	
## Main Changes

-	Rename basic_gui.c/.h into stm32_lcd.c/.h
-	Rename GUI_* APIs into UTILS_LCD_* APIs

## Backward compatibility

-	This version breaks the compatibility with the previous versions

## Dependencies

-	This software must be used with Components/common version v7.0.0 or greater. 

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" checked aria-hidden="true">V1.0.2 / 30-October-2019</label>
<div>

## Main Changes

-	Update st_logo.png inclusion path in Release notes

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" checked aria-hidden="true">V1.0.1 / 10-May-2019</label>
<div>

## Main Changes

-	Add LINE macro in basic_gui.h

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" checked aria-hidden="true">V1.0.0 / 12-April-2019</label>
<div>			
## Main Changes

### Component release

-	First Official release of Basic_GUI utilities drivers in line with STM32Cube BSP drivers development guidelines (UM2298)

## Dependencies

- This software release is compatible with BSP Common V6.0.0 or above

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32 Microcontrollers ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
