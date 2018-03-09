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

/*The SCT (State Configurable Timer) is a feature included in some of LPC's microcontrollers
 * that provides a high resolution PWM (or just another timer).
 * It's like a normal timer but with multiple Compare Match values (16),
 * and can be therefore used to generate several PWM signals with THE SAME PERIOD
 * For more information about the STCPWM peripheral, refer to the Chapter 39 of
 * the LPC43xx user manual
 */


/*==================[inclusions]=============================================*/

#include "sapi_sct.h"

/* Specific modules used:
   #include "scu_18xx_43xx.h" for Chip_SCU funtions
   #include "sct_pwm_18xx_43xx.h" for Chip_SCTPWM funtions
*/

/*==================[macros and definitions]=================================*/

/* Because all pins have their CTOUT in the FUNC1 there is no need to
   save the same number for every pin in this case. */
#define CTOUT_FUNC   FUNC1
/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/*
 * List of ports and pins corresponding to the sct channels.
 * Each channel is asociated with a CTOUT number. Some pins, like
 * LED 1 and LCD1, have the same channel, so you can only generate 1 signal
 * for both. Because of that only one of them will be used.
 */
static pinConfigLpc4337_t SCTdataList[] =
{
/* Sct n° | port | pin | name in board */
/* CTOUT0 */ { 4 , 2 }, /* T_FIL2 */
/* CTOUT1 */ { 4 , 1 }, /* T_FIL1 */
/* CTOUT2 */ { 2 , 10 }, /* LED1 (also for LCD1) */
/* CTOUT3 */ { 4 , 3 }, /* T_FIL3 */
/* CTOUT4 */ { 2 , 12 }, /* LED3 (also for LCD3) */
/* CTOUT5 */ { 2 , 11 }, /* LED2 (also for LCD2) */
/* CTOUT6 */ { 6 , 5 }, /* GPIO2 */
/* CTOUT7 */ { 6 , 12 }, /* GPIO8 */
/* CTOUT8 */ { 1 , 3 }, /* MDC / SPI_MISO */
/* CTOUT9 */ { 1 , 4 }, /* SPI_MOSI */
/* CTOUT10 */ { 1 , 5 }, /* T_COL0 */
/* CTOUT11 */ { 0 , 0 }, /* DO NOT USE */
/* CTOUT12 */ { 7 , 5 }, /* T_COL2 */
/* CTOUT13 */ { 7 , 4 } /* T_COL1 */
};

/*Configuration data for LCD1, LCD2 and LCD3:
 CTOUT_2 { 4 , 4 }, LCD1
 CTOUT_5 { 4 , 5 }, LCD2
 CTOUT_4 { 4 , 6 }, LCD3
 */

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/*
 * @brief:   Initialize the SCT peripheral with the given frequency
 * @param:   frequency:   value in Hz
 * @note:   there can only be 1 frequency in all the SCT peripheral.
 */
void Sct_Init(uint32_t frequency)
{
   /* Source: https://www.lpcware.com/content/faq/how-use-sct-standard-pwm-using-lpcopen */
   /* Initialize the SCT as PWM and set frequency */
   Chip_SCTPWM_Init(LPC_SCT);
   Chip_SCTPWM_SetRate(LPC_SCT, frequency);

   Chip_SCTPWM_Start(LPC_SCT);
}

/*
 * @brief	Enables pwm function for the given pin
 * @param	sctNumber:   pin where the pwm signal will be generated
 */
void Sct_EnablePwmFor(uint8_t sctNumber)
{
   /*Enable SCT function on pin*/
   Chip_SCU_PinMux(SCTdataList[sctNumber].port , SCTdataList[sctNumber].pin , SCU_MODE_INACT , CTOUT_FUNC);
   /*Sets pin as PWM output and gives it an index (SCTdataList[sctNumber].mode+1)*/
   Chip_SCTPWM_SetOutPin(LPC_SCT, sctNumber+1, sctNumber);

   /* Start with 0% duty cycle */
   Sct_SetDutyCycle(sctNumber, Chip_SCTPWM_PercentageToTicks(LPC_SCT,0));
}

/*
 * @brief   Converts a value in microseconds (uS = 1x10^-6 sec) to ticks
 * @param   value:   8bit value, from 0 to 255
 * @return   Equivalent in Ticks for the LPC4337
 */
uint32_t Sct_Uint8ToTicks(uint8_t value)
{
   return ( (Chip_SCTPWM_GetTicksPerCycle(LPC_SCT) * value)/ 255 );
}


/*
 * @brief:   Sets the pwm duty cycle
 * @param:	sctNumber:   pin where the pwm signal is generated
 * @param	value:   8bit value, from 0 to 255
 * @note   For the 'ticks' parameter, see function Sct_Uint8ToTicks
 */
void Sct_SetDutyCycle(uint8_t sctNumber, uint8_t value)
{
   Chip_SCTPWM_SetDutyCycle(LPC_SCT, sctNumber+1, Sct_Uint8ToTicks(value));
}

/*
 * @brief:   Gets the pwm duty cycle
 * @param:	sctNumber:   pin where the pwm signal is generated
 * @return:   duty cycle of the channel, from 0 to 255
 */
 /* TODO: function not tested */
uint8_t Sct_GetDutyCycle(uint8_t sctNumber)
{
   uint8_t value = 0;

   value = (uint8_t) ((Chip_SCTPWM_GetDutyCycle(LPC_SCT, sctNumber+1)*255)/Chip_SCTPWM_GetTicksPerCycle(LPC_SCT));

   return value;
}

/*==================[end of file]============================================*/
