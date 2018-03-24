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

/* Date: 2016-08-15
 *
 * SEOS del ingés Simple Embedded Operating System.
 * Ejemplo de Planificador de tareas cooperativo disparado por tiempo
 * (Time-Triggered), con el planificador (scheduler) y despachador de tareas
 * (dispatcher) todo junto que se ejecuta en una Interrupción periódica de
 * tiempo.
 * Solamente la primer tarea es Real-Time. La suma de la duración de todas las
 * tareas debe ser menor a 1 Tick (1ms en el ejemplo), si alguna se excede un
 * poco de tiempo el sistema va a fallar, ya que se perderá temporización.
 * Las tareas deben ser NO bloqueantes.
 */

/*==================[inclusions]=============================================*/

//#include "seos.h"    // <= own header (optional)
#include "sapi.h"      // <= sAPI header

/*==================[macros and definitions]=================================*/

/* ----------- PERIODICIDAD DE LAS TAREAS ----------- */
#define TASK1_PERIODICITY   40
#define TASK2_PERIODICITY  500
#define TASK3_PERIODICITY 1000

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static tick_t task1Counter = 0;
static tick_t task2Counter = 0;
static tick_t task3Counter = 0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/* ------- TAREAS, PLANIFICADOR y DESPACHADOR ------- */

/* FUNCION que realiza la tarea 1.
   Esta tarea lee una tecla cada 40ms y escribe su valor
   en LEDB */
static void task1(void){
   gpioWrite( LEDB, !gpioRead(TEC1) );
}

/* FUNCION que realiza la tarea 2.
   Esta tarea hace destellar el LED2 (blink) cada
   250ms. */
static void task2(void){
   gpioToggle( LED2 );
}

/* FUNCION que realiza la tarea 3.
   Esta tarea hace destellar el LED3 (blink) cada
   500ms. */
static void task3(void){
   gpioToggle( LED3 );
}

/* FUNCION que contiene el planificador y despachador de tareas. */
static void scheduleAndDispatchTasks(void){

   /* Tarea planificada cada TASK1_PERIODICITY ms */
   if( task1Counter++ == TASK1_PERIODICITY ){
      /* Despacho de la tarea (la ejecuta) */
      task1();

      /* Resetea el contador de la tarea */
      task1Counter = 0;
   }

   /* Tarea planificada cada TASK2_PERIODICITY ms */
   if( task2Counter++ == TASK2_PERIODICITY ){
      /* Despacho de la tarea (la ejecuta) */
      task2();

      /* Resetea el contador de la tarea */
      task2Counter = 0;
   }

   /* Tarea planificada cada TASK3_PERIODICITY ms */
   if( task3Counter++ == TASK3_PERIODICITY ){
      /* Despacho de la tarea (la ejecuta) */
      task3();

      /* Resetea el contador de la tarea */
      task3Counter = 0;
   }

}

/* ------------------- TICK HOOK -------------------- */

/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
static void myTickHook( void *ptr ){
   scheduleAndDispatchTasks();
}

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolucion de 1ms (se ejecuta
      periodicamente una interrupcon cada 1ms que incrementa un contador de
      Ticks obteniendose una base de tiempos). Se agrega ademas un "tick hook"
      nombrado myTickHook. El tick hook es simplemente una funcion que se
      ejecutara periodicamente con cada interrupcion de Tick, este nombre se
      refiere a una funcion "enganchada" a una interrupcion */
   tickInit( 1 );
   tickCallbackSet( myTickHook, NULL );

   /* ----- REPETIR POR SIEMPRE (SUPER LOOP) ----- */

   while(1) {

      /* No se hace nada en el Super loop, simplemente se pone el sistema en
         bajo consumo hasta que ocurra la proxima interrupcion, en este caso
         la de Tick. Comente esta duncion y note el aumento de temperatura
         del microcontrolador */
      sleepUntilNextInterrupt();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
