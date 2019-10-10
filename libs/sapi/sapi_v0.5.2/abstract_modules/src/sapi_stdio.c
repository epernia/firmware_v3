/* Copyright 2017, Agustin Bassi.
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
 *
    Copyright 2001 Georges Menie
    https://www.menie.org/georges/embedded/small_printf_source_code.html

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
	putchar is the only external dependency for this file,
	if you have a working putchar, just remove the following
	define. If the function should be called something else,
	replace outbyte(c) by your own function call.
 */

/*
 * Date: 2017-11-01
 */

/*==================[inclusions]=============================================*/

#include "sapi_stdio.h"

/*==================[macros and definitions]=================================*/

#define putchar(c)      outbyte(c)
#define PAD_RIGHT       1
#define PAD_ZERO        2
#define PRINT_BUF_LEN   12

/*==================[internal data declaration]==============================*/

static uartMap_t UartPrintf = UART_USB;

/*==================[internal functions declaration]=========================*/

static void printchar(char **str, int c);

static int prints(char **out, const char *string, int width, int pad);

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);

static int print(char **out, int *varg);

/*==================[internal data definition]===============================*/

void _outbyte(int c)
{
   uartWriteByte(UartPrintf, (char)c);
}

void outbyte(int c)
{
   static char prev = 0;
   if (c < ' ' && c != '\r' && c != '\n' && c != '\t' && c != '\b')
      return;
   if (c == '\n' && prev != '\r') _outbyte('\r');
   _outbyte(c);
   prev = c;
}

/**
 * Imprime un char en un string o por la UART.
 * Si str es distinto de null (es decir sprintf)
 * entra en el if y guarda el char en str.
 * Sino, llama a putchar, que a fin de cuentas
 * termina sacando un dato por la UART.
 * @param str
 * @param c
 */
static void printchar(char **str, int c)
{
   extern void putchar(int c);
   if (str) {
      **str = c;
      ++(*str);
   } else {
      (void)putchar(c);
      //(void)outbyte(c);// @Eric fix newlib commits
   }
}

static int prints(char **out, const char *string, int width, int pad)
{
   register int pc = 0, padchar = ' ';

   if (width > 0) {
      register int len = 0;
      register const char *ptr;
      for (ptr = string; *ptr; ++ptr) ++len;
      if (len >= width) width = 0;
      else width -= len;
      if (pad & PAD_ZERO) padchar = '0';
   }
   if (!(pad & PAD_RIGHT)) {
      for ( ; width > 0; --width) {
         printchar (out, padchar);
         ++pc;
      }
   }
   for ( ; *string ; ++string) {
      printchar (out, *string);
      ++pc;
   }
   for ( ; width > 0; --width) {
      printchar (out, padchar);
      ++pc;
   }
   return pc;
}

static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase)
{
   char print_buf[PRINT_BUF_LEN];
   register char *s;
   register int t, neg = 0, pc = 0;
   register unsigned int u = i;

   if (i == 0) {
      print_buf[0] = '0';
      print_buf[1] = '\0';
      return prints (out, print_buf, width, pad);
   }

   if (sg && b == 10 && i < 0) {
      neg = 1;
      u = -i;
   }

   s = print_buf + PRINT_BUF_LEN-1;
   *s = '\0';

   while (u) {
      t = u % b;
      if( t >= 10 )
         t += letbase - '0' - 10;
      *--s = t + '0';
      u /= b;
   }

   if (neg) {
      if( width && (pad & PAD_ZERO) ) {
         printchar (out, '-');
         ++pc;
         --width;
      } else {
         *--s = '-';
      }
   }

   return pc + prints (out, s, width, pad);
}

static int print(char **out, int *varg)
{
   register int width, pad;
   register int pc = 0;
   register char *format = (char *)(*varg++);
   char scr[2];

   for (; *format != 0; ++format) {
      if (*format == '%') {
         ++format;
         width = pad = 0;
         if (*format == '\0') break;
         if (*format == '%') goto out;
         if (*format == '-') {
            ++format;
            pad = PAD_RIGHT;
         }
         while (*format == '0') {
            ++format;
            pad |= PAD_ZERO;
         }
         for ( ; *format >= '0' && *format <= '9'; ++format) {
            width *= 10;
            width += *format - '0';
         }
         if( *format == 's' ) {
            register char *s = *((char **)varg++);
            pc += prints (out, s?s:"(null)", width, pad);
            continue;
         }
         if( *format == 'd' ) {
            pc += printi (out, *varg++, 10, 1, width, pad, 'a');
            continue;
         }
         if( *format == 'x' ) {
            pc += printi (out, *varg++, 16, 0, width, pad, 'a');
            continue;
         }
         if( *format == 'X' ) {
            pc += printi (out, *varg++, 16, 0, width, pad, 'A');
            continue;
         }
         if( *format == 'u' ) {
            pc += printi (out, *varg++, 10, 0, width, pad, 'a');
            continue;
         }
         if( *format == 'c' ) {
            /* char are converted to int then pushed on the stack */
            scr[0] = *varg++;
            scr[1] = '\0';
            pc += prints (out, scr, width, pad);
            continue;
         }
      } else {
out:
         printchar (out, *format);
         ++pc;
      }
   }
   if (out) **out = '\0';
   return pc;
}

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* Se asume que sizeof(void *) == sizeof(int) */

/**
 * Configura la UART por la que va a salir el printf.
 * No es necesario usarla en esta version.
 * @param uartPrintf UART para printf
 * @return TRUE
 */
bool_t stdioInit( uartMap_t uartPrintf )
{
   UartPrintf = uartPrintf;
   return TRUE;
}

/**
 * Realiza un printf sobre la UART seleccionada
 * @param uartPrintf UART_USB, UART_232 o UART_485
 * @param format el string formateado con argumentos.
 * @return TRUE si salio bien, FALSE caso contrario.
 */
int stdioPrintf(uartMap_t uartPrintf, const char *format, ...)
{
   register int *varg = (int *)(&format);
   UartPrintf = uartPrintf;
   return print(0, varg);
}

/**
 * Realiza un prinft sobre un array.
 * Esta funcion es una adaptacion de la funcion
 * sprintf de la libreria standard de C.
 * @param out el array a guardar los datos.
 * @param format el string formateado con argumentos.
 * @return TRUE si salio bien, FALSE caso contrario.
 */
int stdioSprintf(char *out, const char *format, ...)
{
   register int *varg = (int *)(&format);
   return print(&out, varg);
}

/*==================[end of file]============================================*/

