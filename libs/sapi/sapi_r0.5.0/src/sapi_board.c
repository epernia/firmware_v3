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

/*==================[inclusions]=============================================*/

#include "sapi_board.h"

#include "sapi_tick.h"
#include "sapi_gpio.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Set up and initialize board hardware */
void boardConfig(void) {

   /* Read clock settings and update SystemCoreClock variable */
   SystemCoreClockUpdate();

   Board_Init(); // From Board module (modules/lpc4337_m4/board)

   /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
   tickConfig( 1, 0 );

   /* Inicializar GPIOs */
   gpioConfig( 0, GPIO_ENABLE );

   /* Configuración de pines de entrada para Teclas de la EDU-CIAA-NXP */
   gpioConfig( TEC1, GPIO_INPUT );
   gpioConfig( TEC2, GPIO_INPUT );
   gpioConfig( TEC3, GPIO_INPUT );
   gpioConfig( TEC4, GPIO_INPUT );

   /* Configuración de pines de salida para Leds de la EDU-CIAA-NXP */
   gpioConfig( LEDR, GPIO_OUTPUT );
   gpioConfig( LEDG, GPIO_OUTPUT );
   gpioConfig( LEDB, GPIO_OUTPUT );
   gpioConfig( LED1, GPIO_OUTPUT );
   gpioConfig( LED2, GPIO_OUTPUT );
   gpioConfig( LED3, GPIO_OUTPUT );


   /* Configuración de pines de entrada de la CIAA-NXP */
   gpioConfig( DI0, GPIO_INPUT );
   gpioConfig( DI1, GPIO_INPUT );
   gpioConfig( DI2, GPIO_INPUT );
   gpioConfig( DI3, GPIO_INPUT );
   gpioConfig( DI4, GPIO_INPUT );
   gpioConfig( DI5, GPIO_INPUT );
   gpioConfig( DI6, GPIO_INPUT );
   gpioConfig( DI7, GPIO_INPUT );

   /* Configuración de pines de salida de la CIAA-NXP */
   gpioConfig( DO0, GPIO_OUTPUT );
   gpioConfig( DO1, GPIO_OUTPUT );
   gpioConfig( DO2, GPIO_OUTPUT );
   gpioConfig( DO3, GPIO_OUTPUT );
   gpioConfig( DO4, GPIO_OUTPUT );
   gpioConfig( DO5, GPIO_OUTPUT );
   gpioConfig( DO6, GPIO_OUTPUT );
   gpioConfig( DO7, GPIO_OUTPUT );

}

/*==================[end of file]============================================*/
