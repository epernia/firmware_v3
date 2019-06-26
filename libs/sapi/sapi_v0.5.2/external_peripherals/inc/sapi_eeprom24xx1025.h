/* Copyright 2017, Eric Pernia.
 */

/* Date: 2017-07-27 */

/*
Microchip 24xx1025 I2C EEPROM
-----------------------------

http://ww1.microchip.com/downloads/en/DeviceDoc/20001941L.pdf

The Microchip Technology Inc. 24AA1025/24LC1025/24FC1025 (24XX1025*) is a
128K x 8 (1024K bit) Serial Electrically Erasable PROM, capable of operation
across a broad voltage range (1.7V to 5.5V).
It has been developed for advanced, low-power applications such as personal
communications or data acquisition. This device has both byte write and page
write capability of up to 128 bytes of data.

- 128K x 8 (1024 Kbit) 
- 128-Byte Page Write Buffer 
- Page Write Time 5 ms Max.
- Hardware Write-Protect Pin
- Factory Programming Available
- Operating voltage 1.7V to 5.5V 
- 2-Wire Serial Interface, I2C Compatible
-  - Cascadable up to Four Devices
-  - 100 kHz and 400 kHz Clock Compatible 

I2C address:

The control byte consists of a 4-bit control code; for the24XX1025, this is set
as  ?1010?  binary  for  read  andwrite operations. The next bit of the control
byte is theblock select bit (B0). This bit acts as the A16 addressbit for
accessing the entire array. The next two bits ofthe control byte are the Chip
Select bits (A1, A0). TheChip Select bits allow the use of up to four 24XX1025
devices on the same bus and are used to select whichdevice is accessed. 
The Chip Select bits in the controlbyte must correspond to the logic levels on
thecorresponding A1 and A0 pins for the device torespond. These bits are in
effect the two Most Significant bits (MSb) of the word address.

==> I2C Address = 0b 1010 B0 A1 A0 

This device has an internal addressing boundary limitation that is divided into
two segments of 512K bits.Block select bit ?B0? to control access to each
segment.
    
Each device has internal addressing boundarylimitations. This divides each part
into two segments of512K bits. The block select bit ?B0? controls access toeach
?half?.Sequential read operations are limited to 512K blocks. To read through
four devices on the same bus, eightrandom Read commands must be given.

Send:  
                           A15    A8 A7     A0
   controlByte(I2Caddress) AddressHi AddressLo
   
A0 of the control byte asaddress bit A17 and A1 as address bit A18.
B0 acts as the A16 address

When doing a write of less than 128 bytes the data in the rest of the page
isrefreshed along with the data bytes beingwritten. This will force the entire
page to endure a write cycle, for this reasonendurance is specified per page

Page write operations are limited to writ-ing bytes within a single physical
page,regardless of the number of bytes actuallybeing written.

// CIAA-NXP ------------------------------------------------------------

I2C Address = 0b 1010 B0  0  0  (A1 y A0 = 0 en CIAA-NXP)

   I2C Address = 0x50 (B0 = 0)
   I2C Address = 0x54 (B0 = 1)
    
No se puede controlar el pin WP en CIAA-NXP, está a 0 (deshabilitado)

*/   

#ifndef _SAPI_EEPROM_24XX1025_H_
#define _SAPI_EEPROM_24XX1025_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"              /* <= sAPI header */

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// EEPROM24xx1025 total memory size in bytes
#define EEPROM_1024_K_BIT              1024/8 // Memory size [bytes]

// EEPROM24xx1025 single page size (in bytes)
#define EEPROM24xx1025_PAGE_SIZE       128   // [bytes per page]

// EEPROM24xx1025 I2C address
#define EEPROM24xx1025_CTRL_CODE       0xA  // 0b1010 

// Function aliases
#define eeprom24xx1025ReadByte         eeprom24xx1025ReadRandom

// Function utilities
#define EEPROM_ADDRESS_HIGH(address)   ( (uint8_t)((address&(0xFFFFFF00))>>8) )
#define EEPROM_ADDRESS_LOW(address)    ( (uint8_t)( address&(0x000000FF)) )

// ----------------------------------------------------------------------------

// EEPROM24xx1025 total memory size in bytes
// #define EEPROM24xx1025_MEMORY_SIZE   64000 // 64 KBytes (0xFA00, 1025 Kbits)
#define EEPROM24xx1025_LAST_MEMORY_ADDRESS   0xFFFF
#define EEPROM24xx1025_MEMORY_SIZE           (EEPROM24xx1025_LAST_MEMORY_ADDRESS  + 1) // 64 KBytes (1025 Kbits)

// EEPROM24xx1025_MEMORY_SIZE / EEPROM24xx1025_PAGE_SIZE
#define EEPROM24xx1025_PAGE_AMOUNT           EEPROM24xx1025_MEMORY_SIZE / EEPROM24xx1025_PAGE_SIZE

/*==================[typedef]================================================*/

