/* 
Copyright 2015-2017, Eric Pernia.
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

// Date: 2017-12-06
/*==================[inlcusiones]============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

//#include "c_i18n_es.h" // <= para traducir el codigo C al espaï¿½ol (opcional)
//#include "c_i18n_es.h" // <= para traducir la sAPI al espaï¿½ol (opcional)

/*==================[definiciones y macros]==================================*/

//#define UART_SELECTED   UART_GPIO
//#define UART_SELECTED   UART_485
#define UART_SELECTED   UART_USB
//#define UART_SELECTED   UART_ENET
//#define UART_SELECTED   UART_232

/*==================[definiciones de datos internos]=========================*/

CONSOLE_PRINT_ENABLE

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/


// Caracter personalizado carita feliz :)
const char smile[8] = {
   0b00000000,
   0b00001010,
   0b00001010,
   0b00001010,
   0b00000000,
   0b00010001,
   0b00001110,
   0b00000000,
};

// Caracter personalizado letra e
const char e_char[8] = {
   0b01110,
   0b10000,
   0b10000,
   0b01100,
   0b01000,
   0b10000,
   0b10001,
   0b01110
};

// Caracter personalizado letra r
const char r_char[8] = {
   0b00000,
   0b00000,
   0b00000,
   0b01110,
   0b01000,
   0b01000,
   0b01000,
   0b01000
};

// Caracter personalizado letra i
const char i_char[8] = {
   0b00000,
   0b00100,
   0b00000,
   0b00100,
   0b00100,
   0b00100,
   0b00100,
   0b00110
};

// Caracter personalizado letra c
const char c_char[8] = {
   0b00000,
   0b00000,
   0b00000,
   0b00110,
   0b01000,
   0b01000,
   0b01001,
   0b00110
};

//Temperatura - Termometro
const char tempChar[8] = {
   0b01110,
   0b01010,
   0b01010,
   0b01110,
   0b01110,
   0b10111,
   0b11111,
   0b01110
};

// Humedad - Gota
const char humChar[8] = { 
   0b00100,
   0b00100,
   0b01110,
   0b10111,
   0b10111,
   0b10011,
   0b01110,
   0b00000
};

// Viento
const char vieChar[8] = {
   0b00111,
   0b11100,
   0b00000,
   0b00111,
   0b11100,
   0b00000,
   0b00111,
   0b11100
};

// Simbolo grados
const char graChar[8] = { 
   0b01110,
   0b01010,
   0b01110,
   0b00000,
   0b00000,
   0b00000,
   0b00000,
   0b00000
};


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida de consola
   consolePrintConfigUart( UART_USB, 115200 );


   // Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
   lcdInit( 16, 2, 5, 8 );

   // Cargar el caracter a CGRAM
   // El primer parámetro es el código del caracter (0 a 7).
   // El segundo es el puntero donde se guarda el bitmap (el array declarado
   // anteriormente)
   lcdCreateChar( 0, tempChar );
   lcdCreateChar( 1, humChar );
   lcdCreateChar( 2, vieChar );

   lcdCreateChar( 3, e_char );
   lcdCreateChar( 4, r_char );
   lcdCreateChar( 5, i_char );
   lcdCreateChar( 6, c_char );

   lcdCreateChar( 7, graChar );

   char tempString[] = "25";
   char humString[] = "50";
   char vieString[] = "10";

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      lcdClear(); // Borrar la pantalla

      lcdGoToXY( 1, 1 ); // Poner cursor en 1, 1

      lcdSendStringRaw( "Tmp" );
      lcdData(0);

      lcdSendStringRaw( " Hum" );
      lcdData(1);

      lcdSendStringRaw( " Vie" );
      lcdData(2);

      lcdGoToXY( 1, 2 );  // Poner cursor en 1, 2

      lcdSendStringRaw( tempString );
      lcdData(7);
      lcdSendStringRaw( "C" );

      lcdGoToXY( 6, 2 );  // Poner cursor en 7, 2
      lcdSendStringRaw( humString );
      lcdSendStringRaw( "%" );

      lcdGoToXY( 11, 2 ); // Poner cursor en 13, 2
      lcdSendStringRaw( vieString );
      lcdSendStringRaw( "km/h" );

      delay(3000);

      lcdClear(); // Borrar la pantalla
      lcdGoToXY( 1, 1 ); // Poner cursor en 1, 1
      lcdData(3);
      lcdData(4);
      lcdData(5);
      lcdData(6);

      delay(2000);

   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
