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

/*==================[inclusions]=============================================*/

#include "sapi.h"    // <= sAPI header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){
bool_t cyclesCondition = FALSE;
uint32_t cyclesElapsed = 0;
uint32_t msElapsed = 0, usElapsed = 0;

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

   // Configura el contador de ciclos con el clock de la EDU-CIAA NXP
   cyclesCounterConfig(EDU_CIAA_NXP_CLOCK_SPEED);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
	   // Resetea el contador de ciclos
	   cyclesCounterReset();
	   //Ejecuta dos condiciones para mostrar el valor de los ciclos
	   if (cyclesCondition){
		   // Espera un tiempo aleatorio
		   delay(3);
		   // Guarda en una variable los ciclos leidos
		   cyclesElapsed = cyclesCounterRead();
		   // Convierte el valor de ciclos en micro segundos
		   usElapsed = cyclesCounterToUs(cyclesElapsed);
		   // Imprime por pantalla el valor de los ciclos y los micro segundos transcurridos.
		   stdioPrintf(UART_USB, "Los ciclos en esperar 3 ms son: %d. En micro segundos (aprox) %d\n\r", cyclesElapsed, usElapsed);
	   } else {
		   // Espera un tiempo aleatorio
		   delay(141);
		   // Guarda en una variable los ciclos leidos
		   cyclesElapsed = cyclesCounterRead();
		   // Convierte el valor de ciclos en mili segundos
		   msElapsed = cyclesCounterToMs(cyclesElapsed);
		   // Imprime por pantalla el valor de los ciclos y los mili segundos transcurridos.
		   stdioPrintf(UART_USB, "Los ciclos en esperar 141 ms son: %d. En mili segundos (aprox) %d\n\n\r", cyclesElapsed, msElapsed);
	   }
	   cyclesCondition = !cyclesCondition;
	   gpioToggle(LEDB);
	   delay (1000);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
