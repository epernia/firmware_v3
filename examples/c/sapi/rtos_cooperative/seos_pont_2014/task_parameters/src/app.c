/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

// The maximum number of tasks required at any one time during the execution
// of the program. MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCHEDULER_MAX_TASKS   (2)

/*==================[inlcusiones]============================================*/

//#include "cooperative_os_scheduler_03_seos_Pont2014.h"   // <= own header (optional)
#include "sapi.h"        // <= sAPI header

#include "seos_pont_2014_isr.h"       // <= dispatcher and task management header
#include "seos_pont_2014_scheduler.h" // <= scheduler and system initialization header

#include "task1.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   task1_Init(LEDB);
   task1_Init(LED1);
   task1_Init(LED2);

   // FUNCION que inicializa el planificador de tareas
   schedulerInit();

   // Se agrega la tarea tarea1 al planificador
   schedulerAddTask( task1_Update, // funcion de tarea a agregar
                     (void*)LEDB,  // parametro de la tarea
                     0,            // offset de ejecucion en ticks
                     100           // periodicidad de ejecucion en ticks
                   );

   // Se agrega la tarea tarea2 al planificador
   schedulerAddTask( task1_Update, (void*)LED1, 1, 500 );

   // Se agrega la tarea tarea3 al planificador
   schedulerAddTask( task1_Update, (void*)LED2, 2, 1000 );

   // FUNCION que inicializa la interrupcion que ejecuta el planificador de
   // tareas con tick cada 1ms.
   schedulerStart( 1 );

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      // Se despachan (ejecutan) las tareas marcadas para su ejecucion.
      // Luego se pone el sistema en bajo consumo hasta que ocurra la
      // proxima interrupcion, en este caso la de Tick.
      // Al ocurrir la interrupcion de Tick se ejecutara el planificador
      // que revisa cuales son las tareas a marcar para su ejecucion.
      schedulerDispatchTasks();
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
