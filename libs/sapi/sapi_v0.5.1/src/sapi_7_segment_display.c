/* Copyright 2016, Eric Pernia.
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

/*
 * Date: 2016-07-28
 */

/*==================[inclusions]=============================================*/

#include "sapi_7_segment_display.h"   /* <= own header */

#include "sapi_delay.h"               /* <= delay header */
#include "sapi_gpio.h"                /* <= GPIO header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/


// Symbols formed by segmens
/*
------------+------+---------
  Segmentos | HEX  | Simbolo
------------+------+---------
   hgfedcba |      |
 0b00111111 | 0x0F |   0
 0b00000110 | 0x00 |   1
 0b01011011 | 0x00 |   2
 0b01001111 | 0x00 |   3
 0b01100110 | 0x00 |   4
 0b01101101 | 0x00 |   5
 0b01111101 | 0x00 |   6
 0b00000111 | 0x00 |   7
 0b01111111 | 0x00 |   8
 0b01101111 | 0x00 |   9

 0b01011111 | 0x00 |   a 10
 0b01111100 | 0x00 |   b 11
 0b01011000 | 0x00 |   c 12
 0b01011110 | 0x00 |   d 13
 0b01111011 | 0x00 |   e 14
 0b01110001 | 0x00 |   F 15

 0b01110111 | 0x00 |   A 16
 0b00111001 | 0x00 |   C 17
 0b01111001 | 0x00 |   E 18
 0b01110110 | 0x00 |   H
 0b00011110 | 0x00 |   J
 0b00111000 | 0x00 |   L
 0b01110011 | 0x00 |   P
 0b00111110 | 0x00 |   U

 0b10000000 | 0x00 |   .

             a
           -----
       f /     / b
        /  g  /
        -----
    e /     / c
     /  d  /
     -----    O h = dp (decimal point).

*/
const uint8_t display7SegmentOutputs[26] = {
   0b00111111, // 0
   0b00000110, // 1
   0b01011011, // 2
   0b01001111, // 3
   0b01100110, // 4
   0b01101101, // 5
   0b01111101, // 6
   0b00000111, // 7
   0b01111111, // 8
   0b01101111, // 9

   0b01011111, // a
   0b01111100, // b
   0b01011000, // c
   0b01011110, // d
   0b01111011, // e
   0b01110001, // f

   0b01110111, // A
   0b00111001, // C
   0b01111001, // E
   0b01110110, // H
   0b00011110, // J
   0b00111000, // L
   0b01110011, // P
   0b00111110, // U

   0b10000000, // .

   0b00000000  // display off
};


/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Test 7-segment display connected pins */
/*
----------------+------------+-----------+------------
 Segment ON     | BIN Value  | HEX Value | Output pin
----------------+------------+-----------+------------
 Segment 'a' ON | 0b00000001 |   0x20    | ..... (to be complete)
 Segment 'b' ON | 0b00000010 |   0x80    | .....
 Segment 'c' ON | 0b00000100 |   0x40    | .....
 Segment 'd' ON | 0b00001000 |   0x02    | .....
 Segment 'e' ON | 0b00010000 |   0x04    | .....
 Segment 'f' ON | 0b00100000 |   0x10    | .....
 Segment 'g' ON | 0b01000000 |   0x08    | .....
 Segment 'h' ON | 0b10000000 |   0x80    | .....
----------------+------------+-----------+------------

           a
         -----
	  f /     / b
	   /  g  /
	   -----
       e /     / c
	/  d  /
	-----    O h = dp (decimal pint).

*/
void display7SegmentTestPins( gpioMap_t* display7SegmentPins, gpioMap_t pin )
{

   uint8_t i = 0;

   for(i=0; i<=7; i++) {
      gpioWrite( display7SegmentPins[i], ON  );
      if( i == 0 )
         gpioWrite( pin, ON );
      delay(1000);
      gpioWrite( display7SegmentPins[i], OFF );
      if( i == 0 )
         gpioWrite( pin, OFF );
   }

}


/* Configure 7-segment display GPIOs as Outputs */
void display7SegmentPinInit( gpioMap_t* display7SegmentPins )
{
   uint8_t i = 0;
   for( i=0; i<=7; i++ )
      gpioInit( display7SegmentPins[i], GPIO_OUTPUT );
}

static void digitsWrite( gpioMap_t d, DisplayCommonType_t c, int val)
{
   if (c == DISP7_ANODE) // WARN: VERIFICAR
      val = !val;
   gpioWrite( d, val );
}

/*
typedef enum {
   DISP7_ANODE,
   DISP7_CATODE
} DisplayCommonType_t;

typedef struct {
   gpioMap_t *digits;
   gpioMap_t *segments;
   uint8_t nDigits;
   uint8_t currentDigit;
   DisplayCommonType_t comm;
   uint8_t *buffer;
} Display7Segment_t;
 */

void display7SegmentInit(Display7Segment_t *disp, gpioMap_t* segments,
                         gpioMap_t *digits, uint8_t nDigits,
                         DisplayCommonType_t common, uint8_t *buf)
{
   disp->digits = digits;
   disp->segments = segments;
   disp->nDigits = nDigits;
   disp->currentDigit = 0;
   disp->comm = common;
   disp->buffer = buf;
   for (int i=0; i<nDigits; i++) {
      buf[i] = 0;
      gpioInit( digits[i], GPIO_OUTPUT);
      digitsWrite( digits[i], disp->comm, 0);
   }
   for (int i=0; i<8; i++) {
      gpioInit( segments[i], GPIO_OUTPUT);
      digitsWrite( segments[i], disp->comm, 0);
   }
}

void display7SegmentWriteIndex( Display7Segment_t* disp, uint8_t digit, uint8_t idx )
{
   disp->buffer[digit] = idx;
}

void display7SegmentWriteInt( Display7Segment_t* disp, uint32_t val )
{
   for (int i=0; i<disp->nDigits; i++) {
      uint8_t digit = val % 10;
      display7SegmentWriteIndex(disp, i, digit );
      val /= 10;
   }
}

void display7SegmentWriteHex( Display7Segment_t* disp, uint32_t val )
{
   for (int i=0; i<disp->nDigits; i++) {
      uint8_t nibble = (val >> (4 * i)) & 0xF;
      display7SegmentWriteIndex(disp, i, nibble );
   }
}

/* Write a symbol on 7-segment display */
void display7SegmentWrite( gpioMap_t* display7SegmentPins, DisplayCommonType_t c, uint8_t symbolIndex )
{

   uint8_t i = 0;

   for( i=0; i<=7; i++ ) {
      int val = display7SegmentOutputs[symbolIndex] & (1<<i);
      if (c == DISP7_ANODE) // WARN: VERIFICAR
         val = !val;
      gpioWrite( display7SegmentPins[i], val );
   }
}

void display7SegmentClear( Display7Segment_t* disp )
{
   for (int i=0; i<disp->nDigits; i++) {
      disp->buffer[i] = 0;
   }
}

void display7SegmentRefresh( Display7Segment_t *disp )
{
   digitsWrite( disp->digits[disp->currentDigit], disp->comm, 0 );
   disp->currentDigit++;
   if (disp->currentDigit >= disp->nDigits)
      disp->currentDigit = 0;
   digitsWrite( disp->digits[disp->currentDigit], disp->comm, 0 );
}


/*==================[end of file]============================================*/
