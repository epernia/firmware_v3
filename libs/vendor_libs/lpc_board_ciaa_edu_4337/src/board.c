/*
    LPCOpen Edu-CIAA Board port
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

    Objetivo y alcance:

    Exponer funcionalidad basica de la placa Edu-CIAA en proyectos que, por
    algun motivo, solo utilizaran la libraria LPCOpen.

    Realizado segun la defincion de la API de Board en board_api.h. Se agregan
    funciones para manejar los botones TEC_1 a TEC_4, obtener samples de los
    canales ADC y demas.

    La mayoria de los pines expuestos en P1 y P2 se configuran e inician segun
    su funcion original: ETHERNET, 232_RX/TX, SPI, GPIO, I2C y ADC.

    Los pins GPIO se inician en direccion INPUT con pullups, tal como si fuesen
    pulsadores.

    El puerto ETHERNET de la Edu-CIAA requiere un poncho especifico que provea
    el "physical layer". Por ende, no existe en esta placa codigo para iniciar
    dicho integrado, sino simplemente la configuracion del MUX para activar
    los pines del puerto ETHERNET como RMII y el clock.

    Para CAN, display de caracteres con 4 bits de datos (puerto LCD*) y teclado
    (puerto T_*) tambien se requiere conectar ponchos o hardware externo.

    El puerto serie de DEBUG (conectado al FT2232 y expuesto como un puerto
    serie USB en la PC de desarrollo) se configura por defecto de la siguiente
    forma: 115200, 8N1. Aunque como pasa con I2C, SPI, ADC y demas, puede
    configurarse a gusto previo a la inclusion de board.h (ver ese archivo para
    conocer el nombre de los #defines) o bien puede reconfigurarse luego.

    El MUXING de los pines se configura en board_sysinit.c. Si bien el objetivo
    de ese archivo es configurar la interfaz a memoria SDRAM previo a la
    llamada a main(), en la Edu-CIAA su uso queda completamente desvirtuado. De
    todos modos valoro que board.c quede libre de setear listas y listas de
    pines, funciones, etc. que se configuran y usan solo en el inicio.

    ----------------------------------------------------------------------------

    Changelog:

    v0.9 - 4/4/2018, sgermino:
        Primer version. Para completar la funcionalidad basica de la placa
        seria necesario agregar funciones para enviar y recibir datos por
        RS-485.
*/


#include "board.h"
#if defined(DEBUG_ENABLE) && !defined(DEBUG_UART)
    #error "Definir DEBUG_UART como LPC_USART{numero de UART}"
#endif


// Configuracion del modulo CHIP. El clock de la Edu-CIAA utiliza un cristal de
// 12 Mhz.
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;


struct gpio_t
{
    uint8_t port;
    uint8_t pin;
};


static const struct gpio_t GpioLeds[] = {
    {5, 0}, {5, 1}, {5, 2}, {0, 14}, {1, 11}, {1, 12}
};


static const struct gpio_t GpioButtons[] = {
    {0, 4}, {0, 8}, {0, 9}, {1, 9}
};


static const struct gpio_t GpioPorts[] = {
    {3, 0}, {3, 3}, {3, 4}, {5,15}, {5,16}, {3, 5}, {3, 6}, {3, 7}, {2, 8}
};


static CHIP_ADC_CHANNEL curADCChannel = 0xFF;


#define GPIO_LEDS_SIZE      (sizeof(GpioLeds) / sizeof(struct gpio_t))
#define GPIO_BUTTONS_SIZE   (sizeof(GpioButtons) / sizeof(struct gpio_t))
#define GPIO_PORTS_SIZE     (sizeof(GpioPorts) / sizeof(struct gpio_t))


static void Board_LED_Init ()
{
    for (uint32_t i = 0; i < GPIO_LEDS_SIZE; ++i)
    {
        const struct io_port_t *io = &GpioLeds[i];
        Chip_GPIO_SetPinDIROutput   (LPC_GPIO_PORT, io->port, io->pin);
        Chip_GPIO_SetPinState       (LPC_GPIO_PORT, io->port, io->pin, false);
    }
}


static void Board_BTN_Init ()
{
    for (uint32_t i = 0; i < GPIO_BUTTONS_SIZE; ++i)
    {
        const struct io_port_t *io = &GpioButtons[i];
        Chip_GPIO_SetPinDIRInput (LPC_GPIO_PORT, io->port, io->pin);
    }
}


static void Board_GPIO_Init ()
{
    for (uint32_t i = 0; i < GPIO_PORTS_SIZE; ++i)
    {
        const struct io_port_t *io = &GpioPorts[i];
        Chip_GPIO_SetPinDIRInput (LPC_GPIO_PORT, io->port, io->pin);
    }
}


static void Board_I2C_Init ()
{
    Chip_I2C_Init           (I2C0);
    Chip_SCU_I2C0PinConfig  (BOARD_I2C_MODE);
    Chip_I2C_SetClockRate   (I2C0, BOARD_I2C_SPEED);
}


