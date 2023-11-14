##  <b>Nx_TCP_Echo_Server application description</b>

This application provides an example of Azure RTOS NetX/NetXDuo stack usage.
It shows how to develop a NetX TCP server to communicate with a remote client using
the NetX TCP socket API.

The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX resources are created.

 + A <i>NX_PACKET_POOL</i>is allocated
 + A <i>NX_IP</i> instance using that pool is initialized
 + The <i>ARP</i>, <i>ICMP</i>, <i>UDP</i> and <i>TCP</i> protocols are enabled for the <i>NX_IP</i> instance
 + A <i>DHCP client is created.</i>

The application then creates 2 threads with the same priorities:

 + **AppMainThread** (priority 10, PreemtionThreashold 10) : created with the <i>TX_AUTO_START</i> flag to start automatically.
 + **AppTCPThread** (priority 10, PreemtionThreashold 10) : created with the <i>TX_DONT_START</i> flag to be started later.

The **AppMainThread** starts and perform the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppTCPThread**

The **AppTCPThread**, once started:

  + Creates a <i>TCP</i> server socket
  + Listen indefinitely on new client connections.
  + As soon as a new connection is established, the <i>TCP</i> server socket starts receiving data packets from the client and prints the data on the HyperTerminal, then resend the same packet to the client.
  + At each received message the blue led is toggled.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + The response messages sent by the server are printed on the HyerTerminal
 + If the [echotool](https://github.com/PavelBansky/EchoTool/releases/tag/v1.5.0.0) utility  messages similar to the shown below can be seen on the console:

```
Reply from 10.48.1.254:6000, time 0 ms OK
Reply from 10.48.1.254:6000, time 0 ms OK
Reply from 10.48.1.254:6000, time 0 ms OK
Reply from 10.48.1.254:6000, time 0 ms OK
Reply from 10.48.1.254:6000, time 4 ms OK
Reply from 10.48.1.254:6000, time 0 ms OK

```

#### <b>Error behaviors</b>

+ The Red LED is toggling to indicate any error that have occurred.
+ In case the message exchange is not completed the HyperTerminal is not printing the received messages.

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
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
```
#### <b>Known limitations</b>
None

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

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively  the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32mp13xx_hal_conf.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, TCP, UART

### <b>Hardware and Software environment</b>

  - This example runs on STM32MP13xx devices
  - This example has been tested with STMicroelectronics STM32MP135F-DK board
    and can be easily tailored to any other supported device and development board.

  - This application uses USART4 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Optionally edit the file <code> NetXDuo/App/app_netxduo.h</code> and update the <i>DEFAULT_PORT</i> to connect on.
 - Rebuild and run DDR_Init example to initialize the DDR memory
 - Rebuild all files and load your image into target memory
 - For STM32CubeIDE, in order to keep DDR initialized, please remove "monitor reset" line in "Debug Configurations->Startup>Initialization Commands"
 - Run the application
 - Run the [echotool](https://github.com/PavelBansky/EchoTool/releases/tag/v1.5.0.0) utility on a windows console as following:

       c:\> echotool.exe  <the board IP address> /p tcp  /r  <DEFAULT_PORT> /n 10 /d "Hello World"

       Example : c:\> echotool.exe 10.48.1.254 /p tcp /r 6000 /n 10 /d "Hello World"
