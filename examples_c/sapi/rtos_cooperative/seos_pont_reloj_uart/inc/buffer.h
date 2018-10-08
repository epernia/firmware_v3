/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

#ifndef _BUFFER_H_
#define _BUFFER_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define TX_BUFFER_LENGTH     20

#define ERROR_UART_FULL_BUFF  15
#define ERROR_UART_TX_TIMEOUT 16

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void Write_String_To_Buffer( uint8_t *strPtr );

uint8_t Read_Char_From_Buffer( void );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _BUFFER_H_ */
