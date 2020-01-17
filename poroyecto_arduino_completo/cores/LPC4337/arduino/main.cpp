/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define ARDUINO_MAIN
#include "Arduino.h"


// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void init_tecs_leds()
{
     /* Inicializar GPIOs */
   gpioConfig( (gpioMap_t) 0, // don't care with GPIO_ENABLE 
               GPIO_ENABLE );

   /* Configuracion de pines de entrada para Teclas de la CIAA-NXP */
   gpioConfig( TEC1, GPIO_INPUT );
   gpioConfig( TEC2, GPIO_INPUT );
   gpioConfig( TEC3, GPIO_INPUT );
   gpioConfig( TEC4, GPIO_INPUT );
/*
   gpioConfig( DI0, GPIO_INPUT );
   gpioConfig( DI1, GPIO_INPUT );
   gpioConfig( DI2, GPIO_INPUT );
   gpioConfig( DI3, GPIO_INPUT );
   gpioConfig( DI4, GPIO_INPUT );
   gpioConfig( DI5, GPIO_INPUT );
   gpioConfig( DI6, GPIO_INPUT );
   gpioConfig( DI7, GPIO_INPUT );
*/
   /* Configuracion de pines de salida para Leds de la CIAA-NXP */
   gpioConfig( LEDR, GPIO_OUTPUT );
   gpioConfig( LEDG, GPIO_OUTPUT );
   gpioConfig( LEDB, GPIO_OUTPUT );
   gpioConfig( LED1, GPIO_OUTPUT );
   gpioConfig( LED2, GPIO_OUTPUT );
   gpioConfig( LED3, GPIO_OUTPUT ); 
/*
   gpioConfig( DO0, GPIO_OUTPUT ); 
   gpioConfig( DO1, GPIO_OUTPUT ); 
   gpioConfig( DO2, GPIO_OUTPUT ); 
   gpioConfig( DO3, GPIO_OUTPUT ); 
   gpioConfig( DO4, GPIO_OUTPUT ); 
   gpioConfig( DO5, GPIO_OUTPUT ); 
   gpioConfig( DO6, GPIO_OUTPUT ); 
   gpioConfig( DO7, GPIO_OUTPUT ); 
*/
   /*Configiración incial de pwm */
   pwmConfig( (pwmMap_t)0,    PWM_ENABLE );

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */
   adcConfig( ADC_ENABLE ); /* ADC */
   dacConfig( DAC_ENABLE ); /* DAC */

}

/*
 * \brief Main entry point of Arduino application
 */

int main( void )
{
  /* Inicializar la placa */
  boardConfig();

  init_tecs_leds();
/*
   gpioWrite(DO0, ON);
   gpioWrite(DO1, OFF);
   gpioWrite(DO4, ON);
   gpioWrite(DO5, OFF);

   gpioWrite(LEDB, ON);
   gpioWrite(LED3, ON);
*/
  /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
  tickConfig( 1, 0 );

#if defined(USBCON)
	usbd_interface_init();
#endif

	setup();

	for (;;)
	{
		loop();
	}

	return 0;
}
