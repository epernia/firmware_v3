/* Copyright 2015, Pablo Ridolfi
 *
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

#ifndef MPU_H_
#define MPU_H_

/*==================[inclusions]=============================================*/

#include "board.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/** MPU region sizes */
typedef enum _mpuRegionSize_e
{
	MPU_REGION_SIZE_RESERVED0,
	MPU_REGION_SIZE_RESERVED1,
	MPU_REGION_SIZE_RESERVED2,
	MPU_REGION_SIZE_RESERVED3,
	MPU_REGION_SIZE_32,
	MPU_REGION_SIZE_64,
	MPU_REGION_SIZE_128,
	MPU_REGION_SIZE_256,
	MPU_REGION_SIZE_512,
	MPU_REGION_SIZE_1k,
	MPU_REGION_SIZE_2k,
	MPU_REGION_SIZE_4k,
	MPU_REGION_SIZE_8k,
	MPU_REGION_SIZE_16k,
	MPU_REGION_SIZE_32k,
	MPU_REGION_SIZE_64k,
	MPU_REGION_SIZE_128k,
	MPU_REGION_SIZE_256k,
	MPU_REGION_SIZE_512k,
	MPU_REGION_SIZE_1M,
	MPU_REGION_SIZE_2M,
	MPU_REGION_SIZE_4M,
	MPU_REGION_SIZE_8M,
	MPU_REGION_SIZE_16M,
	MPU_REGION_SIZE_32M,
	MPU_REGION_SIZE_64M,
	MPU_REGION_SIZE_128M,
	MPU_REGION_SIZE_256M,
	MPU_REGION_SIZE_512M,
	MPU_REGION_SIZE_1G,
	MPU_REGION_SIZE_2G,
	MPU_REGION_SIZE_4G
}mpuRegionSize_e;

/** MPU permission types */
typedef enum _mpuAccessPermission_e
{
	MPU_AP_NO_ACCESS,
	MPU_AP_PRIVILEGED_ONLY,
	MPU_AP_USER_READ_ONLY,
	MPU_AP_FULL_ACCESS,
	MPU_AP_UNPREDICTABLE,
	MPU_AP_PRIVILEGED_READ_ONLY,
	MPU_AP_READ_ONLY_6,
	MPU_AP_READ_ONLY_7
}mpuAccessPermission_e;

/** MPU region attributes */
typedef enum _mpuMemoryAttributes_
{
	MPU_ATTR_ROM_tex_sCb 		= 0b000010,
	MPU_ATTR_PERIPH_tex_Scb		= 0b000100,
	MPU_ATTR_PERIPH_tex_ScB	  	= 0b000101,
	MPU_ATTR_INT_SRAM_tex_SCb 	= 0b000110,
	MPU_ATTR_EXT_SRAM_tex_SCB 	= 0b000111
}mpuMemoryAttributes;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @brief Setup and Enable a MPU region.
 * @param addr		Address
 * @param region	Region number
 * @param size		Region size
 * @param ap		Access permissions
 * @param attrib	Attributes
 * @param srd		Subregion Disable field
 * @param xn		eXecute Never flag
 * @param enable	Enable flag
 */
void mpuRegionSetup(unsigned int addr, unsigned int region,
		mpuRegionSize_e size, mpuAccessPermission_e ap, mpuMemoryAttributes attrib,
		unsigned int srd, unsigned int xn, unsigned int enable);

/** @brief Disable a MPU region.
 *
 * @param region 	Region to disable.
 */
void mpuRegionDisable(unsigned int region);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MPU_H_ */
