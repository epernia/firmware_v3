/* Copyright 2017, Danilo Zecchin.
 * Copyright 2018, Eric Pernia.
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
 * Date: 2017-11-17
 */

/* 
HC-SR04 connections:

   +--------------------------------------+
   |     *****     +----- +     *****     |  
   |   *+++++++*   +------+   *+++++++*   |   
   |  *+++++++++*   HCSR04   *+++++++++*  | 
   |   *+++++++*              *+++++++*   | 
   |     *****    o  o  o  o    *****     |
   +--------------+--+--+--+--------------+
                  |  |  |  |
                  V  T  E  G 
                  C  r  c  N
                  C  i  h  D
                     g  o

     EDU-CIAA-NXP +5V <--> Vcc HCSR04
   EDU-CIAA-NXP GPIO0 <--> Trig HCSR04
   EDU-CIAA-NXP GPIO2 <--> Echo HCSR04
     EDU-CIAA-NXP GND <--> GND HCSR04
*/

/*===========================================================================*/

#include "sapi.h"        // <= Inclusion de la Biblioteca sAPI

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   boardConfig(); // Inicializar y configurar la plataforma
   uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB

	// Inicializar el sensor ultrasonico HC-SR04
	ultrasonicSensorConfig( ULTRASONIC_SENSOR_0, ULTRASONIC_SENSOR_ENABLE );

	/* Configurar PWM */
	pwmConfig( 0, PWM_ENABLE );
	pwmConfig( PWM7, PWM_ENABLE_OUTPUT );

	delay(100); // Retardo inicial de 100 ms
   
   // Variables enteras de 32 bits para guardar la distancia en pulgadas
   // y centimetros
	uint32_t distanceInInches, distanceInCms;
   
   // ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE)
	{
		// Obtenemos la distancia actual medida por el sensor en pulgadas
		distanceInInches = ultrasonicSensorGetDistance(ULTRASONIC_SENSOR_0, INCH);
      
		// Obtenemos la distancia actual medida por el sensor en centimetros
		distanceInCms = ultrasonicSensorGetDistance(ULTRASONIC_SENSOR_0, CM);

		// Actualizamos el valor del PWM asociado al LED1 para ajustar su
      // intensidad luminica con la distancia en pulgadas
		pwmWrite( PWM7, distanceInInches );

		// Reportamos las distancias medidas en centimetros y pulgadas
		printf( "Distance: %d cm, %d \".\r\n", distanceInCms, distanceInInches );
      
		delay(1000); // Espero 1 s hasta la proxima lectura (minimo 50 ms)
	}
   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamente sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
