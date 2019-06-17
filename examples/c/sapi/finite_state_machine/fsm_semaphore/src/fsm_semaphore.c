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

// FSM DECLARATIONS

// FSM state names
typedef enum{
   ROJO,
   ROJO_AMARILLO,
   AMARILLO,
   VERDE,
} fsmState_t;

// FSM functions
void fsmError( void );
void fsmInit( void );
void fsmUpdate( void );

// FSM IMPLEMENTATIONS

// Variable that hold the current state
fsmState_t fsmState;


/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   fsmInit();

   // ----- Repeat for ever -------------------------
   while( true ){
      fsmUpdate();
   }
   return 0;
}



// FSM Error Handler Function
void fsmError( void )
{
   // Error handler, example, restart FSM:
   fsmState = ROJO;
}

// FSM Initialize Function
void fsmInit( void )
{
   boardInit();
   ledsInit();
   fsmState = ROJO;   // Set initial state
}

// FSM Update Sate Function
void fsmUpdate( void )
{
   static uint32_t contador = 0;
   
   switch( fsmState ){

      case ROJO:         
         /* UPDATE OUTPUTS */
         ledOn( LED_ROJO );
         ledOff( LED_AMARILLO );
         ledOff( LED_VERDE ); 
         
         /* CHECK TRANSITION CONDITIONS */
         contador++;
         if( contador >= 3 ){
            fsmState = ROJO_AMARILLO;
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
            fsmState = VERDE;
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
            fsmState = ROJO;
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
            fsmState = AMARILLO;
            contador = 0;
         }
      break;

      default:
         fsmError();
      break;
   }
}
