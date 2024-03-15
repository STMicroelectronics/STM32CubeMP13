---
pagetitle: Release Notes for Projects of STM32CubeMP13 Firmware Package
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>\ STM32CubeMP13 Firmware Projects\ </mark>
Copyright &copy; 2023 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>


# Purpose

The projects are a set of applicative tasks that aim to explain and provide use
cases of the different products features based on the products hardware (Boards,
STM32 Core(s) features, interconnections and peripherals) and built around the
different firmware components. They can be directly used or adapted by the
STM32Cube13 Firmware Package users.
They provide basic functionalities, show main features or demonstrate
performance. They are provided with preconfigured projects for the main
supported toolchains.

In the STM32CubeMP13 Firmware Package, the projects are organized in a sequential scheme based on the following levels:

- Boards : Hardware platform (discovery boards)
- Project Category:
  - Applications
  - Examples
  - External Loader
  - Templates

The exhaustive list of projects is provided in this table:
[STM32CubeProjectsList](STM32CubeProjectsList.html).

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section0" checked aria-hidden="true">
<label for="collapse-section0" checked aria-hidden="true">__V1.1.0 / 23-Feb-2023__</label>
<div>

## Main Changes

**Major Maintenance DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series)

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                       Path
 -------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP135F-DK       Examples                 []{.icon-st-update}          Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications             []{.icon-st-update}          Projects/STM32MP135C-DK/Applications
   STM32MP135F-DK       External Loader          []{.icon-st-update}          Projects/STM32MP135C-DK/External_Loader   
   STM32MP135F-DK       Templates                []{.icon-st-update}	      Projects/STM32MP135C-DK/Templates          

 
 - TouchScreen support BSP examples added for G911 component.
 - Cube IDE toolchain GCC 12.3 support added.
 - IAR porting on Examples is done, Applications and Templates not yet ported for IAR
 - Added support for boot from SD and using SDCard as storage medium at same time in SD_Ext_Loader.
 - Example added to demonstrate Cryp SAES shared key feature. 


  
 : Projects **STM32MP13XX_CUSTOM_HW** 

 Board                      Project category          Version                       Path
 ------------------------   ------------------------  ---------------------       ------------------------------------------
   STM32MP13XX_CUSTOM_HW     Examples                 []{.icon-st-update}  		   Projects/STM32MP13XX_CUSTOM_HW/Examples                                                                  
   STM32MP13XX_CUSTOM_HW     Applications             []{.icon-st-update}          Projects/STM32MP13XX_CUSTOM_HW/Applications                                                                                         
   STM32MP13XX_CUSTOM_HW     External Loader          []{.icon-st-update}          Projects/STM32MP13XX_CUSTOM_HW/External_Loader                                                             
   STM32MP13XX_CUSTOM_HW     Templates                []{.icon-st-update}  		   Projects/STM32MP13XX_CUSTOM_HW/Templates                                                                           

 - Addition of External Loader for eMMC flash using cube programmer for custom board.
 - FSBLA eMMC(FSBLA_Sdmmc2) application added for custom board.
 - FileX example added with eMMC support.

 **Notes**

- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to test some feature not available on STM32MP135F-DK Board : ex FMC-NAND or QSPi NOR or eMMC
- The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)
- Execution information :
  - Engineering boot mode:
    - All the examples could be run on SYSRAM memory, except LTDC_Display_2Layers, DCMIPP, TouchScreen(the binary size of these examples are larger than SYSRAM Size) => For these examples, DDR_Init should be run before.
    - For the same reason, all the AzureRTOS applications should be run after DDR_Init example.
  - SDCard boot mode:
    - The applications or examples (CubeExample) could be executed from SDCard. For the instance, the FSBLA_Sdmmc and the CubeExample should be copied using cube programmer 
	  (External_Loader is included in form of binaries for this purpose) or manually in the SD Card. 
      - At the boot, the bootrom will copy the FSBLA_SDMMC in SYSRAM and execute it,
      - FSBLA_Sdmmc:
        - will initialize the clocks, the SDMMC and the DDR,
        - will read from SDMMC1 the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample
		
  - NOR boot mode (not possible on STM32MP135F_DK board because no QSPI_NOR available, but feature has been validated with internal ST Board customized - STM32MP13XX_CUSTOM_HW)
    - The applications or examples (CubeExample) could be executed from NOR. For the instance, the FSBLA_SNOR and the CubeExample could be copied using CubeProgrammer 
	  (External_Loader is included in form of binaries for this purpose) or with the provided debug tool "QSPI_NOR_Programmer". 
      - At the boot, the bootrom will copy the FSBLA_SNOR in SYSRAM and execute it,
      - FSBLA_SNOR:
        - will initialize the clocks, the QSPI-NOR and the DDR,
        - will read from QSPI-NOR the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample

  - eMMC boot mode:
    - The applications or examples (CubeExample) could be executed from eMMC(SDMMC2). For the instance, the FSBLA_eMMC and the CubeExample should be copied 
	  using cube programmer(External_Loader is included in form of binaries for this purpose) in the eMMC. 
      - At the boot, the bootrom will copy the FSBLA_eMMC in SYSRAM and execute it,
      - FSBLA_eMMC:
        - will initialize the clocks, the SDMMC2(eMMC) and the DDR,
        - will read from eMMC the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample

