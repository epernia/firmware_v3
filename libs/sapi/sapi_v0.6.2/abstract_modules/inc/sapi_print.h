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
 */
 

/* Date: 2017-04-17 */

#ifndef _SAPI_PRINT_H_
#define _SAPI_PRINT_H_

/*

API
---

// Initialize
void printSetUart( print_t* printer, uartMap_t uart );
void printInit( print_t printer, uartMap_t uart, uint32_t baudRate );


// Print String
void printString( print_t printer, const char* string );
void printlnString( print_t printer, const char* string );
void printEnter( print_t printer );

// Print Integer
void printIntFormat( print_t printer, int64_t number, numberFormat_t format );
void printUIntFormat( print_t printer, uint64_t number, numberFormat_t format );
void printlnIntFormat( print_t printer, int64_t number, numberFormat_t format );
void printlnUIntFormat( print_t printer, uint64_t number, numberFormat_t format );
void printInt( print_t printer, int64_t number );
void printUInt( print_t printer, uint64_t number );
void printlnInt( print_t printer, int64_t number );
void printlnUInt( print_t printer, uint64_t number );
void printHex( uint64_t number, uint8_t bitSize );
*/

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_convert.h"
#include "sapi_peripheral_map.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define PRINT_ENTER_STRING   "\r\n"

// Any printer

#define printlnString(printer,string);  {printString((printer),(string));\
                                        printEnter((printer));}


#define printInt(printer,number)        printIntFormat((printer),(number),(DEC_FORMAT))

#define printUInt(printer,number)       printUIntFormat((printer),(number),(DEC_FORMAT))


#define printlnInt(printer,number);     {printInt((printer),number);\
                                        printEnter((printer));}

#define printlnUInt(printer,number);    {printUInt((printer),number);\
                                        printEnter((printer));}

// Ver que pueden molestar dentro de un bucle!
#define printlnIntFormat(printer,number,format);   {printIntFormat((printer),(number),(format));\
                                                   printEnter((printer));}

#define printlnUIntFormat(printer,number,format);  {printUIntFormat((printer),(number),(format));\
                                                   printEnter((printer));}

#define printlnHex(printer,number,bitSize);        {printHex((printer),(number),(bitSize));\
                                                   printEnter((printer));}

#define printInitUart printInitUart

/*==================[typedef]================================================*/

typedef uartMap_t print_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// Initialize
void printSetUart( print_t* printer, uartMap_t uart );
void printInitUart( print_t* printer, uartMap_t uart, uint32_t baudRate );

// Print Char
void printChar( print_t printer, const char aChar );

// Print String
void printString( print_t printer, const char* string );
void printEnter( print_t printer );

// Print Integer
void printIntFormat( print_t printer, int64_t number, numberFormat_t format );
void printUIntFormat( print_t printer, uint64_t number, numberFormat_t format );
void printHex( print_t printer, uint64_t number, uint8_t bitSize );

/*==================[examples]===============================================*/

/*
printlnString( printer, "Uart Debug configurada." );
printlnString( printer, "Uart LoRa RN2903 configurada." );

printString( printer, "Numero de algo: " );
printInt( printer, 45454578 );
printlnString( printer, "" );

printlnInt( printer, -457 );
printlnUInt( printer, -457 );

printString( printer, "Numero de algo: " );
printIntFormat( printer, 45454578, DEC_FORMAT );
printlnString( printer, "" );

printString( printer, "Numero de algo: 0b" );
printIntFormat( printer, 45454578, BIN_FORMAT );
printlnString( printer, "" );

printString( printer, "Numero de algo: 0x" );
printIntFormat( printer, 45454578, HEX_FORMAT );
printlnString( printer, "" );
*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_PRINT_H_ */
