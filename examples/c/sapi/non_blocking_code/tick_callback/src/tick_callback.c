// Utilizar las funciones de manejo del Timer SysTick de la arquitectura ARM
// Cortex M mediante las funciones tick de la biblioteca sAPI.

#include "sapi.h"

int main( void )
{
   boardInit(); // Adentro llama a tickInit(1);
	
   bool_t tec1 = 0;
   tick_t tiempoEnTicks = 0;

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      tiempoEnTicks = tickRead();
      if( tiempoEnTicks >= 5000 ){
         gpioToggle( LED1 );
         tickWrite(0);
      }
   }

   return 0;
}
