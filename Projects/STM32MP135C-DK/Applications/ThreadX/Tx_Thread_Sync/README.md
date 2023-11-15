
## <b>Tx_Thread_Sync application description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX synchronization APIs.
The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, the application creates 2 threads with the same priorities :

  - 'ThreadOne' (Priority : 10; Preemption Threshold : 10)
  - 'ThreadTwo' (Priority : 10; Preemption Threshold : 10)

The function "Led_Toggle()" is the entry function for both threads to toggle the leds.Therefore it is considered as a "critical section" that needs protection with a 'SyncObject' flag in the file "app_threadx.h"
Each thread is running in an infinite loop as following:

- 'ThreadOne':
  + try to acquire the 'SyncObject' immediately.
  + On Success toggle the 'LED_GREEN' each 500ms for 5 seconds.
  + Release the 'SyncObject'
  + Sleep for 10ms.
  + repeat the steps above

- 'ThreadTwo':
  + try to acquire the 'SyncObject' immediately.
  + On Success toggle the 'LED_RED' each 500ms for 5 seconds.
  + Release the 'SyncObject'
  + Sleep for 10ms.
  + Repeat the steps above.

By default the 'SyncObject' is defined as  "TX_MUTEX" .It is possible to use a binany "TX_SEMAPHORE" by tuning
the compile flags in the file "app_threadx.h".

#### <b>Expected success behavior</b>

  - 'LED_GREEN' toggles every 500ms for 5 seconds
  - 'LED_RED' toggles every 500ms for 5 seconds
  - Messages on HyperTerminal :
     + "** ThreadXXX : waiting for SyncObject !! **" : When thread is waiting for the SyncObject.
	 + "** ThreadXXX : waiting SyncObject released **" : When thread put the SyncObject.
	 + "** ThreadXXX : waiting SyncObject acquired **" : When thread get the SyncObject.

#### <b>Error behaviors</b>

'LED_RED' toggles every 1 second if any error occurs.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

 1. Before starting the application :
 - The DDR memory need to be initialized with DDR_Init project.
 
#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.s" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).

### <b>Keywords</b>

RTOS, ThreadX, Thread, Semaphore, Mutex


### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135xx devices
  - This example has been tested with STMicroelectronics STM32MP135F-DK boards
    and can be easily tailored to any other supported device and development board.
  - A virtual COM port appears in the HyperTerminal:
      - Hyperterminal configuration:
        + Data Length = 8 Bits
        + One Stop Bit
        + No parity
        + BaudRate = 115200 baud
        + Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all Tx_Thread_Sync_A7 application files and load your image into target memory
 - Run the application
