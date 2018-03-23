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

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef enum{
   BIN_FORMAT = 2,
   DEC_FORMAT = 10,
   HEX_FORMAT = 16
} numberFormat_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// C++ version 0.4 char* style "itoa":
bool_t int64ToString( int64_t value, char* result, uint8_t base );
bool_t uint64ToString( uint64_t value, char* result, uint8_t base );

char* uintToAsciiHex( uint64_t value, uint8_t bitSize );

char* intToString( int64_t value );

/*==================[examples]===============================================*/

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_CONVERT_H_ */
