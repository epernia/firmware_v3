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

#include "seos_pont_2014_scheduler.h"  // <= own header
#include "sapi.h"                      // <= sAPI header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

// Keeps track of time since last error was recorded (see below)
//static int32_t errorTickCount;

// The code of the last error (reset after ~1 minute)
//static char lastErrorCode;

/*==================[internal functions declaration]=========================*/

static void schedulerGoToSleep(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

// The array of tasks
sTask_t schedulerTasks[SCHEDULER_MAX_TASKS];

// Used to display the error code
char errorCode = 0;

/*==================[internal functions definition]==========================*/


/*==================[external functions definition]==========================*/

/*------------------------------------------------------------------*
schedulerDispatchTasks()
This is the 'dispatcher' function. When a task (function)
is due to run, schedulerDispatchTasks() will run it.
This function must be called (repeatedly) from the main loop.
-*------------------------------------------------------------------*/
void schedulerDispatchTasks( void ){ // Despachador (corre en el main)

   int32_t index;
   // Dispatches (runs) the next task (if one is ready)
   for (index = 0; index < SCHEDULER_MAX_TASKS; index++){

      if (schedulerTasks[index].runMe > 0){

         (*schedulerTasks[index].pTask)(schedulerTasks[index].taskParam); // Run the task
         schedulerTasks[index].runMe -= 1;  // Reset/reduce runMe flag

         // Periodic tasks will automatically run again
         // - if this is a 'one shot' task, remove it from the array
         if (schedulerTasks[index].period == 0){
            schedulerDeleteTask(index);
         }

      }
   }

   // Report system status
   schedulerReportStatus();
   // The scheduler enters idle mode at this point32_t
   schedulerGoToSleep();
}

/*------------------------------------------------------------------*
schedulerAddTask()
Causes a task (function) to be executed at regular intervals
or after a user-defined delay
Fn_P - The name of the function which is to be scheduled.

NOTE: All scheduled functions must be 'void, void' -
that is, they must take no parameters, and have
a void return type.

DELAY - The interval (TICKS) before the task is first executed

PERIOD - If 'PERIOD' is 0, the function is only called once,
at the time determined by 'DELAY'. If PERIOD is non-zero,
then the function is called repeatedly at an interval
determined by the value of PERIOD (see below for examples
which should help clarify this)

RETURN VALUE:
Returns the position in the task array at which the task has been
added. If the return value is SCHEDULER_MAX_TASKS then the task could
not be added to the array (there was insufficient space). If the
return value is < SCHEDULER_MAX_TASKS, then the task was added
successfully.
Note: this return value may be required, if a task is
to be subsequently deleted - see schedulerDeleteTask().

EXAMPLES:
Task_ID = schedulerAddTask(Do_X,1000,0);
Causes the function Do_X() to be executed once after 1000 sch ticks.
Task_ID = schedulerAddTask(Do_X,0,1000);
Causes the function Do_X() to be executed regularly, every 1000 sch ticks.
Task_ID = schedulerAddTask(Do_X,300,1000);
Causes the function Do_X() to be executed regularly, every 1000 ticks.
Task will be first executed at T = 300 ticks, then 1300, 2300, etc.
-*------------------------------------------------------------------*/
int32_t schedulerAddTask( callBackFuncPtr_t pFunction, //void (* pFunction)(void),
                          void* TASKPARAM,
                          const int32_t DELAY,
                          const int32_t PERIOD
                        ){

   int32_t index = 0;

   // First find a gap in the array (if there is one)
   while( (schedulerTasks[index].pTask != 0) && (index < SCHEDULER_MAX_TASKS) ){
      index++;
   }

   // Have we reached the end of the list?
   if( index == SCHEDULER_MAX_TASKS ){
      // Task list is full
      // Set the global error variable
      errorCode = 2; // ERROR_schedulerTOO_MANYTasks;
      // Also return an error code
      return SCHEDULER_MAX_TASKS;
   }

   // If we're here, there is a space in the task array
   schedulerTasks[index].pTask  = pFunction;
   schedulerTasks[index].delay  = DELAY;
   schedulerTasks[index].period = PERIOD;
   schedulerTasks[index].taskParam = TASKPARAM;
   schedulerTasks[index].runMe  = 0;

   return index; // return position of task (to allow later deletion)
}

/*------------------------------------------------------------------*
schedulerDeleteTask()
Removes a task from the scheduler. Note that this does
*not* delete the associated function from memory:
it simply means that it is no longer called by the scheduler.

taskIndex - The task index. Provided by schedulerAddTask().

RETURN VALUE: RETURN_ERROR or RETURN_NORMAL
-*------------------------------------------------------------------*/
int8_t schedulerDeleteTask( int32_t taskIndex ){

   int8_t returnCode;

   if(schedulerTasks[taskIndex].pTask == 0){
      // No task at this location...
      // Set the global error variable
      errorCode = 2; // ERROR_SCH_CANNOT_DELETE_TASK;
      // ...also return an error code
      returnCode = -1; // RETURN_ERROR;
   }
   else{
      returnCode = 0; // RETURN_NORMAL;
   }
   schedulerTasks[taskIndex].pTask = 0x0000;
   schedulerTasks[taskIndex].delay = (int32_t)0;
   schedulerTasks[taskIndex].period = (int32_t)0;
   schedulerTasks[taskIndex].runMe = (int8_t)0;
   schedulerTasks[taskIndex].taskParam = 0;
   return returnCode; // return status
}

/*------------------------------------------------------------------*
schedulerReportStatus()
Simple function to display error codes.
This version displays code on a port with attached LEDs:
adapt, if required, to report errors over serial link, etc.
Errors are only displayed for a limited period
(60000 ticks = 1 minute at 1ms tick interval).
After this the the error code is reset to 0.
This code may be easily adapted to display the last
error 'for ever': this may be appropriate in your
application.
-*------------------------------------------------------------------*/
void schedulerReportStatus( void ){

#ifdef SCH_REPORT_ERRORS
   // ONLY APPLIES IF WE ARE REPORTING ERRORS
   // Check for a new error code
   if( errorCode != lastErrorCode ){

      // Negative logic on LEDs assumed
      errorPort = 255 - errorCode;
      lastErrorCode = errorCode;

      if( errorCode!= 0 ){
         errorTickCount = 60000;
      }
      else{
         errorTickCount = 0;
      }
   }
   else{
      if( errorTickCount != 0 ){
         if( --errorTickCount == 0 ){
            errorCode = 0; // Reset error code
         }
      }
   }
#endif

}

/*------------------------------------------------------------------*
schedulerGoToSleep()
This scheduler enters 'idle mode' between clock ticks
to save power. The next clock tick will return the processor
to the normal operating state.
*** May wish to disable this if using a watchdog ***
*** ADAPT AS REQUIRED FOR YOUR HARDWARE ***
-*------------------------------------------------------------------*/
static void schedulerGoToSleep(){
   // Se pone el sistema en bajo consumo hasta que ocurra la proxima
   // interrupcion, en este caso la de Tick.
   sleepUntilNextInterrupt();
}

/*==================[end of file]============================================*/
