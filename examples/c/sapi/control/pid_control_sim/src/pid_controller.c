/*=====[pid_controller]========================================================
 * Copyright 2018 Diego Fernandez <dfernandez202@gmail.com>
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.1.0
 * Creation Date: 2018/09/24
 */

/*=====[Inclusions of private function dependencies]=========================*/

#include "pid_controller.h"

/*=====[Implementations of public functions]=================================*/

void pidInit( pidConfig_t* config, pidState_t* state )
{
   state->pastD = 0;
   state->pastY = 0;
   state->futureI = 0;
}

float pidControl( pidConfig_t* config, pidState_t* state, float y, float r )
{
   float P = 0;
   float D = 0;
   float I = 0;
   float U = 0;

   /*----- Calculate controller's output U ----------------------------------*/

   // P[k] = Kp * (b*r[k] - y[k]);
   P = config->Kp * (config->b * r - y);

   // D[k] = (Kd/(Kd + N*h)) * D[k-1] - (N*h*Kd/(Kd+N*h)) * (y[k]-y[k-1]);
   D = ( config->Kd * state->pastD 
         - config->N * config->h * config->Kd * (y - state->pastY) )
       / (config->Kd + config->N * config->h);

   // I[k] se calculo en la enterior iteracion (en la primera se asume 0)
   I = state->futureI;

   // U[k] = P[k] + I[k] + D[k]
   U = P + I + D;

   /*----- Update controller for next iteration -----------------------------*/

   state->pastD = D;
   state->pastY = y;
   // I[k+1] = I[k] + Ki*h*e[k], con e[k] = r[k] - y[k]
   state->futureI = I + config->Ki * config->h * (r - y);

   /*----- Return controller's output U -------------------------------------*/
   return U;
}
