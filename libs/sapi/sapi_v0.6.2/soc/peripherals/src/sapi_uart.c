/* Copyright 2014, Pablo Ridolfi (UTN-FRBA).
 * Copyright 2014, Juan Cecconi.
 * Copyright 2015-2017, Eric Pernia.
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
 *
 */

/* Date: 2016-02-26 */

/*==================[inclusions]=============================================*/

#include "sapi_uart.h"

#include "string.h"
#include "sapi_circularBuffer.h"

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef struct {
   LPC_USART_T*      uartAddr;
   lpc4337ScuPin_t   txPin;
   lpc4337ScuPin_t   rxPin;
   IRQn_Type         uartIrqAddr;
} uartLpcInit_t;

/*==================[internal data declaration]==============================*/

#ifdef SAPI_USE_INTERRUPTS
static callBackFuncPtr_t rxIsrCallbackUART0 = 0;
static void* rxIsrCallbackUART0Params = NULL;
static callBackFuncPtr_t rxIsrCallbackUART2 = 0;
static void* rxIsrCallbackUART2Params = NULL;
static callBackFuncPtr_t rxIsrCallbackUART3 = 0;
static void* rxIsrCallbackUART3Params = NULL;

static callBackFuncPtr_t txIsrCallbackUART0 = 0;
static void* txIsrCallbackUART0Params = NULL;
static callBackFuncPtr_t txIsrCallbackUART2 = 0;
static void* txIsrCallbackUART2Params = NULL;
static callBackFuncPtr_t txIsrCallbackUART3 = 0;
static void* txIsrCallbackUART3Params = NULL;
#endif /* SAPI_USE_INTERRUPTS */

static const uartLpcInit_t lpcUarts[] = {
// { uartAddr, { txPort, txpin, txfunc }, { rxPort, rxpin, rxfunc }, uartIrqAddr  },
   // UART_GPIO (GPIO1 = U0_TXD, GPIO2 = U0_RXD)
   { LPC_USART0, { 6, 4, FUNC2 }, { 6, 5, FUNC2 }, USART0_IRQn }, // 0
   // UART_485 (RS485/Profibus)
   { LPC_USART0, { 9, 5, FUNC7 }, { 9, 6, FUNC7 }, USART0_IRQn }, // 1
   // UART not routed
   {  LPC_UART1, { 0, 0, 0     }, { 0, 0, 0     }, UART1_IRQn  }, // 2
   // UART_USB
   { LPC_USART2, { 7, 1, FUNC6 }, { 7, 2, FUNC6 }, USART2_IRQn }, // 3
   // UART_ENET
   { LPC_USART2, { 1,15, FUNC1 }, { 1,16, FUNC1 }, USART2_IRQn }, // 4
   // UART_232
   { LPC_USART3, { 2, 3, FUNC2 }, { 2, 4, FUNC2 }, USART3_IRQn }  // 5
};

static const lpc4337ScuPin_t lpcUart485DirPin = {
   6, 2, FUNC2
};


/*
   callBackFuncPtr_t txIsrCallback;
   callBackFuncPtr_t rxIsrCallback;
*/

/*==================[internal functions declaration]=========================*/

#ifdef SAPI_USE_INTERRUPTS
static void uartProcessIRQ( uartMap_t uart );

/*==================[internal functions definition]==========================*/

