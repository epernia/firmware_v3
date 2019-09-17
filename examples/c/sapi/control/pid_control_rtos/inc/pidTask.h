/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/09/16
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __USER_TASKS_H__
#define __USER_TASKS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

void pidControlTask( void* taskParmPtr );  // Task declaration

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __USER_TASKS_H__ */
