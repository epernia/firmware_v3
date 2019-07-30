/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/04/26
 * Version: 1
 *===========================================================================*/

#include "semaphore.h"
#include "sapi.h"

// Version 1 - Temporizado con delay bloqueante -------------------------------

int main (void)
{
   fsmSemaphoreInit();

   while(true) {
      fsmSemaphoreUpdate();
      delay(1000);
   }

   return 0;
}


// Version 2 - Temporizado con delay no bloqueante ----------------------------
/*
int main (void)
{
   delay_t tiempo1;
   delayInit( &tiempo1, 1000 );

   while(true) {
      if( delayRead(&tiempo1) ) {
         fsmUpdate();
      }   
   }
   return 0;
}
*/

// Version 3 - Temporizado con Tick callback ----------------------------------
/*
void miFun( void * a )
{
   static cont = 0;
   cont++;
   if( cont >= 1000 ){
      fsmUpdate();
      cont = 0;
   }
}

int main (void)
{
   boardInit(); // Adentro ejecuta tickInit(1);
   tickCallbackSet( miFun, 0 );
   //tick_t time = tickRead();
   //tickWrite(10);

   while(true) {
      // Do other stuff
   }
   return 0;
}
*/