## Development Toolchains and Compilers
- STM32CubeIDE V1.15.0
- IAR Embedded Workbench for ARM (EWARM): v9.50

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- The projects are not generated with STM32CubeMX tool.
- IAR ported examples are not provided for all project and some IAR example compile with warnings but run Ok.
- IAR does not have option to connect under reset for st-link probe yet in IAR 9.50. 

## Backward Compatibility

- Not applicable

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" checked aria-hidden="true">__V1.0.0 / 01-Nov-2023__</label>
<div>

## Main Changes

**Mass Market DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series)

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                       Path
 -------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP135F-DK       Examples                 []{.icon-st-update}          Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications             []{.icon-st-update}          Projects/STM32MP135C-DK/Applications
   STM32MP135F-DK       External Loader          []{.icon-st-add}             Projects/STM32MP135C-DK/External_Loader   
   STM32MP135F-DK       Templates                []{.icon-st-update}	      Projects/STM32MP135C-DK/Templates          

 
 - MMU & CACHE has been enabled for all examples, applications and Templates.
 - Addition of External Loader for SDCard flash using cube programmer.  


  
 : Projects **STM32MP13XX_CUSTOM_HW** 

 Board                      Project category          Version                       Path
 ------------------------   ------------------------  ---------------------       ------------------------------------------
   STM32MP13XX_CUSTOM_HW     Examples                 []{.icon-st-update}  		   Projects/STM32MP13XX_CUSTOM_HW/Examples                                                                  
   STM32MP13XX_CUSTOM_HW     Applications             []{.icon-st-update}          Projects/STM32MP13XX_CUSTOM_HW/Applications                                                                                         
   STM32MP13XX_CUSTOM_HW     External Loader          []{.icon-st-add}             Projects/STM32MP13XX_CUSTOM_HW/External_Loader                                                             
   STM32MP13XX_CUSTOM_HW     Templates                []{.icon-st-update}  		   Projects/STM32MP13XX_CUSTOM_HW/Templates                                                                           

 - Quality related fixes for all examples and applications and addition of External Loader for XSPI flash using cube programmer.
 - QSPI dependencies are replaced with XSPI

 **Notes**

- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to test some feature not available on STM32MP135F-DK Board : ex FMC-NAND or QSPi NOR
- The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)
- Execution information :
  - Engineering boot mode:
    - All the examples could be run on SYSRAM memory, except LTDC_Display_2Layers (the binary size of this example is larger than SYSRAM Size) => For this example, DDR_Init should be run before.
    - For the same reason, all the AzureRTOS applications should be run after DDR_Init example.
  - SDCard boot mode:
    - The applications or examples (CubeExample) could be executed from SDCard. For the instance, the FSBLA_Sdmmc and the CubeExample should be copied manually in the SD Card. 
      - At the boot, the bootrom will copy the FSBLA_SDMMC in SYSRAM and execute it,
      - FSBLA_Sdmmc:
        - will initialize the clocks, the SDMMC and the DDR,
        - will read from SDMMC1 the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample
  - NOR boot mode (not possible on STM32MP135F_DK board because no QSPI_NOR available, but feature has been validated with internal ST Board customized - STM32MP13XX_CUSTOM_HW)
    - The applications or examples (CubeExample) could be executed from NOR. For the instance, the FSBLA_SNOR and the CubeExample could be copied with the debug tool "QSPI_NOR_Programmer". 
      - At the boot, the bootrom will copy the FSBLA_SNOR in SYSRAM and execute it,
      - FSBLA_SNOR:
        - will initialize the clocks, the QSPI-NOR and the DDR,
        - will read from QSPI-NOR the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample

