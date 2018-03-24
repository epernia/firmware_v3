/* Copyright 2017, Sebastian Pablo Bedin.
   Copyright 2018, Eric Pernia.
   All rights reserved.

This file is part sAPI library for microcontrollers.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

// First version date: 2017-11-13

/* 
DHT11 connections:
                                                DHT11
                                           +--------------+         
             EDU-CIAA-NXP +3.3V <--> 1 ----| +++  +++  +++|
   EDU-CIAA-NXP GPIO1 (pull-up) <--> 2 ----| +++  +++  +++|       
                 (SIN CONEXION) <--> 3 ----| +++  +++  +++|
               EDU-CIAA-NXP GND <--> 4 ----| +++  +++  +++|
                                           +--------------+

                                           DHT11 on a board
                                          +----------------+
                                          |    +-----------+--+         
             EDU-CIAA-NXP GPIO1 <--> S ----o --| +++  +++  +++|
             EDU-CIAA-NXP +3.3V <--> + ----o --| +++  +++  +++|       
              EDU-CIAA-NXP GND  <--> - ----o --| +++  +++  +++|
                                          |  --| +++  +++  +++|
                                          |    +-----------+--+
                                          +----------------+
*/

/*==================[inclusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

/*==================[definiciones y macros]==================================*/

// UART list:
//  - UART_USB or UART_ENET
//  - UART_232
//  - UART_GPIO or UART_485

#define UART_DEBUG UART_USB

/*==================[definiciones de datos internos]=========================*/

//CONSOLE_PRINT_ENABLE
DEBUG_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static void format( float valor, char *dst, uint8_t pos );

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida de consola
   debugPrintConfigUart( UART_DEBUG, 115200 );
   
	char buffout[64];
	float hum = 0, temp = 0;
   
   dht11Init(GPIO1);

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
		if( dht11Read( &hum, &temp ) ) {
         
			gpioWrite( LEDG, ON );
			gpioWrite( LEDR, OFF );
         
			debugPrintString( "Temperatura: ");
			format( temp, buffout, 0 );
			debugPrintString( buffout );
			debugPrintlnString( " grados C." );

			debugPrintString( "Humedad: ");
			format( hum, buffout, 0 );
			debugPrintString( buffout );
			debugPrintlnString( " %." );
         
			debugPrintEnter();

		} else {
			gpioWrite( LEDG, OFF );
			gpioWrite( LEDR, ON );
         
			debugPrintlnString( "Error al leer DHT11." );         
			debugPrintEnter();
		}   
		delay(1000);
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

static void format( float valor, char *dst, uint8_t pos ){
	uint16_t val;
	val = 10 * valor;
	val = val % 1000;
	dst[pos] = (val / 100) + '0';
	pos++;
	dst[pos] = (val % 100) / 10 + '0';
	pos++;
	dst[pos] = '.';
	pos++;
	dst[pos] = (val % 10)  + '0';
	pos++;
	dst[pos] = '\0';
}

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
