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

// File creation date: 2016-03-01

//==================[inclusions]===============================================

#include "sapi_parser.h"

#include "string.h"
#include "sapi_circularBuffer.h"

//==================[external functions declaration]===========================

// Check for Receive a given pattern

waitForReceiveStringOrTimeoutState_t waitForReceiveStringOrTimeout(
   uartMap_t uart, waitForReceiveStringOrTimeout_t* instance )
{

   uint8_t receiveByte;
   //char receiveBuffer[100];

   switch( instance->state ) {

   case PARSER_RECEIVE_STRING_CONFIG:

      delayInit( &(instance->delay), instance->timeout );

      instance->stringIndex = 0;

      instance->state = PARSER_RECEIVE_STRING_RECEIVING;

      break;

   case PARSER_RECEIVE_STRING_RECEIVING:

      if( uartReadByte( uart, &receiveByte ) ) {

         //uartWriteByte( UART_DEBUG, receiveByte ); // TODO: DEBUG
         /*            if( (instance->stringIndex) <= 100 ){
                        receiveBuffer[instance->stringIndex] = receiveByte;
                     }
         */
         if( (instance->string)[(instance->stringIndex)] == receiveByte ) {

            (instance->stringIndex)++;

            if( (instance->stringIndex) == (instance->stringSize - 1) ) {
               instance->state = PARSER_RECEIVE_STRING_RECEIVED_OK;

//                  receiveBuffer[instance->stringIndex] = '\0';

               //uartWriteString( UART_DEBUG, receiveBuffer ); // TODO: DEBUG
               //uartWriteString( UART_DEBUG, "\r\n" );        // TODO: DEBUG
            }

         }

      }

      if( delayRead( &(instance->delay) ) ) {
         instance->state = PARSER_RECEIVE_STRING_TIMEOUT;
         //uartWriteString( UART_DEBUG, "\r\n" ); // TODO: DEBUG
      }

      break;

   case PARSER_RECEIVE_STRING_RECEIVED_OK:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;

   case PARSER_RECEIVE_STRING_TIMEOUT:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;

   default:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;
   }

   return instance->state;
}

// Recibe bytes hasta que llegue el string patron que se le manda en el
// parametro string, stringSize es la cantidad de caracteres del string.
// Devuelve TRUE cuando recibio la cadena patron, si paso el tiempo timeout
// en milisegundos antes de recibir el patron devuelve FALSE.
// No almacena los datos recibidos!! Simplemente espera a recibir cierto patron.
bool_t waitForReceiveStringOrTimeoutBlocking(
   uartMap_t uart, char* string, uint16_t stringSize, tick_t timeout )
{

   bool_t retVal = TRUE; // True if OK

   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;

   waitTextState = PARSER_RECEIVE_STRING_CONFIG;

   waitText.state = PARSER_RECEIVE_STRING_CONFIG;
   waitText.string =  string;
   waitText.stringSize = stringSize;
   waitText.timeout = timeout;

   while( waitTextState != PARSER_RECEIVE_STRING_RECEIVED_OK &&
          waitTextState != PARSER_RECEIVE_STRING_TIMEOUT ) {
      waitTextState = waitForReceiveStringOrTimeout( uart, &waitText );
   }

   if( waitTextState == PARSER_RECEIVE_STRING_TIMEOUT ) {
      retVal = FALSE;
   }

   return retVal;
}


// Store bytes until receive a given pattern
waitForReceiveStringOrTimeoutState_t receiveBytesUntilReceiveStringOrTimeout(
   uartMap_t uart, waitForReceiveStringOrTimeout_t* instance,
   char* receiveBuffer, uint32_t* receiveBufferSize )
{

   uint8_t receiveByte;
   static uint32_t i = 0;
   //uint32_t j = 0;
   //uint32_t savedReceiveBufferSize = *receiveBufferSize;

   switch( instance->state ) {

   case PARSER_RECEIVE_STRING_CONFIG:

      delayInit( &(instance->delay), instance->timeout );

      instance->stringIndex = 0;
      i = 0;

      instance->state = PARSER_RECEIVE_STRING_RECEIVING;

      break;

   case PARSER_RECEIVE_STRING_RECEIVING:

      if( uartReadByte( uart, &receiveByte ) ) {

         //uartWriteByte( UART_DEBUG, receiveByte ); // TODO: DEBUG
         if( i < *receiveBufferSize ) {
            receiveBuffer[i] = receiveByte;
            i++;
         } else {
            instance->state = PARSER_RECEIVE_STRING_FULL_BUFFER;
            *receiveBufferSize = i;
            i = 0;
            return instance->state;
         }

         if( (instance->string)[(instance->stringIndex)] == receiveByte ) {

            (instance->stringIndex)++;

            if( (instance->stringIndex) == (instance->stringSize - 1) ) {
               instance->state = PARSER_RECEIVE_STRING_RECEIVED_OK;
               *receiveBufferSize = i;
               /*
               // TODO: For debug purposes
               for( j=0; j<i; j++ ){
                  uartWriteByte( UART_DEBUG, receiveBuffer[j] );
               }
               uartWriteString( UART_DEBUG, "\r\n" );
               */
               i = 0;
            }

         }

      }

      if( delayRead( &(instance->delay) ) ) {
         instance->state = PARSER_RECEIVE_STRING_TIMEOUT;
         //uartWriteString( UART_DEBUG, "\r\n" ); // TODO: DEBUG
         *receiveBufferSize = i;
         i = 0;
      }

      break;

   case PARSER_RECEIVE_STRING_RECEIVED_OK:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;

   case PARSER_RECEIVE_STRING_TIMEOUT:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;

   case PARSER_RECEIVE_STRING_FULL_BUFFER:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;

   default:
      instance->state = PARSER_RECEIVE_STRING_CONFIG;
      break;
   }

   return instance->state;
}

// Guarda todos los bytes que va recibiendo hasta que llegue el string
// patron que se le manda en el parametro string, stringSize es la cantidad
// de caracteres del string.
// receiveBuffer es donde va almacenando los caracteres recibidos y
// receiveBufferSize es el tamaño de buffer receiveBuffer.
// Devuelve TRUE cuando recibio la cadena patron, si paso el tiempo timeout
// en milisegundos antes de recibir el patron devuelve FALSE.
bool_t receiveBytesUntilReceiveStringOrTimeoutBlocking(
   uartMap_t uart, char* string, uint16_t stringSize,
   char* receiveBuffer, uint32_t* receiveBufferSize,
   tick_t timeout )
{

   bool_t retVal = TRUE; // True if OK

   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;

   waitTextState = PARSER_RECEIVE_STRING_CONFIG;

   waitText.state = PARSER_RECEIVE_STRING_CONFIG;
   waitText.string =  string;
   waitText.stringSize = stringSize;
   waitText.timeout = timeout;

   while( waitTextState != PARSER_RECEIVE_STRING_RECEIVED_OK &&
          waitTextState != PARSER_RECEIVE_STRING_TIMEOUT ) {
      waitTextState = receiveBytesUntilReceiveStringOrTimeout(
                         uart, &waitText,
                         receiveBuffer, receiveBufferSize );
   }

   if( waitTextState == PARSER_RECEIVE_STRING_TIMEOUT ) {
      retVal = FALSE;
   }

   return retVal;
}

//==================[end of file]==============================================