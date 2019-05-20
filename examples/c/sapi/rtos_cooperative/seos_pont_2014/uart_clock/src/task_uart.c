/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inlcusiones]============================================*/

#include "task_uart.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void taskUartInit( uint32_t baudrate )
{
   uartConfig( UART_USB, baudrate );
}

void taskUart( void* taskParam )
{
   uint8_t txByte = 0;

   txByte = Read_Char_From_Buffer();

   // Si el txByte es distinto de 0 envio lo que me llego
   if( txByte != 0 ){
      // Envio un byte si el canal esta libre
      uartWriteByte( UART_USB, txByte );
   }
}

/*==================[fin del archivo]========================================*/
