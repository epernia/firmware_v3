/* Copyright 2018, Eric Pernia.
 * All rights reserved.
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
 
 #include <sapi.h>

// Macros publicas

#define LED_ROJO     LED2
#define LED_AMARILLO LED1
#define LED_VERDE    LEDG

// Declaraciones de funciones publicas

void ledsInit( void );
void ledOn( int32_t pin );
void ledOff( int32_t pin );

// FSM DECLARATIONS

// FSM state names
typedef enum{
   ROJO,
   ROJO_AMARILLO,
   AMARILLO,
   VERDE,
} fsmSemaphoreState_t;

// FSM functions
void fsmSemaphoreError( void );
void fsmSemaphoreInit( void );
void fsmSemaphoreUpdate( void );

// FSM IMPLEMENTATIONS

// Variable that hold the current state
fsmSemaphoreState_t fsmSemaphoreState;


/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   fsmSemaphoreInit();

   // ----- Repeat for ever -------------------------
   while( true ){
      fsmSemaphoreUpdate();
   }
   return 0;
}



// FSM Error Handler Function
void fsmSemaphoreError( void )
{
   // Error handler, example, restart FSM:
   fsmSemaphoreState = ROJO;
}

// FSM Initialize Function
void fsmSemaphoreInit( void )
{
   boardInit();
   ledsInit();
   fsmSemaphoreState = ROJO;   // Set initial state
}

// FSM Update Sate Function
void fsmSemaphoreUpdate( void )
{
   static uint32_t contador = 0;
   
   switch( fsmSemaphoreState ){

      case ROJO:         
         /* UPDATE OUTPUTS */
         ledOn( LED_ROJO );
         ledOff( LED_AMARILLO );
         ledOff( LED_VERDE ); 
         
         /* CHECK TRANSITION CONDITIONS */
         contador++;
         if( contador >= 3 ){
            fsmSemaphoreState = ROJO_AMARILLO;
            contador = 0;
         }
      break;
      
      case ROJO_AMARILLO:            
         /* UPDATE OUTPUTS */
         ledOn( LED_ROJO );
         ledOn( LED_AMARILLO );
         ledOff( LED_VERDE ); 
         
         /* CHECK TRANSITION CONDITIONS */
         contador++;
         if( contador >= 1 ){
            fsmSemaphoreState = VERDE;
            contador = 0;
         }
      break;
      
      case AMARILLO:
         /* UPDATE OUTPUTS */
         ledOff( LED_ROJO );
         ledOn( LED_AMARILLO );
         ledOff( LED_VERDE ); 
         
         /* CHECK TRANSITION CONDITIONS */
         contador++;
         if( contador >= 1 ){
            fsmSemaphoreState = ROJO;
            contador = 0;
         }
      break;
      
      case VERDE:         
         /* UPDATE OUTPUTS */
         ledOn( LED_ROJO );
         ledOff( LED_AMARILLO );
         ledOff( LED_VERDE ); 
         
         /* CHECK TRANSITION CONDITIONS */
         contador++;
         if( contador >= 2 ){
            fsmSemaphoreState = AMARILLO;
            contador = 0;
         }
      break;

      default:
         fsmSemaphoreError();
      break;
   }
}



void ledsInit( void )
{
   gpioInit( LED_ROJO, GPIO_OUTPUT );
   gpioInit( LED_AMARILLO, GPIO_OUTPUT );
   gpioInit( LED_VERDE, GPIO_OUTPUT );
}

void ledOn( int32_t pin )
{
   gpioWrite( pin, ON );
}

void ledOff( int32_t pin )
{
   gpioWrite( pin, OFF );
}
