/* Copyright 2017, Agustin Bassi.
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

/* Date: 2017-30-10 */

// More information at:
// https://groups.google.com/forum/#!msg/embebidos32/tPntHHUSnyE/S3CDyCwXsaMJ

#ifndef _SAPI_CYCLES_COUNTER_H_
#define _SAPI_CYCLES_COUNTER_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// TODO: Check CPU core speed using SystemCoreClock
#define EDU_CIAA_NXP_CLOCK_SPEED   204000000

#define cyclesCounterConfig        cyclesCounterInit

// Registro DWT_CTRL para configurar el contador de ciclos de clock.
#define DWT_CTRL   (HW_REG_32_RW(0xE0001000))

// Registro DWT_CYCCNT donde se cuentan los ciclos de clock.
#define DWT_CYCCNT   (HW_REG_32_RW(0xE0001004))

// Read Cycles Counter
#define cyclesCounterRead()        DWT_CYCCNT

// Reset Cycles Counter
#define cyclesCounterReset()       (DWT_CYCCNT = 0)

/*==================[external functions definition]==========================*/

bool_t cyclesCounterInit( uint32_t clockSpeed );

float cyclesCounterToNs( uint32_t cycles );

float cyclesCounterToUs( uint32_t cycles );

float cyclesCounterToMs( uint32_t cycles );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_CYCLES_COUNTER_H_ */
