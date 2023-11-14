/**
 ******************************************************************************
 * @file    app_threadx.c
 * @author  MCD Application Team
 * @brief   ThreadX applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2020-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 ******************************************************************************
 */

#include "app_threadx.h"
#include "txm_module.h"
#include "mmu_stm32mp13xx_azurertos.h"
#include <stdio.h>

// Import symbols from linker.
extern char end;            // End of the application (all code and data)
extern char __MEM_END__;    // End of main memory

extern TXM_MODULE_MANAGER_MEMORY_FAULT_INFO _txm_module_manager_memory_fault_info;

/*********************************************************************************
 * FileX.
 *********************************************************************************/
/* Buffer for FileX FX_MEDIA sector cache. It is put in a non-cachable area (SYSRAM) to avoid cache maintenance. */
static UCHAR media_memory[FX_STM32_SD_DEFAULT_SECTOR_SIZE] __attribute__ ((aligned (4), section(".SdBufferSection")));

/* Define FileX global data structures.  */
static FX_MEDIA sdio_disk;

/*********************************************************************************
 * ModuleManager.
 *********************************************************************************/
#define ModuleManager_threadStackSize (32*1024)
#define ModuleManager_dataSize        (128*1024)
#define ModuleManager_objectDataSize  (128*1024)

/* Define the ThreadX object control blocks */
static TX_THREAD ModuleManager_thread;
static TX_QUEUE ModuleManager_residentQueue;

/* Define the module data pool area. */
static TX_BYTE_POOL ModuleManager_bytePool;
static void *ModuleManager_data;

/* Define the object pool area. */
static void *ModuleManager_objectMemory;

/* Define the count of memory faults. */
static ULONG ModuleManager_memoryFaults = 0;

static void Module_load(TXM_MODULE_INSTANCE *module, char *moduleName, char *fileName)
{
  UINT status;
  ULONG properties;

  /* Load the module from a file. */
  status = txm_module_manager_file_load(module, moduleName, &sdio_disk, fileName);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Get module properties. */
  status = txm_module_manager_properties_get(module, &properties);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Print loaded module info. */
  printf("Module \"%s\" is loaded from file \"%s\"\n", module->txm_module_instance_name, fileName);
  printf("Module code section size: %i bytes, data section size: %i\n", (int)module->txm_module_instance_code_size, (int)module->txm_module_instance_data_size);
  printf("Module Attributes:\n");
  printf("  - Compiled for %s compiler\n", ((properties >> 25) == 1) ? "CubeIDE (GNU)" : ((properties >> 24) == 1) ? "ARM KEIL" : "IAR EW");
  printf("  - Shared/external memory access is %s\n", ((properties & 0x04) == 0) ? "Disabled" : "Enabled");
  printf("  - MPU protection is %s\n", ((properties & 0x02) == 0) ? "Disabled" : "Enabled");
  printf("  - %s mode execution is enabled for the module\n\n", ((properties & 0x01) == 0) ? "Privileged" : "User");
}

static VOID Module_buildMessage(char *p_msg, ULONG r_msg)
{
  memset(p_msg, 0, 64);

  switch (r_msg)
  {
  case WRITING_TO_READWRITE:
    memcpy(p_msg, "Writing to ReadWrite Region", 27);
    break;
  case WRITING_TO_READONLY:
    memcpy(p_msg, "Writing to ReadOnly Region", 26);
    break;
  case READING_FROM_READWRITE:
    memcpy(p_msg, "Reading from ReadWrite Region", 29);
    break;
  case READING_FROM_READONLY:
    memcpy(p_msg, "Reading from ReadOnly Region", 28);
    break;
  case PROCESSING_FINISHED:
    memcpy(p_msg, "All operations were done", 24);
    break;
  default:
    memcpy(p_msg, "Invalid option", 14);
    break;
  }
}

/*-------------------------------------------------------------------------------
 * Module 1.
 *-------------------------------------------------------------------------------*/
#define MODULE1_FILE_NAME "Tx_Module_A7.bin"

static TXM_MODULE_INSTANCE module1;

