// FSM DECLARATIONS

#include "semaphore.h"
#include "led.h"

// FSM state names
typedef enum {
   ROJO,
   ROJO_AMARILLO,
   VERDE,
   AMARILLO,
} fsmSemaphoreState_t;

// FSM functions
static void fsmSemaphoreError( void );

// FSM IMPLEMENTATIONS

// Variable that hold the current state
static fsmSemaphoreState_t fsmSemaphoreState;

// FSM Error Handler Function
static void fsmSemaphoreError( void )
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

   bool_t flagRojo = false;

   switch( fsmSemaphoreState ) {

   case ROJO:
      /* UPDATE OUTPUTS */
      if( flagRojo == false ) {
         // Codigo a ejecutar cuando entra al estado
         ledOn( LED_ROJO );
         ledOff( LED_AMARILLO );
         ledOff( LED_VERDE );
         flagRojo = true;
      }
      
      // Actualizar salida

      /* CHECK TRANSITION CONDITIONS */
      contador++;
      if( contador >= 3 ) {
         fsmSemaphoreState = ROJO_AMARILLO;
         contador = 0;
      }
      
      if( fsmSemaphoreState != ROJO ) {
         flagRojo = false;
         // Codigo a ejecutar cuando sale de del estado
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
