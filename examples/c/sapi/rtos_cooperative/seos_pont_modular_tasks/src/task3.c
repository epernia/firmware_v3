/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "task3.h"  // <= own header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/


/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

// Funcion que inicializa la tarea
void task3_Init( void ){
   gpioConfig(LED2,GPIO_OUTPUT);
}

// Funcion que se ejecuta periodicamente
void task3_Update( void* taskParam ){   
   gpioToggle(LED2);
}

/*==================[end of file]============================================*/
