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

void checkForPressedKeys( void* unused )
{
   /* Read keys */
   if( !gpioRead( TEC1 ) ){
      usbDeviceKeyboardPress( USB_KEY_C ); // 'c' or 'C'
   }
   else if( !gpioRead( TEC2 ) ){
      usbDeviceKeyboardPress( USB_KEY_I ); // 'i' or 'I'
   }
   else if( !gpioRead( TEC3 ) ){         
      usbDeviceKeyboardPress( USB_KEY_A ); // 'a' or 'A'
   }
   else if( !gpioRead( TEC4 ) ){         
      usbDeviceKeyboardPress( USB_KEY_ENTER ); // Enter
   }
}

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

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
