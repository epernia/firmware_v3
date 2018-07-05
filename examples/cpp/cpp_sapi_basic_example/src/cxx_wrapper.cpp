#include "cxx_wrapper.hpp"
/*==================[definiciones de datos internos]=========================*/

DEBUG_PRINT_ENABLE
CONSOLE_PRINT_ENABLE

using namespace sapi;

Board_t Board;

Board_t::Board_t()
{
   boardConfig();
   debugPrintConfigUart( UART_USB, 115200 );
   consolePrintConfigUart( UART_232, 115200 );

   // Inicializar UART_USB como salida de consola
   debugPrintlnString( "UART_USB configurada." );
   consolePrintlnString( "UART_232 configurada." );
}
