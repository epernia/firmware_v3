/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include <programa.h>

/*==================[definiciones y macros]==================================*/

using namespace sapi;

/*==================[definiciones de datos internos]=========================*/

DEBUG_PRINT_ENABLE
CONSOLE_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

DigitalIn buttons[] = { { TEC2 }, { TEC3 }, { TEC4 }, { TEC1 } };
DigitalOut leds[] =   { { LED1 }, { LED2 }, { LED3 }, { LEDR } };

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   DigitalOut blinkLed(LEDB);

   // Inicializar UART_USB como salida de consola
   debugPrintlnString( "UART_USB configurada." );
   consolePrintlnString( "UART_232 configurada." );

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( true ) {
      // Pasar el estado de los botones a los leds
      // (invertidos porque al cerrar conectan a masa)
      for (int i=0; i<arraySize(buttons); i++)
         leds[i] = !buttons[i];

      /* Intercambiar el valor del pin conectado a LED blinking */
      blinkLed = !blinkLed;

      if( blinkLed == ON ) {
         // Si esta encendido mostrar por UART_USB "LEDB encendido."
         debugPrintlnString( "Blinking led encendido." );
         consolePrintlnString( "Blinking led encendido." );
      } else {
         // Si esta apagado mostrar por UART_USB "LEDB apagado."
         debugPrintlnString( "Blinging led apagado." );
         consolePrintlnString( "Blinging led apagado." );
      }

      /* Retardo bloqueante durante 250ms */
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
