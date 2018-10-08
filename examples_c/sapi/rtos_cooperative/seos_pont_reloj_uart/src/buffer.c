/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inclusions]=============================================*/

#include "buffer.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

static void Write_Char_To_Buffer( uint8_t data );

/*==================[internal data definition]===============================*/

static uint8_t TX_Indice_escritura = 0;
static uint8_t TX_Indice_lectura = 0;
static uint8_t TX_Buffer[TX_BUFFER_LENGTH];

/*==================[external data definition]===============================*/

uint8_t Buffer_Error_code;

/*==================[internal functions definition]==========================*/

static void Write_Char_To_Buffer( uint8_t data )
{
   // Write to the buffer *only* if there is space
   if (TX_Indice_escritura < TX_BUFFER_LENGTH){
      TX_Buffer[TX_Indice_escritura] = data;
      TX_Indice_escritura++;
   }
   else{
      // Write buffer is full
      Buffer_Error_code = ERROR_UART_FULL_BUFF;
   }
}

/*==================[external functions definition]==========================*/

void Write_String_To_Buffer( uint8_t *strPtr )
{
   uint8_t i = 0;
   while ( strPtr[i] != '\0'){
      Write_Char_To_Buffer( strPtr[i] );
      i++;
   }
}

uint8_t Read_Char_From_Buffer(void)
{
   uint8_t ret_val = 0;

   // Hay byte en el buffer Tx para transmitir?
   if (TX_Indice_lectura < TX_Indice_escritura){
      ret_val = TX_Buffer[TX_Indice_lectura];
      TX_Indice_lectura++;
   }
   else{
      // No hay datos disponibles para enviar
      TX_Indice_lectura = 0;
      TX_Indice_escritura = 0;
      ret_val = 0;
   }

   return ret_val;
}

/*==================[end of file]============================================*/
