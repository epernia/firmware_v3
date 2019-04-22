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

#ifndef _COOPERATIVE_OS_SCHEDULER_H_
#define _COOPERATIVE_OS_SCHEDULER_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"         /* <= sAPI header */

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/* The maximum number of tasks required at any one time during the execution
   of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT */
#ifndef SCHEDULER_MAX_TASKS
   #define SCHEDULER_MAX_TASKS   (10)
#endif

/*==================[typedef]================================================*/

/* Store in DATA area, if possible, for rapid access.
   Total memory per task is 7 bytes. */
typedef struct{
   // Pointer to the task (must be a 'void (void)' function)
   callBackFuncPtr_t pTask;  // void (* pTask)(void);
   // Delay (ticks) until the function will (next) be run
   // - see schedulerAddTask() for further details
   void* taskParam; // Parametro de tarea
   // Delay (ticks) until the function will (next) be run
   // - see schedulerAddTask() for further details
   int32_t delay;
   // Interval (ticks) between subsequent runs.
   // - see schedulerAddTask() for further details
   int32_t period;
   // Incremented (by scheduler) when task is due to execute
   int32_t runMe;
} sTask_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// FUNCION que contiene el despachador de tareas.
void schedulerDispatchTasks( void );

// FUNCION que añade una tarea al planificador.
int32_t schedulerAddTask( callBackFuncPtr_t pFunction, //void (* pFunction)(void),
                          void* TASKPARAM,
                          const int32_t DELAY,
                          const int32_t PERIOD
                        );

// FUNCION que remueve una tarea del planificador.
int8_t schedulerDeleteTask( int32_t taskIndex );

// FUNCION que reporta el estado del sistema.
void schedulerReportStatus( void );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _COOPERATIVE_OS_SCHEDULER_H_ */
