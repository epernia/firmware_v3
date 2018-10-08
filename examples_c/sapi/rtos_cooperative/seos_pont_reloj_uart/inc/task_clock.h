/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

#ifndef _TASK_CLOCK_H_
#define _TASK_CLOCK_H_

/*==================[inclusiones]============================================*/

#include "sapi.h"
#include "buffer.h"

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/

void taskClockInit( uint8_t h, uint8_t m, uint8_t s );
void taskClock( void* taskParam );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _TASK_CLOCK_H_ */
