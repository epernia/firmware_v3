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
#include "sapi_cyclesCounter.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Set up and initialize board hardware */
void boardInit(void)
{
   // Read clock settings and update SystemCoreClock variable
   SystemCoreClockUpdate();

   cyclesCounterInit( SystemCoreClock );

   // Inicializar el conteo de Ticks con resolucion de 1ms (si no se usa freeRTOS)
   #ifndef USE_FREERTOS
      tickInit( 1 );
   #endif

   // Configure GPIO pins for each board
   #if BOARD==ciaa_nxp

      // Inicializar GPIOs
      gpioInit( 0, GPIO_ENABLE );

      // Configuracion de pines de entrada de la CIAA-NXP
      gpioInit( DI0, GPIO_INPUT );
      gpioInit( DI1, GPIO_INPUT );
      gpioInit( DI2, GPIO_INPUT );
      gpioInit( DI3, GPIO_INPUT );
      gpioInit( DI4, GPIO_INPUT );
      gpioInit( DI5, GPIO_INPUT );
      gpioInit( DI6, GPIO_INPUT );
      gpioInit( DI7, GPIO_INPUT );

      // Configuracion de pines de salida de la CIAA-NXP
      gpioInit( DO0, GPIO_OUTPUT );
      gpioInit( DO1, GPIO_OUTPUT );
      gpioInit( DO2, GPIO_OUTPUT );
      gpioInit( DO3, GPIO_OUTPUT );
      gpioInit( DO4, GPIO_OUTPUT );
      gpioInit( DO5, GPIO_OUTPUT );
      gpioInit( DO6, GPIO_OUTPUT );
      gpioInit( DO7, GPIO_OUTPUT );

      //#error CIAA-NXP

   #elif BOARD==edu_ciaa_nxp

      // Inicializar GPIOs
      gpioInit( 0, GPIO_ENABLE );

      // Configuracion de pines de entrada para Teclas de la EDU-CIAA-NXP
      gpioInit( TEC1, GPIO_INPUT );
      gpioInit( TEC2, GPIO_INPUT );
      gpioInit( TEC3, GPIO_INPUT );
      gpioInit( TEC4, GPIO_INPUT );

      // Configuracion de pines de salida para Leds de la EDU-CIAA-NXP
      gpioInit( LEDR, GPIO_OUTPUT );
      gpioInit( LEDG, GPIO_OUTPUT );
      gpioInit( LEDB, GPIO_OUTPUT );
      gpioInit( LED1, GPIO_OUTPUT );
      gpioInit( LED2, GPIO_OUTPUT );
      gpioInit( LED3, GPIO_OUTPUT );

      //#error EDU-CIAA-NXP

   #elif BOARD==ciaa_z3r0
      #error CIAA-Z3R0

   #elif BOARD==pico_ciaa
      #error PicoCIAA

   #else
      #error BOARD compile variable must be defined

   #endif

}

/*==================[end of file]============================================*/
