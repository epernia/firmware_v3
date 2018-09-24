/* Copyright 2018, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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

/*==================[inclusions]=============================================*/

#include <stdio.h>

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

// La UART_USB se inicializa en el startup del microcontrolador a
// 115200 baudios  para salida y entrada estandar.

// Recordar en config.mk poner USE_NANO=n (usar newlib completa
// en lugar de newlib nano) para tener soporte de flotantes en 
// printf(), sin embargo, esto causa que el programa ocupe MUCHA
// mas RAM y FLASH.

int main( void )
{
   printf( "Pi vale aproximadamente %f\r\n\r\n", 3.14 );
   
   char text[50];
   int num;
   float numf;
   
   printf( "Ingrese una palabra (hasta 50 caracteres, sin espacios) y presione enter:\r\n" );
   scanf( "%s", text );
   printf( "La palabra ingresada es: %s\r\n\r\n", text );

   printf( "Ingrese un numero entero y presione enter:\r\n" );
   scanf( "%d", &num );
   printf( "El numero entero ingresado es: %d\r\n\r\n", num );
   
   printf( "Ingrese un numero flotante y presione enter:\r\n" );
   scanf( "%f", &numf );
   printf( "El numero flotante ingresado es: %f\r\n\r\n", numf );
   
   printf( "Fin del programa.\r\n" );

   while(1);     

   return 0;
}

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*==================[end of file]============================================*/
