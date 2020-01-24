#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

mkdir -p ${PB}/src
mkdir -p ${PB}/inc

PROGRAM_NAME_UPPER=$(echo $PROGRAM_NAME | tr '[:lower:]' '[:upper:]')

#$LICENSE_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

cat <<EOF > ${PB}/config.mk
# Compile options

VERBOSE=n
OPT=g
USE_NANO=n
SEMIHOST=n
USE_FPU=y

# Libraries

USE_LPCOPEN=y
USE_SAPI=y
EOF

if [ -f "$LICENSE_PATH/license_file_header.sh" ]
then
	. "$LICENSE_PATH/license_file_header.sh"
else
	echo "Error, license_file_header.sh not found."
	FILE_HEADER=""
fi

cat <<EOF > ${PB}/src/${PROGRAM_NAME}.c
$FILE_HEADER

/*=====[Definition macros of private constants]==============================*/

// The maximum number of tasks required at any one time during the execution
// of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCHEDULER_MAX_TASKS   (10)

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "seos_pont.h"
#include "userTasks.h"

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   boardInit();
   
   // Initialize myTask   
   myTaskInit( 0 ); // Parameter passed into the task init.

   // Initialize scheduler
   schedulerInit();

   // Se agrega la tarea tarea1 al planificador
   schedulerAddTask( myTaskUpdate, // Function that implements the task update.
                     0,            // Parameter passed into the task update.
                     0,            // Execution offset in ticks.
                     500           // Periodicity of task execution in ticks.
                   );

   // Initialize task scheduler each 1ms.
   schedulerStart( 1 );

   while( true ){
      // Dispatch (execute) tasks that are mark to be execute by scheduler.
      schedulerDispatchTasks();
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
EOF


cat <<EOF > ${PB}/src/userTasks.c
$FILE_HEADER

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"
 
/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Task initialization implementation
void myTaskInit( void* taskParmPtr )
{
   printf( "Blinky with SEOS Pont and sAPI.\r\n" );
}

// Task update implementation
void myTaskUpdate( void* taskParmPtr )
{
   gpioToggle( LED );
   printf( "Blink!\r\n" );
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

EOF

cat <<EOF > ${PB}/inc/userTasks.h
$FILE_HEADER

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __USER_TASKS_H__
#define __USER_TASKS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

// Task initialization declaration
void myTaskInit( void* taskParmPtr );

// Task update declaration
void myTaskUpdate( void* taskParmPtr );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __USER_TASKS_H__ */
EOF
