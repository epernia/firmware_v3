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
 */

/*
 * Date: 2018-26-09
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"         /* <= sAPI header */

/* Callbacks de UART_USB - Declaraciones */
void uartUsbReceiveCallback( void *unused );
void uartUsbSendCallback( void *unused );

/* Callbacks de CDC UART - Declaraciones */
void cdcUartReceiveCallback( void *unused );
void cdcUartSendCallback( void *unused );

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 115200);   
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_USB, UART_RECEIVE, uartUsbReceiveCallback, NULL);
   // Seteo un callback al evento de transmisor libre y habilito su interrupcion
   uartCallbackSet(UART_USB, UART_TRANSMITER_FREE, uartUsbSendCallback, NULL);
   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_USB, true);
   
   /* Inicializar la UART_232 */
   cdcUartConfig(115200);
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   cdcUartCallbackSet(UART_RECEIVE, cdcUartReceiveCallback, NULL);
   // Seteo un callback al evento de transmisor libre y habilito su interrupcion
   cdcUartCallbackSet(UART_TRANSMITER_FREE, cdcUartSendCallback, NULL);   
   // Habilito todas las interrupciones de UART_232
   cdcUartInterrupt(true);

   // Envío para identificar cual es cual y para arrancar la secuencia (si no
   // hasta que no se envie un byte desde cada terminal no funciona)
   uartWriteByte(UART_USB, 'u');
   cdcUartWriteByte('2');

   /* ------------- REPETIR POR SIEMPRE ------------- */

   while(1) {
      /* Sleep until next Interrupt happens */
      sleepUntilNextInterrupt();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/* Callbacks - Implementaciones */

uint8_t dataToSendToCdcUart = 0;
bool_t dataToSendToCdcUartPending = FALSE;

// Recibo de la PC en la UART_USB
void uartUsbReceiveCallback( void *unused )
{
   dataToSendToCdcUart = uartRxRead(UART_USB);
   dataToSendToCdcUartPending = TRUE;
}
// Envio a la PC desde la CDC_UART
void cdcUartSendCallback( void *unused )
{
   if(dataToSendToCdcUartPending){
      cdcUartTxWrite(dataToSendToCdcUart);
      dataToSendToCdcUart = 0;
      dataToSendToCdcUartPending = FALSE;
   }
}

uint8_t dataToSendToUartUsb = 0;
bool_t dataToSendToUartUsbPending = FALSE;

// Recibo de la PC en la CDC_UART
void cdcUartReceiveCallback( void *unused )
{
   dataToSendToUartUsb = cdcUartRxRead();
   dataToSendToUartUsbPending = TRUE;
}
// Envio a la PC desde la UART_USB
void uartUsbSendCallback( void *unused )
{
   if(dataToSendToUartUsbPending){
      uartTxWrite(UART_USB, dataToSendToUartUsb);
      dataToSendToUartUsb = 0;
      dataToSendToUartUsbPending = FALSE;
   }
}

/*==================[end of file]============================================*/
