/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


/* FreeRTOS.org includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "timers.h" /* For the xTimerPendFunctionCallFromISR() function. */

#include "sapi.h"


/* Demo includes. */
/*	#include "supporting_functions.h"
	Not available in the freeRTOS version included in the firmware_v2 modules */


/* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;
#define	vPrintString(str) debugPrintString(str)


const char *pcTextForMain = "\r\nExample 22 - Experimenting with event groups\r\n";

/* Definitions for the event bits in the event group. */
#define mainFIRST_TASK_BIT	( 1UL << 0UL ) /* Event bit 0, which is set by a task. */
#define mainSECOND_TASK_BIT	( 1UL << 1UL ) /* Event bit 1, which is set by a task. */
#define mainISR_BIT			( 1UL << 2UL ) /* Event bit 2, which is set by an ISR. */

/* Sets up system hardware */
static void prvSetupHardware(void);

/* The tasks to be created. */
static void vIntegerGenerator( void *pvParameters );
static void vEventBitSettingTask( void *pvParameters );
static void vEventBitReadingTask( void *pvParameters );


/* The number of the simulated interrupt used in this example.  Numbers 0 to 2
are used by the FreeRTOS Windows port itself, so 3 is the first number available
to the application. */
/* #define mainINTERRUPT_NUMBER	3	*/
/* The interrupt number to use for the software interrupt generation.  This
 * could be any unused number.  In this case the first chip level (non system)
 * interrupt is used, which happens to be the watchdog on the LPC1768.  WDT_IRQHandler */
/* interrupt is used, which happens to be the DAC on the LPC4337 M4.  DAC_IRQHandler */
#define mainSW_INTERRUPT_ID		(0)

/* Macro to force an interrupt. */
#define mainTRIGGER_INTERRUPT()	NVIC_SetPendingIRQ(mainSW_INTERRUPT_ID)

/* Macro to clear the same interrupt. */
#define mainCLEAR_INTERRUPT()	NVIC_ClearPendingIRQ(mainSW_INTERRUPT_ID)

/* The priority of the software interrupt.  The interrupt service routine uses
 * an (interrupt safe) FreeRTOS API function, so the priority of the interrupt must
 * be equal to or lower than the priority set by
 * configMAX_SYSCALL_INTERRUPT_PRIORITY - remembering that on the Cortex-M3 high
 * numeric values represent low priority values, which can be confusing as it is
 * counter intuitive. */
#define mainSOFTWARE_INTERRUPT_PRIORITY	(5)


/* A function that can be deferred to run in the RTOS daemon task.  The function
prints out the string passed to it using the pvParameter1 parameter. */
void vPrintStringFromDaemonTask( void *pvParameter1, uint32_t ulParameter2 );

/* The service routine for the (simulated) interrupt.  This is the interrupt
that sets an event bit in the event group. */
/*static uint32_t ulEventBitSettingISR( void );*/
/* Enable the software interrupt and set its priority. */
static void prvSetupSoftwareInterrupt();


/* The service routine for the interrupt.  This is the interrupt that the
 * task will be synchronized with.  void vSoftwareInterruptHandler(void); */
/* the watchdog on the LPC1768 => WDT_IRQHandler */ /* the DAC on the LPC4337 M4.  DAC_IRQHandler */
#define vSoftwareInterruptHandler (DAC_IRQHandler)

/*-----------------------------------------------------------*/

/* Declare the event group in which bits are set from both a task and an ISR. */
EventGroupHandle_t xEventGroup;


