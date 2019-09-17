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

#define TICKRATE_HZ                20   // 2 generate 1 Hz, 20 generate 10 Hz

//#define DAC_REFERENCE_VALUE_HIGH   511  // 1023 = 3.3V, 511 = 1.65V
//#define DAC_REFERENCE_VALUE_LOW    0    // 1023 = 3.3V, 0 = 0V

#define DAC_REFERENCE_VALUE_HIGH   666  // 1023 = 3.3V, 666 = 2.15V
#define DAC_REFERENCE_VALUE_LOW    356  // 1023 = 3.3V, 356 = 1.15V

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

static bool_t tickCallback( void* ptr );

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
   // ----- Configurations --------------------------

   boardInit();
   dacInit( DAC_ENABLE );

   gpioInit( GPIO8, GPIO_OUTPUT ); // Configurate GPIO8 as OUTPUT

   // Confugurate TIMER1 in TICKER mode 
   timerTickerConfig( TIMER1, TICKRATE_HZ );
   timerTickerSetTickEvent( TIMER1, tickCallback );

   // Enable TIMER1
   timerSetPower( TIMER1, ON );

   printf( "TIMER1 generates a singal of %d Hz in DAC and LEDB\r\n", TICKRATE_HZ/2 );
   printf( "DAC oscilate between %.2f V and %.2f V\r\n\r\n", (float)DAC_REFERENCE_VALUE_LOW * 3.3/1023.0, (float)DAC_REFERENCE_VALUE_HIGH * 3.3/1023.0 );

   // ----- Repeat for ever -------------------------
   while(TRUE) {
      //gpioToggle(LEDB);
      //delay(500);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

// When the TIMER1 COMPARE_MATCH event is performed, execute this function:
static bool_t tickCallback( void* ptr )
{
   static uint16_t lastDacValue = DAC_REFERENCE_VALUE_LOW;

   dacWrite( DAC, lastDacValue );

   gpioToggle( LEDB ); // Show also in LEDB
   gpioToggle( GPIO8 ); // Show also in GPIO8

   if( lastDacValue == DAC_REFERENCE_VALUE_HIGH ){
      lastDacValue = DAC_REFERENCE_VALUE_LOW;
   } else{
      lastDacValue = DAC_REFERENCE_VALUE_HIGH;
   }
}