/*=====[pid_controller]========================================================
 * Copyright 2018 Diego Fernandez <dfernandez202@gmail.com>
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.1.0
 * Creation Date: 2018/09/24
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _PID_CONTROLLER_H_
#define _PID_CONTROLLER_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definitions of public data types]====================================*/

typedef struct {
   float Kp;
   float Ki;
   float Kd;
   float h;
   float N;
   float b;
} pidConfig_t;

typedef struct {
   float pastD;
   float pastY;
   float futureI;
} pidState_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

void pidInit( pidConfig_t* config, pidState_t* state );

float pidControl( pidConfig_t* config, pidState_t* state, float y, float r );

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _PID_CONTROLLER_H_ */
