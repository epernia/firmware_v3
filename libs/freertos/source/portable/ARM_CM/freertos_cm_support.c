#include <stdint.h>

/* Board specifics. */
#if (BOARD == ciaa_nxp)
   #include <freertos_lpc4337.h>
#elif (BOARD == edu_ciaa_nxp)
   #include <freertos_lpc4337.h>
#elif (BOARD == lpcxpresso1769)
   #include <freertos_lpc4337.h>
#endif

#include <freertos_cm_support.h>

typedef uint32_t (*freeRtosInterruptCallback_t)(void);

volatile freeRtosInterruptCallback_t freeRtosInterruptCallback = NULL;

/*-----------------------------------------------------------*/

/*
 * Raise a simulated interrupt represented by the bit mask in ulInterruptMask.
 * Each bit can be used to represent an individual interrupt - with the first
 * two bits being used for the Yield and Tick interrupts respectively.
*/
void vPortGenerateSimulatedInterrupt( uint32_t ulInterruptNumber )
{
   NVIC_SetPendingIRQ( ulInterruptNumber ); // mainSW_INTERRUPT_ID
}
/*-----------------------------------------------------------*/

/*
 * Install an interrupt handler to be called by the simulated interrupt handler
 * thread.  The interrupt number must be above any used by the kernel itself
 * (at the time of writing the kernel was using interrupt numbers 0, 1, and 2
 * as defined above).  The number must also be lower than 32.
 *
 * Interrupt handler functions must return a non-zero value if executing the
 * handler resulted in a task switch being required.
 */
void vPortSetInterruptHandler( uint32_t ulInterruptNumber, uint32_t (*pvHandler)( void ) )
{
   if( pvHandler != NULL ) {
      freeRtosInterruptCallback = pvHandler;
   }

	/* The interrupt service routine uses an (interrupt safe) FreeRTOS API
	 * function so the interrupt priority must be at or below the priority defined
	 * by configSYSCALL_INTERRUPT_PRIORITY. */
	NVIC_SetPriority( mainSW_INTERRUPT_ID, mainSOFTWARE_INTERRUPT_PRIORITY );

	/* Enable the interrupt. */
   NVIC_EnableIRQ( mainSW_INTERRUPT_ID );
}
/*-----------------------------------------------------------*/

// ISR Handler
void vSoftwareInterruptHandler( void )
{
   NVIC_ClearPendingIRQ( mainSW_INTERRUPT_ID );

   // Execute Tick Hook function if pointer is not NULL
   if( freeRtosInterruptCallback != NULL ) {
      (* freeRtosInterruptCallback )();
   }
}
/*-----------------------------------------------------------*/
