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

/*
 * Date: 2017-11-01
 */

#ifndef _SAPI_RGB_H_
#define _SAPI_RGB_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define MAX_AMOUNT_OF_RGB_LEDS 9

#define rgbConfig rgbInit

/*==================[typedef]================================================*/

// Colores predefinidos
typedef enum Color {
	BLACK,
	WHITE,
	VIOLET,
	RED,
	CYAN,
	BLUE,
	YELLOW,
	GREEN
} Color_t;

typedef enum LedRgbMap {
	RGB_1,
	RGB_2,
	RGB_3,
	RGB_4,
	RGB_5,
	RGB_6,
	RGB_7,
	RGB_8,
	RGB_9,
} LedRgbMap_t;

/*==================[external functions declaration]=========================*/

/** Configura los pines que se van a manejar como salidas PWM por soft. */
bool_t rgbInit( LedRgbMap_t rgbLed, uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue );

/** Manda por los pines PWM uno de los posibles colores definidos en color_t*/
void rgbWriteColor( LedRgbMap_t rgbLed, Color_t color );

/** Manda una configuracion individual de cada canal. */
void rgbWriteRaw( LedRgbMap_t rgbLed, int8_t dutyRed, int8_t dutyGreen, int8_t dutyBlue );

/** Setea el brillo sin cambiar el color (multiplica por una constante). */
void rgbWriteBright( LedRgbMap_t rgbLed, uint32_t bright );

uint8_t rgbReadDutyRed( LedRgbMap_t rgbLed );

uint8_t rgbReadDutyGreen( LedRgbMap_t rgbLed );

uint8_t rgbReadDutyBlue( LedRgbMap_t rgbLed );

void rgbToggleLed( LedRgbMap_t rgbLed );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_RGB_H_ */
