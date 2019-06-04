/* Copyright 2016, Sebastian Guarino
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
 * Date: 2018-26-09
 */

#include "sapi.h"         /* <= sAPI header */

/* Callbacks - Declaraciones */

void uartUsbReceiveCallback( void *unused );
void uartUsbSendCallback( void *unused );

/* Declaraciones variables */
uint8_t *dataToSend = "Hello World. Hello World.\r\n";
uint8_t *pDataToSend;
bool_t toSend = false;

uint8_t dataReceived = 0;
bool_t dataToSendPending = FALSE;


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 115200);   
   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_USB, true);
   
   /* ------------- REPETIR POR SIEMPRE ------------- */

   //uartWriteByte(UART_USB, 'u');
   while(1) {
	  delay(1000);
	  pDataToSend = dataToSend;
	  uartCallbackSet(UART_USB, UART_TRANSMITER_FREE, uartUsbSendCallback, NULL);
	  //uartWriteByte(UART_USB, 'u');
	  uartSetPendingInterrupt(UART_USB);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/* Callbacks - Implementaciones */

// Envio a la PC desde la UART_USB hasta NULL y deshabilito Callback
void uartUsbSendCallback( void *unused )
{
	while (*pDataToSend != '\0') {
		if (uartTxReady(UART_USB)) {
			uartTxWrite(UART_USB, *pDataToSend++);
		} else
			break;
	}
	if (*pDataToSend == '\0') {
		uartCallbackClr( UART_USB, UART_TRANSMITER_FREE);
	}
}
