/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "HardwareSerial.h"


// Constructors ////////////////////////////////////////////////////////////////
Stream_Serial::Stream_Serial(uartMap_t uart, uint32_t baud)
{
    this_uart = uart;
    this_baud = baud;

    uartConfig(uart, baud);
}


// Public Methods //////////////////////////////////////////////////////////////

void Stream_Serial::begin(unsigned long baud)
{
    this_baud = baud;

    uartConfig(this_uart, baud);
}

int Stream_Serial::read(void)
{
  uint8_t c;
  if (uartReadByte( this_uart, &c ))
    return c;
  else
    return -1;
}

size_t Stream_Serial::write(uint8_t c)
{
  uartWriteByte( this_uart, c);
   return 0; //FIXME: @Eric ingrese un valor al azar para que no chille la compilacion
}

int Stream_Serial::available() {
   return -1; //FIXME: @Eric ingrese un valor al azar para que no chille la compilacion
}

int Stream_Serial::peek(void) {
   return -1; //FIXME: @Eric ingrese un valor al azar para que no chille la compilacion
}

void Stream_Serial::flush(void) {

}


Stream_Serial Serial(UART_USB, 115200);
