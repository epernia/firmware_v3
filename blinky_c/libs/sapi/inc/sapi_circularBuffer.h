/* Copyright 2016, Eric Pernia.
 * All rights reserved.
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

#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

/*==================[inclusions]=============================================*/

#include <stdint.h>

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define CIRCULAR_BUFFER_SET_SIZE(val)   ((val)+1)

#define circularBufferNew( buffName, elementSize, amountOfElements );   circularBuffer_t buffName;\
   uint8_t buffName##_BufferMemory[ ((amountOfElements) + 1) * (elementSize) ];

#define circularBufferUse( buffName );   extern circularBuffer_t buffName;

#define circularBufferConfig( buffName, elementSize, amountOfElements );   circularBuffer_Config( &(buffName), buffName##_BufferMemory, amountOfElements, elementSize );

/*==================[typedef]================================================*/

typedef void (*callBackFuncPtr_t)(void);

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

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void circularBuffer_Config(
   circularBuffer_t* buffer,    // buffer structure
   uint8_t* bufferMemory,       // buffer array of memory
   uint32_t amountOfElements,   // amount of elements in buffer
   uint32_t elementSize         // each element size in bytes
                         );

void circularBufferSetEmptyBufferCallback(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t emptyBufferCallback  // pointer to emptyBuffer function
                                         );

void circularBufferSetFullBufferCallback(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t fullBufferCalback    // pointer to fullBuffer function
                                        );

circularBufferStatus_t circularBufferRead( circularBuffer_t* buffer,
                                           uint8_t *dataByte );

circularBufferStatus_t circularBufferWrite( circularBuffer_t* buffer,
                                            uint8_t *dataByte );

/*==================[cplusplus]==============================================*/



/*==================[example]==============================================*/

// Before main
/*
// EL BUFFER ES DE ITEMS DE A 1 BYTE

void emptyBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer vacio.\r\n" );
}

void fullBuff( void ){
   uartWriteString( UART_DEBUG, "Buffer lleno.\r\n" );
}

// Before while(1)

static uint8_t txData = '1';
static uint8_t rxData;

# define MY_BUFF_SIZE   CIRCULAR_BUFFER_SET_SIZE(8) // Recordar ingresar tamaño requerido + 1, porque deja un lugar en la cola

uint8_t myBuffMemory[ MY_BUFF_SIZE ];
circularBuffer_t myBuff;

circularBufferConfig(
         &(myBuff),
         myBuffMemory,
         MY_BUFF_SIZE,
         emptyBuff, fullBuff
      );
*/

// After while(1)
/*
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
*/
// -----------------------------------------



// -----------------------------------------

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _CIRCULAR_BUFFER_H_ */
