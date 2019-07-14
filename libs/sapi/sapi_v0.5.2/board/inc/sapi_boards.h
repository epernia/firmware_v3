/* Copyright 2015, Eric Pernia.
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

#ifndef _SAPI_BOARDS_H_
#define _SAPI_BOARDS_H_

/*==================[inclusions]=============================================*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

// BOARDs macro definitions
#define ciaa_sim_ia32         1   // BOARD for ARCH=x86 CPUTYPE=ia32
#define ciaa_sim_ia64         2   // BOARD for ARCH=x86 CPUTYPE=ia64
#define ciaa_nxp              3   // BOARD for ARCH=cortexM4 CPUTYPE=lpc43xx
#define edu_ciaa_nxp          4   // BOARD for ARCH=cortexM4 CPUTYPE=lpc43xx
#define ciaa_fsl              5   // BOARD for ARCH=cortexM4 CPUTYPE=k60_120
#define ciaa_pic              6   // BOARD for ARCH=mips CPUTYPE=pic32
#define pico_ciaa             7   // BOARD for ARCH=cortexM4 CPUTYPE=lpc5410x
#define ciaa_leon3_fpga_nfp   8   // BOARD for ARCH=sparcV8 CPUTYPE=leon3nfp
#define ciaa_z3r0             9   // BOARD for ARCH=cortexM0plus CPUTYPE=egm32hg322
#define ciaa_7st             10   // BOARD for ARCH=cortexM7 CPUTYPE=?

/*
// Usage example:   
#if (BOARD == ciaa_sim_ia32)
   #error CIAA-SIM_IA32 not supported yet!
#elif (BOARD == ciaa_sim_ia64)
   #error CIAA-SIM_IA64 not supported yet!
#elif (BOARD == ciaa_nxp)
   #error CIAA-NXP not supported yet!
#elif (BOARD == edu_ciaa_nxp)
   #error EDU-CIAA-NXP not supported yet!
#elif (BOARD == ciaa_fsl)
   #error CIAA-FSL not supported yet!
#elif (BOARD == ciaa_pic)
   #error CIAA-PIC not supported yet!
#elif (BOARD == pico_ciaa)
   #error picoCIAA not supported yet!
#elif (BOARD == ciaa_leon3_fpga_nfp)
   #error CIAA-LEON3-FPGA-NFP not supported yet!
#elif (BOARD == ciaa_z3r0)
   #error CIAA-Z3R0 not supported yet!
#elif (BOARD == ciaa_7st)
   #error CIAA-7-ST not supported yet!
#endif   
*/

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_BOARDS_H_ */
