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

#ifndef _SAPI_CONVERT_H_
#define _SAPI_CONVERT_H_

/*

API
---

// C++ version 0.4 char* style "itoa":
bool_t int64ToString( int64_t value, char* result, uint8_t base );
bool_t uint64ToString( uint64_t value, char* result, uint8_t base );

char* uintToAsciiHex( uint64_t value, uint8_t bitSize );
*/

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[typedef]================================================*/

typedef enum {
   BIN_FORMAT = 2,
   DEC_FORMAT = 10,
   HEX_FORMAT = 16
} numberFormat_t;

/*==================[external functions declaration]=========================*/

bool_t int64ToString( int64_t value, char* result, uint8_t base );
bool_t uint64ToString( uint64_t value, char* result, uint8_t base );
bool_t uint64ToString2Digits( uint64_t value, char* result, uint8_t base );   // 1 --> "01", 25 --> "25" (completa con un cero a izquierda)

char* floatToString( float value, char* result, int32_t precision );
char* uintToAsciiHex( uint64_t value, char* result, uint8_t bitSize ); // 0x3F1 1 --> "03F1" (completa con ceros a izquierda para formar bien los bytes)

uint8_t* int32ToByteArray( int32_t value, uint8_t* byteArray );
uint8_t* floatToByteArray( float value, uint8_t* byteArray );

int32_t byteArrayToInt32( uint8_t* byteArray );
float byteArrayToFloat( uint8_t* byteArray );

uint8_t* variableToByteArray( void* var, uint32_t sizeOfVar, uint8_t* byteArray );
void* byteArrayToVariable( void* var, uint32_t sizeOfVar, uint8_t* byteArray );

char* hourMinSecToStringHHMMSS( uint8_t hour,  // (3,58,2) --> "03:58:02"
                                uint8_t min, 
                                uint8_t sec,
                                char* result );

char* dayHourToStringDH( uint32_t day, uint8_t hour, char* result );

// Funcion no reentrante. Cuidado con el RTOS!!!
char* intToStringGlobal( int64_t value );

// Funcion no reentrante. Cuidado con el RTOS!!!
char* floatToStringGlobal( double value, uint32_t decDigits );
   
// Funcion no reentrante. Cuidado con el RTOS!!!
char* uintToAsciiHexGlobal( uint64_t value, uint8_t bitSize );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_CONVERT_H_ */
