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
 *
 */

/* Date: 2017-12-05 */

/*==================[inlcusiones]============================================*/

#include "sapi_lcd.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

typedef struct {
   uint16_t lineWidth;
   uint16_t amountOfLines;
   uint16_t charWidth;
   uint16_t charHeight;
   uint8_t x;
   uint8_t y;
} lcd_t;

/*==================[definiciones de datos externos]=========================*/

static lcd_t lcd;

/*==================[declaraciones de funciones internas]====================*/

static uint8_t lcdDataValue = 0;
static uint8_t lcdEnStatus = OFF;
static uint8_t lcdRsStatus = OFF;
static uint8_t lcdBacklightStatus = 0;

static void lcdI2cWritePins( uint8_t _data );
static void lcdPinSet( uint8_t pin, bool_t status );

static void lcdEnablePulse( void );
static void lcdSendNibble( uint8_t nibble );

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/


#define PCF8574T_I2C_A0           1 // 1 connected to VDD, 0 to VSS
#define PCF8574T_I2C_A1           1 // 1 connected to VDD, 0 to VSS
#define PCF8574T_I2C_A2           1 // 1 connected to VDD, 0 to VSS

#define PCF8574T_I2C_BASE         0x20 // 0x40(fixed)
#define PCF8574T_I2C_ADDRESS      ( PCF8574T_I2C_BASE | ((PCF8574T_I2C_A2)<<2) | ((PCF8574T_I2C_A1)<<1) | PCF8574T_I2C_A0 )

static uint8_t pcf8574TGpioPortDirections = 0x00;
static uint8_t pcf8574TGpioPortValue = 0x00;
static uint8_t pcf8574TI2cAddress = PCF8574T_I2C_ADDRESS;



static void pcf8574TInit( uint8_t i2c, uint8_t i2cAddress );
static void pcf8574TGpioPortInit( uint8_t directions );
static void pcf8574TGpioPortWrite( uint8_t portValue );
static uint8_t pcf8574TGpioPortRead( void );
static void pcf8574TGpioInit( pcf8574T_gpio_t pin,
                              pcf8574T_gpioDirection_t direction );
static bool_t pcf8574TGpioRead( pcf8574T_gpio_t pin );
static void pcf8574TGpioWrite( pcf8574T_gpio_t pin, bool_t value );


static void pcf8574TInit( uint8_t i2c, uint8_t i2cAddress )
{

   pcf8574TI2cAddress = i2cAddress;
//   i2cInit( I2C0, 100000 );

   pcf8574TGpioPortInit( 0x00 ); // Init all GPIOs as outputs
   pcf8574TGpioPortWrite( 0x00 ); // Init all as zeros
   /*
   while(true){
      pcf8574TGpioPortWrite( 0x00 );
      delay(2000);
      pcf8574TGpioPortWrite( 0xFF );
      delay(2000);
   }
   */
}

static void pcf8574TGpioPortInit( uint8_t directions )
{
   pcf8574TGpioPortDirections = directions;
   i2cWrite( I2C0, pcf8574TI2cAddress, &directions, 1, TRUE );
}

static void pcf8574TGpioPortWrite( uint8_t portValue )
{
   pcf8574TGpioPortValue = portValue;
   // Or with pcf8574TGpioPortDirections to keep pins initialized as inputs
   uint8_t transmitDataBuffer = portValue | pcf8574TGpioPortDirections;
   i2cWrite( I2C0, pcf8574TI2cAddress, &transmitDataBuffer, 1, TRUE );
}

static uint8_t pcf8574TGpioPortRead( void )
{
   //uint8_t dataToReadBuffer[1] = { 0 };
   uint8_t receiveDataBuffer = 0;
   i2cRead( I2C0, pcf8574TI2cAddress,
            NULL, 0, TRUE,
            &receiveDataBuffer, 1, TRUE );
}

static void pcf8574TGpioInit( pcf8574T_gpio_t pin,
                              pcf8574T_gpioDirection_t direction )
{
   uint8_t directions = pcf8574TGpioPortDirections;
   if( direction ) {
      directions |= (1<<pin);
   } else {
      directions &= ~(1<<pin);
   }
   pcf8574TGpioPortInit( directions );
}

static bool_t pcf8574TGpioRead( pcf8574T_gpio_t pin )
{
   return pcf8574TGpioPortRead() & (1<<pin);
}

static void pcf8574TGpioWrite( pcf8574T_gpio_t pin, bool_t value )
{
   uint8_t portValue = pcf8574TGpioPortValue;
   if( value ) {
      portValue |= (1<<pin);
   } else {
      portValue &= ~(1<<pin);
   }
   pcf8574TGpioPortWrite( portValue );
}