static void uartProcessIRQ( uartMap_t uart )
{
   uint8_t status = Chip_UART_ReadLineStatus( lpcUarts[uart].uartAddr );

   // Rx Interrupt
   if(status & UART_LSR_RDR) { // uartRxReady
      // Execute callback
      if( ( uart == UART_GPIO ) && (rxIsrCallbackUART0 != 0) )
         (*rxIsrCallbackUART0)(rxIsrCallbackUART0Params);

      if( ( uart == UART_USB )  && (rxIsrCallbackUART2 != 0) )
         (*rxIsrCallbackUART2)(rxIsrCallbackUART2Params);

      if( ( uart == UART_232 )  && (rxIsrCallbackUART3 != 0) )
         (*rxIsrCallbackUART3)(rxIsrCallbackUART3Params);
   }

   // Tx Interrupt
   if( ( status & UART_LSR_THRE ) && // uartTxReady
       ( Chip_UART_GetIntsEnabled( lpcUarts[uart].uartAddr ) & UART_IER_THREINT ) ) {

      // Execute callback
      if( ( uart == UART_GPIO ) && (txIsrCallbackUART0 != 0) )
         (*txIsrCallbackUART0)(txIsrCallbackUART0Params);

      if( ( uart == UART_USB )  && (txIsrCallbackUART2 != 0) )
         (*txIsrCallbackUART2)(txIsrCallbackUART2Params);

      if( ( uart == UART_232 )  && (txIsrCallbackUART3 != 0) )
         (*txIsrCallbackUART3)(txIsrCallbackUART3Params);
   }
}
#endif /* SAPI_USE_INTERRUPTS */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// #define UART_IER_RBRINT      (1 << 0)	/*!< RBR Interrupt enable */
// #define UART_IER_THREINT     (1 << 1)	/*!< THR Interrupt enable */
// #define UART_IER_RLSINT      (1 << 2)	/*!< RX line status interrupt enable */
// #define UART_IER_MSINT       (1 << 3)	/*!< Modem status interrupt enable - valid for 11xx, 17xx/40xx UART1, 18xx/43xx UART1  only */
// #define UART_IER_CTSINT      (1 << 7)	/*!< CTS signal transition interrupt enable - valid for 17xx/40xx UART1, 18xx/43xx UART1 only */
// #define UART_IER_ABEOINT     (1 << 8)	/*!< Enables the end of auto-baud interrupt */
// #define UART_IER_ABTOINT     (1 << 9)	/*!< Enables the auto-baud time-out interrupt */
// #define UART_IER_BITMASK     (0x307)	/*!< UART interrupt enable register bit mask  - valid for 13xx, 17xx/40xx UART0/2/3, 18xx/43xx UART0/2/3 only*/
// #define UART1_IER_BITMASK    (0x30F)	/*!< UART1 interrupt enable register bit mask - valid for 11xx only */
// #define UART2_IER_BITMASK    (0x38F)	/*!< UART2 interrupt enable register bit mask - valid for 17xx/40xx UART1, 18xx/43xx UART1 only */


#ifdef SAPI_USE_INTERRUPTS

// UART Global Interrupt Enable/Disable
void uartInterrupt( uartMap_t uart, bool_t enable )
{
   if( enable ) {
      // Interrupt Priority for UART channel
      NVIC_SetPriority( lpcUarts[uart].uartIrqAddr, 5 ); // FreeRTOS Requiere prioridad >= 5 (numero mas alto, mas baja prioridad)
      // Enable Interrupt for UART channel
      NVIC_EnableIRQ( lpcUarts[uart].uartIrqAddr );
   } else {
      // Disable Interrupt for UART channel
      NVIC_DisableIRQ( lpcUarts[uart].uartIrqAddr );
   }
}