## Development Toolchains and Compilers
- STM32CubeIDE V1.12.1 (10.3-2021.10)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- The projects are not generated with STM32CubeMX tool.
- The projects are not yet supported with IAR Embedded Workbench for ARM (EWARM) toolchain.

## Backward Compatibility

- Not applicable

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__V0.7.0 / 27-June-2023__</label>
<div>

## Main Changes

**5th ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series)

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                       Path
 -------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP135F-DK       Examples                   []{.icon-st-update}          Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications               []{.icon-st-update}          Projects/STM32MP135C-DK/Applications          
   STM32MP135F-DK       Templates                  						        Projects/STM32MP135C-DK/Templates          

- 2 new examples implemented:
  - DCMIPP_CaptureMode_A7
  - DCMIPP_SnapshotMode_A7	
 
- 3 new application implemented:
  - Low_power_LPLV_Stop2
  - Low_power_Standby
  - Tx_MPU
 
 - MMU & CACHE has been enabled for LTDC, GPIO, ThreadX, USBX, Fx_SRAM_File_Edit_Standalone applications.
  


  
 : Projects **STM32MP13XX_CUSTOM_HW** 

 Board                      Project category          Version                       Path
 ------------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP13XX_CUSTOM_HW     Examples                   				            Projects/STM32MP13XX_CUSTOM_HW/Examples   
   STM32MP13XX_CUSTOM_HW     Applications               []{.icon-st-update}	        Projects/STM32MP13XX_CUSTOM_HW/Applications          
   STM32MP13XX_CUSTOM_HW     Templates                  				            Projects/STM32MP13XX_CUSTOM_HW/Templates          

- 1 application implemented:
  - Fx_Nand_Write_Read_File_Sequencer

 **Notes**

- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to test some feature not available on STM32MP135F-DK Board : ex FMC-NAND or QSPi NOR
- The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)
- Execution information :
  - Engineering boot mode:
    - All the examples could be run on SYSRAM memory, except LTDC_Display_2Layers (the binary size of this example is larger than SYSRAM Size) => For this example, DDR_Init should be run before.
    - For the same reason, all the AzureRTOS applications should be run after DDR_Init example.
  - SDCard boot mode:
    - The applications or examples (CubeExample) could be executed from SDCard. For the instance, the FSBLA_Sdmmc and the CubeExample should be copied manually in the SD Card. 
      - At the boot, the bootrom will copy the FSBLA_SDMMC in SYSRAM and execute it,
      - FSBLA_Sdmmc:
        - will initialize the clocks, the SDMMC and the DDR,
        - will read from SDMMC1 the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample
  - NOR boot mode (not possible on STM32MP135F_DK board because no QSPI_NOR available, but feature has been validated with internal ST Board customized - STM32MP13XX_CUSTOM_HW)
    - The applications or examples (CubeExample) could be executed from NOR. For the instance, the FSBLA_SNOR and the CubeExample could be copied with the debug tool "QSPI_NOR_Programmer". 
      - At the boot, the bootrom will copy the FSBLA_SNOR in SYSRAM and execute it,
      - FSBLA_SNOR:
        - will initialize the clocks, the QSPI-NOR and the DDR,
        - will read from QSPI-NOR the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample

## Development Toolchains and Compilers
- STM32CubeIDE V1.12.1 (10.3-2021.10)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- The projects are not generated with STM32CubeMX tool.
- The projects are not yet supported with IAR Embedded Workbench for ARM (EWARM) toolchain.


## Backward Compatibility

- Not applicable

</div>
:::


::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V0.6.0 / 16-December-2022__</label>
<div>

## Main Changes

**4th ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series)

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                       Path
 -------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP135F-DK       Examples                   []{.icon-st-update}          Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications               []{.icon-st-update}          Projects/STM32MP135C-DK/Applications          
   STM32MP135F-DK       Templates                                               Projects/STM32MP135C-DK/Templates          

