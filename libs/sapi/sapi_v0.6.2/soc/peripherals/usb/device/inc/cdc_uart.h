/*
 * @brief Programming API used with Virtual Communication port
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef _CDC_UCOM_H_
#define _CDC_UCOM_H_

#include "lpc_app_usbd_cfg.h"
#include "sapi_datatypes.h"
#include "sapi_uart.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup EXAMPLES_USBDROM_18XX43XX_CDC_UART
 * @{
 */

/**
 * @brief	USB to UART bridge port init routine
 * @param	hUsb		: Handle to USBD stack instance
 * @param	pDesc		: Pointer to configuration descriptor
 * @param	pUsbParam	: Pointer USB param structure returned by previous init call
 * @return	Always returns LPC_OK.
 */
ErrorCode_t cdcUartLpcInit( USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc,
                            USBD_API_INIT_PARAM_T *pUsbParam );


//-------------------------------------------------------------

// UART Initialization
#define cdcUartConfig cdcUartInit
void cdcUartInit( uint32_t baudRate );

// Read 1 byte from RX FIFO, check first if exist aviable data
//bool_t uartReadByte( uartMap_t uart, uint8_t* receivedByte );
// Blocking, Write 1 byte to TX FIFO
void cdcUartWriteByte( const uint8_t value );

// Blocking, Send a string
//void uartWriteString( uartMap_t uart, const char* str );

// Blocking, Send a Byte Array
//void uartWriteByteArray( uartMap_t uart, const uint8_t* byteArray, uint32_t byteArrayLen );

//-------------------------------------------------------------

// Return TRUE if have unread data in RX FIFO
//bool_t uartRxReady( uartMap_t uart );

// Return TRUE if have space in TX FIFO
//bool_t uartTxReady( uartMap_t uart );

// Send directly to buffer
// Read from RX FIFO
uint8_t cdcUartRxRead( void );
// Write in TX FIFO
void cdcUartTxWrite( uint8_t value );

//-------------------------------------------------------------
// Interrupts
//-------------------------------------------------------------

#ifdef SAPI_USE_INTERRUPTS

// UART Global Interrupt Enable/Disable
void cdcUartInterrupt( bool_t enable );

// UART Interrupt event Enable and set a callback
void cdcUartCallbackSet( uartEvents_t event, callBackFuncPtr_t callbackFunc, 
                         void* callbackParam );
                 
// UART Interrupt event Disable
void cdcUartCallbackClr( uartEvents_t event );

#endif /* SAPI_USE_INTERRUPTS */
//-------------------------------------------------------------

/**
 * @}
 */

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _CDC_UCOM_H_ */
