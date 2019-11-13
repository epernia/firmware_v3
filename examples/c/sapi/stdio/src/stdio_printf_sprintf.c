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
 */

/* Date: 2017-30-10 */

/*==================[inclusions]=============================================*/

#include "sapi.h"     // <= sAPI header
//#include "stdio_printf_sprintf.h"

/*==================[macros and definitions]=================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){
char *ptr = "Hello world!";
char *np = 0;
int i = 5;
unsigned int bs = sizeof(int)*8;
int mi;
char buf[80];
   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

	mi = (1 << (bs-1)) + 1;
	stdioPrintf(UART_USB, "%s\n", ptr);
	stdioPrintf(UART_USB, "stdioPrintf test\n");
	// stdioPrintf(UART_USB, "%s is null pointer\n", np); // FIXME not work with cookies
	stdioPrintf(UART_USB, "%d = 5\n", i);
	stdioPrintf(UART_USB, "%d = - max int\n", mi);
	stdioPrintf(UART_USB, "char %c = 'a'\n", 'a');
	stdioPrintf(UART_USB, "hex %x = ff\n", 0xff);
	stdioPrintf(UART_USB, "hex %02x = 00\n", 0);
	stdioPrintf(UART_USB, "signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	stdioPrintf(UART_USB, "%d %s(s)%", 0, "message");
	stdioPrintf(UART_USB, "\n");
	stdioPrintf(UART_USB, "%d %s(s) with %%\n", 0, "message");
	stdioSprintf(buf, "justif: \"%-10s\"\n", "left");
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, "justif: \"%10s\"\n", "right");
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, " 3: %04d zero padded\n", 3);
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, " 3: %-4d left justif.\n", 3);
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, " 3: %4d right justif.\n", 3);
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, "-3: %04d zero padded\n", -3);
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, "-3: %-4d left justif.\n", -3);
	stdioPrintf(UART_USB, "%s", buf);
	stdioSprintf(buf, "-3: %4d right justif.\n", -3);
	stdioPrintf(UART_USB, "%s", buf);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
	   /* En consola se deberia ver:
	   * Hello world!
	   * printf test
	   * (null) is null pointer
	   * 5 = 5
	   * -2147483647 = - max int
	   * char a = 'a'
	   * hex ff = ff
	   * hex 00 = 00
	   * signed -3 = unsigned 4294967293 = hex fffffffd
	   * 0 message(s)
	   * 0 message(s) with %
	   * justif: "left      "
	   * justif: "     right"
	   *  3: 0003 zero padded
	   *  3: 3    left justif.
	   *  3:    3 right justif.
	   * -3: -003 zero padded
	   * -3: -3   left justif.
	   * -3:   -3 right justif.
	   */
	   gpioToggle(LEDB);
	   delay(1000);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