typedef struct{
   // I2C port connected to EEPROM, example I2C0
   int32_t i2c;
   // Use this if fixed address
   bool_t A0;          // EEPROM I2C addres, depends of A0 and A1 pins status and B0 value
   bool_t A1;          // EEPROM I2C addres, depends of A0 and A1 pins status and B0 value
   // Use this if controlling address from MCU
   int32_t gpioA0;     // GPIO conected to A0 of EEPROM
   int32_t gpioA1;     // GPIO conected to A1 of EEPROM  
   // Use this if controlling EEPROM power from MCU
   int32_t gpioPower;  // GPIO to manage power of EEPROM
   // Use this if controlling WP pin from MCU
   int32_t gpioWP;     // GPIO conected to Write Proyection Pin of EEPROM
   // EEPROM capacity
   int32_t pageSize;   // EEPROM page size [bytes]
   int32_t memorySize; // EEPROM total memory size [bytes]
} Eeprom24xx1025_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

//-----------------------------------------------------------------------------
// MANAGEMENT
//-----------------------------------------------------------------------------

uint8_t eeprom24xx1025I2cAddress( Eeprom24xx1025_t* eeprom, uint32_t address );

// Enable or disable the peripheral energy and clock
bool_t eeprom24xx1025PowerSet( Eeprom24xx1025_t* eeprom, bool_t power );

//-----------------------------------------------------------------------------
// INITIALIZATION
//-----------------------------------------------------------------------------

bool_t eeprom24xx1025Init( Eeprom24xx1025_t* eeprom,
                           int32_t i2c, bool_t A0, bool_t A1,
                           int32_t gpioA0, int32_t gpioA1, 
                           int32_t gpioPower, int32_t gpioWP,
                           int32_t pageSize, int32_t memorySize );

//-----------------------------------------------------------------------------
// WRITE OPERATIONS
//-----------------------------------------------------------------------------

// Byte Write
bool_t eeprom24xx1025WriteByte( Eeprom24xx1025_t* eeprom, 
                                uint32_t memoryAddress, uint8_t byteToWrite );

// Page Write
bool_t eeprom24xx1025WritePage( Eeprom24xx1025_t* eeprom, uint32_t pageAddress,
                                uint8_t* byteBuffer, uint32_t byteBufferSize );

//-----------------------------------------------------------------------------
// READ OPERATIONS
//-----------------------------------------------------------------------------

// Random Read
bool_t eeprom24xx1025ReadRandom( Eeprom24xx1025_t* eeprom, 
                                 uint32_t memoryAddress, uint8_t* readedByte );

//-----------------------------------------------------------------------------
// EEPROM Test
//-----------------------------------------------------------------------------
/**
 * EEPROM Test
 * Performs a test on the 24xx1025 EEPROM memory.
 * @param uint8_t startingPage. The page where to start the test.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint8_t. The size of buffer (must be lower than page size).
 * @return bool_t. Whether the test was passed or not.
 * TODO: Implement this test with byte check
 */
bool_t eeprom24xx1025Test( uint32_t startingPage, uint8_t* buff, uint8_t buffSize );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[example]================================================*/

/*
#include "sapi.h"
// MAIN FUNCTION, ENTRY POINT TO THE PROGRAM AFTER POR OR RESET.
int main( void ){

   // ---------- SETUP ----------------------------------------
   // Initialize and configure board
   boardInit();

   CONSOLE_PRINT_ENABLE

   consolePrintConfigUart( UART_DEBUG, 9600 );
   consolePrintlnString( "Console UART printer configurated." );

   // Incicializar I2C0
   i2cConfig( I2C0, 100000 );

   // Initialize EEPROM
   eeprom24xx1025Init();

   uint8_t myByte = 0xEE;
   uint16_t address = 0x0000;

   address = 0xCAFE;
   consolePrintString( "Memory address example: 0x" );
   consolePrintHex( address, 16 );
   consolePrintlnString( "." );

   // Memory address High
   consolePrintString( "Memory address High: 0x" );
   consolePrintHex( EEPROM_ADDRESS_HIGH(address), 16 );
   consolePrintlnString( "." );

   // Memory address Low
   consolePrintString( "Memory address Low: 0x" );
   consolePrintHex( EEPROM_ADDRESS_LOW(address), 16 );
   consolePrintlnString( "." );

   consolePrintEnter();

   //
   for( address = 0x00FE; address <= 0x01FD; address++ ){
      // Byte Write
      if( eeprom24xx1025WriteByte( address, myByte ) ){
         consolePrintString( "Se escribio: 0x" );
         consolePrintHex( myByte, 8 );
         consolePrintString( ", en la direccion: 0x" );
         consolePrintHex( address, 16 );
         consolePrintlnString( "." );
      } else{
         consolePrintlnString( "No se pudo escribir EEPROM." );
      }
      myByte++;
   }

   consolePrintEnter();

   for( address = 0x00FE; address <= 0x01FD; address++ ){
      // Random Read (Byte read)
      myByte = 0;
      if( eeprom24xx1025ReadRandom( address, &myByte ) ){
         consolePrintString( "Se leyo: 0x" );
         consolePrintHex( myByte, 8 );
         consolePrintString( ", en la direccion: 0x" );
         consolePrintHex( address, 16 );
         consolePrintlnString( "." );
      } else{
         consolePrintlnString( "No se pudo leer EEPROM." );
      }
   }

   consolePrintEnter();

   while( TRUE );
   return 0;
}
*/

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_EEPROM_24XX1025_H_ */
