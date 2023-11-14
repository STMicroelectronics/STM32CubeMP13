
##  <b>Nx_Iperf application description</b>

This application provides an example of Azure RTOS NetXDuo stack usage .
It shows performance when using different modes : TCP_server, UDP_server, TCP_client and UDP_client.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all NetXDuo resources are created.

 + A NX_PACKET_POOL **AppPool** is allocated
 + A NX_IP instance **IpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

The application creates 1 thread :

 + **AppMainThread** (priority 4, PreemtionThreashold 4) : created with the <i>TX_AUTO_START</i> flag to start automatically.

The **AppMainThread** starts and perform the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the *nx_iperf_entry*

The **nx_iperf_entry**, once started:

  + Creates a NetXDuo Iperf demo web page.

The application then creates 4 threads with the same priorities :

   + **thread_tcp_rx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_tcp_tx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_rx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_tx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + When the Web HTTP server is successfully started.Then the user can test the performance on the web browser after entring the url http://@IP.
 + To execute each Iperf test you must do the following steps and have the expected result in this link https://docs.microsoft.com/en-us/azure/rtos/netx-duo/netx-duo-iperf/chapter3 .

#### <b>Error behaviors</b>

+ The Red LED is toggling to indicate any error that have occurred.

#### <b>Assumptions if any</b>

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
  heth.Init.MACAddr[0] =   0x00;
  heth.Init.MACAddr[1] =   0x80;
  heth.Init.MACAddr[2] =   0xE1;
  heth.Init.MACAddr[3] =   0x00;
  heth.Init.MACAddr[4] =   0x00;
  heth.Init.MACAddr[5] =   0x00;
```
#### <b>Known limitations</b>

  - The packet pool is not optimized. It can be less than that by reducing NX_PACKET_POOL_SIZE in file "app_netxduo.h" and NX_APP_MEM_POOL_SIZE in file "app_azure_rtos_config.h". This update can decrease NetXDuo performance.

### <b>Notes</b>

#### <b>Prerequisites</b>
Before running the application :
- The DDR memory need to be initialized with DDR_Init example.
- The STM32MP135F-DK board should be connected to the LAN via the ethernet connector ETH1.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).

#### <b>NetX Duo usage hints</b>

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, Iperf, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32MP135xx devices
  - This application has been tested with STMicroelectronics STM32MP135F-DK boards
    and can be easily tailored to any other supported device and development board.

  - This application uses UART4 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild and run DDR_Init example to initialize the DDR memory
 - Rebuild all files and load your image into target memory
 - For STM32CubeIDE, in order to keep DDR initialized, please remove "monitor reset" line in "Debug Configurations->Startup>Initialization Commands"
 - Run the application
 - Run the [iperf tool] by following steps described in this link https://docs.microsoft.com/en-us/azure/rtos/netx-duo/netx-duo-iperf/chapter3 .
