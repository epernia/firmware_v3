/* Copyright 2019, Eric Pernia.
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
 */

#include <sapi.h>

/* -------------- Declarations ----------------------- */

// Semaphore ----------------------

typedef enum {
   ROJO,
   ROJO_AMARILLO,
   AMARILLO,
   VERDE,
} fsmSemaphoreState_t;

void fsmSemaphoreInit( void );
void fsmSemaphoreUpdate( void );
void fsmSemaphoreError( void );

fsmSemaphoreState_t fsmSemaphoreState; // Sate variable

#define LED_ROJO     LED2
#define LED_AMARILLO LED1
#define LED_VERDE    LEDG

void ledsInit( void );
void ledOn( int32_t pin );
void ledOff( int32_t pin );

// Debounce button ----------------

typedef enum {
   STATE_BUTTON_UP,
   STATE_BUTTON_DOWN,
   STATE_BUTTON_FALLING,
   STATE_BUTTON_RISING
} fsmButtonState_t;

void fsmButtonInit( void );
void fsmButtonUpdate( gpioMap_t tecla );
void fsmButtonError( void );

void buttonPressed( void );
void buttonReleased( void );

fsmButtonState_t fsmButtonState; // Sate variable

/* ------------- Main function ----------------------- */

int main(void)
{
   // Inicializo la placa
   boardInit();

   // Inicializo temporizacion y MEF Boton
   delay_t actualizarMefBoton;
   delayInit( &actualizarMefBoton, 40 );
   fsmButtonInit();

   // Inicializo temporizacion y MEF Semaforo
   delay_t actualizarMefSemaforo;
   delayInit( &actualizarMefSemaforo, 1000 );
   fsmSemaphoreInit();

   // Inicializo temporizacion del LED
   delay_t actualizarLed;
   delayInit( &actualizarLed, 250 );

   while(TRUE) {

      // Actualizo MEF Boton cada tiempo actualizarMefBoton
      if( delayRead(&actualizarMefBoton) ) {
         fsmButtonUpdate(TEC1);
      }

      // Actualizo MEF Semaforo cada tiempo actualizarMefSemaforo
      if( delayRead(&actualizarMefSemaforo) ) {
         fsmSemaphoreUpdate();
      }

      // Actualizo el valor del LED cada tiempo actualizarLed
      if( delayRead(&actualizarLed) ) {
         gpioToggle(LED3);
      }

   }
   return 0;
}

/* -------------- Implementations -------------------- */

// Semaphore ----------------------

void fsmSemaphoreInit( void )
{
   ledsInit();
   fsmSemaphoreState = ROJO;   // Set initial state
}
// ---------------------------------

void fsmSemaphoreUpdate( void )
{
   static uint32_t contador = 0;

   switch( fsmSemaphoreState ) {

   case ROJO:
      /* UPDATE OUTPUTS */
      ledOn( LED_ROJO );
      ledOff( LED_AMARILLO );
      ledOff( LED_VERDE );

      /* CHECK TRANSITION CONDITIONS */
      contador++;
      if( contador >= 3 ) {
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
      if( contador >= 1 ) {
         fsmSemaphoreState = VERDE;
         contador = 0;
      }
      break;

   case VERDE:
      /* UPDATE OUTPUTS */
      ledOff( LED_ROJO );
      ledOff( LED_AMARILLO );
      ledOn( LED_VERDE );

      /* CHECK TRANSITION CONDITIONS */
      contador++;
      if( contador >= 2 ) {
         fsmSemaphoreState = AMARILLO;
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
      if( contador >= 1 ) {
         fsmSemaphoreState = ROJO;
         contador = 0;
      }
      break;

   default:
      fsmSemaphoreError();
      break;
   }
}
// ---------------------------------

void fsmSemaphoreError( void )
{
   // Error handler, example, restart FSM:
   fsmSemaphoreState = ROJO;
}
// ---------------------------------

void ledsInit( void )
{
   gpioInit( LED_ROJO, GPIO_OUTPUT );
   gpioInit( LED_AMARILLO, GPIO_OUTPUT );
   gpioInit( LED_VERDE, GPIO_OUTPUT );
}
// ---------------------------------

void ledOn( int32_t pin )
{
   gpioWrite( pin, ON );
}
// ---------------------------------

void ledOff( int32_t pin )
{
   gpioWrite( pin, OFF );
}
// ---------------------------------

// Debounce button ----------------

void fsmButtonInit( void )
{
   fsmButtonState = BUTTON_UP;  // Set initial state
}
// ---------------------------------

void fsmButtonUpdate( gpioMap_t tecla )
{
   static bool_t flagFalling = FALSE;
   static bool_t flagRising = FALSE;

   static uint8_t contFalling = 0;
   static uint8_t contRising = 0;

   switch( fsmButtonState ) {

   case STATE_BUTTON_UP:
      /* CHECK TRANSITION CONDITIONS */
      if( !gpioRead(tecla) ) {
         fsmButtonState = STATE_BUTTON_FALLING;
      }
      break;

   case STATE_BUTTON_DOWN:
      /* CHECK TRANSITION CONDITIONS */
      if( gpioRead(tecla) ) {
         fsmButtonState = STATE_BUTTON_RISING;
      }
      break;

   case STATE_BUTTON_FALLING:
      /* ENTRY */
      if( flagFalling == FALSE ) {
         flagFalling = TRUE;
         gpioWrite(LED1, ON);
         break;
      }
      /* CHECK TRANSITION CONDITIONS */
      if( !gpioRead(tecla) ) {
         fsmButtonState = STATE_BUTTON_DOWN;
         buttonPressed();
      } else {
         fsmButtonState = STATE_BUTTON_UP;
      }

      /* LEAVE */
      if( fsmButtonState != STATE_BUTTON_FALLING ) {
         flagFalling = FALSE;
         gpioWrite(LED1, OFF);
      }
      break;

   case STATE_BUTTON_RISING:
      /* ENTRY */
      if( flagRising == FALSE ) {
         flagRising = TRUE;
         gpioWrite(LED2, ON);
         break;
      }
      /* CHECK TRANSITION CONDITIONS */
      if( gpioRead(tecla) ) {
         fsmButtonState = STATE_BUTTON_UP;
         buttonReleased();
      } else {
         fsmButtonState = STATE_BUTTON_DOWN;
      }

      /* LEAVE */
      if( fsmButtonState != STATE_BUTTON_RISING ) {
         flagRising = FALSE;
         gpioWrite(LED2, OFF);
      }
      break;

   default:
      fsmButtonError();
      break;
   }
}
// ---------------------------------

void fsmButtonError( void )
{
   fsmButtonState = BUTTON_UP;
}
// ---------------------------------

void buttonPressed( void )
{
   static uint8_t cont = 0;
   gpioWrite(LEDR, OFF);
   gpioWrite(LEDG, OFF);
   gpioWrite(LEDB, ON);
   if(cont>5) {
      gpioWrite(LEDR, OFF);
      gpioWrite(LEDG, ON);
      gpioWrite(LEDB, OFF);
   }
   cont++;
}
// ---------------------------------

void buttonReleased( void )
{
   static uint8_t cont = 0;
   gpioWrite(LEDR, ON);
   gpioWrite(LEDG, OFF);
   gpioWrite(LEDB, OFF);
   if(cont>10) {
      gpioWrite(LEDR, OFF);
      gpioWrite(LEDG, OFF);
      gpioWrite(LEDB, OFF);
   }
   cont++;
}
// ---------------------------------