// UART Interrupt event Enable and set a callback
void uartCallbackSet( uartMap_t uart, uartEvents_t event, 
                      callBackFuncPtr_t callbackFunc, void* callbackParam )
{   
   uint32_t intMask;

   switch(event){

      case UART_RECEIVE:
         // Enable UART Receiver Buffer Register Interrupt
         //intMask = UART_IER_RBRINT;
         
         // Enable UART Receiver Buffer Register Interrupt and Enable UART line
         //status interrupt. LPC43xx User manual page 1118
         intMask = UART_IER_RBRINT | UART_IER_RLSINT;
         
         if( callbackFunc != 0 ) {
            // Set callback
            if( (uart == UART_GPIO) || (uart == UART_485) ){
               rxIsrCallbackUART0 = callbackFunc;
               rxIsrCallbackUART0Params = callbackParam;
            }
            if( (uart == UART_USB) || (uart == UART_ENET) ){
               rxIsrCallbackUART2 = callbackFunc;
               rxIsrCallbackUART2Params = callbackParam;
            }            
            if( uart == UART_232 ){
               rxIsrCallbackUART3 = callbackFunc;
               rxIsrCallbackUART3Params = callbackParam;
            }
         } else{
            return;
         }
      break;

      case UART_TRANSMITER_FREE:
         // Enable THRE irq (TX)
         intMask = UART_IER_THREINT;

         if( callbackFunc != 0 ) {
            
            // Set callback
            if( (uart == UART_GPIO) || (uart == UART_485) ){
               txIsrCallbackUART0 = callbackFunc;
               txIsrCallbackUART0Params = callbackParam;
            }
            if( (uart == UART_USB) || (uart == UART_ENET) ){
               txIsrCallbackUART2 = callbackFunc;
               txIsrCallbackUART2Params = callbackParam;
            }            
            if( uart == UART_232 ){
               txIsrCallbackUART3 = callbackFunc;
               txIsrCallbackUART3Params = callbackParam;
            }
         } else{
            return;
         }
      break;

      default:
         return;
   }

   // Enable UART Interrupt
   Chip_UART_IntEnable(lpcUarts[uart].uartAddr, intMask);
}
                 
// UART Interrupt event Disable
void uartCallbackClr( uartMap_t uart, uartEvents_t event )
{
   uint32_t intMask;

   switch(event){

      case UART_RECEIVE:
         // Enable UART Receiver Buffer Register Interrupt
         //intMask = UART_IER_RBRINT;
         
         // Enable UART Receiver Buffer Register Interrupt and Enable UART line
         //status interrupt. LPC43xx User manual page 1118
         intMask = UART_IER_RBRINT | UART_IER_RLSINT;
      break;
      
      case UART_TRANSMITER_FREE:
         // Enable THRE irq (TX)
         intMask = UART_IER_THREINT;
      break;
      
      default:
         return;
   }

   // Disable UART Interrupt
   Chip_UART_IntDisable(lpcUarts[uart].uartAddr, intMask);
}
 
// UART Set Pending Interrupt. Useful to force first character in tx transmission
void uartSetPendingInterrupt(uartMap_t uart) {
   NVIC_SetPendingIRQ(lpcUarts[uart].uartIrqAddr);
}

// UART Clear Pending Interrupt.
void uartClearPendingInterrupt(uartMap_t uart) {
   NVIC_ClearPendingIRQ(lpcUarts[uart].uartIrqAddr);
} 


#endif /* SAPI_USE_INTERRUPTS */

// disable tx and rx interrupt
//Chip_UART_IntDisable( LPC_USART2,
//                      UART_IER_THREINT |
//                      UART_IER_RBRINT );

//case STARTTX:
// disable THRE irq (TX)
//Chip_UART_IntDisable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
// this one calls write
//ciaaDriverUart_txConfirmation(device);
// enable THRE irq (TX)
//Chip_UART_IntEnable((LPC_USART_T *)device->loLayer, UART_IER_THREINT);
//break;
//case SET_FIFO_TRIGGER_LEVEL:
//Chip_UART_SetupFIFOS((LPC_USART_T *)device->loLayer,  UART_FCR_FIFO_EN | UART_FCR_TX_RS | UART_FCR_RX_RS | (int32_t)param);
//break;

//#define UART_RX_FIFO_SIZE       (16)


//-------------------------------------------------------------

// Return TRUE if have unread data in RX FIFO
bool_t uartRxReady( uartMap_t uart )
{
   return Chip_UART_ReadLineStatus( lpcUarts[uart].uartAddr ) & UART_LSR_RDR;
}

// Return TRUE if have space in TX FIFO
bool_t uartTxReady( uartMap_t uart )
{
   return Chip_UART_ReadLineStatus( lpcUarts[uart].uartAddr ) & UART_LSR_THRE;
}

