#include "sapi.h"

void onRx( void *noUsado )
{
   char c = uartRxRead( UART_USB );
   printf( "Recibimos <<%c>> por UART\r\n", c );
}

int main(void)
{
   boardConfig();
   uartConfig( UART_USB, 115200 );
   uartRxInterruptCallbackSet( UART_USB, onRx );
   uartRxInterruptSet( UART_USB, true );
   while(TRUE) {
      sleepUntilNextInterrupt();
   }
   return 0;
}
