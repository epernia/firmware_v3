// Utilización pequeño retardo bloqueante de 1ms y una variable contador

#include "sapi.h"

int main( void )
{
   boardInit();
   bool_t tec1 = 0;
   uint32_t contador = 0;

   while(TRUE) {
      tec1 = !gpioRead(TEC1);
      gpioWrite( LEDB, tec1 );

      delay(1);
      contador++;
      if( contador >= 5000 ){
         gpioToggle( LED1 );
         contador = 0;
      }
   }

   return 0;
}
