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
 

/* Date: 2016-10-06 */

#ifndef _SAPI_CIRCULAR_BUFFER_H_
#define _SAPI_CIRCULAR_BUFFER_H_

/*==================[inclusions]=============================================*/

//#include <stdint.h>
#include "sapi_datatypes.h"        // data types

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define circularBufferConfig circularBufferInit

#define circularBufferNew( buffName, elementSize, amountOfElements )   circularBuffer_t buffName; \
   uint8_t buffName##_BufferMemory[ ((amountOfElements) + 1) * (elementSize) ];

/*
#define circularBufferNew( buffName, elementsSize, amountOfElements )   uint8_t buffName##_BufferMemory[ ((amountOfElements) + 1) * (elementsSize) ]; \
   circularBuffer_t buffName = { .memoryAddress    = (buffName##_BufferMemory), \
                                 .amountOfElements = ((amountOfElements) + 1),  \
                                 .elementSize      = (elementsSize),            \
                                 .readIndex        = 0,                         \
                                 .writeIndex       = 0,                         \
                                 .status           = CIRCULAR_BUFFER_EMPTY      \
   }
 */

#define circularBufferUse( buffName );   extern circularBuffer_t buffName;

#define circularBufferInit( buffName, elementSize, amountOfElements );   circularBuffer_Init( &(buffName), buffName##_BufferMemory, amountOfElements, elementSize );

/*==================[typedef]================================================*/

typedef enum {
   CIRCULAR_BUFFER_NORMAL,
   CIRCULAR_BUFFER_EMPTY,
   CIRCULAR_BUFFER_FULL
} circularBufferStatus_t;


typedef struct {
   uint8_t* memoryAddress;
   uint32_t amountOfElements;
   uint32_t elementSize;
   uint32_t readIndex;
   uint32_t writeIndex;
   circularBufferStatus_t status;
   callBackFuncPtr_t emptyBufferCallback;
   callBackFuncPtr_t fullBufferCalback;
} circularBuffer_t;

/*==================[external functions declaration]=========================*/

void circularBuffer_Init(
   circularBuffer_t* buffer,    // buffer structure
   uint8_t* bufferMemory,       // buffer array of memory
   uint32_t amountOfElements,   // amount of elements in buffer
   uint32_t elementSize         // each element size in bytes
);

void circularBufferEmptyBufferCallbackSet(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t emptyBufferCallback  // pointer to emptyBuffer function
);

void circularBufferFullBufferCallbackSet(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t fullBufferCalback    // pointer to fullBuffer function
);

circularBufferStatus_t circularBufferRead( circularBuffer_t* buffer,
      uint8_t *dataByte );

circularBufferStatus_t circularBufferWrite( circularBuffer_t* buffer,
      uint8_t *dataByte );

/*==================[example]==============================================*/

/*
#include "sapi.h"        // <= Biblioteca sAPI

#ifndef UART_DEBUG
   #define UART_DEBUG UART_USB
#endif

void emptyBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer vacio.\r\n" );
}

void fullBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer lleno.\r\n" );
}

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardInit();

   // Inicializar UART_USB como salida de consola
   consolePrintInitUart( UART_DEBUG, 115200 );


   static uint8_t txData = '1';
   static uint8_t rxData;

   // myBuff, buffer name (strcture)
   // 1, each element size in bytes
   // 8, amount of elements in buffer
   circularBufferNew( myBuff, 1, 8 );
   circularBufferInit( myBuff, 1, 8 );

   circularBufferSetEmptyBufferCallback( &myBuff,     // buffer structure
	                                     emptyBuff ); // pointer to emptyBuffer function

   circularBufferSetFullBufferCallback( &myBuff,      // buffer structure
		                                fullBuff );   // pointer to fullBuffer function

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
	   if( uartReadByte( UART_DEBUG, &rxData ) ){

		  if( rxData == 'w' ){
			 if( circularBufferWrite( &myBuff, &txData ) == CIRCULAR_BUFFER_NORMAL ){
				uartWriteString( UART_DEBUG, "guarde un " );
				uartWriteByte( UART_DEBUG, txData );
				uartWriteString( UART_DEBUG, " en el buffer.\r\n" );
				txData++;
			 }

		  }

		  if( rxData == 'r' ){
			 if( circularBufferRead( &myBuff, &rxData ) == CIRCULAR_BUFFER_NORMAL ){
				uartWriteString( UART_DEBUG, "Lei un " );
				uartWriteByte( UART_DEBUG, rxData );
				uartWriteString( UART_DEBUG, " del buffer.\r\n" );
			 }
		  }
	   }

	   if(txData > '9')
		  txData = '1';
   }

   return 0;
}
*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_CIRCULAR_BUFFER_H_ */
