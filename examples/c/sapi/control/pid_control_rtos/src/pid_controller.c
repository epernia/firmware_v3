/*=====[pid_controller]========================================================
 * Copyright 2018 Diego Fernandez <dfernandez202@gmail.com>
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.3.0
 * Creation Date: 2018/09/24
 * 
 * Changelog:
 * Version 1.3.0, 2019/10/8, Santiago Germino <sgermino@retro-ciaa.com>:
 *    1) pidCalculateControllerOutput(): u_sat updated only on saturation.
 *    2) pidUpdateController(): saturation now detected by u != u_sat.
 *    3) usat_array[]: new storage for real saturated values.
 *    4) pidGatherDebugSamples(): debug code moved inside of their own function.
 *    5) Use of floatToString() in 4).
 *    6) getVoltsSampleFrom() macro moved to pid_controller.h.
 */

/*=====[Inclusions of private function dependencies]=========================*/

#include "pid_controller.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

#ifdef PID_PRINT_RESULT
static float r_array[N_SAMPLES];
static float y_array[N_SAMPLES];
static float u_array[N_SAMPLES];
static float usat_array[N_SAMPLES];
static uint32_t i = 0;
#endif

/*=====[Prototypes (declarations) of private functions]======================*/

#ifdef PID_PRINT_RESULT
static void pidGatherDebugSamples( PIDController_t* pid, float y, float r );
#endif

/*=====[Implementations of public functions]=================================*/

// PID Controller Initialization
void pidInit( PIDController_t* pid,
              float Kp, float Ki, float Kd,
              float h, float N, float b,
              float uMin, float uMax
            )
{
   // Configuration
   pid->config.Kp = Kp;
   pid->config.Ki = Ki;
   pid->config.Kd = Kd;
   pid->config.h = h;
   pid->config.N = N;
   pid->config.b = b;
   pid->config.uMin = uMin;
   pid->config.uMax = uMax;

   // State
   pid->state.P = 0.0f;
   pid->state.I = 0.0f;
   pid->state.D = 0.0f;
   pid->state.pastD = 0.0f;
   pid->state.pastY = 0.0f;
   pid->state.futureI = 0.0f;
   pid->state.u = 0.0f;
   pid->state.u_sat = 0.0f;

   //pidPrintf( pid );
}


// Calculate PID controller output u[k] and return it
float pidCalculateControllerOutput( PIDController_t* pid, float y, float r )
{
   // P[k] = Kp * (b*r[k] - y[k])
   pid->state.P = pid->config.Kp * (pid->config.b * r - y);

   // D[k] = (Kd/(Kd + N*h)) * D[k-1] - (N*h*Kd/(Kd+N*h)) * (y[k]-y[k-1])
   pid->state.D = ( pid->config.Kd * pid->state.pastD
                    - pid->config.N * pid->config.h * pid->config.Kd
                    * (y - pid->state.pastY) )
                  / (pid->config.Kd + pid->config.N * pid->config.h);

   // I[k] se calculo en la enterior iteracion (en la primera se asume 0)
   pid->state.I = pid->state.futureI;

   // u[k] = P[k] + I[k] + D[k]
   pid->state.u = pid->state.P + pid->state.I + pid->state.D;

   // init u_sat with u
   pid->state.u_sat = pid->state.u;

   // Apply saturation of actuator
   if( pid->state.u < pid->config.uMin ) {
      pid->state.u_sat = pid->config.uMin;
   }
   if( pid->state.u > pid->config.uMax ) {
      pid->state.u_sat = pid->config.uMax;
   }

   return pid->state.u_sat;
}



// Update PID controller for next iteration
void pidUpdateController( PIDController_t* pid, float y, float r )
{
   // D[k-1] = D[k]
   pid->state.pastD = pid->state.D;

   // Y[k-1] = Y[k]
   pid->state.pastY = y;

   // Apply anti wind-up:
   // Update integrator only if controller output is not saturated
   if( pid->state.u == pid->state.u_sat ) {
      // I[k+1] = I[k] + Ki*h*e[k], con e[k] = r[k] - y[k]
      pid->state.futureI = pid->state.I
                           + ( pid->config.Ki * pid->config.h * (r - y) );
   }

#ifdef PID_PRINT_RESULT
   /*----- Print PID results if is activated --------------------------------*/
   pidGatherDebugSamples (pid, y, r);
#endif
}


