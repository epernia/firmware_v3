/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-07-28
 */

/*==================[inclusions]=============================================*/

#include "sapi_keypad.h"       /* <= own header */

#include "sapi_delay.h"               /* <= delay header */
#include "sapi_gpio.h"                /* <= GPIO header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/


/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/* Configure keypad pins */
bool_t keypadConfig( keypad_t* keypad,
                     gpioMap_t* keypadRowPins, uint8_t keypadRowSize,
                     gpioMap_t* keypadColPins, uint8_t keypadColSize ){

   bool_t retVal = TRUE;

   uint8_t i = 0;

   // Check if values are not invalid
   if( keypadRowPins == NULL || keypadColPins == NULL ||
       keypadRowSize <= 0 || keypadColSize <= 0  ){
      retVal = FALSE;
   }

   // Configure keypad instance
   keypad->keypadRowPins = keypadRowPins;
   keypad->keypadRowSize = keypadRowSize;
   keypad->keypadColPins = keypadColPins;
   keypad->keypadColSize = keypadColSize;

   // Configure Rows as Outputs
   for( i=0; i<keypadRowSize; i++ ){
      gpioConfig( keypad->keypadRowPins[i], GPIO_OUTPUT );
   }

   // Configure Columns as Inputs with pull-up resistors enable
   for( i=0; i<keypadColSize; i++ ){
      gpioConfig( keypad->keypadColPins[i], GPIO_INPUT_PULLUP );
   }

   return retVal;
}


/* Return TRUE if any key is pressed or FALSE (0) in other cases.
 * If exist key pressed write pressed key on key variable */
bool_t keypadRead( keypad_t* keypad, uint16_t* key ){

   bool_t retVal = FALSE;

   uint8_t r = 0; // Rows
   uint8_t c = 0; // Columns

   // Put all Rows in LOW state
   for( r=0; r<keypad->keypadRowSize; r++ ){
      gpioWrite( keypad->keypadRowPins[r], LOW );
   }

   // Check all Columns to search if any key is pressed
   for( c=0; c<keypad->keypadColSize; c++ ){

      // If reads a LOW state in a column then that key may be pressed
      if( !gpioRead( keypad->keypadColPins[c] ) ){

         delay( 50 ); // Debounce 50 ms

         // Put all Rows in HIGH state except first one
         for( r=1; r<keypad->keypadRowSize; r++ ){
            gpioWrite( keypad->keypadRowPins[r], HIGH );
         }

         // Search what key are pressed
         for( r=0; r<keypad->keypadRowSize; r++ ){

            // Put the Row[r-1] in HIGH state and the Row[r] in LOW state
            if( r>0 ){ // Prevents negative index in array
               gpioWrite( keypad->keypadRowPins[r-1], HIGH );
            }
            gpioWrite( keypad->keypadRowPins[r], LOW );

            // Check Columns[c] at Row[r] to search if the key is pressed
            // if that key is pressed (LOW state) then retuns the key
            if( !gpioRead( keypad->keypadColPins[c] ) ){
               *key = (uint16_t)r * (uint16_t)(keypad->keypadColSize) + (uint16_t)c;
               retVal = TRUE;
               return retVal;
            }
         }

      }
   }

   /*
      4 rows * 5 columns Keypad

         c0 c1 c2 c3 c4
      r0  0  1  2  3  4
      r1  5  6  7  8  9    Press r[i] c[j] => (i) * amountOfColumns + (j)
      r2 10 11 12 13 14
      r3 15 16 17 18 19
   */

   // if no key are pressed then retun FALSE
   return retVal;
}

/*==================[end of file]============================================*/
