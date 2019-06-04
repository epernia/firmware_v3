/* Copyright 2017, Danilo Zecchin.
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
 *
 */


/*
 * Ultrasonic sensor HC-SR04 API
 * Date: 2017-11-17
 */

/*==================[inclusions]=============================================*/

#include "sapi_ultrasonic_hcsr04.h"
#include "sapi_timer.h"
#include "sapi_gpio.h"

/*==================[macros and definitions]=================================*/

#define ULTRASONIC_SENSOR_NUMBER 		3     /* Support for 3 ultrasonic sensors */
#define ULTRASONIC_SENSOR_TRIGGER_GPIO 	GPIO0 /* Trigger mapped to GPIO0 */
#define RAISING_EDGE 					0	  /* IRQs defines */
#define FALLING_EDGE 					1
#define BOTH_EDGES 						2
#define SENSOR_SAMPLING_RATE 			60000 /* Value in uSec. */
#define SENSOR_TRIGGER_PULSE_WIDTH		10 	  /* Value in uSec */
#define TICKS_TO_US_FACTOR				(204000000 / 1000000 )	/* Ticks = us * LPC_MAX_FREQ / 1000000 */
#define US_TO_CMS_FACTOR				58	  /* From datasheet -> distance = pulse width time in us * velocity (340 m/s) / 2 */
#define US_TO_INCHS_FACTOR 				148	  /* From datasheet -> distance = pulse width time in us * velocity (340 m/s) / 2 */

/*==================[internal data declaration]==============================*/

typedef struct {
   uint8_t irqChannel;				/* IRQ channel */
   gpioInitLpc4337_t gpioInit;	/* GPIO config: port and pin */

} ultrasonicSensorIrqInit_t;

typedef struct {
   ultrasonicSensorMap_t ultrasonicSensor; /* Sensor ID */
   uint32_t echoRiseTime,					/* Ticks of echo pulse rise edge */
            echoFallTime,					/* Ticks of echo pulse fall edge */
            lastEchoWidth;					/* Echo pulse width in ticks */
   gpioMap_t echoGpio;						/* Gpio configured for echo pin */
   uint8_t enabled;						/* boolean flag */
   ultrasonicSensorIrqInit_t irqConfig;	/* Sensor IRQ config */
} ultrasonicSensor_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static ultrasonicSensor_t ultrasonicSensors[ULTRASONIC_SENSOR_NUMBER] = {

   // Configure GPIO pins for each board
   #if BOARD==ciaa_nxp
      // Sensor number | echoRiseTime | echoFallTime | lastEchoWidth | echoGpio | enabled | irqConfig (irqChannel, GpioPort, GpioPin) | position */
      {ULTRASONIC_SENSOR_0 , 0 , 0 , 0 , BTN , ULTRASONIC_SENSOR_DISABLE, { 0, { 3, 4 } } },  // 0
      {ULTRASONIC_SENSOR_1 , 0 , 0 , 0 , BTN , ULTRASONIC_SENSOR_DISABLE, { 1, { 5, 16 } } }, // 1
      {ULTRASONIC_SENSOR_2 , 0 , 0 , 0 , BTN , ULTRASONIC_SENSOR_DISABLE, { 2, { 3, 6 }  } }  // 2

   #elif BOARD==edu_ciaa_nxp
      // Sensor number | echoRiseTime | echoFallTime | lastEchoWidth | echoGpio | enabled | irqConfig (irqChannel, GpioPort, GpioPin) | position */
      {ULTRASONIC_SENSOR_0 , 0 , 0 , 0 , GPIO2 , ULTRASONIC_SENSOR_DISABLE, { 0, { 3, 4 } } },  // 0
      {ULTRASONIC_SENSOR_1 , 0 , 0 , 0 , GPIO4 , ULTRASONIC_SENSOR_DISABLE, { 1, { 5, 16 } } }, // 1
      {ULTRASONIC_SENSOR_2 , 0 , 0 , 0 , GPIO6 , ULTRASONIC_SENSOR_DISABLE, { 2, { 3, 6 }  } }  // 2

   #elif BOARD==ciaa_z3r0
      #error CIAA-Z3R0

   #elif BOARD==pico_ciaa
      #error PicoCIAA

   #else
      #error BOARD compile variable must be defined

   #endif
};

