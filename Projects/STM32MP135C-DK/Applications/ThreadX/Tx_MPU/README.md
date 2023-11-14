## **Tx_MPU_A7 application description**

This application provides an example of **Azure RTOS ThreadX** stack usage, it shows how to develop an application using the **ThreadX Module** feature.
It demonstrates how to load, start and unload modules.
In addition, it shows how to use ThreadX memory protection with modules using the Cortex-A7 **MMU**.
This example has originally been designed for microcontrollers with an MPU (Memory Protection Unit) instead of a more advanced MMU (Memory Protection Unit).
So the term MPU and MMU are used interchangeably.

This project is composed of two sub-projects:

- `Tx_Module_Manager_A7`: ThreadX Module Manager code that load and start the module dynamically at runtime.
- `Tx_Module_A7`: ThreadX Module code that is to be loaded and started by the module manager dynamically at runtime.
- `Tx_Module2_A7`: ThreadX Module code that is to be loaded and started by the module manager dynamically at runtime.

At the module manager stage, the main entry function tx_application_define() is called by ThreadX during kernel start, the application creates 1 thread and 1 message queue:
- `ModuleManager` (Prio : 4; Preemption Threshold : 4)
- `ResidentQueue` (Size : 16 * ULONG)

`ModuleManager` thread uses the ThreadX Module Manager APIs to configure, load and start the expected module.  
`ResidentQueue` is used to synchronize operations between Module Manager and the loaded Module.

At the module 1 stage, the main entry function default_module_start() is called by ThreadX during module start, the application creates 1 thread:
- `MainThread` (Prio : 2; Preemption Threshold : 2)

`MainThread` is expected to execute data read and write operations to/from user-defined Shared Memory regions. Memory protection is then demonstrated by trapping the Module's attempt at writing to the shared Read Only region. A Memory Fault is then expected before the unload of the module and the module manager continues to run correctly.

The module 2 try to write to the same read-write region as Module 1. But since this region has not been MMU mapped for Module 2, an access fault is triggered.

#### **Expected success behavior**

- `LED_GREEN` toggles every 500ms.
- `LED_BLUE` is switched on from the module 1 when it begins running.
- Information regarding the module processing progress printed to the serial port.

#### **Error behaviors**

`LED_RED` toggles every 1 second if any error occurs.

#### **Assumptions if any**
None

#### **Known limitations**
None

---

### **Notes**

1. A preamble is required with each Module to expose the module configuration to the Module Manager. Particularly the preamble contains information such as the module unique ID and attributes.
	
	Module Properties (attributes) is a 32bit word, laid out as:
	- Bits 31-24: Compiler ID 0 -> IAR 1 -> ARM 2 -> GNU
	- Bits 23-3: Reserved
	- Bit 2: 0 -> Disable shared/external memory access 1 -> Enable shared/external memory access
	- Bit 1: 0 -> No MPU protection 1 -> MPU protection (must have user mode selected - bit 0 set)
	- Bit 0: 0 -> Privileged mode execution 1 -> User mode execution
	
	For this application demonstrating MPU memory protection on modules, the attributes should be set as follows:
		- MPU protection is enabled.
		- User mode is set for the module
	
	The above configuration results in an attributes word equals `0x02000003`

2. All C files in a module must `#define TXM_MODULE` prior to including `txm_module.h`. Doing so remaps the ThreadX API calls to the module-specific version of the API that invokes the dispatch function in the resident Module Manager to perform the call to the actual API function.

3. The `TXM_MODULE_MANAGER_16_MPU` is a Preprocessor define that should be added in both C and Assembly preprocessor define list to allow the application on the **STM32MP135xx** family to work properly.

#### **ThreadX usage hints**

- ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
- ThreadX is configured with 1000 ticks/sec by default, this should be taken into account when using delays or timeouts at application.  
	It is always possible to reconfigure it in the `tx_user.h`, the `TX_TIMER_TICKS_PER_SECOND` define.
- ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
- ThreadX offers the `tx_application_define()` function, that is automatically called by the `tx_kernel_enter()` API.  
	It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...) but it should not in any way contain a system API call (HAL or BSP).

---

### **Keywords**

RTOS, ThreadX, Threading, Message Queue, Module Manager, Module, MPU, MMU

### **Hardware and Software environment**

- This example runs on **STM32MP135xx** devices
- This example has been tested with **STMicroelectronics STM32MP135F-DK** boards and can be easily tailored to any other supported device and development board.
- A virtual COM port appears in the HyperTerminal.  
	Hyperterminal configuration:
	+ Data Length = 8 Bits
	+ One Stop Bit
	+ No parity
	+ BaudRate = 115200 baud
	+ Flow control: None

### **How to use it ?**

In order to make the program work, you must do the following :
- Open Multi-projects workspace using your pereferred IDE
- Rebuild **Tx_Module_A7** project
- Rebuild **Tx_Module2_A7** project
- Copy the "Tx_Module_A7.bin" output file to a FAT32 formatted SD card
- Copy the "Tx_Module2_A7.bin" output file to a FAT32 formatted SD card
- Insert the SD card it in the **STM32MP135F-DK** board
- Run the **DDR_Init_A7** example to initialize the DRAM
- Rebuild **Tx_Module_Manager_A7** project
- Set the **Tx_Module_Manager_A7** as active application (Set as Active)
- Run the **Tx_Module_Manager_A7** example