- 6 new examples implemented:
  - ETZPC_NonSecure_Access
  - MDMA_Mem2MemTransfer	
  - MDMA_Mem2MemTransfer_Secure
  - PKA_ECCDoubleBaseLadder
  - PKA_ECDSA_Sign
  - USART_SlaveMode_DMA
- 1 new application implemented:
  - Nx_Iperf
  
 : Projects **STM32MP13XX_CUSTOM_HW** 

 Board                      Project category          Version                       Path
 ------------------------   ------------------------  ---------------------        ------------------------------------------
   STM32MP13XX_CUSTOM_HW     Examples                   []{.icon-st-add}            Projects/STM32MP13XX_CUSTOM_HW/Examples   
   STM32MP13XX_CUSTOM_HW     Applications               []{.icon-st-add}            Projects/STM32MP13XX_CUSTOM_HW/Applications          
   STM32MP13XX_CUSTOM_HW     Templates                  []{.icon-st-add}            Projects/STM32MP13XX_CUSTOM_HW/Templates          

- 1 Template implemented
- 1 example implemented:
  - DDR_Init to initialize LPDDR3)
- 4 applications implemented:
  - QSPI_NOR_PROGRAMMER : This application shows how to load a signed binary to an QSPI-NOR (MX25L51245G).
  - FSBLA_SNOR : This application describes how to load a cubeExample binary from QSPI-NOR (MX25L51245G) into DDR (LPDDR3), and execute it.
  - Fx_uSD_File_Edit : 	This application provides an example of Azure RTOS FileX stack usage on board, it shows how to develop a basic SD card file operations application
  - Fx_Nand_Write_Read_File : It demonstrates how to create a Fat File system on the NAND flash using FileX alongside LevelX (without ECC). The application is designed to execute file operations on the Micron MT29F8G08ABACAH4

 **Notes**

- STM32MP13XX_CUSTOM_HW is an internal ST Board customized to test some feature not available on STM32MP135F-DK Board : ex FMC-NAND or QSPi NOR
- The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)
- Execution information :
  - Engineering boot mode:
    - All the examples could be run on SYSRAM memory, except LTDC_Display_2Layers (the binary size of this example is larger than SYSRAM Size) => For this example, DDR_Init should be run before.
    - For the same reason, all the AzureRTOS applications should be run after DDR_Init example.
  - SDCard boot mode:
    - The applications or examples (CubeExample) could be executed from SDCard. For the instance, the FSBLA_Sdmmc and the CubeExample should be copied manually in the SD Card. 
      - At the boot, the bootrom will copy the FSBLA_SDMMC in SYSRAM and execute it,
      - FSBLA_Sdmmc:
        - will initialize the clocks, the SDMMC and the DDR,
        - will read from SDMMC1 the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample
  - NOR boot mode (not possible on STM32MP135F_DK board because no QSPI_NOR available, but feature has been validated with internal ST Board customized - STM32MP13XX_CUSTOM_HW)
    - The applications or examples (CubeExample) could be executed from NOR. For the instance, the FSBLA_SNOR and the CubeExample could be copied with the debug tool "QSPI_NOR_Programmer". 
      - At the boot, the bootrom will copy the FSBLA_SNOR in SYSRAM and execute it,
      - FSBLA_SNOR:
        - will initialize the clocks, the QSPI-NOR and the DDR,
        - will read from QSPI-NOR the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample

## Development Toolchains and Compilers
- STM32CubeIDE V1.11.0 (10.3-2021.10)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- The projects are not generated with STM32CubeMX tool.
- The projects are not yet supported with IAR Embedded Workbench for ARM (EWARM) toolchain.


## Backward Compatibility

- Not applicable

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__V0.5.0 / 29-June-2022__</label>
<div>			

## Main Changes

**3rd ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series)

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                             Path
 -------------------   ------------------------  -----------                        ------------------------------------------
   STM32MP135F-DK       Examples                   V0.5.0 []{.icon-st-add}             Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications               V0.5.0 []{.icon-st-add}             Projects/STM32MP135C-DK/Applications          
   STM32MP135F-DK       Templates                  V0.5.0 []{.icon-st-update}          Projects/STM32MP135C-DK/Templates          

- New example "ADC_MultiChannelSingleConversion" implemented
- Implement AzureRTOS applications:
  - FileX
  - ThreadX
  - USBX
  - NetXDUO
