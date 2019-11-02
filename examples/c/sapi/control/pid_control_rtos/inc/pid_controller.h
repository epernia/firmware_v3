/*=====[pid_controller]========================================================
 * Copyright 2018 Diego Fernandez <dfernandez202@gmail.com>
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.3.0
 * Creation Date: 2018/09/24
 */

/*=====[Ecuaciones PID discretizado]=========================================*/
/*
   u[k] = P[k] + I[k] + D[k]

   con:
         P[k] = Kp ( b r[k] - y[k] )

         I[k+1] = I[k] + Ki h ( r[k] - y[k] )

                    Kd                N h Kd
         D[k] = __________ D[k-1] - __________ ( y[k] - y[k-1] )
                 Kd + N h            Kd + N h

   Constantes del PID:
      Kp = K ganancia proporcional
      Ki = K/Ti ganancia integral
      Kd = K*Td ganancia derivativa

   Otras constantes al discretizar:
      h es el periodo de muestreo [s]
      N es el limite para la gannacia de la accion de la derivada, en el rango
        [3 a 20]
      b sirve para pesar la referencia en la accion proporcional con el
        objetivo de evitar amplificar variaciones. En el rango [0 a 1]

   Señales:
      r[k] señal de referencia
      y[k] señal de salida del sistema
      u[k] señal de salida del PID

Estimar h = periodo de muestreo [s]:
   Ensayar una respuesta al escalon a lazo abierto y medir el tiempo de subida
   (tau_r) y elegir h en el rango:
                                   tau_r / 10  <=  h  <=  tau_r / 4
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

/*=====[Definition macros of public constants]===============================*/

// #define PID_PRINT_RESULT

#ifdef PID_PRINT_RESULT
// Number of samples to save
#define N_SAMPLES       50
#endif

/*=====[Public function-like macros]=========================================*/

// adcRead() returns 10 bits integer sample (uint16_t)
// sampleInVolts = (3.3 / 1023.0) * adcSample
#define getVoltsSampleFrom(adc0Channel) 3.3*(float)adcRead((adc0Channel))/1023.0

/*=====[Definitions of public data types]====================================*/

typedef struct {
   float Kp;   // PID constant Kp
   float Ki;   // PID constant Ki
   float Kd;   // PID constant Kd
   float h;    // h es el periodo de muestreo [s]
   float N;    // N en el rango [3 a 20]
   float b;    // b en el rango [0 a 1]
   float uMin; // Minimum saturation of actuator
   float uMax; // Maximum saturation of actuator
} PIDControllerConfig_t;

typedef struct {
   float P;
   float I;
   float D;
   float pastD;
   float pastY;
   float futureI;
   float u;
   float u_sat;
} PIDControllerState_t;

// PID strcuture (object)
typedef struct {
   PIDControllerConfig_t config;
   PIDControllerState_t state;
} PIDController_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

// PID Controller Initialization
void pidInit( PIDController_t* pid, // PID strcuture (object) by reference
              float Kp,             // PID constant Kp
              float Ki,             // PID constant Ki
              float Kd,             // PID constant Kd
              float h,              // h es el periodo de muestreo [s]
              float N,              // N en el rango [3 a 20]
              float b,              // b en el rango [0 a 1]
              float uMin,           // Minimum saturation of actuator
              float uMax            // Maximum saturation of actuator
            );

// Calculate PID controller output u[k] and return that
float pidCalculateControllerOutput(
   PIDController_t* pid, // PID strcuture (object) by reference
   float y,              // Measure of process output
   float r               // Measure of process reference
);

// Update PID controller for next iteration
void pidUpdateController(
   PIDController_t* pid, // PID strcuture (object) by reference
   float y,              // Measure of process output
   float r               // Measure of process reference
);

// PID printf object
void pidPrintf( PIDController_t* pid );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _PID_CONTROLLER_H_ */


/*=====[Usage example]=======================================================*/
/*
#include "sapi.h"
#include "pid_controller.h"

// Para calcular el tiempo que tarda el algoritmo y establecer un h minimo
//#define COUNT_CYCLES

void taskPIDControl( void* taskParmPtr )
{
   // Controller signals
   float r = 0.0f; // Measure of reference r[k]
   float y = 0.0f; // Measure of system output y[k]
   float u = 0.0f; // Calculated controller's output u[k]

   // h no puede ser menor ni al tiempo del algoritmo, y con va a ser un
   // multiplo del periodo de tick del RTOS
   uint32_t h_ms = 4; // Task periodicity (sample rate)

   // PID Controller structure (like an object instance)
   PIDController_t PsPIDController;

   // PID Controller Initialization
   pidInit( &PsPIDController,
            2.8,                 // Kp
            1.2,                 // Ki
            0.1,                 // Kd
            float(h_ms)/1000.0f, // h en [s]
            20.0f,               // N
            1.0f,                // b
            30.0f,               // u_min
            60.0f                // u_max
          );

   // Peridodic task each h_ms
   portTickType xPeriodicity =  h_ms / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();

   #ifdef COUNT_CYCLES
      // Configura el contador de ciclos con el clock de la EDU-CIAA NXP
      cyclesCounterConfig(EDU_CIAA_NXP_CLOCK_SPEED);
      volatile uint32_t cyclesElapsed = 0;
   #endif

   while(true) {

      #ifdef COUNT_CYCLES
         // Resetea el contador de ciclos
         cyclesCounterReset();
      #endif

      // Leer salida y[k] y refererencia r[k]
      // Example:
      //    y = adcRead( CH1 ) * SCALE_OUT;
      //    r = adcRead( CH2 ) * SCALE_REF;

      // Calculate PID controller output u[k]
      u = pidCalculateControllerOutput( &PsPIDController );

      // Actualizar la salida u[k]
      // Example:
      //    dacWrite( DAC, u * SCALE_PID_OUT );

      // Update PID controller for next iteration
      pidUpdateController( &PsPIDController );

      #ifdef COUNT_CYCLES
         // Leer conteco actual de ciclos
         cyclesElapsed = DWT_CYCCNT;
         volatile float us = cyclesCounterToUs(cyclesElapsed);
      #endif

      // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
      vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}
*/
