// FSM DECLARATIONS

#include "semaphore.h"
#include "led.h"

// FSM state names
typedef enum {
   ROJO,
   ROJO_AMARILLO,
   VERDE,
   AMARILLO,
} fsmState_t;

// FSM functions
static void fsmError( void );

// FSM IMPLEMENTATIONS

// Variable that hold the current state
static fsmState_t fsmState;

// FSM Error Handler Function
static void fsmError( void )
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

   bool_t flagRojo = false;

   switch( fsmState ) {

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
         fsmState = ROJO_AMARILLO;
         contador = 0;
      }
      
      if( fsmState != ROJO ) {
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
         fsmState = VERDE;
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
         fsmState = AMARILLO;
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
         fsmState = ROJO;
         contador = 0;
      }
      break;

   default:
      fsmError();
      break;
   }
}
