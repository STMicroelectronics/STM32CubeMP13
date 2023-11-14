
## <b>Tx_Thread_Creation application description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX thread management APIs.
It demonstrates how to create and destroy multiple threads using Azure RTOS ThreadX APIs. In addition, it shows how to use preemption threshold between threads and change priorities on-fly.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, the application creates 3 threads with different priorities :

  - MainThread (Prio : 5; Preemption Threshold : 5)
  - ThreadOne (Prio : 10; Preemption Threshold : 9)
  - ThreadTwo (Prio : 10; Preemption Threshold : 10)

Once started, the <b>MainThread</b> is suspended waiting for the event flag.
The *ThreadOne* starts to toggle the *LED_GREEN* each 500ms and <b>ThreadTwo</b> cannot as its priority is less than the *ThreadOne* threshold.
After 5 seconds it sends an event *THREAD_ONE_EVT* to the <b>MainThread</b>.

After receiving the *THREAD_ONE_EVT*, the <b>MainThread</b> change the <b>ThreadTwo</b> priority to 8 and its preemption threshold to 8 to be more than the *ThreadOne* threshold then waits for an event.

Now, the <b>ThreadTwo</b> can preempt the <b>ThreadOne</b> and start toggling the *LED_GREEN* each 200ms for 5 seconds. Once done it send the <b>THREAD_TWO_EVT</b> to <b>MainThread</b>.
Once *ThreadTwo_Evt* is received, the <b>MainThread</b> resets the <b>ThreadTwo</b> priority and preemption threshold to their original values (10, 10), <b>ThreadOne</b> is rescheduled and the above scenario is redone.
After repeating the sequence above 3 times, the <b>MainThread</b> should destroy <b>ThreadOne</b> and <b>ThreadTwo</b> and toggles the *LED_GREEN* each 1 second for ever.

####  <b>Expected success behavior</b>

  - LED_GREEN toggles every 500ms for 5 seconds
  - LED_GREEN toggles every 200ms for 5 seconds
  - Success status (After 3 times) :  'LED_GREEN' toggles every 1 second for ever.

#### <b>Error behaviors</b>

LED_RED toggles every 1 second if any error occurs.

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

RTOS, ThreadX, Thread, Event flags, Preemption threshold

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135xx devices
  - This example has been tested with STMicroelectronics STM32MP135F-DK boards.
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all Tx_Thread_Creation_A7 application files and load your image into target memory
 - Run the application
