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

/*==================[inclusions]=============================================*/

#include "sapi_circularBuffer.h"   // <= own header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static bool_t isSetEmptyBufferCallback = FALSE;
static bool_t isSetFullBufferCallback  = FALSE;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
void PLC_IL_ST(void* newValue, PLC_WORD lenght, PLC_EnumModifiers modifier)
{
   extern PLC_SymbolicRegister CR;

   // MEMORY COPY
   uint8_t *source;
   uint8_t *destination;
   uint16_t i = 0;

   destination = newValue;
   source = &(CR.VALUE);

   for( i = 0; i < lenght; i++ )
   {
      if (modifier == N)
      {
         *destination = ~(*source);
      }
      else
      {
         *destination = *source;
      }
      destination++;
      source++;
   }
}
*/

void circularBuffer_Init(
   circularBuffer_t* buffer,    // buffer structure
   uint8_t* bufferMemory,       // buffer array of memory
   uint32_t amountOfElements,   // amount of elements in buffer
   uint32_t elementSize         // each element size in bytes
)
{

   buffer->memoryAddress    = bufferMemory;
   buffer->amountOfElements = amountOfElements + 1;
   buffer->elementSize      = elementSize;
   buffer->readIndex        = 0;
   buffer->writeIndex       = 0;
   buffer->status           = CIRCULAR_BUFFER_EMPTY;
}


void circularBufferEmptyBufferCallbackSet(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t emptyBufferCallback  // pointer to emptyBuffer function
)
{

   // Empty buffer callback
   if( emptyBufferCallback != 0 ) {
      buffer->emptyBufferCallback = emptyBufferCallback;
      isSetEmptyBufferCallback = TRUE;
   }
}


void circularBufferFullBufferCallbackSet(
   circularBuffer_t* buffer,              // buffer structure
   callBackFuncPtr_t fullBufferCalback    // pointer to fullBuffer function
)
{
   // Full buffer callback
   if( fullBufferCalback != 0 ) {
      buffer->fullBufferCalback = fullBufferCalback;
      isSetFullBufferCallback = TRUE;
   }
}


circularBufferStatus_t circularBufferRead( circularBuffer_t* buffer,
      uint8_t *dataByte )
{

   uint8_t i = 0;

   // Is Empty?
   if ( (buffer->readIndex) == (buffer->writeIndex) ) {

      // Error, empty buffer
      buffer->status = CIRCULAR_BUFFER_EMPTY;

      // Execute emptyBufferCallback
      if( isSetEmptyBufferCallback ) {
         (* (buffer->emptyBufferCallback) )(0);
      }

   } else {

      buffer->status = CIRCULAR_BUFFER_NORMAL;

      // TODO:
      for( i=0; i<(buffer->elementSize); i++ ) {
         dataByte[i] = (buffer->memoryAddress)[ buffer->readIndex + i ];
      }

      // Increment readIndex (circular)
      buffer->readIndex = (buffer->readIndex + buffer->elementSize) % (buffer->amountOfElements * buffer->elementSize);

   }

   return buffer->status;
}


circularBufferStatus_t circularBufferWrite( circularBuffer_t* buffer,
      uint8_t *dataByte )
{

   uint8_t i = 0;

   // Is Full?
   if( ((buffer->writeIndex + buffer->elementSize) % (buffer->amountOfElements * buffer->elementSize) ) == (buffer->readIndex) ) {

      // Error, full buffer
      buffer->status = CIRCULAR_BUFFER_FULL;

      // Execute fullBufferCalback
      if( isSetFullBufferCallback ) {
         (* (buffer->fullBufferCalback) )(0);
      }

   } else {

      buffer->status = CIRCULAR_BUFFER_NORMAL;

      // TODO:
      for( i=0; i<(buffer->elementSize); i++ ) {
         (buffer->memoryAddress)[ buffer->writeIndex + i ] = dataByte[i];
      }

      // Increment writeIndex (circular)
      buffer->writeIndex = (buffer->writeIndex + buffer->elementSize) % (buffer->amountOfElements * buffer->elementSize);
   }

   return buffer->status;
}

/*==================[end of file]============================================*/