static void prvSetupSoftwareInterrupt()
{
	/* The interrupt service routine uses an (interrupt safe) FreeRTOS API
	 * function so the interrupt priority must be at or below the priority defined
	 * by configSYSCALL_INTERRUPT_PRIORITY. */
	NVIC_SetPriority(mainSW_INTERRUPT_ID, mainSOFTWARE_INTERRUPT_PRIORITY);

	/* Enable the interrupt. */
	NVIC_EnableIRQ(mainSW_INTERRUPT_ID);
}
/*-----------------------------------------------------------*/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	/* Sets up system hardware */
	boardConfig();
	debugPrintConfigUart( UART_USB, 115200 );

	/* Initial LED state is on, keep a live */
	gpioWrite( LED1, ON );
	gpioWrite( LED2, ON );
	gpioWrite( LED3, ON );
}
/*-----------------------------------------------------------*/

int main(void)
{
	/* Sets up system hardware */
	prvSetupHardware();

	/* Print out the name of this example. */
	vPrintString(pcTextForMain);

	/* Before an event group can be used it must first be created. */
	xEventGroup = xEventGroupCreate();

	/* Create the task that sets event bits in the event group. */
	xTaskCreate( vEventBitSettingTask, "BitSetter", 1000, NULL, 1, NULL );

	/* Create the task that waits for event bits to get set in the event
	group. */
	xTaskCreate( vEventBitReadingTask, "BitReader", 1000, NULL, 2, NULL );

	/* Create the task that is used to periodically generate a software
	interrupt. */
	xTaskCreate( vIntegerGenerator, "IntGen", 1000, NULL, 3, NULL );

	/* Install the handler for the software interrupt.  The syntax necessary
	to do this is dependent on the FreeRTOS port being used.  The syntax
	shown here can only be used with the FreeRTOS Windows port, where such
	interrupts are only simulated. */
	/* vPortSetInterruptHandler( mainINTERRUPT_NUMBER, ulEventBitSettingISR ); */

	/* Enable the software interrupt and set its priority. */
    prvSetupSoftwareInterrupt();

    /* Start the scheduler so the created tasks start executing. */
       vTaskStartScheduler();

	/* The following line should never be reached because vTaskStartScheduler()
	will only return if there was not enough FreeRTOS heap memory available to
	create the Idle and (if configured) Timer tasks.  Heap management, and
	techniques for trapping heap exhaustion, are described in the book text. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

static void vEventBitSettingTask( void *pvParameters )
{
	/* const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL ), xDontBlock = 0;
	Not available in the freeRTOS version included in the firmware_v2 modules */

	const TickType_t xDelay200ms = 200UL/portTICK_RATE_MS;

	for( ;; )
	{
		/* LED state is toggled, keep a live */
		gpioToggle( LED1 );

		/* Delay for a short while before starting the next loop. */
		vTaskDelay( xDelay200ms );

		/* Print out a message to say event bit 0 is about to be set by the
		task, then set event bit 0. */
		vPrintString( "Bit setting task -\t about to set bit 0.\r\n" );
		xEventGroupSetBits( xEventGroup, mainFIRST_TASK_BIT );

		/* Delay for a short while before setting the other bit set within this
		task. */
		vTaskDelay( xDelay200ms );

		/* Print out a message to say event bit 1 is about to be set by the
		task, then set event bit 1. */
		vPrintString( "Bit setting task -\t about to set bit 1.\r\n" );
		xEventGroupSetBits( xEventGroup, mainSECOND_TASK_BIT );
	}
}
/*-----------------------------------------------------------*/

