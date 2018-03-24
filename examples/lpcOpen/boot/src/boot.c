/* Copyright 2016, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of Workspace.
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

/** \addtogroup boot Simple bootloader.
 ** @{ */

/** see README.md!! */

/*==================[inclusions]=============================================*/

#include "boot.h"
#include "board.h"
#include "ciaaUART.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
	Board_Init();
	SystemCoreClockUpdate();
	ciaaUARTInit();
}

/*==================[external functions definition]==========================*/

int main(void)
{
	uint8_t buf[32];
	uint32_t size;
	uint32_t bcount = 0, brecv;
	uint8_t * pram = (uint8_t *)0x10080000;

	initHardware();

	dbgPrint("Waiting for binary. First 4 bytes must be image length.\r\n");

	/* image size (4 bytes) */
	bcount = 0;
	while (bcount < 4) {
		brecv = uartRecv(CIAA_UART_USB, buf+bcount, 1);
		bcount += brecv;
	}

	memcpy(&size, buf, 4);

	/* receive complete image */
	bcount = 0;
	while(bcount < size) {
		brecv = uartRecv(CIAA_UART_USB, buf, 32);
		memcpy(pram+bcount, buf, brecv);
		bcount += brecv;
	}

	dbgPrint("Image received. Booting...\r\n");
	size = 0xFFFFFF;
	while(size > 0) size--; /* wait while message prints... */

	__disable_irq();

	/* Set Vector Table Offset Register */
	SCB->VTOR = 0x10080000;

	/* Get initial SP from first word.
	 * Then get PC from second word.
	 */
	asm volatile(
	"ldr r0,=0x10080000\n"
	"ldr sp,[r0]\n"
	"ldr pc,[r0,4]\n"
	);
}

/** @} doxygen end group definition */
/*==================[end of file]============================================*/
