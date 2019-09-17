/*=====[app]===================================================================
 * Copyright 2018 Diego Fernandez <dfernandez202@gmail.com>
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2018/09/24
 */

/*=====[Inclusions of function dependencies]=================================*/

#include "pid_controller.h"

/*=====[Definition macros of private constants]==============================*/

#define N_SAMPLES   200

/*=====[Definitions of private global variables]=============================*/

static float R[N_SAMPLES];
static float Y_step[N_SAMPLES];
static float Y_PID[N_SAMPLES];
static float U[N_SAMPLES];

static float NumZ[N_SAMPLES] = {0, 0.0201, -0.0191};
static float DenZ[N_SAMPLES] = {1, -1.9598, 0.9608};

/*=====[Main function, program entry point]==================================*/

int main (void)
{
   // ----- Configurations --------------------------
   pidConfig_t pidConfig;
   pidState_t pidState;

   pidConfig.h  = 0.01;
   pidConfig.Kp = 2.6;
   pidConfig.Ki = 0.08 / pidConfig.h;
   pidConfig.Kd = 10 * pidConfig.h;
   pidConfig.b  = 1;
   pidConfig.N  = 20;
   
   boardInit();

   pidInit( &pidConfig, &pidState );

   // Genero r[k] = escalon[k]:
   uint32_t i = 0;   
   for( i=0; i<N_SAMPLES; i++ ){
      if( i < N_SAMPLES/2 ){
         R[i] = 0.0;
      } else{
         R[i] = 1.0;
      }      
      // Aprovecho el loop para resetar los otros arrays:
      Y_step[i] = 0.0;
      Y_PID[i] = 0.0;
      U[i] = 0.0;
   }

   // Calculo Y_step[k], que es la salida del sistema aplicando
   // r[k] = escalon[k]:
   printf("\n\rY_STEP = [0;0;0;");
   uint32_t n = 0;   
   for( n=3; n<N_SAMPLES; n++ ){
      
      Y_step[n] = NumZ[1] * R[n-1] +
                  NumZ[2] * R[n-2] -
                  DenZ[1] * Y_step[n-1] -
                  DenZ[2] * Y_step[n-2];
                  
      printf("%f;", Y_step[n]);
   }
   printf("];\r\n");

   // Calculo Y_PID[k], que es la salida del sistema aplicando:
   // r[k] = escalon[k] y el control PID:
   printf("\n\rY_CONTROL = [0;0;0;");
   for( n=3; n<N_SAMPLES; n++ ){
      
      // Calculo U[k] la salida del controlador PID
      U[n] = pidControl( &pidConfig, &pidState, Y_PID[n-1], R[n] );
      
      // Y_PID[k] que es la salida del sistema aplicando r[k] = escalon[k] y
      // el control PID al sistema:
      Y_PID[n] = NumZ[1] * U[n-1] + 
                 NumZ[2] * U[n-2] -
                 DenZ[1] * Y_PID[n-1] -
                 DenZ[2] * Y_PID[n-2];
      
      printf("%f;", Y_PID[n]);
   }
   printf("];\r\n");

   // ----- Repeat for ever -------------------------
   while(TRUE);

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
