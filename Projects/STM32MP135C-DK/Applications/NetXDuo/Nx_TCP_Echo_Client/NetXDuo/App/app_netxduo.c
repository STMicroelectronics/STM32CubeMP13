/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    app_netxduo.c
* @author  MCD Application Team
* @brief   NetXDuo application file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nx_stm32_eth_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

TX_THREAD AppMainThread;
TX_THREAD AppTCPThread;
TX_THREAD AppLinkThread;

TX_SEMAPHORE Semaphore;

NX_PACKET_POOL AppPool;

ULONG IpAddress;
ULONG NetMask;
NX_IP IpInstance;

NX_DHCP DHCPClient;
NX_TCP_SOCKET TCPSocket;

CHAR *pointer;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static VOID App_Main_Thread_Entry(ULONG thread_input);
static VOID App_TCP_Thread_Entry(ULONG thread_input);
static VOID App_Link_Thread_Entry(ULONG thread_input);

static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
/* USER CODE END PFP */
/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */
  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN MX_NetXDuo_Init */
  printf("\n\rNx_TCP_Echo_Client application started..\n\r");

  /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,  NX_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
 
  /* Create the Packet pool to be used for packet allocation */
  ret = nx_packet_pool_create(&AppPool, "Main Packet Pool", PAYLOAD_SIZE, pointer, NX_PACKET_POOL_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Allocate the memory for Ip_Instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,   2 * DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main NX_IP instance */
  ret = nx_ip_create(&IpInstance, "Main Ip instance", NULL_ADDRESS, NULL_ADDRESS, &AppPool,nx_stm32_eth_driver,
                     pointer, 2 * DEFAULT_MEMORY_SIZE, DEFAULT_PRIORITY);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Allocate the memory for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /*  Enable the ARP protocol and provide the ARP cache size for the IP instance */
  ret = nx_arp_enable(&IpInstance, (VOID *)pointer, DEFAULT_MEMORY_SIZE);
  
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Enable the ICMP */
  ret = nx_icmp_enable(&IpInstance);
  
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Enable the UDP protocol required for  DHCP communication */
  ret = nx_udp_enable(&IpInstance);
  
  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Enable the TCP protocol */
  ret = nx_tcp_enable(&IpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Allocate the memory for main thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,2 *  DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread */
  ret = tx_thread_create(&AppMainThread, "App Main thread", App_Main_Thread_Entry, 0, pointer, 2 * DEFAULT_MEMORY_SIZE,
                         DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Allocate the memory for TCP server thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,2 *  DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* create the TCP server thread */
  ret = tx_thread_create(&AppTCPThread, "App TCP Thread", App_TCP_Thread_Entry, 0, pointer, 2 * DEFAULT_MEMORY_SIZE,
                         DEFAULT_PRIORITY, DEFAULT_PRIORITY, TX_NO_TIME_SLICE, TX_DONT_START);

  if (ret != TX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* Allocate the memory for Link thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,2 *  DEFAULT_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* create the Link thread */
  ret = tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, 2 * DEFAULT_MEMORY_SIZE,
                         LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* create the DHCP client */
  ret = nx_dhcp_create(&DHCPClient, &IpInstance, "DHCP Client");

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* set DHCP notification callback  */

  tx_semaphore_create(&Semaphore, "App Semaphore", 0);
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
* @brief  Main thread entry.
* @param thread_input: ULONG user argument used by the thread entry
* @retval none
*/
static VOID App_Main_Thread_Entry(ULONG thread_input)
{
  UINT ret;

  /* register the IP address change callback */
  ret = nx_ip_address_change_notify(&IpInstance, ip_address_change_notify_callback, NULL);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* start the DHCP client */
  ret = nx_dhcp_start(&DHCPClient);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* wait until an IP address is ready */
  if(tx_semaphore_get(&Semaphore, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    Error_Handler();
  }

  ret = nx_ip_address_get(&IpInstance, &IpAddress, &NetMask);
  
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  PRINT_IP_ADDRESS(IpAddress);
  printf("\n\r");

  /* the network is correctly initialized, start the TCP server thread */
  tx_thread_resume(&AppTCPThread);
  /* this thread is not needed any more, relinquish it */
  tx_thread_relinquish();

  return;
}

/**
* @brief  IP Address change callback.
* @param ip_instance: NX_IP instance registered for this callback.
* @param ptr: VOID* user data pointer
* @retval none
*/
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  tx_semaphore_put(&Semaphore);
}

/**
* @brief  TCP thread entry.
* @param thread_input: thread user data
* @retval none
*/

static VOID App_TCP_Thread_Entry(ULONG thread_input)
{
  UINT ret;
  UINT count = 0;

  ULONG bytes_read;
  UCHAR data_buffer[512];

  ULONG source_ip_address;
  UINT source_port;

  NX_PACKET *server_packet;
  NX_PACKET *data_packet;

  /* create the TCP socket */
  ret = nx_tcp_socket_create(&IpInstance, &TCPSocket, "TCP Server Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY,
                             NX_IP_TIME_TO_LIVE, WINDOW_SIZE, NX_NULL, NX_NULL);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* bind the client socket for the DEFAULT_PORT */
  ret =  nx_tcp_client_socket_bind(&TCPSocket, DEFAULT_PORT, NX_WAIT_FOREVER);
  
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* connect to the remote server on the specified port */
  ret = nx_tcp_client_socket_connect(&TCPSocket, TCP_SERVER_ADDRESS, TCP_SERVER_PORT, NX_WAIT_FOREVER);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  while(count++ < MAX_PACKET_COUNT)
  {
    TX_MEMSET(data_buffer, '\0', sizeof(data_buffer));
    
    /* allocate the packet to send over the TCP socket */
    ret = nx_packet_allocate(&AppPool, &data_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);
    
    if (ret != NX_SUCCESS)
    {
      break;
    }
    
    /* append the message to send into the packet */
    ret = nx_packet_data_append(data_packet, (VOID *)DEFAULT_MESSAGE, sizeof(DEFAULT_MESSAGE), &AppPool, TX_WAIT_FOREVER);
    
    if (ret != NX_SUCCESS)
    {
      nx_packet_release(data_packet);
      break;
    }
    
    /* send the packet over the TCP socket */
    ret = nx_tcp_socket_send(&TCPSocket, data_packet, DEFAULT_TIMEOUT);
    
    if (ret != NX_SUCCESS)
    {
      break;
    }
    
    /* wait for the server response */
    ret = nx_tcp_socket_receive(&TCPSocket, &server_packet, DEFAULT_TIMEOUT);
    
    if (ret == NX_SUCCESS)
    {
      /* get the server IP address and  port */
      nx_udp_source_extract(server_packet, &source_ip_address, &source_port);
      
      /* retrieve the data sent by the server */
      nx_packet_data_retrieve(server_packet, data_buffer, &bytes_read);
      
      /* print the received data */
      PRINT_DATA(source_ip_address, source_port, data_buffer);
      printf("\n\r");
      
      /* release the server packet */
      nx_packet_release(server_packet);
      
      /* toggle the blue led on success */
      BSP_LED_Toggle(LED_BLUE);
    }
    else
    {
      /* no message received exit the loop */
      break;
    }
  }

  /* release the allocated packets */
  nx_packet_release(server_packet);
  
  /* disconnect the socket */
  nx_tcp_socket_disconnect(&TCPSocket, DEFAULT_TIMEOUT);
  
  /* unbind the socket */
  nx_tcp_client_socket_unbind(&TCPSocket);
  
  /* delete the socket */
  nx_tcp_socket_delete(&TCPSocket);
  
  /* print test summary on the UART */
  if (count == MAX_PACKET_COUNT + 1)
  {
    printf("-----------------------------------------------------\n\r\tSUCCESS : %u / %u packets sent\n\r-----------------------------------------------------\n\r", count - 1, MAX_PACKET_COUNT);
    Success_Handler();
  }
  else
  {
    printf("-----------------------------------------------------\n\r\tFAIL : %u / %u packets sent\n\r-----------------------------------------------------\n\r", count - 1, MAX_PACKET_COUNT);
    Error_Handler();
  }
}

/**
* @brief  Link thread entry
* @param thread_input: ULONG thread parameter
* @retval none
*/
static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG actual_status;
  UINT linkdown = 0, status;

  while(1)
  {
    /* Get Physical Link stackavailtus. */
    status = nx_ip_interface_status_check(&IpInstance, 0, NX_IP_LINK_ENABLED,
                                      &actual_status, 10);

    if(status == NX_SUCCESS)
    {
      if(linkdown == 1)
      {
        linkdown = 0;
        status = nx_ip_interface_status_check(&IpInstance, 0, NX_IP_ADDRESS_RESOLVED,
                                      &actual_status, 10);
        if(status == NX_SUCCESS)
        {
          /* The network cable is connected again. */
          printf("The network cable is connected again.\n\r");
          /* Print TCP Echo Client is available again. */
          printf("TCP Echo Client is available again.\n");
        }
        else
        {
          /* The network cable is connected. */
          printf("The network cable is connected.\n\r");
          /* Send command to Enable Nx driver. */
          nx_ip_driver_direct_command(&IpInstance, NX_LINK_ENABLE,
                                      &actual_status);
          /* Restart DHCP Client. */
          nx_dhcp_stop(&DHCPClient);
          nx_dhcp_start(&DHCPClient);
        }
      }
    }
    else
    {
      if(0 == linkdown)
      {
        linkdown = 1;
        /* The network cable is not connected. */
        printf("The network cable is not connected.\n\r");
      }
    }

    tx_thread_sleep(NX_ETH_CABLE_CONNECTION_CHECK_PERIOD);
  }
}

/* USER CODE END 1 */
