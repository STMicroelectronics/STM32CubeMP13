
## <b>Fx_NAND_Write_Read_File application description</b>

This application provides an example of Azure RTOS FileX and LevelX stacks usage on custom
board STM32MP13XX_CUSTOM_HW (doesn't run on STM32MP135F-DK board unless a custom board is added). It demonstrates how to create a Fat File system
on the NAND flash using FileX alongside LevelX. The application is designed to execute file operations on the Micron MT29F8G08ABACAH4
NAND flash device, the code provides all required software code for properly managing it.

 - This application has been tested using an internal custom test board embedding a Micron MT29F8G08ABACAH4 NAND flash device configured as below:

      - MemoryDataWidth = 8 bit
      - PageSize = 4096 bytes
      - SpareAreaSize = 224 bytes
      - BlockSize = 64 pages
      - BlockNbr = 4096
      - PlaneSize = 2048 (NAND memory plane size is measured in number of blocks )
      - PlaneNbr = 2
  - The NAND ECC algorithm is configured in stm32mp13xx_valid_config.h.
    BCH-8 algorithm is used by default but you can change it by enabling:
      - BSP_NAND_ECC_ALGO_HAMMING or BSP_NAND_ECC_ALGO_BCH, to choose between Hamming or BCH algorithm
      - BSP_NAND_ECC_BCH_MODE_BCH4 or BSP_NAND_ECC_BCH_MODE_BCH8, if BSP_NAND_ECC_ALGO_BCH is also defined, to choose between BCH-4 or BCH-8
      - BSP_NAND_ECC_SECTOR_SIZE, if BSP_NAND_ECC_ALGO_HAMMING is also defined, to choose the size of the sector (from 256 to 8192 bytes)
  
  - User should have attention when configuring timings for the NAND memory. for more details about the timings please refer to AN4761.

  - The timings ( ComSpaceTiming and AttSpaceTiming) and GPIOs are configured in the BSP files

The application starts by calling the ThreadX's initialization routine which executes the main thread that handles file operations. 
At this stage, all FileX resources are created, the NAND flash is initialized and a single thread is created:

  - fx_thread (Prio : 1; PreemptionPrio : 1) used for file operations.

The fx_thread will start by formatting the NAND Flash using FileX services. The resulting file system is a FAT32 compatible, with 512 bytes per sector and 1 sector per cluster. 
Optionally, the NAND flash can be erased prior to format, this allows LevelX and FileX to create a clean FAT FileSystem. To enable flash mass erase, 
please set the following flag in "lx_stm32_nand_fmc_driver.h":

Format the NAND : Hold the 'USER' button pressed while booting to format the NAND

Note: When formatting the NAND Flash , sector size parameter should always equal the page size of the underlying NAND hardware.

Upon successful opening of the flash media, FileX continue with creating a file called "STM32.TXT" into the root directory, then write into it some dummy data. Then file is re-opened in read only mode and content is checked.

Through all the steps, FileX/LevelX services are called to print the flash size available before and after the example file is written into the flash. The number of occupied sectors is also shown.

#### <b>Expected success behavior</b>

Successful operation is marked by a toggling BLUE LED light.
Also, information regarding the total and available size of the flash media is printed to the serial port.

#### <b> Error behaviors</b>

On failure, the red LED starts toggling while the BLUE LED is switched OFF.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

Before starting the application :
 - The DDR memory need to be initialized with DDR_Init project.
  
#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
               
               
#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- When calling the fx_media_format() API, total sectors number should be one full block size of sectors less than the NAND flash provides. This will help ensure best performance during the wear level processing.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.


### <b>Keywords</b>

RTOS, ThreadX, FileX, LevelX, File System, NAND, FMC, FAT32

### <b>Hardware and Software environment</b>

  - This application runs on STM32MP13xx devices.
  - This application has been tested using an internal custom test board (STM32MP13XX_CUSTOM_HW) embedding a Micron MT29F8G08ABACAH4 NAND flash device and can be easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
 