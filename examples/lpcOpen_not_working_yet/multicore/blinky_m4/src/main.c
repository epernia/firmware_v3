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

/** @brief Multicore blink example for LPC54102 (M4 core)
 */

/*==================[inclusions]=============================================*/

#include "main.h"
#include "board.h"

/*==================[macros and definitions]=================================*/

/** M0 core image defined in linker script */
extern uint32_t __core_m0slave_START__[];

/** M0 image, stack pointer initial value */
#define M0_STACK __core_m0slave_START__[0]

/** M0 image, reset routine entry point */
#define M0_ENTRY __core_m0slave_START__[1]

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/** @brief delay function
 * @param t desired milliseconds to wait
 */
static void pausems(uint32_t t);

/*==================[internal data definition]===============================*/

/** @brief used for delay counter */
static uint32_t pausems_count;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	SysTick_Config(SystemCoreClock / 1000);
}

static void pausems(uint32_t t)
{
	pausems_count = t;
	while (pausems_count != 0) {
		__WFI();
	}
}

/*==================[external functions definition]==========================*/

void SysTick_Handler(void)
{
	if(pausems_count > 0) pausems_count--;
}

int main(void)
{
	initHardware();

	/* Boot M0 core, using reset vector and stack pointer from the CM0+
	   image in FLASH. */
	Chip_CPU_CM0Boot((uint32_t *)M0_ENTRY, (uint32_t *)M0_STACK);

	while (1)
	{
		DEBUGSTR("hello world!\r\n");
		Board_LED_Toggle(LED);
		pausems(DELAY_MS);
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
