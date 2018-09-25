/* Copyright 2015-2016, Eric Pernia.
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

/* Date: 2017-12-05 */

/*==================[inlcusiones]============================================*/

#include "sapi_lcd.h"    // <= su propio archivo de cabecera (opcional)

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static void lcdEnablePulse( void );

static void lcdSendNibble( uint8_t nibble );

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

static void lcdEnablePulse( void )
{
   lcdPinWrite( LCD_HD44780_EN, ON );     // EN = 1 for H-to-L pulse
   lcdDelay_us( LCD_EN_PULSE_WAIT_US );   // Wait to make EN wider
   lcdPinWrite( LCD_HD44780_EN, OFF );    // EN = 0 for H-to-L pulse
}

static void lcdSendNibble( uint8_t nibble )
{
   lcdPinWrite( LCD_HD44780_D7, ( nibble & 0x80 ) );
   lcdPinWrite( LCD_HD44780_D6, ( nibble & 0x40 ) );
   lcdPinWrite( LCD_HD44780_D5, ( nibble & 0x20 ) );
   lcdPinWrite( LCD_HD44780_D4, ( nibble & 0x10 ) );
}

/*==================[definiciones de funciones externas]=====================*/

void lcdCommand( uint8_t cmd )
{
   lcdSendNibble( cmd & 0xF0 );          // Send high nibble to D7-D4

   lcdPinWrite( LCD_HD44780_RS, OFF );   // RS = 0 for command
   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();
   lcdDelay_us( LCD_LOW_WAIT_US );       // Wait

   lcdSendNibble( cmd << 4 );            // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdData( uint8_t data )
{
   lcdSendNibble( data & 0xF0 );         // Send high nibble to D7-D4

   lcdPinWrite( LCD_HD44780_RS, ON );    // RS = 1 for data
   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();

   lcdSendNibble( data << 4 );           // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdInit( uint16_t lineWidth, uint16_t amountOfLines,
              uint16_t charWidth, uint16_t charHeight )
{
   // Configure LCD Pins as Outputs
   lcdInitPinAsOutput( LCD_HD44780_RS );
   lcdInitPinAsOutput( LCD_HD44780_RW );
   lcdInitPinAsOutput( LCD_HD44780_EN );

   lcdInitPinAsOutput( LCD_HD44780_D4 );
   lcdInitPinAsOutput( LCD_HD44780_D5 );
   lcdInitPinAsOutput( LCD_HD44780_D6 );
   lcdInitPinAsOutput( LCD_HD44780_D7 );

   // Configure LCD for 4-bit mode
   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0
   lcdPinWrite( LCD_HD44780_RS, OFF );   // RS = 0

   lcdPinWrite( LCD_HD44780_EN, OFF );   // EN = 0

   lcdDelay_ms( LCD_STARTUP_WAIT_MS );   // Wait for stable power

   lcdCommand( 0x33 );                   // Command 0x33 for 4-bit mode
   lcdCommandDelay();                    // Wait

   lcdCommand( 0x32 );                   // Command 0x32 for 4-bit mode
   lcdCommandDelay();                    // Wait

   lcdCommand( 0x28 );                   // Command 0x28 for 4-bit mode
   lcdCommandDelay();                    // Wait

   // Initialize LCD
   lcdCommand( 0x0E );                   // Command 0x0E for display on, cursor on
   lcdCommandDelay();                    // Wait

   lcdClear();                           // Command for clear LCD

   lcdCommand( 0x06 );                   // Command 0x06 for Shift cursor right
   lcdCommandDelay();                    // Wait

   lcdDelay_ms( 1 );                     // Wait
}

void lcdGoToXY( uint8_t x, uint8_t y )
{
   uint8_t firstCharAdress[] = { 0x80, 0xC0, 0x94, 0xD4 };   // See table 12-5
   lcdCommand( firstCharAdress[ y - 1 ] + x - 1 );
   lcdDelay_us( LCD_HIGH_WAIT_US );      // Wait
}

void lcdClear( void )
{
   lcdCommand( 0x01 );                   // Command 0x01 for clear LCD
   lcdDelay_ms(LCD_CLR_DISP_WAIT_MS);    // Wait
}

void lcdSendStringRaw( char* str )
{
   uint8_t i = 0;
   while( str[i] != 0 ) {
      lcdData( str[i] );
      i++;
   }
}

void lcdCreateChar( uint8_t charnum, const char* chardata )
{
   uint8_t i;
   charnum &= 0x07;
   lcdCommand( E_SET_CGRAM_ADDR | (charnum << 3) );
   for (i = 0; i < 8; i++) {
      lcdData( chardata[i] );
   }
   delay(1);
}


/*==================[fin del archivo]========================================*/
