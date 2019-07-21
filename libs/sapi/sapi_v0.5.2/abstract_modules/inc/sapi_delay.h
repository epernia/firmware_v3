/* Copyright 2015, Eric Pernia.
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
 
/* Date: 2015-09-23 */

#ifndef _SAPI_DELAY_H_
#define _SAPI_DELAY_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// Fclk = 204 MHz ==> 4.9019607843137254901960784313725 ns
#define INACCURATE_TO_MS       20400   // Number of cycles for 1 ms
#define INACCURATE_TO_US_x10   204     // Number of cycles for 10 ns
#define INACCURATE_MIN_NS      4.901960849761962890625f

#define delayConfig delayInit

/*==================[typedef]================================================*/

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/*==================[external functions declaration]=========================*/

/* ---- Inaccurate Delay ---- */
void delayInaccurateMs( tick_t delay_ms );
void delayInaccurateUs( tick_t delay_us );
void delayInaccurateNs( tick_t delay_ns ); // Resolution ~5 ns

/* ---- Blocking Delay ---- */
void delay( tick_t duration_ms );

/* ---- Non Blocking Delay ---- */
void delayInit( delay_t * delay, tick_t duration );
bool_t delayRead( delay_t * delay );
void delayWrite( delay_t * delay, tick_t duration );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_DELAY_H_ */
