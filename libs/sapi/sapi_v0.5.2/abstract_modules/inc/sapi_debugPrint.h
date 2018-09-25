/* Copyright 2017, Eric Pernia.
 * All rights reserved.
 *
 * Author: Eng. Eric Pernia.
 * Date: 2017-04-17
 */

#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

/*==================[inclusions]=============================================*/

#include "sapi_print.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define DEBUG_PRINT   ON

#define debugPrintConfigUart debugPrintInitUart

/*
API
---

// Initialize
void debugPrintInit( uartMap_t uart, uint32_t baudRate );
void debugPrintSetUart( uartMap_t uart );

// Print String
void debugPrintString( const char* string );
void debugPrintlnString( const char* string );
void debugPrintEnter( void );

// Print Integer
void debugPrintIntFormat( int64_t number, numberFormat_t format );
void debugPrintUIntFormat( uint64_t number, numberFormat_t format );
void debugPrintlnIntFormat( int64_t number, numberFormat_t format );
void debugPrintlnUIntFormat( uint64_t number, numberFormat_t format );
void debugPrintInt( int64_t number );
void debugPrintUInt( uint64_t number );
void debugPrintlnInt( int64_t number );
void debugPrintlnUInt( uint64_t number );
void debugPrintHex( uint64_t number, uint8_t bitSize );
void debugPrintlnHex( uint64_t number, uint8_t bitSize );
*/

// Initialize

#if( DEBUG_PRINT == ON )

// Initialize
#define DEBUG_PRINT_ENABLE                      static print_t debugPrint;
#define debugPrintSetUart(uart)                 printSetUart(&(debugPrint),(uart))
#define debugPrintInitUart(uart,baudRate)       printInitUart(&(debugPrint),(uart),(baudRate))

// Print String
#define debugPrintString(string)                printString((debugPrint),(string))
#define debugPrintEnter()                       printEnter(debugPrint)
#define debugPrintlnString(string)              printlnString((debugPrint),(string))

// Print Integer
#define debugPrintIntFormat(number,format)      printIntFormat((debugPrint),(number),(format))
#define debugPrintUIntFormat(number,format)     printUIntFormat((debugPrint),(number),(format))
#define debugPrintlnIntFormat(number,format)    printlnIntFormat((debugPrint),(number),(format))
#define debugPrintlnUIntFormat(number,format)   printlnUIntFormat((debugPrint),(number),(format))
#define debugPrintInt(number)                   printInt((debugPrint),(number))
#define debugPrintUInt(number)                  printUInt((debugPrint),(number))
#define debugPrintlnInt(number)                 printlnInt((debugPrint),(number))
#define debugPrintlnUInt(number)                printlnUInt((debugPrint),(number))
#define debugPrintHex(number,bitSize)           printHex((debugPrint),(number),(bitSize))
#define debugPrintlnHex(number,bitSize)         printlnHex((debugPrint),(number),(bitSize))

#else

// Initialize
#define DEBUG_PRINT_ENABLE                      (void)
#define debugPrintSetUart(uart)                 do { (void) (uart); (void) (baudRate); } while(0)
#define debugPrintConfigUart(uart,baudRate)     do { (void) (uart); (void) (baudRate); } while(0)

// Print String
#define debugPrintString(string)                do { (void) (string); } while(0)
#define debugPrintEnter()                       (void)
#define debugPrintlnString(string)              do { (void) (string); } while(0)

// Print Integer
#define debugPrintIntFormat(number,format)      do { (void) (number); (void) (format); } while(0)
#define debugPrintUIntFormat(number,format)     do { (void) (number); (void) (format); } while(0)
#define debugPrintlnIntFormat(number,format)    do { (void) (number); (void) (format); } while(0)
#define debugPrintlnUIntFormat(number,format)   do { (void) (number); (void) (format); } while(0)
#define debugPrintInt(number)                   do { (void) (number); } while(0)
#define debugPrintUInt(number)                  do { (void) (number); } while(0)
#define debugPrintlnInt(number)                 do { (void) (number); } while(0)
#define debugPrintlnUInt(number)                do { (void) (number); } while(0)
#define debugPrintHex(number,bitSize)           do { (void) (number); (void) (bitSize); } while(0)
#define debugPrintlnHex(number,bitSize)         do { (void) (number); (void) (bitSize); } while(0)
#endif


/*
#define MACRO(num, str) {\
            printf("%d", num);\
            printf(" is");\
            printf(" %s number", str);\
            printf("\n");\
           }
*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[examples]===============================================*/

/*
debugPrintLnString( "Uart Debug configurada." );
debugPrintLnString( "Uart LoRa RN2903 configurada." );

debugPrintString( "Numero de algo: " );
debugPrintInt( 45454578 );
debugPrintLnString( "" );

debugPrintLnInt( -457 );
debugPrintLnUInt( -457 );

debugPrintString( "Numero de algo: " );
debugPrintIntFormat( 45454578, DEC_FORMAT );
debugPrintLnString( "" );

debugPrintString( "Numero de algo: 0b" );
debugPrintIntFormat( 45454578, BIN_FORMAT );
debugPrintLnString( "" );

debugPrintString( "Numero de algo: 0x" );
debugPrintIntFormat( 45454578, HEX_FORMAT );
debugPrintLnString( "" );
*/

/*==================[end of file]============================================*/
#endif /* #ifndef _DEBUG_PRINT_H_ */