- Implement FSBLA_SDMMC application (to load and execute a cubeExample binary from SDCARD into DDR on STM32MP135F-DK (DDR3).)

 **Notes**

- The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)
- Execution information :
  - Engineering boot mode:
    - All the examples could be run on SYSRAM memory, except LTDC_Display_2Layers (the binary size of this example is larger than SYSRAM Size) => For this example, DDR_Init should be run before.
    - For the same reason, all the AzureRTOS applications should be run after DDR_Init example.
  - SDCard boot mode:
    - The applications or examples (CubeExample) could be executed from SDCard. For the instance, the FSBLA_Sdmmc and the CubeExample should be copied manually in the SD Card. 
      - At the boot, the bootrom will copy the FSBLA_SDMMC in SYSRAM and execute it,
      - FSBLA_Sdmmc:
        - will initialize the clocks, the SDMMC and the DDR,
        - will read from SDMMC1 the CubeExample image length and load CubeExample into DDR.
        - will jump into DDR to execute this CubeExample
 

## Development Toolchains and Compilers
-  STM32CubeIDE V1.10.0 (10.3-2021.10)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- This software is compatible with STM32CubeIDE v1.10.0 version ( compilation issue with version < v1.10.0)

## Known Limitations

- NetXduo applications not yet at ALPHA Level: these limitations will be fixed in future releases.
  - The NetXduo applications lead to potential instabilities and performance issues. 
  - LED management for status to update
- The projects are not generated with STM32CubeMX tool.
- The projects are not yet supported with IAR Embedded Workbench for ARM (EWARM) toolchain.


## Backward Compatibility

- Not applicable

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__V0.4.0 / 04-March-2022__</label>
<div>			

## Main Changes

**2nd ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series) updated for silicon

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                             Path
 -------------------   ------------------------  -----------                        ------------------------------------------
   STM32MP135F-DK       Examples                   V0.4.0 []{.icon-st-add}             Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications               V0.4.0 []{.icon-st-update}          Projects/STM32MP135C-DK/Applications          
   STM32MP135F-DK       Templates                  V0.4.0 []{.icon-st-update}          Projects/STM32MP135C-DK/Templates          

- New example "CRYP_SAES_MDMA_ECB_CBC" implemented
- Other examples reworked

 **Notes**
 
 - The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- None

## Backward Compatibility

- Not applicable

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V0.3.0 / 03-December-2021__</label>
<div>			

## Main Changes

**ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series) updated for silicon

## Contents

### Projects
<small>The components flagged by "[]{.icon-st-update}" have changed since the
previous release. "[]{.icon-st-add}" are new.</small>

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version                             Path
 -------------------   ------------------------  -----------                        ------------------------------------------
   STM32MP135F-DK       Examples                   V0.3.0 []{.icon-st-add}             Projects/STM32MP135C-DK/Examples   
   STM32MP135F-DK       Applications               V0.3.0 []{.icon-st-add}             Projects/STM32MP135C-DK/Applications          
   STM32MP135F-DK       Templates                  V0.3.0 []{.icon-st-update}          Projects/STM32MP135C-DK/Templates           

 **Notes**
 
 - The exhaustive list of projects is provided in this table [STM32CubeProjectsList.html](STM32CubeProjectsList.html)

## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discovery board

## Dependencies

- None

## Known Limitations

- None

## Backward Compatibility

- Not applicable
</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V0.2.0 / 09-June-2021__</label>
<div>			

## Main Changes

**PRE-ALPHA DV** of STM32CubeMP13 Projects (STM32Cube for STM32MP13 Series) updated for silicon

## Contents

### Projects

 : Projects **STM32MP135C-DK**

 Board                 Project category          Version        Path
 -------------------   ------------------------  -----------  ------------------------------------------
   STM32MP135C-DK       Templates                  V0.2.0        Projects/STM32MP135C-DK/Templates          


## Supported Devices and boards
-   STM32MP131 / STM32MP133 and STM32MP135 devices
-   STM32MP135F Discover board

## Dependencies

- None

## Known Limitations

- None

## Backward Compatibility

- Not applicable
</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on <mark>STM32 Microcontrollers</mark> ,
visit: [[www.st.com](http://www.st.com/STM32)]{style="font-color: blue;"}
</footer>