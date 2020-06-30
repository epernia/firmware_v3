/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/05/03
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

typedef enum{
   UART_MODO_NORMAL,
   UART_MODO_ECHO,
   UART_MODO_ASCII,
   UART_MODO_CASE,
}fsmUartState_t;

fsmUartState_t fsmUartState; // Variable de estado

void fsmUartInit( void );
void fsmUartUpdate( void );

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   fsmUartInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      fsmUartUpdate();
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}


void fsmUartInit( void ){
   boardInit();
   fsmUartState = UART_MODO_NORMAL;
   uartWriteString( UART_USB, "UART_MODO_NORMAL\r\n" );
}

void fsmUartUpdate( void ){

   uint8_t data = 0;

   switch( fsmUartState ){

      case UART_MODO_NORMAL:
         if(  uartReadByte( UART_USB, &data ) ){
            if( data == '3' ){
               fsmUartState = UART_MODO_ECHO;
               uartWriteString( UART_USB, "UART_MODO_ECHO\r\n" );
            }
            if( data == '4' ){
               fsmUartState = UART_MODO_ASCII;
               uartWriteString( UART_USB, "UART_MODO_ASCII\r\n" );
            }
            if( data == '5' ){
               fsmUartState = UART_MODO_CASE;
               uartWriteString( UART_USB, "UART_MODO_CASE\r\n" );
            }
         }
      break;

      case UART_MODO_ECHO:
         if(  uartReadByte( UART_USB, &data ) ){
            if( data == '3' ){
               fsmUartState = UART_MODO_NORMAL;
               uartWriteString( UART_USB, "UART_MODO_NORMAL\r\n" );
            } else {
               uartWriteByte( UART_USB, data );
            }
         }
      break;

      case UART_MODO_ASCII:
         if(  uartReadByte( UART_USB, &data ) ){
            if( data == '4' ){
               fsmUartState = UART_MODO_NORMAL;
               uartWriteString( UART_USB, "UART_MODO_NORMAL\r\n" );
            } else {
               uartWriteString( UART_USB, intToStringGlobal(data) );
               uartWriteString( UART_USB, "\r\n" );
            }
         }
      break;

      case UART_MODO_CASE:
         if(  uartReadByte( UART_USB, &data ) ){
            if( data == '5' ){
               fsmUartState = UART_MODO_NORMAL;
               uartWriteString( UART_USB, "UART_MODO_NORMAL\r\n" );
            } else {
               if( (data >= 'a') && (data <= 'z') ){
                  // Convertir a mayuscula. 'a'-'A' == 32
                  uartWriteByte( UART_USB, data - ('a'-'A') ); 
               } else{
                  if( (data >= 'A') && (data <= 'Z') ){
                     // Convertir a minuscula
                     uartWriteByte( UART_USB, data + ('a'-'A') ); 
                  }
                  else{
                     uartWriteByte( UART_USB, data ); 
                  }
               }
            }
         }

         // Recordar que:
         //    'a' != "a"
         //    "a" == { 'a', '\0' }
         //    "hola" == { 'h', 'o', 'l', 'a', 0 }
         
      break;

      default:
         fsmUartState = UART_MODO_NORMAL;
         uartWriteString( UART_USB, "Hubo un error, volviendo a MODO_NORMAL\r\n" );
      break;
   }
}


/*
#include <stdio.h>
int main()
{
    char vec[20];
    scanf( "%s", vec );
    printf( "%s\r\n", vec );
    return 0;
}
*/
