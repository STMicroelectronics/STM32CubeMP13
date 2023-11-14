
## <b>Nx_WebServer application description</b>

This application provides an example of Azure RTOS NetX Duo stack usage on STM32MP135F-DK board, it shows how to develop Web HTTP server based application.
The application is designed to load files and dynamic web pages stored in SD card using a Web HTTP server, the code provides all required features to build a compliant Web HTTP Server.
The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX and FileX resources are created.

 + A NX_PACKET_POOL **EthPool** is allocated

 + A NX_IP instance **IpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + A NX_WEB_HTTP_SERVER **HTTPServer** instance using that pool is initialized
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

 The application then creates 3 threads with different priorities:
 + **AppMainThread** (priority 10, PreemtionThreashold 10) : created with the TX_AUTO_START flag to start automatically.
 + **AppServerThread** (priority 5, PreemtionThreashold 5) : created with the TX_DONT_START flag to be started later.
 + **LedThread** (priority 15, PreemtionThreashold 15) : created with the TX_DONT_START flag to be started later.

The **AppMainThread** starts and perform the following actions:
  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppServerThread**

The **AppServerThread**, once started:
  + Fx_media_open.
  + Starts HTTP server.
  + Each command coming from client (browser) is treated on the callback webserver_request_notify_callback.

The **LedThread**, once resumed from the dashboard:
  + Blue Led is toggling & message printed on HyperTerminal.

#### <b>Expected success behavior</b>

 + The board IP address "IP@" is printed on the HyperTerminal
 + Home page is well displayed on the browser after entering the url: http://IP@/about.html
 + Dashboard is well displayed on the browser after entering the url: http://IP@/dashboard.html
 + On dashboard, You can command Blue LED and watch some infos about threadx and netxduo.


#### <b>Error behaviors</b>

If the WEB HTTP server is not successfully started, the blue LED stays OFF.
In case of other errors, the Web HTTP server does not operate as designed (Files stored in the SD card are not loaded in the web browser).

#### <b>Assumptions if any</b>

- The uSD card must be plugged before starting the application.
- Copy all the content of "NetXDuo/Nx_WebServer/Web_Content" into SD card before starting the application.
- The application is using the DHCP to acquire IP address, thus a DHCP server should be reachable by the board in the LAN used to test the application.
- The application is configuring the Ethernet IP with a static predefined <i>MAC Address</i>, make sure to change it in case multiple boards are connected on the same LAN to avoid any potential network traffic issues.
- The application does not support Ethernet cable hot-plug, therefore the board should be connected to the LAN before running the application.
- The MAC Address is defined in the `main.c`

```
void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
```

#### <b>Known limitations</b>

Hotplug is not implemented for this example, that is, the SD card is expected to be inserted before application running.

### <b>Notes</b>

#### <b>Prerequisites</b>
Before running the application :
- The DDR memory need to be initialized with DDR_Init example.
- The STM32MP135F-DK board should be connected to the LAN via the ethernet connector ETH1.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.s" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).

#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.

#### <b>NetX Duo usage hints</b>

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively  the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32h7xx_hal_conf.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.

### <b>Keywords</b>

RTOS, ThreadX, Network, Web HTPP Server, NetxDuo, FileX, File ,SDMMC, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32MP135xx devices.
  - This application has been tested with STMicroelectronics STM32MP135F-DK boards
    and can be easily tailored to any other supported device and development board.

  - This application uses UART4 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild and run DDR_Init example to initialize the DDR memory
 - Rebuild all files and load your image into target memory
 - For STM32CubeIDE, in order to keep DDR initialized, please remove "monitor reset" line in "Debug Configurations->Startup>Initialization Commands"
 - Run the application