static void Module1_loadAndRun()
{
  UINT status;
  TXM_MODULE_INSTANCE *module = &module1;
  ULONG r_msg = PROCESSING_NOT_STARTED;
  CHAR p_msg[64];

  Module_load(module, "Module1", MODULE1_FILE_NAME);

  /* Enable RCC memory area to allow the module to access LEDs for debugging. */
  status = txm_module_manager_external_memory_enable(module, (void*)RCC_BASE, sizeof(RCC_TypeDef), mmu_peripheral);
  if(status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable GPIOA memory area to allow the module to access LEDs for debugging. */
  status = txm_module_manager_external_memory_enable(module, (void*)GPIOA_BASE, sizeof(GPIO_TypeDef), mmu_peripheral);
  if(status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable shared memory region for module with read-only access permission. */
  status = txm_module_manager_external_memory_enable(module, (void*)READONLY_REGION, SHARED_MEM_SIZE, mmu_readOnly);
  if(status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable shared memory region for module with read and write access permission. */
  status = txm_module_manager_external_memory_enable(module, (void*)READWRITE_REGION, SHARED_MEM_SIZE, mmu_readWrite);
  if(status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Start the module. */
  status = txm_module_manager_start(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  printf("Module 1 execution is started\n");

  /* Get Module's progress messages */
  while (r_msg != PROCESSING_FINISHED)
  {
    if (tx_queue_receive(&ModuleManager_residentQueue, &r_msg, TX_TIMER_TICKS_PER_SECOND) == TX_SUCCESS)
    {
      /* Convert the message to a user friendly string. */
      Module_buildMessage(p_msg, r_msg);

      printf("Module 1 is executing: %s\n", p_msg);

      /* Check if the last executed operation resulted in memory violation. */
      if (ModuleManager_memoryFaults)
      {
        /* A memory access fault just occurred */
        printf("A memory fault occurred while module 1 executed: %s\n", p_msg);
        break;
      }
    }
  }

  printf("Module 1 has finished\n");

  /* Stop the module. */
  status = txm_module_manager_stop(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Unload the module. */
  status = txm_module_manager_unload(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }
}

/*-------------------------------------------------------------------------------
 * Module 2.
 *-------------------------------------------------------------------------------*/
#define MODULE2_FILE_NAME "Tx_Module2_A7.bin"

static TXM_MODULE_INSTANCE module2;

static VOID Module2_loadAndRun()
{
  UINT status;
  TXM_MODULE_INSTANCE *module = &module2;
  ULONG r_msg = PROCESSING_NOT_STARTED;
  CHAR p_msg[64];

  Module_load(module, "Module2", MODULE2_FILE_NAME);

  /* Start the module. */
  status = txm_module_manager_start(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  printf("Module 2 execution is started\n");
  printf("Module 2 will try to write to the same read-write region as Module 1.\n");
  printf("But this region has not been MMU mapped so it should fault.\n");

  /* Get Module's progress messages */
  while (r_msg != PROCESSING_FINISHED)
  {
    if (tx_queue_receive(&ModuleManager_residentQueue, &r_msg, TX_TIMER_TICKS_PER_SECOND) == TX_SUCCESS)
    {
      /* Convert the message to a user friendly string. */
      Module_buildMessage(p_msg, r_msg);

      printf("Module 2 is executing: %s\n", p_msg);

      /* Check if the last executed operation resulted in memory violation. */
      if (ModuleManager_memoryFaults)
      {
        /* A memory access fault just occurred */
        printf("A memory fault occurred while module 2 executed: %s\n", p_msg);
        break;
      }
    }
  }

  printf("Module 2 has finished\n");

  /* Stop the module. */
  status = txm_module_manager_stop(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Unload the module. */
  status = txm_module_manager_unload(module);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }
}

/*-------------------------------------------------------------------------------
 * Module manager thread.
 *-------------------------------------------------------------------------------*/
static VOID ModuleManager_handleFault(TX_THREAD *thread, TXM_MODULE_INSTANCE *module)
{
  /* Just increment the fault counter. */
  ModuleManager_memoryFaults++;

  printf("Memory fault in module \"%s\" in thread \"%s\" at address 0x%08x\n",
      module != NULL ? module->txm_module_instance_name : "none",
      thread != NULL ? thread->tx_thread_name : "none",
      (unsigned int)_txm_module_manager_memory_fault_info.txm_module_manager_memory_fault_info_code_location);

  if (module == NULL)
  {
    Error_Handler();
  }
}

static void ModuleManager_initializeThread()
{
  UINT status;
  CHAR *pointer;

  /* Allocate the stack for resident_queue. */
  status = tx_byte_allocate(&ModuleManager_bytePool, (VOID**) &pointer, 16 * sizeof(ULONG), TX_NO_WAIT);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create the resident_queue. */
  status = tx_queue_create(&ModuleManager_residentQueue, "Resident Queue", TX_1_ULONG, pointer, 16 * sizeof(ULONG));
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Allocate the memory for the module manager. */
  status = tx_byte_allocate(&ModuleManager_bytePool, &ModuleManager_data, ModuleManager_dataSize, TX_NO_WAIT);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Initialize the module manager. */
  status = txm_module_manager_initialize(ModuleManager_data, ModuleManager_dataSize);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Allocate the memory for the module objects. */
  status = tx_byte_allocate(&ModuleManager_bytePool, &ModuleManager_objectMemory, ModuleManager_objectDataSize, TX_NO_WAIT);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create a pool for module objects. */
  status = txm_module_manager_object_pool_create(ModuleManager_objectMemory, ModuleManager_objectDataSize);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Register a fault handler. */
  status = txm_module_manager_memory_fault_notify(ModuleManager_handleFault);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Open the SD disk driver. */
  status = fx_media_open(&sdio_disk, "STM32_SDIO_DISK", fx_stm32_sd_driver, 0, media_memory, FX_STM32_SD_DEFAULT_SECTOR_SIZE);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }
}

/**
 * @brief  Module Manager main thread.
 * @param  thread_input: thread id
 * @retval none
 */
static void ModuleManager_threadEntry(ULONG thread_input)
{
  ModuleManager_initializeThread();

  Module1_loadAndRun();

  Module2_loadAndRun();

  /* Toggle green LED to indicated success of operations. */
  while (1)
  {
    BSP_LED_Toggle(LED_GREEN);
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 2);
  }
}

static void ModuleManager_initialize()
{
  UINT status;
  CHAR *pointer = &end;
  uint32_t memorySize = (uint32_t) &__MEM_END__ - (uint32_t) &end;

  /* Create the byte pool used for all allocations, it uses the whole remaining memory available. */
  status = tx_byte_pool_create(&ModuleManager_bytePool, "Module Manager Byte Pool", pointer, memorySize);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Allocate the stack for Module Manager Thread. */
  status = tx_byte_allocate(&ModuleManager_bytePool, (VOID**) &pointer, ModuleManager_threadStackSize, TX_NO_WAIT);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create the Module Manager Thread. */
  status = tx_thread_create(&ModuleManager_thread, "Module Manager Thread", ModuleManager_threadEntry, 0,
      pointer, ModuleManager_threadStackSize,
      MODULE_MANAGER_THREAD_PRIO, MODULE_MANAGER_THREAD_PREEMPTION_THRESHOLD,
      TX_NO_TIME_SLICE, TX_AUTO_START);
  if (status != TX_SUCCESS)
  {
    Error_Handler();
  }
}

/*********************************************************************************
 * Main ThreadX application function.
 *********************************************************************************/
/**
 * @brief  Application ThreadX Initialization.
 * @param memory_ptr: memory pointer
 * @retval none
 */
VOID tx_application_define(VOID *first_unused_memory)
{
  /* Start PL1 Physical Timer */
  PL1_SetControl(0x1);

  /* Initialize FileX. */
  fx_system_initialize();

  /* Create the main (ModuleManager_thread) thread. */
  ModuleManager_initialize();
}

/*********************************************************************************
 * ThreadX initialization.
 *********************************************************************************/
/**
 * @brief  MX_AZURE_RTOS_Init
 * @param  None
 * @retval None
 */
void MX_AZURE_RTOS_Init(void)
{
  mmu_initialize();
  tx_kernel_enter();
}
