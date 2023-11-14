/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         app_module.c
  * @brief        Module 1
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Specify that this is a module! */
#define TXM_MODULE

/* Include the ThreadX module header. */
#include "txm_module.h"
#include "common.h"

#include "stm32mp13xx_disco.h"

/* Define constants. */
#define DEFAULT_STACK_SIZE      4096
#define DEFAULT_BLOCK_POOL_SIZE 512
#define DEFAULT_BYTE_POOL_SIZE  16384

/* Define the pool space in the bss section of the module. ULONG is used to get word alignment. */
static ULONG default_module_pool_space[DEFAULT_BYTE_POOL_SIZE / 4] __attribute__ ((aligned(32)));

/* Define the ThreadX object control block pointers. */

static TX_THREAD *thread_0;
static TX_BYTE_POOL *byte_pool_0;
static TX_BLOCK_POOL *block_pool_0;
static TX_QUEUE *resident_queue;

/* Function prototypes*/

static void thread_0_entry(ULONG thread_input);
static void Error_Handler(void);

/* Module entry function */
void default_module_start(ULONG id)
{
  CHAR *pointer;
  UINT status;

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /* Allocate all the objects. In MPU mode, modules cannot allocate control blocks within
   their own memory area so they cannot corrupt the resident portion of ThreadX by overwriting
   the control block(s).  */
  status = txm_module_object_allocate((void*) &thread_0, sizeof(TX_THREAD));
  if (status)
  {
    Error_Handler();
  }
  status = txm_module_object_allocate((void*) &byte_pool_0, sizeof(TX_BYTE_POOL));
  if (status)
  {
    Error_Handler();
  }
  status = txm_module_object_allocate((void*) &block_pool_0, sizeof(TX_BLOCK_POOL));
  if (status)
  {
    Error_Handler();
  }

  /* Create a byte memory pool from which to allocate the thread stacks. */
  status = tx_byte_pool_create(byte_pool_0, "module 1 byte pool 0", (UCHAR* )default_module_pool_space, DEFAULT_BYTE_POOL_SIZE);
  if (status)
  {
    Error_Handler();
  }

  /* Allocate the stack for thread 0. */
  status = tx_byte_allocate(byte_pool_0, (VOID**) &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);
  if (status)
  {
    Error_Handler();
  }

  /* Create the main thread. */
  status = tx_thread_create(thread_0, "module 1 thread 0", thread_0_entry, 0, pointer, DEFAULT_STACK_SIZE, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (status)
  {
    Error_Handler();
  }

  /* Allocate the memory for a small block pool. */
  status = tx_byte_allocate(byte_pool_0, (VOID**) &pointer, DEFAULT_BLOCK_POOL_SIZE, TX_NO_WAIT);
  if (status)
  {
    Error_Handler();
  }

  /* Create a block memory pool. */
  status = tx_block_pool_create(block_pool_0, "module 1 block pool 0", sizeof(ULONG), pointer, DEFAULT_BLOCK_POOL_SIZE);
  if (status)
  {
    Error_Handler();
  }

  /* Allocate a block. */
  status = tx_block_allocate(block_pool_0, (VOID**) &pointer, TX_NO_WAIT);
  if (status)
  {
    Error_Handler();
  }

  /* Release the block back to the pool. */
  tx_block_release(pointer);
}

/* Module main thread function */
static void thread_0_entry(ULONG thread_input)
{
  UINT status;
  ULONG s_msg;
  ULONG readbuffer;

  BSP_LED_On(LED_BLUE);

  /* Request access to the queue from the module manager */
  status = txm_module_object_pointer_get(TXM_QUEUE_OBJECT, "Resident Queue", (VOID**) &resident_queue);
  if (status)
  {
    Error_Handler();
  }

  /* Writing to write and read region */
  s_msg = WRITING_TO_READWRITE;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);
  *(ULONG*)READWRITE_REGION = 0xABABABAB;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Reading from write and read region */
  s_msg = READING_FROM_READWRITE;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);
  readbuffer = *(ULONG*)READWRITE_REGION;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Reading from read only region */
  s_msg = READING_FROM_READONLY;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);
  readbuffer = *(ULONG*)READONLY_REGION;
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Writing to read only region.
   * This will generate an exception and the thread will be killed. */
  s_msg = WRITING_TO_READONLY;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);
  *(ULONG*)READONLY_REGION = 0xABABABAB;
  /* The thread should be killed here because of this illegal access, so we will not reach this point. */
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Notify module manager about job finish */
  s_msg = PROCESSING_FINISHED;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);

  /* Suppress unused variable warning */
  UNUSED(readbuffer);

  /* Stay here, waiting for the module manager to stop and unload the module*/
  while (1)
  {
    BSP_LED_Toggle(LED_BLUE);
    tx_thread_sleep(500);
  }
}

static void Error_Handler(void)
{
  while (1)
  {
    BSP_LED_Toggle(LED_RED);
    tx_thread_sleep(500);
  }
}
