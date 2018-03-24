/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

//#include "blinky.h"   // <= own header (optional)
#include "sapi.h"       // <= sAPI header
#include "os.h"         // <= freeOSEK

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();
   
   // ---------- INICIAR SISTEMA OPERATIVO --------------------
	// Starts the operating system in the Application Mode 1
	// This example has only one Application Mode
	StartOS(AppMode1);

	// Starts the operating system in the Application Mode 1
	// This example has only one Application Mode
	StartOS(AppMode1);

	// StartOs shall never returns, but to avoid compiler warnings or errors
	// 0 is returned

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}


void ErrorHook(void)
{
	ShutdownOS(0);
}

TASK(PeriodicTask)
{
   // Intercambia el valor del LEDB
   gpioToggle( LEDB );

	// terminate task
	TerminateTask();
}

TASK(PeriodicTask2)
{
   // Intercambia el valor del LED3
   gpioToggle( LED3 );

	// terminate task
	TerminateTask();
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[end of file]============================================*/