static ultrasonicSensorMap_t ultrasonicSensorsIrqMap[ULTRASONIC_SENSOR_NUMBER] = {
   /* position (irqChannel) | Sensor number */
   /*0*/ ULTRASONIC_SENSOR_0,
   /*1*/ ULTRASONIC_SENSOR_1,
   /*2*/ ULTRASONIC_SENSOR_2
};
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initGPIOIrqs()
{

   Chip_PININT_Init(LPC_GPIO_PIN_INT);
}

static void enableGPIOIrq(uint8_t irqChannel, uint8_t port, uint8_t pin, uint8_t edge)
{

   /*
    * Select irq channel to handle a GPIO interrupt, using its port and pin to specify it
    * From EduCiaa pin out spec: GPIO1[9] -> port 1 and pin 9
    */
   Chip_SCU_GPIOIntPinSel(irqChannel , port, pin);
   /* Clear actual configured interrupt status */
   Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
   /* Set edge interrupt mode */
   Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(irqChannel));

   if ( edge == RAISING_EDGE) {
      /* Enable high edge gpio interrupt */
      Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
   } else if ( edge == FALLING_EDGE) {
      /* Enable low edge gpio interrupt */
      Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
   } else {
      /* Enable high and low edge */
      Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
      Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(irqChannel));
   }

   /* Clear pending irq channel interrupts */
   NVIC_ClearPendingIRQ(PIN_INT0_IRQn + irqChannel);
   /* Enable irqChannel interrupt */
   NVIC_EnableIRQ(PIN_INT0_IRQn + irqChannel);
}

static void disableGPIOIrq(uint8_t irqChannel)
{
   /* Clear pending irq channel interrupts */
   NVIC_ClearPendingIRQ(PIN_INT0_IRQn + irqChannel);
   /* Disable irqChannel interrupt */
   NVIC_DisableIRQ(PIN_INT0_IRQn + irqChannel);
}

static void clearInterrupt(uint8_t irqChannel)
{
   /* Clear interrupt flag for irqChannel */
   Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT,PININTCH(irqChannel));
}

static void serveInterrupt(uint8_t irqChannel)
{
   ultrasonicSensorMap_t aSensor = ultrasonicSensorsIrqMap[irqChannel];
   /* If interrupt was because a rising edge */
   if ( Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH(irqChannel) ) {
      //TODO add method to sapi_timer.h in order to get a timer count -> avoid using LPC_TIMER0 directly

      /* Save actual timer count in echoRiseTime */
      ultrasonicSensors[aSensor].echoRiseTime = Chip_TIMER_ReadCount(LPC_TIMER0);
      /* Clear rise edge irq */
      Chip_PININT_ClearRiseStates(LPC_GPIO_PIN_INT,PININTCH(irqChannel));
   }
   /* If not, interrupt was because a falling edge */
   else {
      //TODO add method to sapi_timer.h in order to get a timer count -> avoid using LPC_TIMER0 directly
      //TODO echoFallTime may not be necesary

      /* Save actual timer count in echoFallTime */
      ultrasonicSensors[aSensor].echoFallTime = Chip_TIMER_ReadCount(LPC_TIMER0);
      /* Compute echo pulse width in timer ticks and save in lastEchoWidth */
      ultrasonicSensors[aSensor].lastEchoWidth = ultrasonicSensors[aSensor].echoFallTime - ultrasonicSensors[aSensor].echoRiseTime;
      /* Clear falling edge irq */
      Chip_PININT_ClearFallStates(LPC_GPIO_PIN_INT,PININTCH(irqChannel));
   }

   /* Clear IRQ status */
   clearInterrupt(irqChannel);
}

/*
 * When timer expires, ULTRASONIC_SENSOR_TRIGGER_GPIO is on
 */
static void timerExpired()
{
   gpioWrite(ULTRASONIC_SENSOR_TRIGGER_GPIO,ON);
}

