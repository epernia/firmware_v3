/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
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

/*==================[macros]=================================================*/

#define UART_485_LPC LPC_USART0  /* UART0 (RS485/Profibus) */
#define UART_USB_LPC LPC_USART2  /* UART2 (USB-UART) */
#define UART_232_LPC LPC_USART3  /* UART3 (RS232) */

/*==================[typedef]================================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal functions definition]==========================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

waitForReceiveStringOrTimeoutState_t waitForReceiveStringOrTimeout(
   uartMap_t uart, waitForReceiveStringOrTimeout_t* instance ){

   uint8_t receiveByte;

   switch( instance->state ){

      case UART_RECEIVE_STRING_CONFIG:

         delayConfig( &(instance->delay), instance->timeout );

         instance->stringIndex = 0;

         instance->state = UART_RECEIVE_STRING_RECEIVING;

      break;

      case UART_RECEIVE_STRING_RECEIVING:

         if( uartReadByte( uart, &receiveByte ) ){

            // TODO: DEBUG
            uartWriteByte( UART_USB, receiveByte );

            if( (instance->string)[(instance->stringIndex)] == receiveByte ){

               (instance->stringIndex)++;

               if( (instance->stringIndex) == (instance->stringSize - 1) ){
                  instance->state = UART_RECEIVE_STRING_RECEIVED_OK;
               }

            }

         }

         if( delayRead( &(instance->delay) ) ){
            instance->state = UART_RECEIVE_STRING_TIMEOUT;
         }

      break;

      case UART_RECEIVE_STRING_RECEIVED_OK:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      case UART_RECEIVE_STRING_TIMEOUT:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;

      default:
         instance->state = UART_RECEIVE_STRING_CONFIG;
      break;
   }

   return instance->state;
}



bool_t waitForReceiveStringOrTimeoutBlocking(
   uartMap_t uart, char* string, uint16_t stringSize, tick_t timeout ){

   bool_t retVal = TRUE; // True if OK

   waitForReceiveStringOrTimeout_t waitText;
   waitForReceiveStringOrTimeoutState_t waitTextState;

   waitTextState = UART_RECEIVE_STRING_CONFIG;

   waitText.state = UART_RECEIVE_STRING_CONFIG;
   waitText.string =  string;
   waitText.stringSize = stringSize;
   waitText.timeout = timeout;

   while( waitTextState != UART_RECEIVE_STRING_RECEIVED_OK &&
          waitTextState != UART_RECEIVE_STRING_TIMEOUT ){
      waitTextState = waitForReceiveStringOrTimeout( uart, &waitText );
   }

   if( waitTextState == UART_RECEIVE_STRING_TIMEOUT ){
      retVal = FALSE;
   }

   return retVal;
}


void uartConfig( uartMap_t uart, uint32_t baudRate ){
   switch(uart){
   case UART_USB:
      Chip_UART_Init(UART_USB_LPC);
      Chip_UART_SetBaud(UART_USB_LPC, baudRate);  /* Set Baud rate */
      Chip_UART_SetupFIFOS(UART_USB_LPC, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0); /* Modify FCR (FIFO Control Register)*/
      Chip_UART_TXEnable(UART_USB_LPC); /* Enable UART Transmission */
      Chip_SCU_PinMux(7, 1, MD_PDN, FUNC6);              /* P7_1,FUNC6: UART2_TXD */
      Chip_SCU_PinMux(7, 2, MD_PLN|MD_EZI|MD_ZI, FUNC6); /* P7_2,FUNC6: UART2_RXD */

      //Enable UART Rx Interrupt
      //   Chip_UART_IntEnable(UART_USB_LPC,UART_IER_RBRINT );   //Receiver Buffer Register Interrupt
      // Enable UART line status interrupt
      //   Chip_UART_IntEnable(UART_USB_LPC,UART_IER_RLSINT ); //LPC43xx User manual page 1118
      //   NVIC_SetPriority(USART2_IRQn, 6);
      // Enable Interrupt for UART channel
      //   NVIC_EnableIRQ(USART2_IRQn);
   break;
   case UART_232:
      Chip_UART_Init(UART_232_LPC);
      Chip_UART_SetBaud(UART_232_LPC, baudRate);  /* Set Baud rate */
      Chip_UART_SetupFIFOS(UART_232_LPC, UART_FCR_FIFO_EN | UART_FCR_TRG_LEV0); /* Modify FCR (FIFO Control Register)*/
      Chip_UART_TXEnable(UART_232_LPC); /* Enable UART Transmission */
      Chip_SCU_PinMux(2, 3, MD_PDN, FUNC2);              /* P2_3,FUNC2: UART3_TXD */
      Chip_SCU_PinMux(2, 4, MD_PLN|MD_EZI|MD_ZI, FUNC2); /* P2_4,FUNC2: UART3_RXD */
   break;
   case UART_485:

   break;
   }
}


bool_t uartReadByte( uartMap_t uart, uint8_t* receivedByte ){

   bool_t retVal = TRUE;

   switch(uart){
   case UART_USB:
      if ( Chip_UART_ReadLineStatus(UART_USB_LPC) & UART_LSR_RDR ) {
         *receivedByte = Chip_UART_ReadByte(UART_USB_LPC);
      } else{
         retVal = FALSE;
      }
   break;
   case UART_232:
      if ( Chip_UART_ReadLineStatus(UART_232_LPC) & UART_LSR_RDR ) {
         *receivedByte = Chip_UART_ReadByte(UART_232_LPC);
      } else{
         retVal = FALSE;
      }
   break;
   case UART_485:
   break;
   }

   return retVal;
}


void uartWriteByte( uartMap_t uart, uint8_t byte ){

   switch(uart){
   case UART_USB:
      while ((Chip_UART_ReadLineStatus(UART_USB_LPC) & UART_LSR_THRE) == 0) {}   // Wait for space in FIFO
      Chip_UART_SendByte(UART_USB_LPC, byte);
   break;
   case UART_232:
      while ((Chip_UART_ReadLineStatus(UART_232_LPC) & UART_LSR_THRE) == 0) {}   // Wait for space in FIFO
      Chip_UART_SendByte(UART_232_LPC, byte);
   break;
   case UART_485:
   break;
   }
}


void uartWriteString( uartMap_t uart, char* str ){
   while(*str != 0){
	  uartWriteByte( uart, (uint8_t)*str );
	  str++;
   }
}

/*==================[ISR external functions definition]======================*/

__attribute__ ((section(".after_vectors")))

/* 0x28 0x000000A0 - Handler for ISR UART0 (IRQ 24) */
void UART0_IRQHandler(void){
}

/* 0x2a 0x000000A8 - Handler for ISR UART2 (IRQ 26) */
void UART2_IRQHandler(void){
}

/* 0x2b 0x000000AC - Handler for ISR UART3 (IRQ 27) */
void UART3_IRQHandler(void){
   //if (Chip_UART_ReadLineStatus(UART_232) & UART_LSR_RDR) {
//      receivedByte = Chip_UART_ReadByte(UART_232);
   //}
}

/*==================[end of file]============================================*/