// PID printf object
void pidPrintf( PIDController_t* pid )
{   
   printf( "PID structure (object)\r\n" );
   printf( "----------------------\r\n\r\n" );
   printf( "Configuration:\r\n" );
   printf( "  Kp = %f\r\n", pid->config.Kp );
   printf( "  Ki = %f\r\n", pid->config.Ki );
   printf( "  Kd = %f\r\n", pid->config.Kd );
   printf( "  h = %f\r\n", pid->config.h );
   printf( "  N = %f\r\n", pid->config.N );
   printf( "  b = %f\r\n", pid->config.b );
   printf( "  uMax = %f\r\n", pid->config.uMax );
   printf( "  uMin = %f\r\n", pid->config.uMin );
   printf( "State:\r\n" );
   printf( "  P[k] = %f\r\n", pid->state.P );
   printf( "  I[k] = %f\r\n", pid->state.I );
   printf( "  D[k] = %f\r\n", pid->state.D );
   printf( "  D[k-1] = %f (past D)\r\n", pid->state.pastD );
   printf( "  y[k-1] = %f (past y)\r\n", pid->state.pastY );
   printf( "  I[k+1] = %f (future I)\r\n", pid->state.futureI );
   printf( "  u[k] = %f\r\n", pid->state.u );
   printf( "  u_sat[k] = %f\r\n", pid->state.u_sat );
   printf( "\r\n" );
}


#ifdef PID_PRINT_RESULT
static void pidGatherDebugSamples( PIDController_t* pid, float y, float r )
{
	char ntos[64];

   // Save and show samples
   if( i< N_SAMPLES ) {
      if (i == 0) {
         uartWriteString( UART_USB, "\r\n\r\nGathering " );
         uint64ToString( N_SAMPLES, ntos, 10 );
         uartWriteString( UART_USB, ntos );
         uartWriteString( UART_USB, " PID samples...\r\n\r\n" );
      }

      // Save N_SAMPLES samples of R[k] and Y[k]
      r_array[i] = r;
      y_array[i] = y;
      u_array[i] = pid->state.u;
      usat_array[i] = pid->state.u_sat;
      i++;

   } else {
      // Indicate that finish take samples
      gpioWrite( LEDB, OFF );
      gpioWrite( LED1, ON );

      // Note thath printf() does not work properly

      // print r[k] samples
      uartWriteString( UART_USB, "r = [ " );
      for( i=0; i<N_SAMPLES; i++) {
         uartWriteString( UART_USB, floatToString( r_array[i], ntos, 5 ) );
         uartWriteString( UART_USB, " " );
      }
      uartWriteString( UART_USB, "];\r\n\r\n" );

      // print y[k] samples
      uartWriteString( UART_USB, "y = [ " );
      for( i=0; i<N_SAMPLES; i++) {
         uartWriteString( UART_USB, floatToString( y_array[i], ntos, 5 ) );
         uartWriteString( UART_USB, " " );
      }
      uartWriteString( UART_USB, "];\r\n\r\n" );

      // print u[k] samples
      uartWriteString( UART_USB, "u = [ " );
      for( i=0; i<N_SAMPLES; i++) {
         uartWriteString( UART_USB, floatToString( u_array[i], ntos, 5 ) );
         uartWriteString( UART_USB, " " );
      }
      uartWriteString( UART_USB, "];\r\n\r\n" );

      // print uSat[k] samples
      uartWriteString( UART_USB, "uSat = [ " );
      for( i=0; i<N_SAMPLES; i++) {
         uartWriteString( UART_USB, floatToString( usat_array[i], ntos, 5 ) );
         uartWriteString( UART_USB, " " );
      }
      uartWriteString( UART_USB, "];\r\n\r\n" );

      uartWriteString( UART_USB, "All samples printed. PROGRAM HALTED.\r\n" );
      gpioWrite( LED2, ON );

      while(1);
   }
}
#endif

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/
