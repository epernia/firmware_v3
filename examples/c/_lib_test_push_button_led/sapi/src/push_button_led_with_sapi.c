/*=============================================================================
 * Copyright (c) 2021, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause
 * Date: 2021/04/24
 * Version: 1.0
 *===========================================================================*/

// Ejemplo para prender un LED haciendo polling	sobre el estado de un pulsador


// Bibliotecas del fabricante LCP Open ========================================

#include <sapi.h>

// Inicio de programa principal ===============================================

int main( void )
{    
   // Inicializo la plataforma ------------------------------------------------

   boardInit();
    
   // Diodo LED: LED1 <---> P2_10, GPIO0[14] ----------------------------------

   gpioInit( LED1, GPIO_OUTPUT );

   // Pulsador: TEC1 <---> P1_0, GPIO0[4] -------------------------------------

   gpioInit( TEC1, GPIO_INPUT );

   while( 1 ) {
       
      // Repetir por siempre --------------------------------------------------
       
      // !(TEC1) --> LED1
      // Negado (!) porque el pulsador esta en 1 por defecto y al presionar da 0
      uint8_t value = gpioRead( TEC1 );
      gpioWrite( LED1, !value );
   }

   return 0;
}
