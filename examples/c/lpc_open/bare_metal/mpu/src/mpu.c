/* Copyright 2015, Pablo Ridolfi
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

/** @brief MPU management functions.
 **
 **/

/** \addtogroup TD2 Técnicas Digitales II
 ** @{ */

/** @addtogroup Peripheral Access Layer
 * 	@{
 */

/*==================[inclusions]=============================================*/

#include "mpu.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

void mpuRegionSetup(unsigned int addr, unsigned int region,
		mpuRegionSize_e size, mpuAccessPermission_e ap, mpuMemoryAttributes attrib,
		unsigned int srd, unsigned int xn, unsigned int enable)
{
	MPU->CTRL = 0; // Disable MPU first

	// Setup procedure for each region
	MPU->RBAR = (addr & 0xFFFFFFE0) | (region & 0xF) | 0x10;
	MPU->RASR = ((xn & 0x1)<<28) | ((ap & 0x7)<<24) |
			((attrib & 0x3F)<<16) | ((srd&0xFF)<<8) |
			((size & 0x1F)<<1)| (enable & 0x1);

	MPU->CTRL = 5; 	// Enable MPU with Default memory map enabled

	return;
}

void mpuRegionDisable(unsigned int region)
{
	MPU->CTRL = 0; // Disable MPU first

	// Function to disable an unused region
	MPU->RBAR = (region & 0xF) | 0x10;
	MPU->RASR = 0; // disable

	MPU->CTRL = 5; 	// Enable MPU with Default memory map enabled

	return;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