static void lcdPinSet( uint8_t pin, bool_t status )
{
#ifdef LCD_HD44780_I2C_PCF8574T
   pcf8574TGpioWrite( pin, status );
#else
   gpioWrite( pin, status );
#endif
}

static void lcdEnablePulse( void )
{
   lcdPinSet( LCD_HD44780_EN, ON );       // EN = 1 for H-to-L pulse
   lcdDelay_us( LCD_EN_PULSE_WAIT_US );   // Wait to make EN wider //lcdDelay_us(1);
   lcdPinSet( LCD_HD44780_EN, OFF );      // EN = 0 for H-to-L pulse
   //lcdDelay_us(50); // commands need > 37us to settle
}

static void lcdSendNibble( uint8_t nibble )
{
   lcdPinSet( LCD_HD44780_D7, ( nibble & 0x80 ) );
   lcdPinSet( LCD_HD44780_D6, ( nibble & 0x40 ) );
   lcdPinSet( LCD_HD44780_D5, ( nibble & 0x20 ) );
   lcdPinSet( LCD_HD44780_D4, ( nibble & 0x10 ) );
}

/*==================[definiciones de funciones externas]=====================*/

void lcdCommand( uint8_t cmd )
{
   lcdSendNibble( cmd & 0xF0 );          // Send high nibble to D7-D4

   lcdPinSet( LCD_HD44780_RS, OFF );   // RS = 0 for command
   lcdPinSet( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();
   lcdDelay_us( LCD_LOW_WAIT_US );       // Wait

   lcdSendNibble( cmd << 4 );            // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdData( uint8_t data )
{
   lcdSendNibble( data & 0xF0 );         // Send high nibble to D7-D4

   lcdPinSet( LCD_HD44780_RS, ON );    // RS = 1 for data
   lcdPinSet( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();

   lcdSendNibble( data << 4 );           // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdInit( uint16_t lineWidth, uint16_t amountOfLines,
              uint16_t charWidth, uint16_t charHeight )
{
   lcd.lineWidth = lineWidth;
   lcd.amountOfLines = amountOfLines;
   lcd.charWidth = charWidth;
   lcd.charHeight = charHeight;
   lcd.x = 0;
   lcd.y = 0;

#ifdef LCD_HD44780_I2C_PCF8574T
   // Init I2C
   pcf8574TInit( I2C0, PCF8574T_I2C_ADDRESS );
   lcdPinSet( LCD_HD44780_BACKLIGHT, ON );
//   delay(100);
#else
   // Configure LCD Pins as Outputs
   lcdInitPinAsOutput( LCD_HD44780_RS );
   lcdInitPinAsOutput( LCD_HD44780_RW );
   lcdInitPinAsOutput( LCD_HD44780_EN );
   lcdInitPinAsOutput( LCD_HD44780_D4 );
   lcdInitPinAsOutput( LCD_HD44780_D5 );
   lcdInitPinAsOutput( LCD_HD44780_D6 );
   lcdInitPinAsOutput( LCD_HD44780_D7 );
#endif

   // Configure LCD for 4-bit mode
   lcdPinSet( LCD_HD44780_RW, OFF );     // RW = 0
   lcdPinSet( LCD_HD44780_RS, OFF );     // RS = 0
   lcdPinSet( LCD_HD44780_EN, OFF );     // EN = 0

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

   lcdCursorSet( LCD_CURSOR_OFF );
   //lcdGoToXY( 0, 0 );
   lcdClearAndHome();
}

void lcdGoToXY( uint8_t x, uint8_t y )
{
   if( x >= lcd.lineWidth || y >= lcd.amountOfLines ) {
      return;
   }
   uint8_t firstCharAdress[] = { 0x80, 0xC0, 0x94, 0xD4 };   // See table 12-5
   //lcdCommand( firstCharAdress[ y - 1 ] + x - 1 ); // Start in {x,y} = {1,1}
   lcdCommand( firstCharAdress[y] + x );             // Start in {x,y} = {0,0}
   lcdDelay_us( LCD_HIGH_WAIT_US );      // Wait
   lcd.x = x;
   lcd.y = y;
}

void lcdClear( void )
{
   lcdCommand( 0x01 );                   // Command 0x01 for clear LCD
   lcdDelay_ms(LCD_CLR_DISP_WAIT_MS);    // Wait
}

void lcdCursorSet( lcdCursorModes_t mode )
{
   lcdCommand( 0b00001100 | mode );
   lcdDelay_ms(LCD_CLR_DISP_WAIT_MS); // Wait
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
   lcdGoToXY( lcd.x, lcd.y );
}

void lcdCreateCustomChar( lcdCustomChar_t* customChar )
{
   lcdCreateChar( customChar->address, customChar->bitmap );
}

void lcdSendCustomChar( lcdCustomChar_t* customChar )
{
   lcdSendCustomCharByIndex( customChar->address );
}


void lcdClearAndHome( void )
{
   lcdClear();
   lcdGoToXY( 0, 0 ); // Poner cursor en 0, 0
   //delay(100);
}

void lcdClearLine( uint8_t line )
{
   lcdClearLineFromTo( line, 0, lcd.lineWidth - 1 );
}

void lcdClearLineFrom( uint8_t line, uint8_t xFrom )
{
   lcdClearLineFromTo( line, xFrom, lcd.lineWidth - 1 );
}

void lcdClearLineFromTo( uint8_t line, uint8_t xFrom, uint8_t xTo )
{
   uint8_t i = 0;

   if( xFrom >= lcd.lineWidth || line >= lcd.amountOfLines ) {
      return;
   }
   if( xFrom > xTo ) {
      return;
   }
   if( xTo >= lcd.lineWidth ) {
      xTo = lcd.lineWidth - 1;
   }

   lcdGoToXY( xFrom, line );
   for( i=xFrom; i<=xTo; i++ ) {
      lcdSendChar( ' ' );
   }
   //lcd.x--;
   lcdGoToXY( xFrom, line );
}

void lcdSendChar( char character )
{
   uint8_t i = 0;

   if( character == '\r' ) {        // Ignore '\r'
   } else if( character == '\n' ) { // Mando enter
      lcdSendEnter();
   } else {
      // Si se extiende en ancho mando enter
      if( lcd.x >= lcd.lineWidth ) {
         lcdSendEnter();
      }
      // Mando el caracter
      lcdData( character );
      lcd.x++;
   }
}

void lcdSendCustomCharByIndex( uint8_t charIndex )
{
   // Si se extiende en ancho mando enter
   if( lcd.x >= lcd.lineWidth ) {
      lcdSendEnter();
   }
   // Mando el caracter
   lcdData( charIndex );
   lcd.x++;
}

void lcdSendEnter( void )
{
   // Si llego abajo no hace nada
   if( lcd.y >= lcd.amountOfLines ) {
      return;
   } else {
      lcd.x = 0;
      lcd.y++;
      lcdGoToXY( lcd.x, lcd.y );
   }
}

void lcdSendStringClearLine( char* str )
{
   lcdSendString( str );
   lcdClearLineFrom( lcd.y, lcd.x );
}

void lcdSendString( char* str )
{
   uint32_t i = 0;
   while( str[i] != 0 ) {
      lcdSendChar( str[i] );
      i++;
   }
}

void lcdSendStringFormXY( char* str, uint8_t x, uint8_t y )
{
   lcdGoToXY( x, y );
   lcdSendString( str );
}

void lcdSendStringFormXYClearLine( char* str, uint8_t x, uint8_t y )
{
   lcdSendStringFormXY( str, x, y );
   lcdClearLineFrom( lcd.y, lcd.x );
}

void lcdSendInt( int64_t value )
{
   lcdSendString( intToStringGlobal(value) );
}

void lcdSendIntClearLine( int64_t value )
{
   lcdSendInt( value );
   lcdClearLineFrom( lcd.y, lcd.x );
}

void lcdSendIntFormXY( int64_t value, uint8_t x, uint8_t y )
{
   lcdGoToXY( x, y );
   lcdSendInt( value );
}

void lcdSendIntFormXYClearLine( int64_t value, uint8_t x, uint8_t y )
{
   lcdSendIntFormXY( value, x, y );
   lcdClearLineFrom( lcd.y, lcd.x );
}


void lcdSendFloat( float value, uint32_t decDigits )
{
   lcdSendString( floatToStringGlobal(value, decDigits) );
}

void lcdSendFloatClearLine( float value, uint32_t decDigits )
{
   lcdSendString( floatToStringGlobal(value, decDigits) );
   lcdClearLineFrom( lcd.y, lcd.x );
}

void lcdSendFloatFormXY( float value, uint32_t decDigits, uint8_t x, uint8_t y )
{
   lcdGoToXY( x, y );
   lcdSendFloat( value, decDigits );
}

void lcdSendFloatFormXYClearLine( float value, uint32_t decDigits, uint8_t x, uint8_t y )
{
   lcdSendFloatFormXY( value, decDigits, x, y );
   lcdClearLineFrom( lcd.y, lcd.x );
}

/*==================[fin del archivo]========================================*/
