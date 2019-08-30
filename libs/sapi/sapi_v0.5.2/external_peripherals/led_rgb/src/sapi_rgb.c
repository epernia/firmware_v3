/* Copyright 2017, Agustin Bassi.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Date: 2017-11-01
 */

/*==================[inlcusiones]============================================*/

#include "sapi_rgb.h"
#include "chip.h"
#include "sapi_timer.h"

/*==================[definiciones y macros]==================================*/

//Cantidad de ticks para que pase un milisegundo
#define TICS_1_MS		 1000
//Tiempo maximo del periodo PWM para que vuelva a comenzar el ciclo
#define PWM_PERIOD      25
//Valor predefinido para indicar el periodo maximo del ciclo de trabajo.
#define MAX_DUTY_CYCLE  25
//Valor predefinido para indicar el periodo minimo del ciclo de trabajo.
#define MIN_DUTY_CYCLE   0

//Define MACRO para setear el timer a 1 ms y configurar el callback del timer a la funcion UpdateLedsStates
#define SetTimerInterrupt() Timer_Init(TIMER3,Timer_microsecondsToTicks(TICS_1_MS),UpdateLedsStates)

// Composicion de un LED RGB
typedef struct RgbLed {
   uint8_t pinRed;
   uint8_t pinGreen;
   uint8_t pinBlue;
   int8_t dutyCycleRed;
   int8_t dutyCycleGreen;
   int8_t dutyCycleBlue;
   int8_t toggleDutyCycleRed;	//Los valores toggle seran guardados para poder togglear con el valor actual del led
   int8_t toggleDutyCycleGreen;
   int8_t toggleDutyCycleBlue;
   bool_t toggled;
   uint8_t bright;	//Factor de escala para el brillo del LED
} RgbLed_t;

/*==================[definiciones de datos internos]=========================*/

/** Arreglo que contiene de manera estatica los datos correspondientes a cada led RGB*/
static RgbLed_t RgbLeds [MAX_AMOUNT_OF_RGB_LEDS];

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static void UpdateLedsStates( void *param);
static void StabilizePwmValues( LedRgbMap_t rgbLed );
static void	InitRgbFirstTime( void );

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/**
 * FUNCION de INTERRUPCION DEL Timer que se ejecuta cada vezque ocurre un Tick.
 */
static void UpdateLedsStates( void *param )
{
// Variable para contar el paso del tiempo
   static uint32_t ticksCounter = 0;
   uint8_t index;

   for (index = 0; index < MAX_AMOUNT_OF_RGB_LEDS; index++) {
      //Si el led fue inicializado
      if (RgbLeds[index].pinRed != 0) {
         if (ticksCounter == RgbLeds[index].dutyCycleRed)  gpioWrite (RgbLeds[index].pinRed,   OFF);
         if (ticksCounter == RgbLeds[index].dutyCycleGreen)gpioWrite (RgbLeds[index].pinGreen, OFF);
         if (ticksCounter == RgbLeds[index].dutyCycleBlue) gpioWrite (RgbLeds[index].pinBlue,  OFF);
      }
   }
   if (++ticksCounter == PWM_PERIOD) {
      for (index = 0; index < MAX_AMOUNT_OF_RGB_LEDS; index++) {
         //Si el led fue inicializado
         if (RgbLeds[index].pinRed != 0) {
            if (RgbLeds[index].dutyCycleRed > MIN_DUTY_CYCLE) 	gpioWrite (RgbLeds[index].pinRed,   ON);
            if (RgbLeds[index].dutyCycleGreen > MIN_DUTY_CYCLE) gpioWrite (RgbLeds[index].pinGreen, ON);
            if (RgbLeds[index].dutyCycleBlue > MIN_DUTY_CYCLE) 	gpioWrite (RgbLeds[index].pinBlue,  ON);
            ticksCounter = 0;
         }
      }
   }
   //return TRUE;
}

