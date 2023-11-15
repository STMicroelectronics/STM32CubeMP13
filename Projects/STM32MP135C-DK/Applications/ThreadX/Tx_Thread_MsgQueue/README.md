
##  <b>Tx_Thread_MsgQueue application description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX message queue APIs.
It demonstrates how to send and receive messages between threads using ThreadX message queue APIs. In addition, it shows how to use the event chaining feature.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, the application creates 3 threads with different
priorities and 2 message queues :

  - MsgSenderThreadOne (Priority : 5; Preemption Threshold : 5)
  - MsgSenderThreadTwo (Priority : 5; Preemption Threshold : 5)
  - MsgReceiverThread (Priority : 10; Preemption Threshold : 10)
  - MsgQueueOne (shared by MsgSenderThreadOne and MsgReceiverThread)
  - MsgQueueTwo (shared by MsgSenderThreadTwo and MsgReceiverThread)

<i> MsgSenderThreadOne</i> puts the message <i>TOGGLE_LED</i> on <i>MsgQueueOne</i> each 200 ms.
<i>MsgSenderThreadTwo</i> puts the message <i>TOGGLE_LED</i> on <i>MsgQueueTwo</i> each 500 ms.
<i>MsgReceiverThread</i> listen on both message queues :

  - When a message is available on <i>MsgQueueOne</i>, the <i>GREEN LED</i> is Toggled once.
  - When a message is available on <i>MsgQueueTwo</i>, the <i>RED LED</i> is Toggled once.

####  <b>Expected success behavior</b>

<i>GREEN_LED</i> toggles every 200ms and <i>RED_LED</i> toggles every 500ms if the application is running successfully.

#### <b>Error behaviors</b>

<i>LED_RED</i> toggles every 1 second if any error occurs.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations </b>
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

RTOS, ThreadX, Thread, Message Queue, Event chaining

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135xx devices.
  - This example has been tested with STMicroelectronics STM32MP135F-DK boards.
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuid and run DDR_Init example to initialize the DDR memory
 - Rebuild all Tx_Thread_MsgQueue_A7 application files and load your image into target memory
 - Run the application