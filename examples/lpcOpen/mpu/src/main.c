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
 
/** @brief MPU example
 */

/** \addtogroup mpu MPU test example
 ** @{ */

/*==================[inclusions]=============================================*/

#include "main.h"
#include "board.h"
#include "mpu.h"

/*==================[macros and definitions]=================================*/

#if defined(lpc1769)
#define CODE_BASE_ADDR 0x00000000
#define DATA_BASE_ADDR 0x10000000
#define TEST_ADDR 0x2007C000
#elif defined(lpc4337_m4)
#define CODE_BASE_ADDR 0x1A000000
#define DATA_BASE_ADDR 0x10000000
#define TEST_ADDR 0x20000000
#endif

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
	SysTick_Config(SystemCoreClock / 1000);

	/* Activate MemFault, UsageFault and BusFault handlers */
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk |
			      SCB_SHCSR_USGFAULTENA_Msk |
			      SCB_SHCSR_BUSFAULTENA_Msk;
}

/*==================[external functions definition]==========================*/

void SVC_Handler(void)
{
    uint32_t control = __get_CONTROL();
    control &= ~1;
    __set_CONTROL(control);
}

int main(void)
{
	initHardware();

	/*
	 * MPU Test
	 * Background region enabled for privileged access.
	 * Setup two regions for user access.
	 * Region 0: Code memory, 512kB.
	 * Region 1: Main RAM memory, 32kB.
	 */
	mpuRegionSetup(CODE_BASE_ADDR, 0, MPU_REGION_SIZE_512k, MPU_AP_FULL_ACCESS,
			MPU_ATTR_ROM_tex_sCb, 0, 0, 1);
	mpuRegionSetup(DATA_BASE_ADDR, 1, MPU_REGION_SIZE_32k, MPU_AP_FULL_ACCESS,
			MPU_ATTR_INT_SRAM_tex_SCb, 0, 0, 1);

	/* Pointer to test address, only accesible in privileged mode */
	int * p = (int *)TEST_ADDR;
	int variable;

	/* this should work just fine */
	*p = 0xABCDEF98;
	variable = *p;

	/* now go to user mode */
	uint32_t control = __get_CONTROL();
	control |= 1;
	__set_CONTROL(control);

	/* try to access to TEST_ADDR
	 * using debug session set variable to 0 to avoid MemManage fault
	 */
	if (variable) {
		variable = *p;
	}

	/* use service call to return to privileged mode */
	__asm volatile("svc 0");

	/* try again  */
	variable = *p;

	/* infinite foo loop */
	while(variable) {

	}
	return 0 ;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
