## <b>Fx_uSD_File_Edit application description</b>

This application provides an example of Azure RTOS FileX stack usage on STM32MP135F-DK board, it shows how to develop a basic SD card file

operations application. The application is designed to handle SD card insertion/removal events, and depending on that state, it starts

and stops file operations from and into the SD card.


The main entry function, tx_application_define(), is called by ThreadX during kernel start. At this stage, all FileX resources are initialized,

the SD card detection event is registered and drivers are initialized.

A single thread is created:

  - fx_thread (Prio : 10; PreemptionPrio : 10) used to initialize the SD driver and starting FileX's file operations.

A message queue is used to signal the SD card detection event to the fx_thread thread:

  - tx_msg_queue (Msg size : 1 (UINT); total messages : 16) used to notify the fx_thread about the SD card insertion status.

The fx_thread starts by checking whether the SD card is initially inserted or not. In the true case, it sends a message to the queue to ensure

that the first iteration starts properly. The wait on the message queue blocks till it receives a new message about whether the SD card is inserted

or removed. Interrupt callback for SD detection is registered and it is used to send the event information to the message queue.

The fx_thread uses FileX services to open the SD media for file operations and attempt to create file STM32.TXT. If the file exists already,

it will be overwritten. Dummy content is then written into the file and it is closed. The file is opened once again in read mode and content

is checked if matches what was previously written.

#### <b>Expected success behavior</b>

- A file named STM32.TXT should be visible in the root directory of the SD card.
- A blinking green LED light marks the success of the file operations.

#### <b>Error behaviors</b>

- On failure, the red LED should start blinking.
- Error handler is called at the spot where the error occurred.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

 1. Before starting the application :
 - The DDR memory need to be initialized with DDR_Init project.
 - The SD card should be present in the SD card connector and formatted initially by user.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.s" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).


#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.


### <b>Keywords</b>

RTOS, ThreadX, FileX, File system, SDMMC, FAT32

### <b>Hardware and Software environment</b>

  - This application runs on STM32MP135xx devices.
  - This application has been tested with STMicroelectronics STM32MP135F-DK boards.
    and can be easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all Fx_uSD_File_Edit_A7 files and load your image into target memory
 - Run the application
