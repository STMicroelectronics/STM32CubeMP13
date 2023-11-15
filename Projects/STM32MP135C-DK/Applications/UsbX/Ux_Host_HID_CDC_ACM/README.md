
## <b>Ux_Host_HID_CDC_ACM application description </b>

This application provides an example of Azure RTOS USBX stack usage on STM32MP135C-DK.
It shows how to develop a composite "USB HID CDC_ACM Host" that is able to enumerate and communicate with:

  - Composite USB HID CDC_ACM device
  - Simple HID/CDC_ACM device,

The application is designed to behave as a composite USB HID CDC_ACM Host, the code provides the required requests to properly manage the enumeration process of a compisite HID CDC_ACM device or a simple HID/CDC_ACM device.
In case of composite or a simple device, the CDC_ACM Class APIs is used to send, receive and display data on uart terminal. Also an HID Class APIs is used to decode HID reports received from a mouse or a keyboard and display data on uart terminal.

The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all USBx resources are initialized.
The application creates 5 threads with different priorities :

  - usbx_app_thread_entry            (Priority : 25; Preemption threshold : 25) used to initialize USB OTG HAL HCD driver and start the Host.
  - cdc_acm_send_app_thread_entry    (Priority : 30; Preemption threshold : 30) used to send data from host to device.
  - cdc_acm_recieve_app_thread_entry (Priority : 30; Preemption threshold : 30) used to receive data from the device.
  - hid_mouse_thread_entry           (Priority : 30; Preemption threshold : 30) used to decode HID reports received  from a mouse.
  - hid_keyboard_thread_entry        (Priority : 30; Preemption threshold : 30) used to decode HID reports received  from a keyboard.

#### <b> Expected success behavior</b>

- In case of composite HID CDC_ACM device or a simple HID/CDC_ACM is plugged to STM32MP135C-DK board, a message will be displayed on the uart console(ST-Link VCP) showing
the Vendor ID and Product ID of the attached device. The host should be able to properly decode composite HID CDC_ACM class or simple HID/CDC_ACM request data sent by the device.
After enumeration phase, the device will be ready to use.

in case of a CDC_ACM
   - The Sent/received data are displayed on respective terminals.

in case of a mouse:
   - (x,y) mouse position
   - Wheel position
   - Pressed mouse buttons

in case of a keyboard:
   - Pressed key

in case of Composite HID CDC_ACM
   - The Sent/received data are displayed on respective terminals.
   - (x,y) mouse position, Wheel position, Pressed mouse buttons or Pressed key in case of a Keyboard

#### <b>Error behaviors</b>

Errors are detected such as (Unsupported device, Enumeration Fail) and the corresponding message is displayed on the HyperTerminal.

#### <b>Assumptions if any</b>

User is familiar with USB 2.0 "Universal Serial BUS" Specification and CDC_ACM class Specification.

#### <b>Known limitations</b>


#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.s" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).

#### <b>USBX usage hints</b>

  - Should make sure to configure the USB pool memory region with attribute "Non-Cacheable" to ensure coherency between CPU and USB DMA

### <b>Keywords</b>

Connectivity, USBX Host, ThreadX, USB, Composite, CDC_ACM, HID, Mouse, Keyboard UART, USART,


### <b>Hardware and Software environment</b>

  - This application runs on STM32MP135xx devices
  - This application has been tested with STMicroelectronics STM32MP135C-DK.
    and can be easily tailored to any other supported device and development board.

- STM32MP135C-DK Set-up
    - Plug the USB CDC_ACM device into the STM32MP135C-DK board CN7 "TYPE-C" port (use Type-C to Type-A-Receptacle adapter)
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.

    A virtual COM port will then appear in the HyperTerminal:
     - Hyperterminal configuration
       - Data Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - Flow control: None

    A second virtual COM port will then appear in the HyperTerminal when plugging the device:
     - Hyperterminal configuration
       - Data Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 9600 baud
       - Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Open the configured uart HyperTerminal in order to display debug messages.
 - Run the application

<b>Notes</b>

- The user has to check the list of the COM ports in Device Manager to find out the number of the COM ports that have been assigned (by OS) to the ST-Link VCP.
