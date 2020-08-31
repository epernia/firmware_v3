/* Copyright 2017, Eric Pernia.
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
as 1010 binary for read and write operations. The next bit of the control
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

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// EEPROM24xx1025 total memory size in bytes
#define EEPROM_1024_K_BIT              1024*1024/8 // Memory size [bytes]

// EEPROM24xx1025 single page size (in bytes)
#define EEPROM24xx1025_PAGE_SIZE       128 // [bytes per page]

// EEPROM24xx1025 I2C address
#define EEPROM24xx1025_CTRL_CODE       0xA // 0b1010 

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

/*==================[external functions declaration]=========================*/

//-----------------------------------------------------------------------------
// MANAGEMENT
//-----------------------------------------------------------------------------

uint8_t eeprom24xx1025I2cAddress( Eeprom24xx1025_t* eeprom, uint32_t address );

// Enable or disable the peripheral energy and clock
bool_t eeprom24xx1025PowerSet( Eeprom24xx1025_t* eeprom, bool_t power );

// Amount of pages
uint32_t eeprom24xx1025AmountOfPagesGet( Eeprom24xx1025_t* eeprom );

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
bool_t eeprom24xx1025WritePage( Eeprom24xx1025_t* eeprom, uint32_t page,
                                uint8_t* byteBuffer, uint32_t byteBufferSize );

//-----------------------------------------------------------------------------
// READ OPERATIONS
//-----------------------------------------------------------------------------

// Current Address Read
bool_t eeprom24xx1025ReadCurrentAddress( Eeprom24xx1025_t* eeprom,
                                         uint32_t memoryAddress,
                                         uint8_t* readedByte );
                                         
// Random Read
bool_t eeprom24xx1025ReadRandom( Eeprom24xx1025_t* eeprom, 
                                 uint32_t memoryAddress, uint8_t* readedByte );

// Sequential Read
bool_t eeprom24xx1025ReadSequential( Eeprom24xx1025_t* eeprom, uint32_t address,
                                     uint8_t* byteBuffer, uint32_t byteBufferSize );

//-----------------------------------------------------------------------------
// EEPROM Test
//-----------------------------------------------------------------------------

/**
 * EEPROM Test Write individual bytes and Read Random
 * @param uint8_t startingPage. The page where to start the test.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint8_t. The size of buffer (must be lower than page size).
 * @return bool_t. Whether the test was passed or not.
 */
bool_t eeprom24xx1025TestWriteBytes( uint32_t startingPage, 
                                     uint8_t* buff, uint32_t buffSize );

/**
 * EEPROM Test Write a hole page
 * @param uint8_t page. The page where save.
 * @param uint8_t* buff. The buffer to save in mamory.
 * @param uint8_t. The size of buffer (must be equal to page size).
 * @return bool_t. Whether the test was passed or not.
 */
bool_t eeprom24xx1025TestWritePage( uint32_t page, 
                                    uint8_t* buff, uint32_t buffSize );

/**
 * EEPROM Test Sequential read
 * Performs a test on the 24xx1025 EEPROM memory.
 */
bool_t eeprom24xx1025TestReadSequential( uint32_t address, 
                                         uint8_t* byteBuffer,
                                         uint32_t byteBufferSize );

/**
 * EEPROM Test Current Address Read
 */
bool_t eeprom24xx1025TestReadCurrentAddress( uint32_t address, 
                                             uint8_t* byteBuffer );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_EEPROM_24XX1025_H_ */
