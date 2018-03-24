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
#include "queue.h"

#include "sapi.h"


/* Demo includes. */
/*	#include "supporting_functions.h"
	Not available in the freeRTOS version included in the firmware_v2 modules */


/* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;
#define	vPrintString(str) debugPrintString(str)


const char *pcTextForMain = "\r\nExample 19 - Sending and receiving on a queue from within an interrupt\r\n";

/* Sets up system hardware */
static void prvSetupHardware(void);

/* The tasks to be created. */
static void vIntegerGenerator( void *pvParameters );
static void vStringPrinter( void *pvParameters );


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


/* The service routine for the (simulated) interrupt.  This is the interrupt
that the task will be synchronized with. */
/* static uint32_t ulExampleInterruptHandler( void ); */
/* Enable the software interrupt and set its priority. */
static void prvSetupSoftwareInterrupt();


/* The service routine for the interrupt.  This is the interrupt that the
 * task will be synchronized with.  void vSoftwareInterruptHandler(void); */
/* the watchdog on the LPC1768 => WDT_IRQHandler */ /* the DAC on the LPC4337 M4.  DAC_IRQHandler */
#define vSoftwareInterruptHandler (DAC_IRQHandler)

/* Declare two variables of type QueueHandle_t.  One queue will be read from
within an ISR, the other will be written to from within an ISR. */
QueueHandle_t xIntegerQueue, xStringQueue;


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

    /* Before a queue can be used it must first be created.  Create both queues
	used by this example.  One queue can hold variables of type uint32_t,
	the other queue can hold variables of type char*.  Both queues can hold a
	maximum of 10 items.  A real application should check the return values to
	ensure the queues have been successfully created. */
    xIntegerQueue = xQueueCreate( 10, sizeof( uint32_t ) );
	xStringQueue = xQueueCreate( 10, sizeof( char * ) );

	/* Create the task that uses a queue to pass integers to the interrupt
	service	routine.  The task is created at priority 1. */
	xTaskCreate( vIntegerGenerator, "IntGen", 1000, NULL, 1, NULL );

	/* Create the task that prints out the strings sent to it from the interrupt
	service routine.  The task is created at the higher priority of 2. */
	xTaskCreate( vStringPrinter, "String", 1000, NULL, 2, NULL );

	/* Install the handler for the software interrupt.  The syntax necessary
	to do this is dependent on the FreeRTOS port being used.  The syntax
	shown here can only be used with the FreeRTOS Windows port, where such
	interrupts are only simulated. */
	/* vPortSetInterruptHandler( mainINTERRUPT_NUMBER, ulExampleInterruptHandler ); */
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

static void vIntegerGenerator( void *pvParameters )
{
	TickType_t xLastExecutionTime;

	/* const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL ), xDontBlock = 0;
	Not available in the freeRTOS version included in the firmware_v2 modules */

	const TickType_t xDelay200ms = 200UL/portTICK_RATE_MS, xDontBlock = 0;
	uint32_t ulValueToSend = 0;
	BaseType_t i;

	/* Initialize the variable used by the call to vTaskDelayUntil(). */
	xLastExecutionTime = xTaskGetTickCount();

	for( ;; )
	{
		/* LED state is toggled, keep a live */
		gpioToggle( LED1 );

		/* This is a periodic task.  Block until it is time to run again.
		The task will execute every 200ms. */
		vTaskDelayUntil( &xLastExecutionTime, xDelay200ms );

		/* Send five numbers to the queue, each value one higher than the
		previous value.  The numbers are read from the queue by the interrupt
		service routine.  The interrupt	service routine always empties the
		queue, so this task is guaranteed to be able to write all five values
		without needing to specify a block time. */
		for( i = 0; i < 5; i++ )
		{
			xQueueSendToBack( xIntegerQueue, &ulValueToSend, xDontBlock );
			ulValueToSend++;
		}

		/* Generate the interrupt so the interrupt service routine can read the
		values from the queue. The syntax used to generate a software interrupt
		is dependent on the FreeRTOS port being used.  The syntax used below can
		only be used with the FreeRTOS Windows port, in which such interrupts
		are only simulated.*/
		vPrintString( "Generator task - About to generate an interrupt.\r\n" );

		/* vPortGenerateSimulatedInterrupt( mainINTERRUPT_NUMBER ); */
        mainTRIGGER_INTERRUPT();

        vPrintString( "Generator task - Interrupt generated.\r\n\r\n\r\n" );
	}
}
/*-----------------------------------------------------------*/

static void vStringPrinter( void *pvParameters )
{
char *pcString;

	for( ;; )
	{
		/* LED state is toggled, keep a live */
		gpioToggle( LED2 );

		/* Block on the queue to wait for data to arrive. */
		xQueueReceive( xStringQueue, &pcString, portMAX_DELAY );

		/* Print out the received string. */
		vPrintString( pcString );
	}
}
/*-----------------------------------------------------------*/

/* static uint32_t ulExampleInterruptHandler( void ) */
void vSoftwareInterruptHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t ulReceivedNumber;

	/* The strings are declared static const to ensure they are not allocated on the
	interrupt service routine's stack, and exist even when the interrupt service
	routine is not executing. */
	static const char *pcStrings[] =
	{
		"String 0\r\n",
		"String 1\r\n",
		"String 2\r\n",
		"String 3\r\n"
	};

	/* LED state is toggled, keep a live */
	gpioToggle( LED3 );

	/* As always, xHigherPriorityTaskWoken is initialized to pdFALSE to be able
	to detect it getting set to pdTRUE inside an interrupt safe API function. */
	xHigherPriorityTaskWoken = pdFALSE;

	/* Read from the queue until the queue is empty. */
	while( xQueueReceiveFromISR( xIntegerQueue, &ulReceivedNumber, &xHigherPriorityTaskWoken ) != errQUEUE_EMPTY )
	{
		/* Truncate the received value to the last two bits (values 0 to 3
		inc.), then use the truncated value as an index into the pcStrings[]
		array to select a string (char *) to send on the other queue. */
		ulReceivedNumber &= 0x03;
		xQueueSendToBackFromISR( xStringQueue, &pcStrings[ ulReceivedNumber ], &xHigherPriorityTaskWoken );
	}


	/* Clear the software interrupt bit using the interrupt controllers
     * Clear Pending register. */
    mainCLEAR_INTERRUPT();

    /* If receiving from xIntegerQueue caused a task to leave the Blocked state,
	and if the priority of the task that left the Blocked state is higher than
	the priority of the task in the Running state, then xHigherPriorityTaskWoken
	will have been set to pdTRUE inside xQueueReceiveFromISR().

    If sending to xStringQueue caused a task to leave the Blocked state, and
    if the priority of the task that left the Blocked state is higher than the
    priority of the task in the Running state, then xHigherPriorityTaskWoken
	will have been set to pdTRUE inside xQueueSendFromISR().


    xHigherPriorityTaskWoken is used as the parameter to portYIELD_FROM_ISR().
	If xHigherPriorityTaskWoken equals pdTRUE then calling portYIELD_FROM_ISR()
    will request a context switch.  If xHigherPriorityTaskWoken is still pdFALSE
	then calling portYIELD_FROM_ISR() will have no effect.

    The implementation of portYIELD_FROM_ISR() used by the Windows port includes
	a return statement, which is why this function does not explicitly return a
    value. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
