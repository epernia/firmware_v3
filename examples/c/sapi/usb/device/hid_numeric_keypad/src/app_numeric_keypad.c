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
 * Date: 2016-11-06
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"         /* <= sAPI header */

// Filas --> Salidas
const uint8_t keypadRowPins1[4] = {
   RS232_TXD, // Row 0
   CAN_RD,    // Row 1
   CAN_TD,    // Row 2
   T_COL1     // Row 3
};

// Columnas --> Entradas con pull-up (MODO = GPIO_INPUT_PULLUP)
const uint8_t keypadColPins1[4] = {
   T_FIL0,    // Column 0
   T_FIL3,    // Column 1
   T_FIL2,    // Column 2
   T_COL0     // Column 3
};

// Teclado
keypad_t keypad;

// Scan codes
const uint16_t keypadToScancodes[16] = {
   USB_KEY_1,         USB_KEY_2, USB_KEY_3,     USB_KEY_A,
   USB_KEY_4,         USB_KEY_5, USB_KEY_6,     USB_KEY_B,
   USB_KEY_7,         USB_KEY_8, USB_KEY_9,     USB_KEY_C,
   USB_KEY_BACKSPACE, USB_KEY_0, USB_KEY_ENTER, USB_KEY_D,
};

/*==================[external functions definition]==========================*/

void checkForPressedKeys( void* unused )
{
   /* Read keys */
   uint16_t tecla;
   if( keypadRead( &keypad, &tecla ) ){
         usbDeviceKeyboardPress( keypadToScancodes[ (uint8_t)tecla ] );
   }
}

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   keypadConfig( &keypad, keypadRowPins1, 4, keypadColPins1, 4 );

   // Configuration routine for HID Keyboard example   
   usbDeviceConfig(USB_HID_KEYBOARD);   
   usbDeviceKeyboardCheckKeysCallbackSet( checkForPressedKeys );
   
   /* ------------- REPETIR POR SIEMPRE ------------- */

   while(1) {

      /* Do Keyboard tasks */      
      usbDeviceKeyboardTasks();
      
      /* Sleep until next Interrupt happens */
      sleepUntilNextInterrupt();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
