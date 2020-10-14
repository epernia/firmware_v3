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

// File creation date: 2017-04-17

/*==================[inclusions]=============================================*/

#include "sapi_convert.h"     // <= own header
#include <string.h>

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

char globalStrConvertBuff[200];

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

// C++ version 0.4 char* style "itoa"
// Modified by Eric Pernia.
bool_t int64ToString( int64_t value, char* result, uint8_t base )
{
   // check that the base if valid
   if( base < 2 || base > 36 ) {
      *result = '\0';
      return FALSE;
   }

   char* ptr = result, *ptr1 = result, tmp_char;
   int64_t tmp_value;

   do {
      tmp_value = value;
      value /= (int64_t)base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * (int64_t)base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return TRUE;
}

// C++ version 0.4 char* style "itoa":
// Written by Luk�s Chmela
// Released under GPLv3.
// Modified by Eric Pernia.
bool_t uint64ToString( uint64_t value, char* result, uint8_t base )
{
   // check that the base if valid
   if( base < 2 || base > 36 ) {
      *result = '\0';
      return FALSE;
   }

   char* ptr = result, *ptr1 = result, tmp_char;
   uint64_t tmp_value;

   do {
      tmp_value = value;
      value /= (uint64_t)base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * (uint64_t)base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return TRUE;
}

bool_t uint64ToString2Digits( uint64_t value, char* result, uint8_t base )
{
   if( value < 10 ) {
      result[0] = '0';
      return uint64ToString( value, result+1, base );
   } else {
      return uint64ToString( value, result, base );
   }
}

#define MAX_PRECISION   (10)
static const double rounders[MAX_PRECISION + 1] = {
   0.5,            // 0
   0.05,           // 1
   0.005,          // 2
   0.0005,         // 3
   0.00005,        // 4
   0.000005,       // 5
   0.0000005,      // 6
   0.00000005,     // 7
   0.000000005,    // 8
   0.0000000005,   // 9
   0.00000000005   // 10
};

// Ftoa implementation from:
// https://github.com/antongus/stm32tpl/blob/master/ftoa.c
char* floatToString( float value, char* result, int32_t precision )
{
   char * ptr = result;
   char * p = ptr;
   char * p1;
   char c;
   long intPart;

   // check precision bounds
   if (precision > MAX_PRECISION)
      precision = MAX_PRECISION;

   // sign stuff
   if (value < 0) {
      value = -value;
      *ptr++ = '-';
   }

   if (precision < 0) { // negative precision == automatic precision guess
      if (value < 1.0) precision = 6;
      else if (value < 10.0) precision = 5;
      else if (value < 100.0) precision = 4;
      else if (value < 1000.0) precision = 3;
      else if (value < 10000.0) precision = 2;
      else if (value < 100000.0) precision = 1;
      else precision = 0;
   }

   // round value according the precision
   if (precision)
      value += rounders[precision];

   // integer part...
   intPart = value;
   value -= intPart;

   if (!intPart)
      *ptr++ = '0';
   else {
      // save start pointer
      p = ptr;

      // convert (reverse order)
      while (intPart) {
         *p++ = '0' + intPart % 10;
         intPart /= 10;
      }

      // save end pos
      p1 = p;

      // reverse result
      while (p > ptr) {
         c = *--p;
         *p = *ptr;
         *ptr++ = c;
      }

      // restore end pos
      ptr = p1;
   }

   // decimal part
   if (precision) {
      // place decimal point
      *ptr++ = '.';

      // convert
      while (precision--) {
         value *= 10.0;
         c = value;
         *ptr++ = '0' + c;
         value -= c;
      }
   }

   // terminating zero
   *ptr = 0;

   return result;
}


/*

// TEST

int main()
{
   float value = 123.456; // param

   static char result[100];
   result[0] = 0;
   ftoa( value, result, 10 );
   //return result;

   printf( "%s\r\n", result);

   value = 0.000054321;
   result[0] = 0;
   ftoa( value, result, 10 );
   printf( "%s\r\n", result);

   value = -0.000054321;
   result[0] = 0;
   ftoa( value, result, 10 );
   printf( "%s\r\n", result);

   value = 1234567.54321;
   result[0] = 0;
   ftoa( value, result, 10 );
   printf( "%s\r\n", result);

   value = -1234567.54321;
   result[0] = 0;
   ftoa( value, result, 10 );
   printf( "%s\r\n", result);
}
*/

// RESULT:
// 123.4560012817
// 0.0000543210
// -0.0000543210
// 1234567.5000000000
// -1234567.5000000000

uint8_t* variableToByteArray( void* var, uint32_t sizeOfVar, uint8_t* byteArray )
{
   memcpy( byteArray, (uint8_t*)var, sizeOfVar );
   return byteArray;
}

void* byteArrayToVariable( void* var, uint32_t sizeOfVar, uint8_t* byteArray )
{
   memcpy( (uint8_t*)var, byteArray, sizeOfVar );
   return var;
}

uint8_t* floatToByteArray( float value, uint8_t* byteArray )
{
   memcpy( byteArray, (uint8_t*) (&value), 4 );
   return byteArray;
}

uint8_t* int32ToByteArray( int32_t value, uint8_t* byteArray )
{
   memcpy( byteArray, (uint8_t*) (&value), 4 );
   return byteArray;
}

float byteArrayToFloat( uint8_t* byteArray )
{
   float value = 0;   
   memcpy( (uint8_t*) (&value), byteArray, 4 );
   return value;
}

int32_t byteArrayToInt32( uint8_t* byteArray )
{
   int32_t value = 0;   
   memcpy( (uint8_t*) (&value), byteArray, 4 );
   return value;
}

// Funcion no reentrante. Cuidado con el RTOS!!!
char* intToStringGlobal( int64_t value )
{
   if( value == 0 ) {
      globalStrConvertBuff[0] = '0';
      globalStrConvertBuff[1] = 0;
   } else {
      globalStrConvertBuff[0] = 0;
      int64ToString( value, globalStrConvertBuff, 10 );
   }
   return globalStrConvertBuff;
}

// Funcion no reentrante. Cuidado con el RTOS!!!
char* floatToStringGlobal( double value, uint32_t decDigits )
{
   globalStrConvertBuff[0] = 0;
   floatToString( value, globalStrConvertBuff, decDigits );
   return globalStrConvertBuff;
}

// Funcion no reentrante. Cuidado con el RTOS!!!
char* uintToAsciiHexGlobal( uint64_t value, uint8_t bitSize )
{
   return uintToAsciiHex( value, globalStrConvertBuff, bitSize );
}
//-----------------------------------------------------------------------------

char* hourMinSecToStringHHMMSS( uint8_t hour, 
                                uint8_t min, 
                                uint8_t sec,
                                char* result )
{
   char uu[3] = { 0, 0, 0 };

   result[0] = 0;   
   uu[0] = 0;
   uint64ToString2Digits( hour, uu, 10 );   
   strcat( result, uu );
   strcat( result, ":" );
   uu[0] = 0;
   uint64ToString2Digits( min, uu, 10 );   
   strcat( result, uu );   
   strcat( result, ":" );
   uu[0] = 0;
   uint64ToString2Digits( sec, uu, 10 );   
   strcat( result, uu );
   return result;
}
//-----------------------------------------------------------------------------

char* dayHourToStringDH( uint32_t day, uint8_t hour, char* result )
{
   char buff[10];   
   result[0] = 0;   
   buff[0] = 0;   
   uint64ToString( day, buff, 10 );   
   strcat( result, buff );
   strcat( result, "d " );
   buff[0] = 0;   
   uint64ToString( hour, buff, 10 );   
   strcat( result, buff );   
   strcat( result, "h" );
   return result;
}
//-----------------------------------------------------------------------------

char* uintToAsciiHex( uint64_t value, char* result, uint8_t bitSize )
{
   uint8_t i = 0;
   uint8_t vectorNumHex[] = "0123456789ABCDEF";
   result[ bitSize/4 ] = 0;

   for( i=0; i<bitSize/4; i++ ) {
      result[(bitSize/4)-i-1] = 
         vectorNumHex[ (uint8_t)(( value & (((uint64_t)0x0F)<<(4*i)) ) >> (4*i)) ];
   }

   return result;
}
//-----------------------------------------------------------------------------

// Aswer true if char c is an ASCII digit (ASCII between '0' and '9')
bool charIsDigit( char c )
{
    return ( (c >= '0') && (c <= '9') );
}

/*==================[end of file]============================================*/
