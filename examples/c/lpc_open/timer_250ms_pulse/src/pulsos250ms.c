/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/07/09
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

// Posibles salidas de toggle pin on match

// ENET_CRS_DV ---> P1_16, FUNC4 = T0_MAT0
// ENET_RXD0   ---> P1_15, FUNC4 = T0_MAT1
// ENET_TXD0   ---> P1_18, FUNC4 = T0_MAT3

// GPIO3       ---> P6_7,  FUNC5 = T2_MAT0
// GPIO4       ---> P6_8,  FUNC5 = T2_MAT1
// GPIO5       ---> P6_9,  FUNC5 = T2_MAT2
// GPIO7       ---> P6_11, FUNC5 = T2_MAT3

// RS232_TXD   ---> P2_3,  FUNC6 = T3_MAT0
// RS232_RXD   ---> P2_4,  FUNC6 = T3_MAT1

#define SIGNAL_PIN_SCU_PORT   6              // GPIO3 (EDU-CIAA-NXP) --> P6
#define SIGNAL_PIN_SCU_PIN    7              // GPIO3 (EDU-CIAA-NXP) --> _7
#define SIGNAL_PIN_FUNC       5              // GPIO3 (EDU-CIAA-NXP) --> FUNC5

#define SIGNAL_TIMER          LPC_TIMER2     // LPC4337 TIMER2
#define SIGNAL_TIMER_CLK      CLK_MX_TIMER2  // LPC4337 TIMER2 CLOCK
#define SIGNAL_TIMER_RESET    RGU_TIMER2_RST // LPC4337 TIMER2 RGU
#define SIGNAL_MATCH_NUMBER   0              // MATCH0

#define SIGNAL_COMPARE_VAUE   51000          // TimerClk = 204 MHz ==> 204 MHz / 51000 = 4 KHz ==> 250 us
#define SIGNAL_INITIAL_STATE  0              // Signal initial state low

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of pubic functions]======================================*/

/*=====[Main function, program entry point after power on or reset]==========*/

void signalInit( void );
void signalCheck( void );

void signalStart( void );
void signalStop( void );

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();

   signalInit();  // Initialize timer to generate signal

   signalStart(); // Start to generate signal

   signalCheck(); // For debug (see output at printf UART)

   // ----- Repeat for ever -------------------------
   while( true ) {
      if( !gpioRead(TEC1) ) { // TEC1 start signal
         signalStart();
         printf( "Signal start.\r\n" );
         delay(250);
      }
      if( !gpioRead(TEC2) ) { // TEC2 stop signal
         signalStop();
         printf( "Signal stop.\r\n" );
         delay(250);
      }
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the
   // case of a PC program.
   return 0;
}


void signalInit( void )
{
   // Set pin to Timer match output mode (generate signal)
   Chip_SCU_PinMux( SIGNAL_PIN_SCU_PORT,
                    SIGNAL_PIN_SCU_PIN,
                    SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
                    SIGNAL_PIN_FUNC );

   // Enable timer clock and reset it
   Chip_TIMER_Init( SIGNAL_TIMER );
   Chip_RGU_TriggerReset( SIGNAL_TIMER_RESET );
   while( Chip_RGU_InReset( SIGNAL_TIMER_RESET ) );

   // Timer setup for match output toggle
   Chip_TIMER_Reset( SIGNAL_TIMER );

   Chip_TIMER_SetMatch( SIGNAL_TIMER,
                        SIGNAL_MATCH_NUMBER,
                        SIGNAL_COMPARE_VAUE );

   Chip_TIMER_ResetOnMatchEnable( SIGNAL_TIMER,
                                  SIGNAL_MATCH_NUMBER );

   Chip_TIMER_ExtMatchControlSet( SIGNAL_TIMER,
                                  SIGNAL_INITIAL_STATE,
                                  TIMER_EXTMATCH_TOGGLE,
                                  SIGNAL_MATCH_NUMBER );
}

void signalCheck( void )
{
   // Get timer peripheral clock rate
   uint32_t timerClkFreq = Chip_Clock_GetRate( SIGNAL_TIMER_CLK );

   printf( "timerClkFreq = %d [Hz]\r\n", timerClkFreq );

   float toggleFreq = (float) timerClkFreq / SIGNAL_COMPARE_VAUE;
   float toggleTime = (1.0 / toggleFreq) * 1000.0 * 1000.0;

   printf( "Toggle frequency = %.2f [Hz]\r\n", toggleFreq );
   printf( "Toggle time = %.2f [us]\r\n", toggleTime );
}

void signalStart( void )
{
   // Enable timer to start to generate signal
   Chip_TIMER_Enable( SIGNAL_TIMER );
}

void signalStop( void )
{
   // Disable timer to stop to generate signal
   Chip_TIMER_Disable( SIGNAL_TIMER );
}

