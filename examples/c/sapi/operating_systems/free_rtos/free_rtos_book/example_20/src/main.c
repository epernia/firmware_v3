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


/* Standard includes. */
#include <stdio.h>
/* #include <conio.h>
   Not available in the tools/arm-none-eabi/include/ */

/* FreeRTOS.org includes. */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "sapi.h"


/* Demo includes. */
/*	#include "supporting_functions.h"
	Not available in the freeRTOS version included in the firmware_v2 modules */


/* The DEBUG* functions are sAPI debug print functions.
   Code that uses the DEBUG* functions will have their I/O routed to
   the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;
#define	vPrintString(str) debugPrintString(str)


const char *pcTextForMain = "\r\nExample 20 - Re-writing vPrintString() to use a semaphore\r\n";

/* Sets up system hardware */
static void prvSetupHardware(void);

/* The task to be created.  Two instances of this task are created. */
static void prvPrintTask( void *pvParameters );

/* The function that uses a mutex to control access to standard out. */
static void prvNewPrintString( const char *pcString );

/*-----------------------------------------------------------*/

/* Declare a variable of type SemaphoreHandle_t.  This is used to reference the
mutex type semaphore that is used to ensure mutual exclusive access to stdout. */
SemaphoreHandle_t xMutex;

/* The tasks block for a pseudo random time between 0 and xMaxBlockTime ticks. */
const TickType_t xMaxBlockTimeTicks = 0x20;


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

    /* Before a semaphore is used it must be explicitly created.  In this example
	a mutex type semaphore is created. */
    xMutex = xSemaphoreCreateMutex();

	/* Check the semaphore was created successfully. */
	if( xMutex != NULL )
	{
		/* Create two instances of the tasks that attempt to write stdout.  The
		string they attempt to write is passed into the task as the task's
		parameter.  The tasks are created at different priorities so some
		pre-emption will occur. */
		xTaskCreate( prvPrintTask, "Print1", 1000, "Task 1 ******************************************\r\n", 1, NULL );
		xTaskCreate( prvPrintTask, "Print2", 1000, "Task 2 ------------------------------------------\r\n", 2, NULL );

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}

	/* The following line should never be reached because vTaskStartScheduler()
	will only return if there was not enough FreeRTOS heap memory available to
	create the Idle and (if configured) Timer tasks.  Heap management, and
	techniques for trapping heap exhaustion, are described in the book text. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

static void prvNewPrintString( const char *pcString )
{
	/* The semaphore is created before the scheduler is started so already
	exists by the time this task executes.

	Attempt to take the semaphore, blocking indefinitely if the mutex is not
	available immediately.  The call to xSemaphoreTake() will only return when
	the semaphore has been successfully obtained so there is no need to check the
	return value.  If any other delay period was used then the code must check
	that xSemaphoreTake() returns pdTRUE before accessing the resource (in this
	case standard out. */
	xSemaphoreTake( xMutex, portMAX_DELAY );
	{
		/* The following line will only execute once the semaphore has been
		successfully obtained - so standard out can be accessed freely. */
		printf( "%s", pcString );
		fflush( stdout );
	}
	xSemaphoreGive( xMutex );

	/* Allow any key to stop the application running.  A real application that
	actually used the key value should protect access to the keyboard too.  A
	real application is very unlikely to have more than one task processing
	key presses though! */
	/*	if( _kbhit() != 0 )
	{
		vTaskEndScheduler();
	}
	   Not available in the tools/arm-none-eabi/include/ */
}
/*-----------------------------------------------------------*/

static void prvPrintTask( void *pvParameters )
{
	char *pcStringToPrint;
	/* const TickType_t xSlowDownDelay = pdMS_TO_TICKS( 5UL );
	Not available in the freeRTOS version included in the firmware_v2 modules */

	const TickType_t xSlowDownDelay = 5UL/portTICK_RATE_MS;

	/* Two instances of this task are created.  The string printed by the task
	is passed into the task using the task's parameter.  The parameter is cast
	to the required type. */
	pcStringToPrint = ( char * ) pvParameters;

	for( ;; )
	{
		/* LED state is toggled, keep a live */
		gpioToggle( LED1 );

		/* Print out the string using the newly defined function. */
		prvNewPrintString( pcStringToPrint );

		/* Wait a pseudo random time.  Note that rand() is not necessarily
		re-entrant, but in this case it does not really matter as the code does
		not care what value is returned.  In a more secure application a version
		of rand() that is known to be re-entrant should be used - or calls to
		rand() should be protected using a critical section. */
		vTaskDelay( rand() % xMaxBlockTimeTicks );

		/* Just to ensure the scrolling is not too fast! */
		vTaskDelay( xSlowDownDelay );
	}
}
