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

/*==================[inclusions]=============================================*/

#include "packer.h"

#include "pack.h"
#include "samples.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

samples salida[64];
samples salida_c[64];

/*==================[external data definition]===============================*/

volatile uint32_t * DWT_CTRL   = (uint32_t *)0xE0001000;
volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
	Board_Init();
	SystemCoreClockUpdate();
}

static void empaquetar_c(int16_t * muestras, int n, uint32_t * salida)
{
	uint32_t i = 0;
	int8_t muestra;

	for(i=0; i<n; i++) {
		if(muestras[i] > 255)
			muestra = 255;
		else if(muestras[i] < 0)
			muestra = 0;
		else
			muestra = muestras[i];
		switch(i%4) {
			case 0:
				*salida = (uint32_t)muestra;
				break;
			case 1:
				*salida |= ((uint32_t)muestra<<8);
				break;
			case 2:
				*salida |= ((uint32_t)muestra<<16);
				break;
			case 3:
				*salida |= ((uint32_t)muestra<<24);
				salida++;
				break;
		}
	}
}

/*==================[external functions definition]==========================*/

int main(void)
{
	initHardware();

	*DWT_CTRL  |= 1;

	*DWT_CYCCNT = 0;
	empaquetar_c(muestras, 256, (uint32_t*)salida_c);

	*DWT_CYCCNT = 0;
	pack(muestras, 256, (uint32_t*)salida);

	while (1) {}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
