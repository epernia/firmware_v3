#include "board.h"
/*
    Ejemplo de uso de LPCOpen en Board Edu-CIAA
    Copyright 2018 Santiago Germino (royconejo@gmail.com)

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1.  Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2.  Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    3.  Neither the name of the copyright holder nor the names of its
        contributors may be used to endorse or promote products derived from
        this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    ----------------------------------------------------------------------------

    [BTN_1] toggles between different blinking half-periods: 50, 100 o 1000
            milliseconds.

    [BTN_2] toggles between one of six leds found on the Edu-CIAA. RGB led
            acts as three independent leds.

    When a button is pressed and to avoid false pushes caused by the "bounce"
    effect (spurious signals as a result of a high-speed bounce in the button
    electrical contacts) it is necessary to use a "debounce" algorithm.

    This is done by ignoring repetitive button presses in a 20 millisecond time
    lapse (look for buttonPressed()).

    SysTick is configured to interrupt every one millsecond.

    __WFI () puts the CPU to sleep until next interruption. In this case, it
    will wake up when a new milliscond ocurrs in the SysTick peripheral.

    ----------------------------------------------------------------------------

    Changelog:

    v1.0 - 6/4/2018, sgermino:
        First version.
*/


volatile uint32_t g_ticks = 0;


void SysTick_Handler()
{
    ++ g_ticks;
}


#define INVALID_LED             0xFF
#define INVALID_HALF_PERIOD     0xFFFFFFFF


const uint8_t Led[] = {
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_1,
    LED_2,
    LED_3,
    INVALID_LED
};

const uint32_t HalfPeriod[] = {
    50,
    100,
    1000,
    INVALID_HALF_PERIOD
};


static bool buttonPressed(uint8_t button, uint32_t *pressed)
{
    if (!Board_TEC_GetStatus(button)) {
        if (! *pressed) {
            *pressed = g_ticks + 20;
            return true;
        }
    }
    else if (*pressed < g_ticks) {
        *pressed = 0;
    }
    return false;
}


int main(void)
{
    SystemCoreClockUpdate();
    Board_Init();
    SysTick_Config(SystemCoreClock / 1000);

    uint32_t ledCurrentIndex    = 0;
    uint32_t halfPeriodIndex    = 0;
    uint32_t ledHalfPeriod      = HalfPeriod[halfPeriodIndex];
    uint32_t ledCurrentCount    = 0;
    uint32_t tec1Pressed        = 0;
    uint32_t tec2Pressed        = 0;

    while (1)
    {
        if (g_ticks >= ledCurrentCount + ledHalfPeriod) {
            ledCurrentCount = g_ticks + ledHalfPeriod;
        }

        Board_LED_Set(Led[ledCurrentIndex], ledCurrentCount > g_ticks);

        if (buttonPressed(BOARD_TEC_1, &tec1Pressed)) {
            if ((ledHalfPeriod = HalfPeriod[++ halfPeriodIndex]) ==
                INVALID_HALF_PERIOD) {
                ledHalfPeriod = HalfPeriod[halfPeriodIndex = 0];
            }
        }

        if (buttonPressed (BOARD_TEC_2, &tec2Pressed)) {
            Board_LED_Set (Led[ledCurrentIndex], true);
            if (Led[++ ledCurrentIndex] == INVALID_LED) {
                ledCurrentIndex = 0;
            }
        }

        __WFI ();
    }

    return 0;
}
