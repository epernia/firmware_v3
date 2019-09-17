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
#include <stdlib.h>

/*=====[Definition macros of private constants]==============================*/

#define DAC_REFERENCE_VALUE_HIGH   666  // 1023 = 3.3V, 666 = 2.15V
#define DAC_REFERENCE_VALUE_LOW    356  // 1023 = 3.3V, 356 = 1.15V

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main (void)
{
   // ----- Configurations --------------------------

   boardInit();

   uartInit( UART_USB, 115200 );

   adcInit( ADC_ENABLE );
   dacInit( DAC_ENABLE );

   gpioWrite( LEDB, ON );

   printf( "Generate a noise signal of in DAC between %.2f V and %.2f V\r\n\r\n",
           (float)DAC_REFERENCE_VALUE_LOW * 3.3/1023.0,
           (float)DAC_REFERENCE_VALUE_HIGH * 3.3/1023.0 );

   uint16_t dacValue = 0;
   uint16_t adcValue = 0;

   // Initialize random seed by reading an ADC not connected pin
   adcValue = adcRead(CH2);
   srand( adcValue );
   printf( "Seed Value = %d\r\n\r\n", adcValue );

   printf( "Begin to generate noise samples:\r\n" );

   // ----- Repeat for ever -------------------------
   while(TRUE) {
      // random = limite_inferior + rand() % (limite_superior +1 - limite_inferior);
      dacValue = DAC_REFERENCE_VALUE_LOW +
                 rand() % (DAC_REFERENCE_VALUE_HIGH+1 - DAC_REFERENCE_VALUE_LOW);
      dacWrite( DAC, dacValue );

      uartWriteString( UART_USB, intToString( dacValue) );
      uartWriteString( UART_USB, "\r\n" );

      delay(4);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
