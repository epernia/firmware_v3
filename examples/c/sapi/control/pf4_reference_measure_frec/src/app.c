/*=====[app]===================================================================
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2018/10/14
 */

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "sapi_timer_modified.h"

/*=====[Definition macros of private constants]==============================*/

#define TIMER0_PRESCALER   10000
#define CAP_NUMB           2

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

static volatile bool_t calculated = FALSE;
static volatile uint32_t tCapturadoAnterior = 0;
static volatile uint32_t tCapturadoActual = 0;
static volatile float result = 0.0;

/*=====[Prototypes (declarations) of private functions]======================*/

static bool_t timer0CaptureCallback( void* ptr );

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
   // ----- Configurations --------------------------

   boardInit(); // Configurar plataforma

   // ENET_TXD1 --> P1.20, FUNC4 --> TIMER0_CAP2
   printf( "Connect DAC output to TXD1 (ETHERNET).\r\n\r\n" );
   
   gpioConfig( ENET_TXD1, GPIO_INPUT_PULLUP ); 

   // Configurar Timer 0 en modo INPUT_CAPTURE (solo en flanco de subida)
   timerInputCaptureConfig( TIMER0,           // Timer
                            TIMER_CAPTURE2,   // Capture Number
                            TIMER0_PRESCALER, // Timer Prescaler
                            TRUE,             // Interrupt on Rising edge
                            FALSE );          // Interrupt on Falling edge

   timerInputCaptureSetCaptureEvent( TIMER0,                  // Timer
                                     TIMER_CAPTURE2,          // Capture Number
                                     timer0CaptureCallback ); // Callback 

   // Enable TIMER0
   timerSetPower( TIMER0, ON );

   printf( "TIMER0 Configure as input capture on pin ENET_TXD1 --> T0_CAP2.\r\n\r\n" );

   printf( "F_CPU: 204 MHz.\r\n" );
   printf( "Prescale_Timer: %d.\r\n", timerGetPrescale(TIMER0) );
   printf( "F_Timer: %.5f MHz.\r\n\r\n", (float)timerGetClock(TIMER0)/1000000.0 );

   delay_t delay1s;
   delayInit( &delay1s, 1000 ); // Periodic delay 1000 ms = 1 s

   // ----- Repeat for ever -------------------------
   while(TRUE) {
      if( delayRead(&delay1s) ){
         printf( "Tmeasure: %.5f us\r\n", result );      
         printf( "Fmeasure: %.5f Hz\r\n", 1000000.0/result );
         printf( "\r\n" );
      }
   }
   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

/*=====[Definitions of private functions]====================================*/

// When a RISING EDDGE event ocurs on TIMER0, execute the function
static bool_t timer0CaptureCallback( void* ptr )
{
   if( calculated == FALSE ){
      tCapturadoAnterior = timerReadCapture( TIMER0, TIMER_CAPTURE2 );
      calculated = TRUE;
   } else{
      tCapturadoActual = timerReadCapture( TIMER0, TIMER_CAPTURE2 );
      // Prescale = 10, CPU_CLK = 204MHz, con *1000 da en ms
      result = (float)( tCapturadoActual - tCapturadoAnterior ) 
               / ((float)timerGetClock( TIMER0)/1000000.0);
      tCapturadoAnterior = tCapturadoActual;
   }
   gpioToggle( LED1 ); // This LED blinks at half rate of measure signal becaus
                       // only search for rising edges
}
