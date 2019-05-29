/* Copyright 2013, Michael J. Pont.
 * Copyright 2016, Eric Pernia.
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

/* Date: 2016-08-16 */

/*==================[inclusions]=============================================*/

#include "seos_pont_2014_isr.h"       // <= own header
#include "seos_pont_2014_scheduler.h" // <= dispatcher and task management header

#include "sapi.h"                     // <= sAPI header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/* The array of tasks (see Sch.C) */

extern sTask_t schedulerTasks[SCHEDULER_MAX_TASKS];

/* The error code variable */
extern uint8_t errorCode;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*------------------------------------------------------------------*
schedulerInit()
Scheduler initialization function. Prepares scheduler
data structures.
Must call this function before using the scheduler.
-*------------------------------------------------------------------*/
void schedulerInit( void ){

   uint32_t i;
   for( i = 0; i < SCHEDULER_MAX_TASKS; i++ ){
      schedulerDeleteTask(i);
   }
   /* Reset the global error variable
      - schedulerDelete_Task() will generate an error code,
        (because the task array is empty) */
   errorCode = 0;

   // Inicializar el conteo de Ticks con resolucion de 1ms, sin tickHook
   if( tickInit( 1 ) ){
//      serialDebugPrintLnString( "Tick configurado a 1ms." );
   }
}

/*------------------------------------------------------------------*
schedulerUpdate()
This is the scheduler ISR. It is called at a rate
determined by the timer settings in the 'init' function.
This version is triggered by Timer 2 interrupts:
timer is automatically reloaded.
-*------------------------------------------------------------------*/
void schedulerUpdate( void *ptr ){ // Planificador (me lo llama la interrupcion periodica de Systick)

   int32_t index;
   //serialDebugPrintLnString( "schedulerUpdate." );

   // NOTE: calculations are in *TICKS* (not milliseconds)
   for( index = 0; index < SCHEDULER_MAX_TASKS; index++ ){
      // Check if there is a task at this location
      if( schedulerTasks[index].pTask ){
         if( --schedulerTasks[index].delay <= 0 ){   /* if (schedulerTasks[index].delay == 0) */
            // The task is due to run
            schedulerTasks[index].runMe += 1; // Inc. the 'runMe' flag
            if( schedulerTasks[index].period ){
               // Schedule regular tasks to run again
               schedulerTasks[index].delay = schedulerTasks[index].period;
            }
         }/*
         else{
            // Not yet ready to run: just decrement the delay
            schedulerTasks[Index].delay -= 1;
         }*/
      }
   }
}

/*------------------------------------------------------------------*
schedulerStart()
Starts the scheduler, by enabling timer interrupts.
NOTE: Usually called after all regular tasks are added,
to keep the tasks synchronized.
NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
-*------------------------------------------------------------------*/
void schedulerStart( tick_t tickRateMs ){

   /* Inicializar el conteo de Ticks con resolucion de tickRateMs ms (se
      ejecuta periodicamente una interrupcon cada tickRateMs ms que
      incrementa un contador de Ticks obteniendose una base de tiempos).
      Se agrega ademas como "tick hook" a la funcion encargada de planificar
      las tareas seosScheduleTasks().
      El tick hook es simplemente una funcion que se ejecutara periodicamente
      con cada interrupcion de Tick, este nombre se refiere a una funcion
      "enganchada" a una interrupcion */

   // Inicializar el conteo de Ticks con resolucion de 1ms, con tickHook
   if( tickInit( tickRateMs ) ){
//      serialDebugPrintLnString( "Comienzo del planificador." );
      tickCallbackSet( schedulerUpdate, NULL );
   }
}

/*==================[end of file]============================================*/
