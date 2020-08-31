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

/* Date: 2017-12-05 */

#ifndef _SAPI_LCD_H_
#define _SAPI_LCD_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
   
//define LCD_HD44780_I2C_PCF8574T // For LCD connected via I2C PCF8574T I/O expander

/* LCD library configuration - Begin */

// LCD Delay HAL
#define lcdDelay_ms(duration)       delayInaccurateMs(duration)
#define lcdDelay_us(duration)       delayInaccurateUs(duration) //delayUs(duration)
#define lcdCommandDelay()           lcdDelay_us(LCD_CMD_WAIT_US)

// LCD GPIO HAL
#define lcdInitPinAsOutput(pin)     gpioInit( (pin), GPIO_OUTPUT )
#define lcdPinWrite( pin, value )   gpioWrite( (pin), (value) )

#ifdef LCD_HD44780_I2C_PCF8574T
   // Configure LCD I2C Pin positions
   #define LCD_HD44780_D7          7 // Data bit 7.
   #define LCD_HD44780_D6          6 // Data bit 4.
   #define LCD_HD44780_D5          5 // Data bit 5.
   #define LCD_HD44780_D4          4 // Data bit 4.
   #define LCD_HD44780_BACKLIGHT   3 // Backlight. LCD backlight anode.
   #define LCD_HD44780_EN          2 // Enable bit.
   #define LCD_HD44780_RW          1 // Read/Write bit. R/W = 0 for write, R/W = 1 for read (LCD_RW pin connected to GND).
   #define LCD_HD44780_RS          0 // Register select bit. RS = 0 to select command register, RS = 1 to select data register.
#else
	#if (BOARD == ciaa_sim_ia32)
	   #error CIAA-SIM_IA32 not supported yet!
	#elif (BOARD == ciaa_sim_ia64)
	   #error CIAA-SIM_IA64 not supported yet!
	#elif (BOARD == ciaa_nxp)
		// Configure LCD pins
		#define LCD_HD44780_D7          GPIO3    // Data bit 7.
		#define LCD_HD44780_D6          GPIO2    // Data bit 4.
		#define LCD_HD44780_D5          GPIO1    // Data bit 5.
		#define LCD_HD44780_D4          GPIO0    // Data bit 4.
		#define LCD_HD44780_BACKLIGHT   SPI_MISO // Backlight.
		#define LCD_HD44780_EN          GPIO8    // Enable bit.
		#define LCD_HD44780_RW          GND      // Read/Write bit. R/W = 0 for write, R/W = 1 for read (LCD_RW pin connected to GND).
		#define LCD_HD44780_RS          GPIO7    // Register select bit. RS = 0 to select command register, RS = 1 to select data register.
		#define LCD_HD44780_RST         GPIO7    // Reset.
	#elif (BOARD == edu_ciaa_nxp)
		// Configure LCD pins
		#define LCD_HD44780_D7          LCD4   // Data bit 7.
		#define LCD_HD44780_D6          LCD3   // Data bit 4.
		#define LCD_HD44780_D5          LCD2   // Data bit 5.
		#define LCD_HD44780_D4          LCD1   // Data bit 4.
		#define LCD_HD44780_BACKLIGHT   GPIO0  // Backlight.
		#define LCD_HD44780_EN          LCDEN  // Enable bit.
		#define LCD_HD44780_RW          GND    // Read/Write bit. R/W = 0 for write, R/W = 1 for read (LCD_RW pin connected to GND).
		#define LCD_HD44780_RS          LCDRS  // Register select bit. RS = 0 to select command register, RS = 1 to select data register.
        #define LCD_HD44780_RST         GPIO1    // Reset.
	#elif (BOARD == ciaa_fsl)
	   #error CIAA-FSL not supported yet!
	#elif (BOARD == ciaa_pic)
	   #error CIAA-PIC not supported yet!
	#elif (BOARD == pico_ciaa)
	   #error picoCIAA not supported yet!
	#elif (BOARD == ciaa_leon3_fpga_nfp)
	   #error CIAA-LEON3-FPGA-NFP not supported yet!
	#elif (BOARD == ciaa_z3r0)
	   #error CIAA-Z3R0 not supported yet!
	#elif (BOARD == ciaa_7st)
	   #error CIAA-7-ST not supported yet!
	#endif
#endif