static void Board_SPI_Init ()
{
    Chip_SSP_Init           (LPC_SSP1);
    Chip_SSP_Set_Mode       (LPC_SSP1, BOARD_SPI_MODE);
    Chip_SSP_SetFormat      (LPC_SSP1, BOARD_SPI_BITS, BOARD_SPI_FORMAT,
                             BOARD_SPI_POLARITY);
    Chip_SSP_SetBitRate     (LPC_SSP1, BOARD_SPI_SPEED);
    Chip_SSP_Enable         (LPC_SSP1);
}


static void Board_ADC_Init ()
{
    ADC_CLOCK_SETUP_T cs;

    Chip_ADC_Init               (LPC_ADC0, &cs);
    Chip_ADC_Set_SampleRate     (LPC_ADC0, &cs, BOARD_ADC_SAMPLE_RATE);
    Chip_ADC_Set_Resolution     (LPC_ADC0, &cs, BOARD_ADC_RESOLUTION);
}


void Board_Debug_Init (void)
{
    Chip_UART_Init          (DEBUG_UART);
    Chip_UART_SetBaudFDR    (DEBUG_UART, DEBUG_UART_BAUD_RATE);
    Chip_UART_ConfigData    (DEBUG_UART, DEBUG_UART_CONFIG);
    Chip_UART_TXEnable      (DEBUG_UART);
}


void Board_UARTPutChar (char ch)
{
    while ( !(Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE));
    Chip_UART_SendByte (DEBUG_UART, (uint8_t) ch);
}


int Board_UARTGetChar (void)
{
    if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR)
    {
       return (int) Chip_UART_ReadByte (DEBUG_UART);
    }

    return EOF;
}


void Board_UARTPutSTR (const char *str)
{
    while (*str != '\0')
    {
        Board_UARTPutChar (*str++);
    }
}


void Board_LED_Set (uint8_t LEDNumber, bool On)
{
    if (LEDNumber >= GPIO_LEDS_SIZE)
    {
        return;
    }

    const struct io_port_t *io = &GpioLeds[LEDNumber];
    Chip_GPIO_SetPinState (LPC_GPIO_PORT, io->port, io->pin, !On);
}


bool Board_LED_Test (uint8_t LEDNumber)
{
    if (LEDNumber >= GPIO_LEDS_SIZE)
    {
        return false;
    }

    const struct io_port_t *io = &GpioLeds[LEDNumber];
    return !Chip_GPIO_GetPinState (LPC_GPIO_PORT, io->port, io->pin);
}


void Board_LED_Toggle (uint8_t LEDNumber)
{
    Board_LED_Set (LEDNumber, !Board_LED_Test(LEDNumber));
}


void Board_Init (void)
{
   DEBUGINIT();
   Chip_GPIO_Init       (LPC_GPIO_PORT);

   Board_LED_Init       ();
   Board_BTN_Init       ();
   Board_GPIO_Init      ();
   Board_I2C_Init       ();
   Board_ADC_Init       ();

   Chip_ENET_RMIIEnable (LPC_ETHERNET);
}


bool Board_BTN_GetStatus (uint8_t button)
{
   if (button >= GPIO_BUTTONS_SIZE)
   {
       return false;
   }

   return Chip_GPIO_GetPinState (LPC_GPIO_PORT, gpioButtons[button].port,
                                 gpioButtons[button].pin);
}


void Board_ADC_ReadBegin (CHIP_ADC_CHANNEL channel)
{
    if (channel < ADC_CH0 || channel > ADC_CH3)
    {
        return 0;
    }

    if (curADCChannel >= ADC_CH0 || curADCChannel <= ADC_CH3)
    {
        Chip_ADC_EnableChannel (LPC_ADC0, curADCChannel, DISABLE);
    }

    curADCChannel = channel;

    Chip_SCU_ADC_Channel_Config (0, channel);
    Chip_ADC_EnableChannel      (LPC_ADC0, channel, ENABLE);
    Chip_ADC_SetBurstCmd        (LPC_ADC0, DISABLE);
    Chip_ADC_SetStartMode       (LPC_ADC0, ADC_START_NOW,
                                 ADC_TRIGGERMODE_RISING);
}


bool Board_ADC_ReadWait ()
{
    return (Chip_ADC_ReadStatus (LPC_ADC0, curADCChannel, ADC_DR_DONE_STAT)
            == RESET);
}


uint16_t Board_ADC_ReadEnd ()
{
    uint16_t data;

    if (Chip_ADC_ReadValue (LPC_ADC0, curADCChannel, &dataADC) != SUCCESS)
    {
        data = 0xFFFF;
    }

    Chip_ADC_EnableChannel (LPC_ADC0, curADCChannel, DISABLE);
    curADCChannel = 0xFF;

    return data;
}
