/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include <program.h>

/*==================[definiciones y macros]==================================*/

using namespace sapi;

/*==================[definiciones de datos internos]=========================*/

DEBUG_PRINT_ENABLE
CONSOLE_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

struct {
   GpioIn button;
   GpioOut led;
} io[] = {
   { { TEC2 }, { LED1 } },
   { { TEC3 }, { LED2 } },
   { { TEC4 }, { LED3 } },
   { { TEC1 }, { LEDR } },
};

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   GpioOut blinkLed(LEDB);

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( true ) {
      // Pasar el estado de los botones a los leds
      // (invertidos porque al cerrar conectan a masa)
      for( auto& e: io )
         e.led = not e.button;

      /* Intercambiar el valor del pin conectado a LED blinking */
      if( blinkLed.toggle().isOn() ) {
         // Si esta encendido mostrar por UART_USB "LEDB encendido."
         debugPrintlnString( "Blinking led encendido." );
         consolePrintlnString( "Blinking led encendido." );
      } else {
         // Si esta apagado mostrar por UART_USB "LEDB apagado."
         debugPrintlnString( "Blinging led apagado." );
         consolePrintlnString( "Blinging led apagado." );
      }

      // Retardo bloqueante durante 250ms
      delay( 100 );
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
