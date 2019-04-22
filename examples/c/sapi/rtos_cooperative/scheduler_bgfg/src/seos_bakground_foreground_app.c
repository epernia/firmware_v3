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

/* Date: 2016-08-16
 *
 * SEOS del ingés Simple Embedded Operating System.
 * Ejemplo de Planificador de tareas cooperativo disparado por tiempo
 * (Time-Triggered). Variante background-foreground. El despachador de tareas
 * (dispatcher) en el while(1) (background) y planificador de tareas
 * (scheduler) se ejecuta en una Interrupción periódica de tiempo (foreground).
 * Solamente la primer tarea es Real-Time. La suma de la duración de todas las
 * tareas debe ser menor a 1 Tick (1ms en el ejemplo), si alguna se excede un
 * poco de tiempo el sistema puede recuperarse, es decir, no pierde
 * temporización pero sin embargo se atrasa un poco en la ejecución.
 * Las tareas deben ser NO bloqueantes.
 */

/*==================[inclusions]=============================================*/

//#include "seos_bakground_foreground_app.h"   // <= own header (optional)
#include "sapi.h"        // <= sAPI header

#include "tasks.h"                     // <= tasks header
#include "seos_bakground_foreground.h" // <= scheduler and dispatcher header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializacion de las tareas. */
   tasksInit();

   /* Inicializacion de la interrpción periódica
      del sistema operativo cada 1ms. */
   seosInterruptInit(1);

   /* ----- REPETIR POR SIEMPRE (SUPER LOOP) ----- */

   while(1) {

      /* Se despachan (ejecutan) las tareas marcadas para su ejecucion. */
      seosDispatchTask();

      /* Se pone el sistema en bajo consumo hasta que ocurra la proxima
         interrupcion, en este caso la de Tick. */
      sleepUntilNextInterrupt();

      /* Al ocurrir la interrupcion de Tick se ejecutara el planificador
         que revisa cuales son las tareas a marcar para su ejecucion. */

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
