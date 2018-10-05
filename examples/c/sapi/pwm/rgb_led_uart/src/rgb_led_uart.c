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

#include "sapi.h"

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   uint8_t dataUart;
   bool_t flagToggle = FALSE;
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida de consola
   uartConfig(UART_USB, 115200);

   // Mensaje de bienvenida y modo de uso
   stdioPrintf(UART_USB, "Bienvenido al controlador RGB por UART.\n\r");
   stdioPrintf(UART_USB, "R = ROJO\n\r");
   stdioPrintf(UART_USB, "G = VERDE\n\r");
   stdioPrintf(UART_USB, "B = AZUL\n\r");
   stdioPrintf(UART_USB, "V = VIOLETA\n\r");
   stdioPrintf(UART_USB, "Y = AMARILLO\n\r");
   stdioPrintf(UART_USB, "W = BLANCO\n\r");
   stdioPrintf(UART_USB, "C = CELESTE\n\r");
   stdioPrintf(UART_USB, "N = NEGRO\n\r");
   stdioPrintf(UART_USB, "0 = TOGGLE LED\n\r");

   // En la configuracion del driver RGB se le deben pasar los pines
   // En este caso se pasan los pines del LED RGB, pero pueden ser cuales quiera.
   rgbConfig (RGB_1, LEDR, LEDG, LEDB);

   // Se pueden crear hasta 9 LEDs RGB de la misma manera que el anterior. Ej:
   // rgbConfig (RGB_2, GPIO1, GPIO2, GPIO3);

   // Los colores y mapa de LEDs RGB estan en sapi/inc/sapi_rgb.h

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {

      // Si llega un dato por la UART correcto cambia de color el LED
      if (uartReadByte(UART_USB, &dataUart)) {
         if 		(dataUart == 'R') rgbWriteColor(RGB_1, RED);
         else if (dataUart == 'G') rgbWriteColor(RGB_1, GREEN);
         else if (dataUart == 'B') rgbWriteColor(RGB_1, BLUE);
         else if (dataUart == 'V') rgbWriteColor(RGB_1, VIOLET);
         else if (dataUart == 'Y') rgbWriteColor(RGB_1, YELLOW);
         else if (dataUart == 'W') rgbWriteColor(RGB_1, WHITE);
         else if (dataUart == 'C') rgbWriteColor(RGB_1, CYAN);
         else if (dataUart == 'N') rgbWriteColor(RGB_1, BLACK);
         else if (dataUart == '0') flagToggle = !flagToggle;
         stdioPrintf(UART_USB, "\n\r Color = %c", dataUart);
         delay(500);
      }

      if (gpioRead(TEC4)) {
         // Incrementar el duty cycle de cada LED mediante las teclas TEC1, TEC2 y TEC3.
         if (!gpioRead(TEC1)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1) + 1, rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1));
            delay(250);
         }
         if (!gpioRead(TEC2)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1) + 1, rgbReadDutyBlue(RGB_1));
            delay(250);
         }
         if (!gpioRead(TEC3)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1) + 1);
            delay(250);
         }
      } else {
         // Decrementar el duty cycle de cada LED mediante las teclas TEC1, TEC2 y TEC3.
         if (!gpioRead(TEC1)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1) - 1, rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1));
            delay(250);
         }
         if (!gpioRead(TEC2)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1) - 1, rgbReadDutyBlue(RGB_1));
            delay(250);
         }
         if (!gpioRead(TEC3)) {
            rgbWriteRaw(RGB_1, rgbReadDutyRed(RGB_1), rgbReadDutyGreen(RGB_1), rgbReadDutyBlue(RGB_1) - 1);
            delay(250);
         }
      }

      // Si desde la UART activaron el toggle se togglea el LED con el ultimo color guardado.
      if (flagToggle) {
         rgbToggleLed(RGB_1);
         delay (500);
      }
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
