/* Copyright 2016, Eric Pernia.
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
 * Date: 2016-07-28
 */

/*==================[inclusions]=============================================*/

//#include "keypad_7segment.h"   // <= own header (optional)
#include "sapi.h"                // <= sAPI header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Configuracion de pines para el Teclado Matricial*/

   // Teclado
   keypad_t keypad;

   // Filas --> Salidas
   uint8_t keypadRowPins1[4] = {
      RS232_TXD, // Row 0
      CAN_RD,    // Row 1
      CAN_TD,    // Row 2
      T_COL1     // Row 3
   };

   // Columnas --> Entradas con pull-up (MODO = GPIO_INPUT_PULLUP)
   uint8_t keypadColPins1[4] = {
      T_FIL0,    // Column 0
      T_FIL3,    // Column 1
      T_FIL2,    // Column 2
      T_COL0     // Column 3
   };

   // Vector de conversion entre indice de tecla presionada y el índice del
   // display 7 segmentos
   uint16_t keypadToDesplayKeys[16] = {
                                           1,    2,    3, 0x0a,
                                           4,    5,    6, 0x0b,
                                           7,    8,    9, 0x0c,
                                        0x0e,    0, 0x0f, 0x0d
                                      };

   // Variable para guardar la tecla leida
   uint16_t tecla = 0;

   keypadConfig( &keypad, keypadRowPins1, 4, keypadColPins1, 4 );

   // Display config
   uint8_t disp1_digits[] = { LCD1, LCD2, LCD3, LCD4 };
   uint8_t disp1_pins[]= { GPIO5, GPIO7, GPIO6, GPIO1, GPIO2, GPIO4, GPIO3, GPIO8 };
   uint8_t disp1_buf[] = { 0, 0, 0, 0 };
   Display7Segment_t disp1;
   void display7SegmentInit(Display7Segment_t *disp, gpioMap_t* segments,
                            gpioMap_t *digits, uint8_t nDigits,
                            DisplayCommonType_t common, uint8_t *buf);
   display7SegmentInit( &disp1, disp1_pins, disp1_digits, 4, DISP7_ANODE, disp1_buf );
   display7SegmentWriteInt( &disp1, 0 );

   // Loop
   while(1) {
      display7SegmentRefresh( &disp1 );
      if( keypadRead( &keypad, &tecla ) ) {
         switch(tecla) {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 7:
         case 8:
         case 9:
            display7SegmentWriteInt( &disp1, tecla );
            break;
         case 0xA: // +
            display7SegmentClear( &disp1 );
            display7SegmentWriteIndex( &disp1, 0, 22);
            break;
         case 0xB: // *
            display7SegmentClear( &disp1 );
            display7SegmentWriteIndex( &disp1, 0, 25);
            break;
         case 0xC: // /
            display7SegmentWriteHex(&disp1, 0x000D);
            break;
         case 0xD: // -
            disp1_buf[0] = 0b01000000; // g -> encendido
            disp1_buf[0] = 0;
            disp1_buf[0] = 0;
            disp1_buf[0] = 0;
            break;
         }
      }
      delay(10);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
