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

#ifndef _SAPI_LCD_H_
#define _SAPI_LCD_H_

/*==================[inclusions]=============================================*/

#include "sapi.h"        // <= Biblioteca sAPI

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// Configure LCD pins
#define LCD_HD44780_RS   LCDRS   // RS = 0 to select command register, RS = 1 to select data register
#define LCD_HD44780_EN   LCDEN   // Enable
#define LCD_HD44780_RW   0       // R/W = 0 for write, R/W = 1 for read

#define LCD_HD44780_D7   LCD4
#define LCD_HD44780_D6   LCD3
#define LCD_HD44780_D5   LCD2
#define LCD_HD44780_D4   LCD1

// LCD delay Times
#define LCD_EN_PULSE_WAIT_US   25    // 25 us
#define LCD_LOW_WAIT_US        25    // 25 us
#define LCD_HIGH_WAIT_US       100   // 100 us

#define LCD_CMD_WAIT_US        45    // Wait time for every command 45 us, except:
#define LCD_CLR_DISP_WAIT_MS   3     // - Clear Display 1.52 ms
#define LCD_RET_HOME_WAIT_MS   3     // - Return Home  1.52 ms
// - Read Busy flag and address 0 us

#define LCD_STARTUP_WAIT_MS    1000  // 3000 ms

// LCD delay HAL
#define lcdDelay_ms(duration)       delay(duration)
#define lcdDelay_us(duration)       delayInaccurateUs(duration) //delayUs(duration)
#define lcdCommandDelay()           lcdDelay_us(LCD_CMD_WAIT_US)
#define lcdInitPinAsOutput(pin)     gpioInit( (pin), GPIO_OUTPUT );
#define lcdPinWrite( pin, value )   gpioWrite( (pin), (value) )

#define lcdConfig lcdInit

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
enum enLCDCursorModes {
   E_LCD_CURSOR_OFF      = 0x00,
   E_LCD_CURSOR_ON       = 0x02,
   E_LCD_CURSOR_ON_BLINK = 0x03
};

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void lcdCommand( uint8_t cmd );

void lcdData( uint8_t data );

void lcdInit( uint16_t lineWidth, uint16_t amountOfLines,
              uint16_t charWidth, uint16_t charHeight );

void lcdGoToXY( uint8_t x, uint8_t y );

void lcdClear( void );

void lcdSendStringRaw( char* str );

void lcdCreateChar( uint8_t charnum, const char* chardata );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_LCD_H_ */
