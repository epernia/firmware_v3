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
 */

// File creation date: 2016-03-01

#ifndef _SAPI_PARSER_H_
#define _SAPI_PARSER_H_

/*==================[inclusions]=============================================*/

#include <sapi_datatypes.h>
#include <sapi_delay.h>

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef enum{
   PARSER_PATTERN_MATCH,
   PARSER_TIMEOUT,
   PARSER_RECEIVING,
} parserStatus_t;

typedef struct{
   parserStatus_t state;
   char const*    stringPattern;
   uint16_t       stringPatternLen;
   uint16_t       stringIndex;
   tick_t         timeout;
   delay_t        delay;
} parser_t;

/*==================[external functions declaration]=========================*/

// Initialize parser
void parserInit( parser_t* instance,
                 char const* stringPattern, uint16_t stringPatternLen, 
                 tick_t timeout );

// Check for Receive a given pattern
parserStatus_t parserUpdate( parser_t* instance, char const receivedChar );

/*==================[end of file]============================================*/
#endif
