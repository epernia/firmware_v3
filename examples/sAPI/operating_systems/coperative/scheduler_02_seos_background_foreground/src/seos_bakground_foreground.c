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

/* Date: 2016-08-16 */

/*==================[inclusions]=============================================*/

#include "seos_bakground_foreground.h"        /* <= own header */

#include "tasks.h"        /* <= tasks header */

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

static tick_t task1RunFlag = 0;
static tick_t task2RunFlag = 0;
static tick_t task3RunFlag = 0;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* ----------- PLANIFICADOR y DESPACHADOR ----------- */

/* FUNCION que contiene el planificador de tareas. */
void seosScheduleTasks(void *ptr){
   
   /* Tarea planificada cada TASK1_PERIODICITY ms */
   if( task1Counter++ == TASK1_PERIODICITY ){
      /* Planifica la tarea (activa el flag para
         que la ejecute el despachador) */
      task1RunFlag = 1;

      /* Resetea el contador de la tarea */
      task1Counter = 0;
   }

   /* Tarea planificada cada TASK2_PERIODICITY ms */
   if( task2Counter++ == TASK2_PERIODICITY ){
      /* Planifica la tarea (activa el flag para
         que la ejecute el despachador) */
      task2RunFlag = 1;

      /* Resetea el contador de la tarea */
      task2Counter = 0;
   }

   /* Tarea planificada cada TASK3_PERIODICITY ms */
   if( task3Counter++ == TASK3_PERIODICITY ){
      /* Planifica la tarea (activa el flag para
         que la ejecute el despachador) */
      task3RunFlag = 1;

      /* Resetea el contador de la tarea */
      task3Counter = 0;
   }
}

/* FUNCION que contiene el despachador de tareas. */
void seosDispatchTask(void){

   /* Tarea planificada cada TASK1_PERIODICITY ms */
   if( task1RunFlag ){
      /* Si el flag esta en 1 despacha de la tarea 
         (la ejecuta) */
      task1();

      /* Resetea el flag de ejecucion de la tarea */
      task1RunFlag = 0;
   }

   /* Tarea planificada cada TASK2_PERIODICITY ms */
   if( task2RunFlag ){
      /* Si el flag esta en 1 despacha de la tarea 
         (la ejecuta) */
      task2();

      /* Resetea el flag de ejecucion de la tarea */
      task2RunFlag = 0;
   }

   /* Tarea planificada cada TASK3_PERIODICITY ms */
   if( task3RunFlag ){
      /* Si el flag esta en 1 despacha de la tarea 
         (la ejecuta) */
      task3();

      /* Resetea el flag de ejecucion de la tarea */
      task3RunFlag = 0;
   }
}

/* FUNCION que inicializa la interrupcion que ejecuta
   el planificador de tareas. */
void seosInterruptInit( tick_t tickRateMs ){
   /* Inicializar el conteo de Ticks con resolucion de tickRateMs ms (se 
      ejecuta periodicamente una interrupcion cada tickRateMs ms que 
      incrementa un contador de Ticks obteniendose una base de tiempos).
      Se agrega ademas como "tick hook" a la funcion encargada de planificar 
      las tareas seosScheduleTasks(). 
      El tick hook es simplemente una funcion que se ejecutara periodicamente 
      con cada interrupcion de Tick, este nombre se refiere a una funcion 
      "enganchada" a una interrupcion */
   tickInit( 1 );
   tickCallbackSet( seosScheduleTasks, NULL );
}

/*==================[end of file]============================================*/
