/* Copyright 2019, Martin Ribelotta
 *
 * Based on API from old sapi_stdio from Agustin Bassi.
 *
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

#ifdef USE_TINYPRINTF
#include "sapi_stdio.h"
#include <printf.h>

bool_t stdioInit( uartMap_t uartPrintf )
{
   return TRUE;
}

static void outfunc(char c, void *arg)
{
   uartWriteByte((uartMap_t) arg, c);
}

int stdioPrintf(uartMap_t uartPrintf, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   int ret = vfctprintf(outfunc, (void*) uartPrintf, format, args);
   va_end(args);
   return ret;
}

int stdioSprintf(char *out, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   int ret = vsprintf(out, format, args);
   va_end(args);
   return ret;
}

#else

#define _GNU_SOURCE

#include "sapi_stdio.h"

#include <stdio.h>
#include <stdarg.h>

static ssize_t uartfd_read(void *_cookie, char *_buf, size_t _n)
{
   ssize_t count = 0;
   while (count < _n) {
      uint8_t c;
      if (uartReadByte((uartMap_t) _cookie, &c))
         *_buf++ = c;
   }
   return count;
}

static ssize_t uartfd_write(void *_cookie, const char *_buf, size_t _n)
{
   uartWriteByteArray((uartMap_t) _cookie, (uint8_t*) _buf, (uint32_t) _n);
   return _n;
}

static const cookie_io_functions_t uartfd_vtable = {
   .read = uartfd_read,
   .write = uartfd_write,
   .seek = NULL,
   .close = NULL,
};

bool_t stdioInit( uartMap_t uartPrintf )
{
   return TRUE;
}

int stdioPrintf(uartMap_t uartPrintf, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   FILE *f = fopencookie((void*) uartPrintf, "w+", uartfd_vtable);
   int ret = vfprintf(f, format, args);
   fclose(f);
   va_end(args);
   return ret;
}

int stdioSprintf(char *out, const char *format, ...)
{
   va_list args;
   va_start(args, format);
   int ret = vsprintf(out, format, args);
   va_end(args);
   return ret;
}

#endif