/**
 * Funcion para estabilizar los valores de ciclos de trabajo de cada LED.
 * Puede darse el caso que los valores superen los valores minimos o maximos.
 */
static void   StabilizePwmValues   (LedRgbMap_t rgbLed)
{
   //Estabilizar valores para el rojo
   if       (RgbLeds[rgbLed].dutyCycleRed < MIN_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
   } else if  (RgbLeds[rgbLed].dutyCycleRed > MAX_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleRed   = MAX_DUTY_CYCLE;
   }
   //Estabilizar valores para el verde
   if       (RgbLeds[rgbLed].dutyCycleGreen < MIN_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
   } else if  (RgbLeds[rgbLed].dutyCycleGreen > MAX_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleGreen = MAX_DUTY_CYCLE;
   }
   //Estabilizar valores para el azul
   if       (RgbLeds[rgbLed].dutyCycleBlue < MIN_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
   } else if  (RgbLeds[rgbLed].dutyCycleBlue > MAX_DUTY_CYCLE) {
      RgbLeds[rgbLed].dutyCycleBlue  = MAX_DUTY_CYCLE;
   }
}

/**
 * Inicializa el timer pwm y el arreglo con los datos de cada LED RGB.
 */
static void	  InitRgbFirstTime     ()
{
   static bool_t rgbInitialized;
   uint8_t auxVariable;
   if (!rgbInitialized) {
      Timer_Init(TIMER3, Timer_microsecondsToTicks(TICS_1_MS), UpdateLedsStates);
//		SetTimerInterrupt();
      // Inicializa todos los leds RGB.
      for (auxVariable = 0; auxVariable < MAX_AMOUNT_OF_RGB_LEDS; auxVariable++) {
         RgbLeds[auxVariable].pinRed         = 0;
         RgbLeds[auxVariable].pinGreen       = 0;
         RgbLeds[auxVariable].pinBlue        = 0;
         RgbLeds[auxVariable].dutyCycleRed   = 0;
         RgbLeds[auxVariable].dutyCycleGreen = 0;
         RgbLeds[auxVariable].dutyCycleBlue  = 0;
         RgbLeds[auxVariable].bright         = 100;
      }
      rgbInitialized = TRUE;
   }
}

/*==================[definiciones de funciones externas]=====================*/

/**
 * Funcion para configurar el driver RGB.
 * Se le debe pasar desde la aplicacion, cual es el pin rojo, verde y azul.
 * Ademas configura el timer3 para que genere una interrupcion cada 1 ms
 * y cuando ocurra la interrupcion llame a la funcion UpdateLedsStates(), que sirve
 * para actualizar el ciclo de trabajo de cada led.
 * Devuelve TRUE.
 */
bool_t rgbInit       (LedRgbMap_t rgbLed, uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue)
{
   InitRgbFirstTime();

   RgbLeds[rgbLed].pinRed   = pinRed;
   RgbLeds[rgbLed].pinGreen = pinGreen;
   RgbLeds[rgbLed].pinBlue  = pinBlue;

   gpioConfig(RgbLeds[rgbLed].pinRed,   GPIO_OUTPUT);
   gpioConfig(RgbLeds[rgbLed].pinGreen, GPIO_OUTPUT);
   gpioConfig(RgbLeds[rgbLed].pinBlue,  GPIO_OUTPUT);

   return TRUE;
}

/**
 * Manda por los pines PWM uno de los posibles colores definidos en color_t
 */
void 	rgbWriteColor  (LedRgbMap_t rgbLed, Color_t color)
{
   switch (color) {
   case WHITE:
      RgbLeds[rgbLed].dutyCycleBlue  = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MAX_DUTY_CYCLE;
      break;
   case VIOLET:
      RgbLeds[rgbLed].dutyCycleBlue  = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
      break;
   case CYAN:
      RgbLeds[rgbLed].dutyCycleBlue  = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MAX_DUTY_CYCLE;
      break;
   case BLUE:
      RgbLeds[rgbLed].dutyCycleBlue  = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
      break;
   case YELLOW:
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MAX_DUTY_CYCLE;
      break;
   case RED:
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MAX_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
      break;
   case GREEN:
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MAX_DUTY_CYCLE;
      break;
   case BLACK:
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
      break;
   default:
      RgbLeds[rgbLed].dutyCycleBlue  = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen = MIN_DUTY_CYCLE;
      break;
   }
   RgbLeds[rgbLed].toggled = FALSE;
}

