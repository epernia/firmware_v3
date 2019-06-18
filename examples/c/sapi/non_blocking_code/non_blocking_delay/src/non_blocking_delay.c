// Utilizar las funciones de delay no bloqueante de la biblioteca sAPI

#include "sapi.h"

int main( void )
{
   boardInit();
   bool_t tec1 = 0;
	
   delay_t led1Delay;
   delayConfig( &led1Delay, 5000 );

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      if( delayRead(&led1Delay) ){
         gpioToggle( LED1 );
      }
   }

   return 0;
}
