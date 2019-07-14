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

#ifndef _SAPI_GPIO_H_
#define _SAPI_GPIO_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define gpioConfig  gpioInit
#define pinInit     gpioInit
#define pinRead     gpioRead
#define pinWrite    gpioWrite
#define pinValueGet gpioRead
#define pinValueSet gpioWrite

/*==================[typedef]================================================*/

/* Pin modes */
/*
 *  INPUT  =  0    (No PULLUP or PULLDOWN)
 *  OUTPUT =  1
 *  INPUT_PULLUP
 *  INPUT_PULLDOWN
 *  INPUT_REPEATER (PULLUP and PULLDOWN)
 *  INITIALIZE
 */
typedef enum {
   GPIO_INPUT, GPIO_OUTPUT,
   GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN,
   GPIO_INPUT_PULLUP_PULLDOWN,
   GPIO_ENABLE
} gpioInit_t;


/* ----- Begin Pin Init Structs NXP LPC4337 ----- */

typedef struct {
   int8_t port;
   int8_t pin;
} gpioInitLpc4337_t;

typedef struct {
   pinInitLpc4337_t pinName;
   int8_t func;
   gpioInitLpc4337_t gpio;
} pinInitGpioLpc4337_t;

/* ------ End Pin Init Structs NXP LPC4337 ------ */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

bool_t gpioInit( gpioMap_t pin, gpioInit_t config );
bool_t gpioRead( gpioMap_t pin );
bool_t gpioWrite( gpioMap_t pin, bool_t value );
bool_t gpioToggle( gpioMap_t pin );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_GPIO_H_ */