// Read from RX FIFO
uint8_t uartRxRead( uartMap_t uart )
{
   return Chip_UART_ReadByte( lpcUarts[uart].uartAddr );
}

// Read all bytes from RX FIFO and discards
void uartRxFlush( uartMap_t uart )
{
   while ( uartRxReady(uart) ) {
      uartRxRead(uart);
   }
}

// Write in TX FIFO
void uartTxWrite( uartMap_t uart, const uint8_t value )
{
   Chip_UART_SendByte( lpcUarts[uart].uartAddr, value );
}

//-------------------------------------------------------------

// UART Initialization
void uartInit( uartMap_t uart, uint32_t baudRate )
{
   // Initialize UART
   Chip_UART_Init( lpcUarts[uart].uartAddr );
   // Set Baud rate
   Chip_UART_SetBaud( lpcUarts[uart].uartAddr, baudRate );
   
   //Chip_UART_ConfigData(LPC_UART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
   
   // Restart FIFOS using FCR (FIFO Control Register).
   // Set Enable, Reset content, set trigger level
   Chip_UART_SetupFIFOS( lpcUarts[uart].uartAddr,
                         UART_FCR_FIFO_EN |
                         UART_FCR_TX_RS   |
                         UART_FCR_RX_RS   |
                         UART_FCR_TRG_LEV0 );
	/*Chip_UART_SetupFIFOS(lpcUarts[uart].uartAddr, 
                          (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));*/
   
   // Dummy read
   Chip_UART_ReadByte( lpcUarts[uart].uartAddr );
   
   // Enable UART Transmission
   Chip_UART_TXEnable( lpcUarts[uart].uartAddr );
   
   // Configure SCU UARTn_TXD pin
   Chip_SCU_PinMux( lpcUarts[uart].txPin.lpcScuPort,
                    lpcUarts[uart].txPin.lpcScuPin,
                    MD_PDN,
                    lpcUarts[uart].txPin.lpcScuFunc );
                    
   // Configure SCU UARTn_RXD pin
   Chip_SCU_PinMux( lpcUarts[uart].rxPin.lpcScuPort,
                    lpcUarts[uart].rxPin.lpcScuPin,
                    MD_PLN | MD_EZI | MD_ZI,
                    lpcUarts[uart].rxPin.lpcScuFunc );

   // Specific configurations for RS485
   if( uart == UART_485 ) {
      // Specific RS485 Flags
      Chip_UART_SetRS485Flags( LPC_USART0,
                               UART_RS485CTRL_DCTRL_EN |
                               UART_RS485CTRL_OINV_1     );
      // UARTn_DIR extra pin for RS485
      Chip_SCU_PinMux( lpcUart485DirPin.lpcScuPort,
                       lpcUart485DirPin.lpcScuPin,
                       MD_PDN,
                       lpcUart485DirPin.lpcScuFunc );
   }
}

