
## <b>Ux_Device_HID application description</b>

This application provides an example of Azure RTOS USBX stack usage on STM32MP135F-DK board,
it shows how to develop USB Device Human Interface "HID" mouse based application.

The application is designed to emulate an USB HID mouse device, the code provides all required device descriptors framework
and associated Class descriptor report to build a compliant USB HID mouse device.

At the beginning ThreadX call the entry function tx_application_define(), at this stage, all USBx resources
are initialized, the HID Class driver is registered and the application creates 2 threads with the same priorities :

  - usbx_app_thread_entry (Prio : 20; PreemptionPrio : 20) used to initialize USB OTG HAL PCD driver and start the device.
  - usbx_hid_thread_entry (Prio : 20; PreemptionPrio : 20) used to send HID reports to move automatically the PC host machine cursor.

To customize the HID application by sending the mouse position step by step every 10ms.
For each 10ms, the application calls the GetPointerData() API to update the mouse position (x, y) and send
the report buffer through the ux_device_class_hid_event_set() API.

#### <b>Expected success behavior</b>

When plugged to PC host, the STM32MP135F-DK must be properly enumerated as an USB HID mouse device.
During the enumeration phase, device provides host with the requested descriptors (Device, configuration, string).
Those descriptors are used by host driver to identify the device capabilities.
Once the STM32MP135F-DK USB device successfully completed the enumeration phase, the device sends a HID report after a USER1 button press.
Each report sent should move the mouse cursor by one step on host side.

In this application :
  - One EXTI line (EXTI14) is configured to generate an interrupt on each falling edge.
  - When falling edge is detected by pressing USER1 push-button, the User_Button_State variable should be updated.

#### <b>Error behaviors</b>

Host PC shows that USB device does not operate as designed (Mouse enumeration failed, the mouse pointer doesn't move).

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" Specification and HID class Specification.

#### <b>Known limitations</b>


### <b>Notes</b>

 - The DDR memory need to be initialized with DDR_Init project.

On STM32MP135F-DK:
  - EXTI14 is connected to USER1 push-button.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
   
#### <b>USBX usage hints</b>


### <b>Keywords</b>

RTOS, ThreadX, USBXDevice, USB_OTG, High Speed, HID, Mouse,

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP135xx devices.
  - This example has been tested with STMicroelectronics STM32MP135F-DK boards and can be easily tailored to any other supported device and development board.
  - Connect the STM32MP135F-DK board CN7 to the PC through "MICRO-USB" to "Standard A" cable.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild and run DDR_Init example to initialize the DDR memory
 - Rebuild all files of Ux_Device_HID application and load your image into target memory
 - Run the application
