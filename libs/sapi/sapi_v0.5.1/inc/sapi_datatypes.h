/* Copyright 2015-2016, Eric Pernia.
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

/* Date: 2015-09-23 */

#ifndef _SAPI_DATATYPES_H_
#define _SAPI_DATATYPES_H_

/*==================[inclusions]=============================================*/

#include "stdint.h"
#include "chip.h"
#include "board.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
/* Functional states */
#ifndef ON
   #define ON     1
#endif
#ifndef OFF
   #define OFF    0
#endif

/* Electrical states */
#ifndef HIGH
   #define HIGH   1
#endif
#ifndef LOW
   #define LOW    0
#endif

/* Logical states */

#ifndef FALSE
   #define FALSE  0
#endif
#ifndef TRUE
   #define TRUE   (!FALSE)
#endif

/*==================[typedef]================================================*/

/* Define Boolean Data Type */
typedef uint8_t bool_t;

/* Define real Data Types (floating point) */
typedef float  real32_t;
//typedef double real64_t; // In LPC4337 float = double (Floating Point single precision, 32 bits)

/* Define Tick Data Type */
typedef uint64_t tick_t;

/*
 * Function Pointer definition
 * --------------------------------------
 * param:  void * - For passing arguments
 * return: bool_t - For Error Reports
 */
typedef bool_t (*sAPI_FuncPtr_t)(void *);

/*
 * Function Pointer definition
 * --------------------------------------
 * param:  void
 * return: void
 */
typedef void (*callBackFuncPtr_t)(void *);

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*
 * Null Function Pointer definition
 * --------------------------------------
 * param:  void * - Not used
 * return: bool_t - Return always true
 */
bool_t sAPI_NullFuncPtr(void *);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_DATATYPES_H_ */
