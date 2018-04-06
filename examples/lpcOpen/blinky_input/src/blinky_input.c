#include "board.h"
/*
    Ejemplo de uso de LPCOpen en Board Edu-CIAA
    Copyright (C) 2018 Santiago Germino.
    royconejo@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    ----------------------------------------------------------------------------

    [BTN_1] controla la velocidad de parpadeo de los leds: 50, 100 o 1000
            milisegundos de tiempo de encendido. Multiplicar por dos para el
            tiempo total del periodo de encendido + apagado.

    [BTN_2] selecciona uno de los 6 leds de la Edu-CIAA. El led RGB se toma como
            tres colores por separado.

    Una vez que se presiona un pulsador, para evitar falsas presiones causadas
    por el efecto "rebote" (señales involuntarias causados por el rebote
    a alta velocidad de las conexiones electricas internas del pulsador) es
    necesario implementar algun algoritmo de "antirebote".

    El objetivo es ignorar conexiones y desconexiones repetitivas del mismo
    boton en un lapso de 20 milisegundos desde la primer presion. Esto se
    implementa en la funcion buttonPressed ().

    El SysTick se configura para generar una interrupcion cada un milisegundo.

    __WFI () pone a dormir el CPU hasta la proxima interrupcion. En este caso,
    eso sera hasta que ocurra un nuevo milisegundo en el SysTick.

    ----------------------------------------------------------------------------

    Changelog:

    v1.0 - 6/4/2018, sgermino:
        Version inicial.
*/


volatile uint32_t g_ticks = 0;


void SysTick_Handler ()
{
    ++ g_ticks;
}


#define INVALID_LED             0xFF
#define INVALID_HALF_PERIOD     0xFFFFFFFF


const uint8_t Led[] =
{
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_1,
    LED_2,
    LED_3,
    INVALID_LED
};

const uint32_t HalfPeriod[] =
{
    50,
    100,
    1000,
    INVALID_HALF_PERIOD
};


static bool buttonPressed (uint8_t button, uint32_t *pressed)
{
    if (!Board_TEC_GetStatus (button))
    {
        if (! *pressed)
        {
            *pressed = g_ticks + 20;
            return true;
        }
    }
    else if (*pressed < g_ticks)
    {
        *pressed = 0;
    }
    return false;
}


int main (void)
{
    SystemCoreClockUpdate   ();
    Board_Init              ();
    SysTick_Config          (SystemCoreClock / 1000);

    uint32_t ledCurrentIndex    = 0;
    uint32_t halfPeriodIndex    = 0;
    uint32_t ledHalfPeriod      = HalfPeriod[halfPeriodIndex];
    uint32_t ledCurrentCount    = 0;
    uint32_t tec1Pressed        = 0;
    uint32_t tec2Pressed        = 0;

    while (1)
    {
        if (g_ticks >= ledCurrentCount + ledHalfPeriod)
        {
            ledCurrentCount = g_ticks + ledHalfPeriod;
        }

        Board_LED_Set (Led[ledCurrentIndex], ledCurrentCount > g_ticks);

        if (buttonPressed (BOARD_TEC_1, &tec1Pressed))
        {
            if ((ledHalfPeriod = HalfPeriod[++ halfPeriodIndex]) ==
                    INVALID_HALF_PERIOD)
            {
                ledHalfPeriod = HalfPeriod[halfPeriodIndex = 0];
            }

        }

        if (buttonPressed (BOARD_TEC_2, &tec2Pressed))
        {
            Board_LED_Set (Led[ledCurrentIndex], true);
            if (Led[++ ledCurrentIndex] == INVALID_LED)
            {
                ledCurrentIndex = 0;
            }
        }

        __WFI ();
    }

    return 0;
}
