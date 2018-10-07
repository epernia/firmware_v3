/* Copyright 2017, Alejandro Permingeat.
 * Copyright 2017, Rodrigo Furlani
 * Copyright 2017, Eric Pernia.
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
 * Date: 2017-11-10
 */

/*===========================================================================*/

#include "sapi.h"        // <= Inclusion de la Biblioteca sAPI

#define ADDRESS_MAG  0x0D  //direccion magnetometro

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   boardConfig(); // Inicializar y configurar la plataforma
   uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB

   // Estructura de configuracion del QMC5883L
   QMC5883L_config_t qmc5883L_configValue;
   
   // Cargo la Estructura de configuracion del QMC5883L con valores por defecto
   qmc5883lPrepareDefaultInit( &qmc5883L_configValue );
   
   // Cambio los valores que deseo configurar de la Estructura de configuracion
   // del QMC5883L y dejo el resto con sus valore por defecto
   qmc5883L_configValue.mode = QMC5883L_continuous_measurement;

   // Inicializar QMC5883L
   qmc5883lInit( qmc5883L_configValue );

   // Variables para almacenar los valores leidos del sensor
   int16_t qmc5883l_x_raw;
   int16_t qmc5883l_y_raw;
   int16_t qmc5883l_z_raw;

   // ---------- REPETIR POR SIEMPRE --------------------------
   while(TRUE) {

      // Leer magnetometro
      qmc5883lRead( &qmc5883l_x_raw, &qmc5883l_y_raw, &qmc5883l_z_raw );
      // Se debe esperar minimo 67ms entre lecturas su la tasa es de 15Hz
      // para leer un nuevo valor del magnetometro

      // Informar valores
      printf( "QMC5883L eje x: %d\r\n", qmc5883l_x_raw );
      printf( "QMC5883L eje y: %d\r\n", qmc5883l_y_raw );
      printf( "QMC5883L eje z: %d\r\n\r\n", qmc5883l_z_raw );

      delay(1000); // Espero 1 segundo.
   }
   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamente sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}
