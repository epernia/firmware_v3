/*=============================================================================
 * Author: Eric Pernia <ericpernia@gmail.com>
 * Date: 2019/05/23
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "cxx_wrapper.hpp"

/*=====[Definition macros of private constants]==============================*/

using namespace sapi;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

struct {
   GpioIn button;
   GpioOut led;
} io[] = {
   { { BUTTON }, { LED } },
};

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   GpioOut blinkLed(LED);

   blinkLed = ON;
   delay( 1000 );
   blinkLed = OFF;
   
   // ----- Repeat for ever -------------------------
   while( true ) {
      // Buttons --> Leds (with not because push = 0)
      for( auto& e: io )
         e.led = not e.button;
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
