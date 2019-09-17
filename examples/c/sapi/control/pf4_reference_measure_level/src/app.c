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

/*=====[Definition macros of private constants]==============================*/

#define THRESHOLD          511

#define N_SAMPLES          10000

#define SAMPLING_PERIOD    4 // 4ms

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/


/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
   // ----- Configurations --------------------------

   boardInit(); // Configurar plataforma

   printf( "Connect DAC output to ADC0 CH1\r\n" );
   
   adcInit( ADC_ENABLE );
   
   uint16_t sample = 0;
   uint16_t sampleArray[N_SAMPLES];

   // ----- Repeat for ever -------------------------
   /*
   while(TRUE) {
      sample = adcRead(CH1);
      if( sample >= THRESHOLD ) {
         gpioWrite(LED1,ON);
      } else {
         gpioWrite(LED1,OFF);
      }
      printf("%.4f\r\n", (float)sample/1023.0*3.3);
      delay(4);
   }
   */

   // Sincronize ADC Sample whit first sample >= THRESHOLD
   while(TRUE) {
      sample = adcRead(CH1);
      if( sample >= THRESHOLD ) {
         break;
      }
   }

   // Tack 100 samples (sample every h = 4 ms)
   sampleArray[0] = sample;
   uint32_t i;
   for( i=1; i<N_SAMPLES; i++){
      sampleArray[i] = adcRead(CH1); // 2.5 us to take a sample evrey 400 KHz
      delay(SAMPLING_PERIOD);   // 4 ms
   }

   // Print ADC samples
   printf( "Samples = [ " );
   for( i=0; i<N_SAMPLES; i++){
      printf("%.4f ", (float)sampleArray[i]/1023.0*3.3);
      fflush(stdout);
   }
   printf( "];\r\n" );
   
   gpioWrite(LEDB, ON);
   // End loop for ever
   while(1);
   
   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}

/*=====[Definitions of private functions]====================================*/