/*
 * When trigger time expires (TIMER0 reachs TIMECOMPAREMATCH1),
 * ULTRASONIC_SENSOR_TRIGGER_GPIO is off
 */
static void triggerExpired()
{
   gpioWrite(ULTRASONIC_SENSOR_TRIGGER_GPIO,OFF);
}

/*==================[external functions definition]==========================*/

/*
 * @Brief   Configure an ultrasonic sensor
 * @param   aSensor:   sensor number (0 to 2)
 * @param   aConfig:   enable or disable sensor
 * @return   nothing
 */
void ultrasonicSensorInit(ultrasonicSensorMap_t aSensor, ultrasonicSensorInit_t aConfig)
{
   static uint8_t firstTime = 1;

   if ( aConfig == ULTRASONIC_SENSOR_ENABLE ) {

      /* If first time, trigger GPIO is set as output */
      if ( firstTime ) {
         gpioInit(ULTRASONIC_SENSOR_TRIGGER_GPIO, GPIO_OUTPUT);
         initGPIOIrqs();
      }

      /* Enable sensor echo GPIO pin as INPUT */
      gpioInit(ultrasonicSensors[aSensor].echoGpio, GPIO_INPUT);

      /* IRQ Enable to capture sensor echo pulse edges */
      enableGPIOIrq(ultrasonicSensors[aSensor].irqConfig.irqChannel,
                    ultrasonicSensors[aSensor].irqConfig.gpioInit.port,
                    ultrasonicSensors[aSensor].irqConfig.gpioInit.pin,
                    BOTH_EDGES);

      /* If first time, TIMER0 is set to cycle sensor trigger ever SENSOR_SAMPLING_RATE */
      if ( firstTime ) {
         /* Set TIMER0 to call "timerExpired" function every SENSOR_SAMPLING_RATE */
         Timer_Init(TIMER0,Timer_microsecondsToTicks(SENSOR_SAMPLING_RATE),timerExpired);
         /* Set TIMERCOMPAREMATCH1 to SENSOR_TRIGGER_PULSE_WIDTH. When timer counter reachs this value, "triggerExpired" function is called */
         Timer_EnableCompareMatch(TIMER0,TIMERCOMPAREMATCH1, Timer_microsecondsToTicks(SENSOR_TRIGGER_PULSE_WIDTH), triggerExpired);
         firstTime = 0;
      }

   } else {
      disableGPIOIrq(ultrasonicSensors[aSensor].irqConfig.irqChannel);
   }

   ultrasonicSensors[aSensor].enabled = aConfig;

}

/*
 * @Brief   retrieves sensor actual distance in the specified unit
 * @param   aSensor:	sensor number (0 to 2)
 * @param   anUnit:		returned value unit (centimeters, inch, etc)
 * @return  float value with measured distance
 */
float ultrasonicSensorGetDistance(ultrasonicSensorMap_t aSensor, unitMap_t anUnit)
{
   float result;

   /* Convert pulse width in timer ticks to microseconds */
   result = (float)((ultrasonicSensors[aSensor].lastEchoWidth/TICKS_TO_US_FACTOR));

   /* If CM requested */
   if ( anUnit == CM )
      /* Convert time in microseconds to distance in centimeters (from formula distance = pulse width time in us * velocity (340 m/s) / 2 )*/
      result = result / US_TO_CMS_FACTOR;
   else
      /* Convert time in microseconds to distance in inchs (from formula distance = pulse width time in us * velocity (340 m/s) / 2 )*/
      result = result / US_TO_INCHS_FACTOR;

   return(result);
}

/*==================[ISR external functions definition]======================*/
/*
 * @Brief:   GPIO Echo interrupt handler for each sensor
 */
#ifdef SAPI_USE_INTERRUPTS
#ifndef OVERRIDE_SAPI_HCSR04_GPIO_IRQ
void GPIO0_IRQHandler(void)
{
   serveInterrupt(0);
}

void GPIO1_IRQHandler(void)
{
   serveInterrupt(1);
}

void GPIO2_IRQHandler(void)
{
   serveInterrupt(2);
}
#endif
#endif
/*==================[end of file]============================================*/
