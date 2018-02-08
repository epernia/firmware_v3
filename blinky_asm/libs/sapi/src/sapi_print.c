/* Copyright 2017, Eric Pernia.
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
 *
 */

/* Date: 2017-04-17 */

/*==================[inclusions]=============================================*/

#include "sapi_print.h"     // <= own header

#include "sapi_convert.h"   // <= Convert header
#include "sapi_uart.h"      // <= UART header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

// Print uart configuration

void printSetUart( print_t* printer, uartMap_t uart ){
   *printer = uart;
}

void printConfigUart( print_t* printer, uartMap_t uart, uint32_t baudRate ){
   *printer = uart;
   uartConfig( uart, baudRate );
}


// Print String

void printString( print_t printer, char* string ){
   uartWriteString( printer, string );
}

void printEnter( print_t printer ){
   uartWriteString( printer, PRINT_ENTER_STRING );
}


// Print Integer

void printIntFormat( print_t printer, int64_t number, numberFormat_t format ){

   char strNumber[65];

   if( int64ToString( number, strNumber, format ) ){
      uartWriteString( printer, strNumber );
   }
}

void printUIntFormat( print_t printer, uint64_t number, numberFormat_t format ){

   char strNumber[65];

   if( uint64ToString( number, strNumber, format ) ){
      uartWriteString( printer, strNumber );
   }
}

void printHex( print_t printer, uint64_t number, uint8_t bitSize ){
   printString( printer, uintToAsciiHex( number , bitSize ) );
}


/*==================[end of file]============================================*/
