/* Copyright 2016, Ian Olivieri
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

/* Date: 2016-02-10 */

#ifndef SAPI_SCT_H_
#define SAPI_SCT_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/

/*  SCT names are defined in sAPI_PeripheralMap.h:
 * NOTE: CTOUT11 has no SCT mode associated, so it can't be used!

typedef enum{
   CTOUT0, CTOUT1, CTOUT2, CTOUT3, CTOUT4, CTOUT5, CTOUT6, CTOUT7, CTOUT8,
   CTOUT9, CTOUT10, CTOUT11, CTOUT12, CTOUT13
} SctMap_t;
*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
/*
 * @brief:   Initialize the SCT peripheral with the given frequency
 * @param:   frequency:   value in Hz
 * @note:   there can only be 1 frequency in all the SCT peripheral.
 */
void Sct_Init(uint32_t frequency);

/*
 * @brief	Enables pwm function for the given pin
 * @param	sctNumber:   pin where the pwm signal will be generated
 */
void Sct_EnablePwmFor(uint8_t sctNumber);

/*
 * @brief   Converts a value in microseconds (uS = 1x10^-6 sec) to ticks
 * @param   value:   8bit value, from 0 to 255
 * @return   Equivalent in Ticks for the LPC4337
 */
uint32_t Sct_Uint8ToTicks(uint8_t value);

/*
 * @brief:   Sets the pwm duty cycle
 * @param:	sctNumber:   pin where the pwm signal is generated
 * @param	value:   8bit value, from 0 to 255
 * @note   For the 'ticks' parameter, see function Sct_Uint8ToTicks
 */
void Sct_SetDutyCycle(uint8_t sctNumber, uint8_t value);

/*
 * @brief:   Gets the pwm duty cycle
 * @param:	sctNumber:   pin where the pwm signal is generated
 * @return:   duty cycle of the channel, from 0 to 255
 */
uint8_t Sct_GetDutyCycle(uint8_t sctNumber);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* SAPI_SCT_H_ */