/* static uint32_t ulEventBitSettingISR( void )*/
void vSoftwareInterruptHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken;
	/* The string is not printed within the interrupt service, but is instead
	sent to the RTOS daemon task for printing.  It is therefore declared static to
	ensure the compiler does not allocate the string on the stack of the ISR (as the
	ISR's stack frame will not exist when the string is printed from the daemon
	task. */
	static const char *pcString = "Bit setting ISR -\t about to set bit 2.\r\n";

	/* As always, xHigherPriorityTaskWoken is initialized to pdFALSE. */
	xHigherPriorityTaskWoken = pdFALSE;

	/* Print out a message to say bit 2 is about to be set.  Messages cannot be
	printed from an ISR, so defer the actual output to the RTOS daemon task by
	pending a function call to run in the context of the RTOS daemon task. */
	xTimerPendFunctionCallFromISR( vPrintStringFromDaemonTask, ( void * ) pcString, 0, &xHigherPriorityTaskWoken );

	/* Set bit 2 in the event group. */
	xEventGroupSetBitsFromISR( xEventGroup, mainISR_BIT, &xHigherPriorityTaskWoken );

	/* xEventGroupSetBitsFromISR() writes to the timer command queue.  If
	writing to the timer command queue results in the RTOS daemon task leaving
	the Blocked state, and if the priority of the RTOS daemon task is higher
	than the priority of the currently executing task (the task this interrupt
	interrupted) then xHigherPriorityTaskWoken will have been set to pdTRUE
	inside xEventGroupSetBitsFromISR().

	xHigherPriorityTaskWoken is used as the parameter to portYIELD_FROM_ISR().
	If xHigherPriorityTaskWoken equals pdTRUE then calling portYIELD_FROM_ISR()
    will request a context switch.  If xHigherPriorityTaskWoken is still pdFALSE
	then calling portYIELD_FROM_ISR() will have no effect.

	The implementation of portYIELD_FROM_ISR() used by the Windows port includes
	a return statement, which is why this function does not explicitly return a
	value. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

static void vEventBitReadingTask( void *pvParameters )
{
	const EventBits_t xBitsToWaitFor = ( mainFIRST_TASK_BIT | mainSECOND_TASK_BIT | mainISR_BIT );
	EventBits_t xEventGroupValue;

	for( ;; )
	{
		/* LED state is toggled, keep a live */
		gpioToggle( LED2 );

		/* Block to wait for event bits to become set within the event group. */
		xEventGroupValue = xEventGroupWaitBits( /* The event group to read. */
												xEventGroup,

												/* Bits to test. */
												xBitsToWaitFor,

												/* Clear bits on exit if the
												unblock condition is met. */
												pdTRUE,

												/* Don't wait for all bits. */
												pdFALSE,

												/* Don't time out. */
												portMAX_DELAY );

		/* Print a message for each bit that was set. */
		if( ( xEventGroupValue & mainFIRST_TASK_BIT ) != 0 )
		{
			vPrintString( "Bit reading task -\t event bit 0 was set\r\n" );
		}

		if( ( xEventGroupValue & mainSECOND_TASK_BIT ) != 0 )
		{
			vPrintString( "Bit reading task -\t event bit 1 was set\r\n" );
		}

		if( ( xEventGroupValue & mainISR_BIT ) != 0 )
		{
			vPrintString( "Bit reading task -\t event bit 2 was set\r\n" );
		}

		vPrintString( "\r\n" );
	}
}
/*-----------------------------------------------------------*/

void vPrintStringFromDaemonTask( void *pvParameter1, uint32_t ulParameter2 )
{
	/* The string to print is passed into this function using the pvParameter1
	parameter. */
	vPrintString( ( const char * ) pvParameter1 );
}
/*-----------------------------------------------------------*/

static void vIntegerGenerator( void *pvParameters )
{
	TickType_t xLastExecutionTime;
	/* const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
	Not available in the freeRTOS version included in the firmware_v2 modules */

	const TickType_t xDelay500ms = 500UL/portTICK_RATE_MS;

	/* Initialize the variable used by the call to vTaskDelayUntil(). */
	xLastExecutionTime = xTaskGetTickCount();

	for( ;; )
	{
		/* This is a periodic task.  Block until it is time to run again.
		The task will execute every 500ms. */
		vTaskDelayUntil( &xLastExecutionTime, xDelay500ms );

		/* Generate the interrupt that will set a bit in the event group. */
		/* vPortGenerateSimulatedInterrupt( mainINTERRUPT_NUMBER ); */
        mainTRIGGER_INTERRUPT();
	}
}