// LCD delay Times
#define LCD_EN_PULSE_WAIT_US   25    // 25 us
#define LCD_LOW_WAIT_US        25    // 25 us
#define LCD_HIGH_WAIT_US       100   // 100 us
#define LCD_CMD_WAIT_US             110   // Wait time for every command 45 us, except:
#define LCD_CLR_DISP_WAIT_MS   3     // - Clear Display 1.52 ms
#define LCD_RET_HOME_WAIT_MS   3     // - Return Home  1.52 ms
#define LCD_RBUSY_ADDR_WAIT_US 0     // - Read Busy flag and address 0 us
#define LCD_STARTUP_WAIT_MS   1000   // 1000 ms

/* LCD library configuration - End */
   
// For backward compatibility
#define lcdConfig lcdInit


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
//#define LCD_CURSORON            0x02
//#define LCD_CURSOROFF           0x00
//#define LCD_BLINKON             0x01
//#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00


//uint32_t lcdHd44780Gpios[8] = 

typedef enum{
   PCF8574T_P0 = (1<<0),
   PCF8574T_P1 = (1<<1),
   PCF8574T_P2 = (1<<2),
   PCF8574T_P3 = (1<<3),
   PCF8574T_P4 = (1<<4),
   PCF8574T_P5 = (1<<5),
   PCF8574T_P6 = (1<<6),
   PCF8574T_P7 = (1<<7),
} pcf8574T_gpio_t;

typedef enum{
   PCF8574_GPIO_INPUT = 1,
   PCF8574_GPIO_OUTPUT = 0,
} pcf8574T_gpioDirection_t;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2)


/*==================[typedef]================================================*/

// Enumeration defining the HD44780 commands
enum enLcdCommands {
   E_CLEAR_DISPLAY        = 0x01,
   E_RETURN_HOME          = 0x02,
   E_ENTRY_MODE_SET       = 0x04,
   E_DISPLAY_ON_OFF_CTRL  = 0x08,
   E_CURSOR_DISPLAY_SHIFT = 0x10,
   E_FUNCTION_SET         = 0x20,
   E_SET_CGRAM_ADDR       = 0x40,
   E_SET_DDRAM_ADDR       = 0x80
};

// This enumeration defines the available cursor modes
typedef enum{
   LCD_CURSOR_OFF      = 0x00,
   LCD_CURSOR_ON       = 0x02,
   LCD_CURSOR_ON_BLINK = 0x03
} lcdCursorModes_t;

typedef struct{
   const uint8_t address;   // Custom character address
   const uint8_t bitmap[8]; // Custom character bitmap
} lcdCustomChar_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

// BIBLIOTECA NO REENTRANTE, CUIDADO CON EL RTOS!!!

void lcdInit( uint16_t lineWidth, uint16_t amountOfLines,
              uint16_t charWidth, uint16_t charHeight );
void lcdCommand( uint8_t cmd );
void lcdData( uint8_t data );

void lcdGoToXY( uint8_t x, uint8_t y );
void lcdClear( void );
void lcdCursorSet( lcdCursorModes_t mode );
void lcdCreateChar( uint8_t charnum, const char* chardata );

void lcdCreateCustomChar( lcdCustomChar_t* customChar );
void lcdSendCustomChar( lcdCustomChar_t* customChar );

void lcdClearAndHome( void );
void lcdClearLine( uint8_t line );
void lcdClearLineFrom( uint8_t line, uint8_t xFrom );
void lcdClearLineFromTo( uint8_t line, uint8_t xFrom, uint8_t xTo );
void lcdSendStringRaw( char* str );

void lcdSendEnter( void );
void lcdSendChar( char character );
void lcdSendCustomCharByIndex( uint8_t charIndex );

void lcdSendString( char* str );
void lcdSendStringClearLine( char* str );
void lcdSendStringFormXY( char* str, uint8_t x, uint8_t y );
void lcdSendStringFormXYClearLine( char* str, uint8_t x, uint8_t y );

void lcdSendInt( int64_t value );
void lcdSendIntClearLine( int64_t value );
void lcdSendIntFormXY( int64_t value, uint8_t x, uint8_t y );
void lcdSendIntFormXYClearLine( int64_t value, uint8_t x, uint8_t y );

void lcdSendFloat( float value, uint32_t decDigits );
void lcdSendFloatClearLine( float value, uint32_t decDigits );
void lcdSendFloatFormXY( float value, uint32_t decDigits, uint8_t x, uint8_t y );
void lcdSendFloatFormXYClearLine( float value, uint32_t decDigits, uint8_t x, uint8_t y );

#define lcdSendStringLn(str)   lcdSendString(str); \
                               lcdSendEnter()

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_LCD_H_ */
