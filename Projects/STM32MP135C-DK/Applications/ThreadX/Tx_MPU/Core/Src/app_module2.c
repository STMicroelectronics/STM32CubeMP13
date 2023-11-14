/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         app_module2.c
  * @brief        Module 2
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
#define DEFAULT_BYTE_POOL_SIZE  16384

/* Define the pool space in the bss section of the module. ULONG is used to get word alignment. */
static ULONG default_module_pool_space[DEFAULT_BYTE_POOL_SIZE / 4] __attribute__ ((aligned(32)));

/* Define the ThreadX object control block pointers. */

static TX_THREAD *thread_0;
static TX_BYTE_POOL *byte_pool_0;
static TX_QUEUE *resident_queue;

/* Function prototypes*/

static void thread_0_entry(ULONG thread_input);
static void Error_Handler(void);

/* Module entry function */
void default_module_start(ULONG id)
{
  UINT status;
  CHAR *pointer;

  /* Allocate all the objects. In MPU mode, modules cannot allocate control blocks within
     their own memory area so they cannot corrupt the resident portion of ThreadX by overwriting
     the control block(s). */
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

  /* Create a byte memory pool from which to allocate the thread stacks. */
  status = tx_byte_pool_create(byte_pool_0, "module 2 byte pool 0", (UCHAR* )default_module_pool_space, DEFAULT_BYTE_POOL_SIZE);
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
  status = tx_thread_create(thread_0, "module 2 thread 0", thread_0_entry, 0, pointer, DEFAULT_STACK_SIZE, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (status)
  {
    Error_Handler();
  }
}

/* Module main thread function */
void thread_0_entry(ULONG thread_input)
{
  UINT status;
  ULONG s_msg;

  /* Request access to the queue from the module manager */
  status = txm_module_object_pointer_get(TXM_QUEUE_OBJECT, "Resident Queue", (VOID**) &resident_queue);
  if (status)
  {
    Error_Handler();
  }

  /* Writing to write and read region that is not memory mapped.
   * This will generate an exception and the thread will be killed. */
  s_msg = WRITING_TO_READWRITE;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);
  *(uint32_t*)READWRITE_REGION = 0xABABABAB;
  /* The thread should be killed here because of this illegal access, so we will not reach this point. */
  tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 10);

  /* Notify module manager about job finish */
  s_msg = PROCESSING_FINISHED;
  tx_queue_send(resident_queue, &s_msg, TX_NO_WAIT);

  /* Stay here, waiting for the module manager to stop and unload the module*/
  tx_thread_sleep(TX_WAIT_FOREVER);
}

static void Error_Handler(void)
{
  /* Nothing to do, block here */
  tx_thread_sleep(TX_WAIT_FOREVER);
}
