/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

#include "sapi.h"        // <= Biblioteca sAPI

uint8_t TecladoPinesFilas[4] = {
                                 RS232_TXD,
                                 CAN_RD,
                                 CAN_TD,
                                 T_COL1,
                               };

uint8_t TecladoPinesColumnas[4] = {
                                    T_FIL0,
                                    T_FIL3,
                                    T_FIL2,
                                    T_COL0,
                                  };
/*
// Test del teclado
int main( void )
{
   boardConfig();   
   uint8_t pin = 0;   
   for( pin=0; pin<4; pin++ ){      
      // Configuro las filas como salida
      gpioInit( TecladoPinesFilas[pin], GPIO_OUTPUT );
      gpioWrite( TecladoPinesFilas[pin], OFF );
      // Configuro las columnas como entradas, con resistencias de pull-up internas
      gpioInit( TecladoPinesColumnas[pin], GPIO_INPUT_PULLUP );
   }
   while( TRUE ) {   
      printf( "%d, %d, %d, %d\r\n", 
                  gpioRead( TecladoPinesColumnas[0] ),
                  gpioRead( TecladoPinesColumnas[1] ),
                  gpioRead( TecladoPinesColumnas[2] ),
                  gpioRead( TecladoPinesColumnas[3] )
               );            
      delay(100);
   }
   return 0;
}
*/

int8_t indiceACaracter[] = {
                              '1', '2', '3', '\r',  // 1/2 Enter
                              '4', '5', '6', 'B' ,
                              '7', '8', '9', 'C' ,
                              '*', '0', '#', 8   ,  // 8 = Retroceso
                           };

#define TECLADO_SCAN 40
#define TECLADO_MOSTRAR 100

void tecladoInit( void );
int8_t escanearTeclado( void );

int main( void )
{
   boardConfig();
   
   int8_t tecla = -1;
   
   tecladoInit();
   delay_t escaneoTeclado;
   delayInit( &escaneoTeclado, TECLADO_SCAN );
   
   delay_t mostrarTeclado;
   delayInit( &mostrarTeclado, TECLADO_MOSTRAR );
   
   while( TRUE ) {
      if( delayRead(&escaneoTeclado) ){
         tecla = escanearTeclado();         
      }      
      if( delayRead(&mostrarTeclado) ){
         if( tecla >= 0 ){
            //printf( "%d\r\n", tecla );
            if( tecla == '\r' ){
               printf( "\r\n" );
            } else{               
               printf( "%c", tecla );
            }
            fflush(stdout);
         }     
      }
      /*
      printf( "$%d %d;\r\n", adcRead(CH1), adcRead(CH2) );
      delay(100);
      */
   }
   return 0;
}

void tecladoInit( void )
{
   uint8_t pin = 0;    
   for( pin=0; pin<4; pin++ ){      
      // Configuro las filas como salida
      gpioInit( TecladoPinesFilas[pin], GPIO_OUTPUT );
      gpioWrite( TecladoPinesFilas[pin], ON );
      // Configuro las columnas como entradas, con resistencias de pull-up internas
      gpioInit( TecladoPinesColumnas[pin], GPIO_INPUT_PULLUP );
   }
}

int8_t escanearTeclado( void ) // devuelve el indice de tecla [0,15] o -1 si no hay tecla presionada
{
   int8_t f = 0;
   int8_t i = 0;
   int8_t c = 0;
   
   // Escanear todas las filas
   for( f=0; f<4; f++ ){

      // Prendo todas las filas (el estado normal por las resistencias de pull-up)
      for( i=0; i<4; i++ ){  
         gpioWrite( TecladoPinesFilas[i], ON );
      }

      // Apago la fila que quiero escanear
      gpioWrite( TecladoPinesFilas[f], OFF );
      
      // Chequeo si alguna columna lee el OFF de mi fila actual
      for( c=0; c<4; c++ ){
         // Chequeo que la columna actual este en OFF (hay una en OFF si se presiona algun boton)
         if( gpioRead( TecladoPinesColumnas[c] ) == OFF ){
            return indiceACaracter[4*f + c];
         }
      }      
   }
   return -1;
}