/**
 * Manda una configuracion individual de cada canal del LED RGB seleccionado.
 */
void 	rgbWriteRaw    (LedRgbMap_t rgbLed, int8_t dutyRed, int8_t dutyGreen, int8_t dutyBlue)
{
   RgbLeds[rgbLed].dutyCycleRed   = dutyRed;
   RgbLeds[rgbLed].dutyCycleGreen = dutyGreen;
   RgbLeds[rgbLed].dutyCycleBlue  = dutyBlue;
   RgbLeds[rgbLed].toggled        = FALSE;
   //Estabiliza los valores por si se pasan del rango permitido.
   StabilizePwmValues(rgbLed);
}

/**
 * Setea el brillo sin cambiar el color (multiplica por una constante).
 */
void 	rgbWriteBright (LedRgbMap_t rgbLed, uint32_t bright)
{
   RgbLeds[rgbLed].bright  = bright;
   RgbLeds[rgbLed].toggled = FALSE;
}

/**
 * Lee el valor del duty cycle del LED Rojo
 * @param rgbLed led rgb a leer el duty cycle
 * @return valor del duty cycle
 */
uint8_t rgbReadDutyRed	(LedRgbMap_t rgbLed)
{
   return RgbLeds[rgbLed].dutyCycleRed;
}

/**
 * Lee el valor del duty cycle del LED Verde
 * @param rgbLed led rgb a leer el duty cycle
 * @return valor del duty cycle
 */
uint8_t rgbReadDutyGreen(LedRgbMap_t rgbLed)
{
   return RgbLeds[rgbLed].dutyCycleGreen;
}

/**
 * Lee el valor del duty cycle del LED Azul
 * @param rgbLed led rgb a leer el duty cycle
 * @return valor del duty cycle
 */
uint8_t rgbReadDutyBlue	(LedRgbMap_t rgbLed)
{
   return RgbLeds[rgbLed].dutyCycleBlue;
}

/**
 * Realiza un toggle del LED con el color que tenga guardado
 * @param rgbLed led a togglear.
 */
void rgbToggleLed (LedRgbMap_t rgbLed)
{
   if (!RgbLeds[rgbLed].toggled) {
      RgbLeds[rgbLed].toggleDutyCycleRed   = RgbLeds[rgbLed].dutyCycleRed;
      RgbLeds[rgbLed].toggleDutyCycleGreen = RgbLeds[rgbLed].dutyCycleGreen;
      RgbLeds[rgbLed].toggleDutyCycleBlue  = RgbLeds[rgbLed].dutyCycleBlue;
      RgbLeds[rgbLed].dutyCycleRed         = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleGreen       = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].dutyCycleBlue        = MIN_DUTY_CYCLE;
   } else {
      RgbLeds[rgbLed].dutyCycleRed         = RgbLeds[rgbLed].toggleDutyCycleRed;
      RgbLeds[rgbLed].dutyCycleGreen       = RgbLeds[rgbLed].toggleDutyCycleGreen;
      RgbLeds[rgbLed].dutyCycleBlue        = RgbLeds[rgbLed].toggleDutyCycleBlue;
      RgbLeds[rgbLed].toggleDutyCycleRed   = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].toggleDutyCycleGreen = MIN_DUTY_CYCLE;
      RgbLeds[rgbLed].toggleDutyCycleBlue  = MIN_DUTY_CYCLE;
   }
   RgbLeds[rgbLed].toggled = !RgbLeds[rgbLed].toggled;
}
/*==================[fin del archivo]========================================*/
