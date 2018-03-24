/* Copyright 2015, Pablo Ridolfi
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

/** @brief PWM example with Timer.
 *
 * PWM signal on Board LED 0, period 1ms, duty between 10% and 90%.
 */

 /** \addtogroup pwm PWM example
 ** @{ */

/*==================[inclusions]=============================================*/

#include "board.h"
#include "main.h"

/*==================[macros and definitions]=================================*/

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
    SysTick_Config(SystemCoreClock/1000);
    Board_Init();
    Board_LED_Set(0, false);

   /* Timer */
   Chip_TIMER_Init(LPC_TIMER1);
   Chip_TIMER_PrescaleSet(LPC_TIMER1,
#ifdef lpc1769
         Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_TIMER1) / 1000000 - 1
#else
		 Chip_Clock_GetRate(CLK_MX_TIMER1) / 1000000 - 1
#endif
   );

   /* Match 0 (period) */
   Chip_TIMER_MatchEnableInt(LPC_TIMER1, 0);
   Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 0);
   Chip_TIMER_StopOnMatchDisable(LPC_TIMER1, 0);
   Chip_TIMER_SetMatch(LPC_TIMER1, 0, 1000);

   /* Match 1 (duty) */
   Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
   Chip_TIMER_ResetOnMatchDisable(LPC_TIMER1, 1);
   Chip_TIMER_StopOnMatchDisable(LPC_TIMER1, 1);
   Chip_TIMER_SetMatch(LPC_TIMER1, 1, 100);

   Chip_TIMER_Reset(LPC_TIMER1);
   Chip_TIMER_Enable(LPC_TIMER1);

   NVIC_EnableIRQ(TIMER1_IRQn);
}

static void pausems(uint32_t t)
{
   pausems_count = t;
   while(pausems_count != 0) {
      __WFI();
   }
}

/*==================[external functions definition]==========================*/

void SysTick_Handler(void)
{
   if (pausems_count != 0) {
      pausems_count--;
   }
}

void TIMER1_IRQHandler(void)
{
   if (Chip_TIMER_MatchPending(LPC_TIMER1, 0)) {
      Chip_TIMER_ClearMatch(LPC_TIMER1, 0);
      Board_LED_Set(0, 1);
   }
   if (Chip_TIMER_MatchPending(LPC_TIMER1, 1)) {
      Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
      Board_LED_Set(0, 0);
   }
}

int main(void)
{
   int duty = 100;

	initHardware();

   while(1) {
      pausems(1000);

      duty += 100;
      if(duty == 1000) duty = 100;

      Chip_TIMER_SetMatch(LPC_TIMER1, 1, duty);
      Chip_TIMER_Reset(LPC_TIMER1);
      Chip_TIMER_ClearMatch(LPC_TIMER1, 1);
      Chip_TIMER_ClearMatch(LPC_TIMER1, 0);
   }
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
