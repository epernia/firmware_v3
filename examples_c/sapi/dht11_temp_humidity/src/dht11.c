/* Copyright 2017, Sebastian Pablo Bedin.
   Copyright 2018, Eric Pernia.
   All rights reserved.

This file is part sAPI library for microcontrollers.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

// First version date: 2017-11-13

/*
DHT11 connections:
                                                DHT11
                                           +--------------+
             EDU-CIAA-NXP +3.3V <--> 1 ----| +++  +++  +++|
   EDU-CIAA-NXP GPIO1 (pull-up) <--> 2 ----| +++  +++  +++|
                 (SIN CONEXION) <--> 3 ----| +++  +++  +++|
               EDU-CIAA-NXP GND <--> 4 ----| +++  +++  +++|
                                           +--------------+

                                           DHT11 on a board
                                          +----------------+
                                          |    +-----------+--+
             EDU-CIAA-NXP GPIO1 <--> S ----o --| +++  +++  +++|
             EDU-CIAA-NXP +3.3V <--> + ----o --| +++  +++  +++|
              EDU-CIAA-NXP GND  <--> - ----o --| +++  +++  +++|
                                          |  --| +++  +++  +++|
                                          |    +-----------+--+
                                          +----------------+
*/

/*===========================================================================*/

#include "sapi.h"        // <= Inclusion de la Biblioteca sAPI

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   boardConfig(); // Inicializar y configurar la plataforma
   uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB

   dht11Init( GPIO1 ); // Inicializo el sensor DHT11

   // Variables para almacenar humedad y temperatura
   float humidity = 0, temperature = 0;

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {
      // Lectura del sensor DHT11, devuelve true si pudo leer correctamente
      if( dht11Read(&humidity, &temperature) ) {
         // Si leyo bien prendo el LEDG y enciendo el LEDR
         gpioWrite( LEDG, ON );
         gpioWrite( LEDR, OFF );
         // Informo los valores de los sensores
         printf( "Temperatura: %.2f grados C.\r\n", temperature );
         printf( "Humedad: %.2f  %.\r\n\r\n", humidity );
      } else {
         // Si leyo mal apago el LEDG y enciendo el LEDR
         gpioWrite( LEDG, OFF );
         gpioWrite( LEDR, ON );
         // Informo el error de lectura
         printf( "Error al leer DHT11.\r\n\r\n");
      }
      delay(1000); // Espero 1 segundo.
   }
   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamente sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
