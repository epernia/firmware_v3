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

#ifndef _SAPI_7_SEGMENT_DISPLAY_H_
#define _SAPI_7_SEGMENT_DISPLAY_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[macros]=================================================*/

#define DISPLAY_7_SEGMENT_OFF 25

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

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
     -----    O h = dp (decimal point).

*/
void display7SegmentTestPins( gpioMap_t* display7SegmentPins, gpioMap_t pin );

/* Configure 7-segment display GPIOs as Outputs */
void display7SegmentPinConfig( gpioMap_t* display7SegmentPins );

/* Write a symbol on 7-segment display */
void display7SegmentWrite( gpioMap_t* display7SegmentPins, uint8_t symbolIndex );


/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_7_SEGMENT_DISPLAY_H_ */
