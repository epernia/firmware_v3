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
 *
 */

/* Date: 2017-30-10 */

// More information at:
// https://groups.google.com/forum/#!msg/embebidos32/tPntHHUSnyE/S3CDyCwXsaMJ

/*==================[inclusions]=============================================*/

#include "sapi_cyclesCounter.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/** Registros correspondientes al nucleo. Probados con Cortex-M3 y Cortex-M4. */

//Registro para configurar el contador de ciclos de clock.
//volatile uint32_t* DWT_CTRL   = (uint32_t*)0xE0001000;

//Registro donde se cuentan los ciclos de clock.
//volatile uint32_t* DWT_CYCCNT = (uint32_t*)0xE0001004;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static uint32_t ClockSpeed = EDU_CIAA_NXP_CLOCK_SPEED;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/**
 * Funcion para configurar los registros para contar ciclos de clock.
 * @return TRUE si esta OK, FALSE en caso de error.
 */
bool_t cyclesCounterInit( uint32_t clockSpeed )
{
   //Asigna  a la variable local ClockSpeed el valor recibido como argumento.
   ClockSpeed = clockSpeed;
   //Iniciar el contador de ciclos de clock.
   DWT_CTRL  |= 1; // *DWT_CTRL  |= 1;
   return TRUE;
}

/**
 * Funcion para leer el registro con la cuenta de ciclos de clock.
 * @return el valor del contador de ciclos de clock.
 */
/*
uint32_t cyclesCounterRead( void )
{
   return *DWT_CYCCNT;
}
*/

/**
 * Resetea el contador de ciclos de clock.
 * Esta funcion debe ser llamada antes de querer
 * contar el tiempo a medir. Luego de ejecutar la accion,
 * se debe llamar a la funcion cyclesCounterRead(). Asi se
 * obtendra la cantidad de ciclos que pasaron.
 */
/*
void cyclesCounterReset( void )
{
   //resetea el contador de ciclos de clock
   *DWT_CYCCNT = 0;
}
*/



/**
 * Funcion que convierte el valor en ciclos a nano segundos.
 * Para que esta cuenta se realice correctamente, se tuvo que haber
 * llamado previamente a la funcion cyclesCounterInit (CLOCK_SPEED);
 * @param cycles la cantidad de ciclos.
 * @return el valor convertido a nano segundos.
 */
float cyclesCounterToNs( uint32_t cycles )
{
   float valueInNanoSeconds = 0.0;
   valueInNanoSeconds = (float)cycles/((float)ClockSpeed/1000000000.0);
   return valueInNanoSeconds;
}


/**
 * Funcion que convierte el valor en ciclos a micro segundos.
 * Para que esta cuenta se realice correctamente, se tuvo que haber
 * llamado previamente a la funcion cyclesCounterInit (CLOCK_SPEED);
 * @param cycles la cantidad de ciclos.
 * @return el valor convertido a micro segundos.
 */
float cyclesCounterToUs( uint32_t cycles )
{
   float valueInMicroSeconds = 0.0;
   valueInMicroSeconds = (float)cycles/((float)ClockSpeed/1000000.0);
   return valueInMicroSeconds;
}

/**
 * Funcion que convierte el valor en ciclos a mili segundos.
 * Para que esta cuenta se realice correctamente, se tuvo que haber
 * llamado previamente a la funcion cyclesCounterInit (CLOCK_SPEED);
 * @param cycles la cantidad de ciclos.
 * @return el valor convertido a mili segundos.
 */
float cyclesCounterToMs( uint32_t cycles )
{
   float valueInMilliSeconds = 0.0;
   valueInMilliSeconds = (float)cycles/((float)ClockSpeed/1000.0);
   return valueInMilliSeconds;
}

/*==================[end of file]============================================*/