// UART Initialization 2
void uartInit2( uartMap_t uart, uint32_t baudRate, 
                uint8_t dataBits, uint8_t parity, uint8_t stopBits )
{
   // Initialize UART
   Chip_UART_Init( lpcUarts[uart].uartAddr );
   // Set dataBits, stopBits and parity
   
   uint32_t config = 0;
   config = (dataBits-5) | ((stopBits-1) << 2);
   if( parity != UART_PARITY_NONE ){
      config |= UART_LCR_PARITY_EN;
      if( parity == UART_PARITY_EVEN ){
         config |= UART_LCR_PARITY_EVEN;
      }
      if( parity == UART_PARITY_ODD ){
         config |= UART_LCR_PARITY_ODD;
      }
   } else{
      config |= UART_LCR_PARITY_DIS;
   }   
   // example: config = UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_EN | UART_LCR_PARITY_EVEN;
   Chip_UART_ConfigData( lpcUarts[uart].uartAddr, config );

   // Set Baud rate
   Chip_UART_SetBaud( lpcUarts[uart].uartAddr, baudRate );
   // Restart FIFOS using FCR (FIFO Control Register).
   // Set Enable, Reset content, set trigger level
   Chip_UART_SetupFIFOS( lpcUarts[uart].uartAddr,
                         UART_FCR_FIFO_EN |
                         UART_FCR_TX_RS   |
                         UART_FCR_RX_RS   |
                         UART_FCR_TRG_LEV0 );
   // Dummy read
   Chip_UART_ReadByte( lpcUarts[uart].uartAddr );
   // Enable UART Transmission
   Chip_UART_TXEnable( lpcUarts[uart].uartAddr );
   // Configure SCU UARTn_TXD pin
   Chip_SCU_PinMux( lpcUarts[uart].txPin.lpcScuPort,
                    lpcUarts[uart].txPin.lpcScuPin,
                    MD_PDN,
                    lpcUarts[uart].txPin.lpcScuFunc );
   // Configure SCU UARTn_RXD pin
   Chip_SCU_PinMux( lpcUarts[uart].rxPin.lpcScuPort,
                    lpcUarts[uart].rxPin.lpcScuPin,
                    MD_PLN | MD_EZI | MD_ZI,
                    lpcUarts[uart].rxPin.lpcScuFunc );

   // Specific configurations for RS485
   if( uart == UART_485 ) {
      // Specific RS485 Flags
      Chip_UART_SetRS485Flags( LPC_USART0,
                               UART_RS485CTRL_DCTRL_EN |
                               UART_RS485CTRL_OINV_1     );
      // UARTn_DIR extra pin for RS485
      Chip_SCU_PinMux( lpcUart485DirPin.lpcScuPort,
                       lpcUart485DirPin.lpcScuPin,
                       MD_PDN,
                       lpcUart485DirPin.lpcScuFunc );
   }
}

// Read 1 byte from RX FIFO, check first if exist aviable data
bool_t uartReadByte( uartMap_t uart, uint8_t* receivedByte )
{
   bool_t retVal = TRUE;
   if ( uartRxReady(uart) ) {
      *receivedByte = uartRxRead(uart);
   } else {
      retVal = FALSE;
   }
   return retVal;
}

// Blocking Write 1 byte to TX FIFO
void uartWriteByte( uartMap_t uart, const uint8_t value )
{
   // Wait for space in FIFO (blocking)
   while( uartTxReady( uart ) == FALSE );
   // Send byte
   uartTxWrite( uart, value );
}

// Blocking Send a string
void uartWriteString( uartMap_t uart, const char* str )
{
   while( *str != 0 ) {
      uartWriteByte( uart, (uint8_t)*str );
      str++;
   }
}

// Blocking, Send a Byte Array
void uartWriteByteArray( uartMap_t uart,
                         const uint8_t* byteArray, uint32_t byteArrayLen )
{
   uint32_t i = 0;
   for( i=0; i<byteArrayLen; i++ ) {
      uartWriteByte( uart, byteArray[i] );
   }
}

/*==================[ISR external functions definition]======================*/

#ifdef SAPI_USE_INTERRUPTS

__attribute__ ((section(".after_vectors")))

// UART0 (GPIO1 y GPIO2 or RS485/Profibus)
// 0x28 0x000000A0 - Handler for ISR UART0 (IRQ 24)
void UART0_IRQHandler(void)
{
   uartProcessIRQ( UART_GPIO );
}

// UART2 (USB-UART) or UART_ENET
// 0x2a 0x000000A8 - Handler for ISR UART2 (IRQ 26)
void UART2_IRQHandler(void)
{
   uartProcessIRQ( UART_USB );
}

// UART3 (RS232)
// 0x2b 0x000000AC - Handler for ISR UART3 (IRQ 27)
void UART3_IRQHandler(void)
{
   uartProcessIRQ( UART_232 );
}
#endif /* SAPI_USE_INTERRUPTS */

/*==================[end of file]============================================*/