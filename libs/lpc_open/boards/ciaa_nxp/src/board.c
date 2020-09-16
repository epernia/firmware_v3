/*
    LPCOpen CIAA-NXP Board port
    Copyright 2018 Santiago Germino (royconejo@gmail.com)
    Copyright 2019 Eric Pernia (ericpernia@gmail.com)

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

    This source is a new LPCOpen board port for Edu-CIAA.

    It exposes the same API found in board_api.h, along with new functions for
    input handling (TEC_1 to TEC_4 buttons), getting ADC samples and such.

    Most functionality exposed on P1/P2 headers are initialized and configured
    based on original intend/label: ETHERNET, 232_RX/TX, SPI, GPIO, I2C & ADC.

    GPIO pins are configured as INPUT with pullups as if they were buttons.

    ETHERNET port requires a specific PONCHO with an ethernet Physical layer
    (PHY). As a result, there is no code ported to initialize that IC as it
    could be any. All there is here is MUX configuration to enable RMII and
    start the peripheral clock.

    External hardware is also required for CAN, 4 bit Character display (LCD
    port) and keyboard (port T_*).

    DEBUG UART (wired to FT2232 IC and exposed on the development PC as a
    USB serial port) is configured with this default settings: 115200, 8N1.

    But -as it happens with I2C, SPI, ADC, ect- its defaults can be configured
    at will before including board.h (please look at that file to know the
    corresponding #defines)

    Pin MUXING is configured on board_sysinit.c. While it is known the intend of
    that file is to configure a SDRAM interface before main(), I prefer to have
    the amount of mux configurations enclosed on that file alone, letting this
    one cleaner.

    ----------------------------------------------------------------------------

    Changelog:

    v0.9.1 - 20/4/2018, sgermino:
        Fixed LED state reversal, now On = true. Ethernet (RMII) not enabled by
        default, USE_RMII must be defined. Added DEBUG_UART_IRQ and DEBUG_UART_
        IRQHANDLER. CAN port not configured, stack not implemented: this will be
        addressed on a CESE 2018 final work.

    v0.9 - 4/4/2018, sgermino:
        First version. TODO: To be feature complete on a stock Edu-CIAA, it
        should be neccesary to implement RS-485 communication.
*/


#include "board.h"
#include "string.h"
#if defined(DEBUG_ENABLE) && !defined(DEBUG_UART)
#error "Definir DEBUG_UART como LPC_USART{numero de UART}"
#endif


// CHIP module configuration. Edu-CIAA clock is based on a 12 Mhz crystal.
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;


struct gpio_t {
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


static ADC_CHANNEL_T curADCChannel = 0xFF;


#define GPIO_LEDS_SIZE      (sizeof(GpioLeds) / sizeof(struct gpio_t))
#define GPIO_BUTTONS_SIZE   (sizeof(GpioButtons) / sizeof(struct gpio_t))
#define GPIO_PORTS_SIZE     (sizeof(GpioPorts) / sizeof(struct gpio_t))


static void Board_LED_Init()
{
   for (uint32_t i = 0; i < GPIO_LEDS_SIZE; ++i) {
      const struct gpio_t *io = &GpioLeds[i];
      Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, io->port, io->pin);
      Chip_GPIO_SetPinState(LPC_GPIO_PORT, io->port, io->pin, false);
   }
}


static void Board_TEC_Init()
{
   for (uint32_t i = 0; i < GPIO_BUTTONS_SIZE; ++i) {
      const struct gpio_t *io = &GpioButtons[i];
      Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, io->port, io->pin);
   }
}


static void Board_GPIO_Init()
{
   for (uint32_t i = 0; i < GPIO_PORTS_SIZE; ++i) {
      const struct gpio_t *io = &GpioPorts[i];
      Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, io->port, io->pin);
   }
}


static void Board_I2C_Init()
{
   Chip_I2C_Init(I2C0);
   Chip_SCU_I2C0PinConfig(BOARD_I2C_MODE);
   Chip_I2C_SetClockRate(I2C0, BOARD_I2C_SPEED);
}


static void Board_SPI_Init()
{
   Chip_SSP_Init(LPC_SSP1);
   Chip_SSP_Set_Mode(LPC_SSP1, BOARD_SPI_MODE);
   Chip_SSP_SetFormat(LPC_SSP1, BOARD_SPI_BITS, BOARD_SPI_FORMAT,
                      BOARD_SPI_POLARITY);
   Chip_SSP_SetBitRate(LPC_SSP1, BOARD_SPI_SPEED);
   Chip_SSP_Enable(LPC_SSP1);
}


static void Board_ADC_Init()
{
   ADC_CLOCK_SETUP_T cs;

   Chip_ADC_Init(LPC_ADC0, &cs);
   Chip_ADC_SetSampleRate(LPC_ADC0, &cs, BOARD_ADC_SAMPLE_RATE);
   Chip_ADC_SetResolution(LPC_ADC0, &cs, BOARD_ADC_RESOLUTION);
}


void Board_Debug_Init(void)
{
   Chip_UART_Init(DEBUG_UART);
   Chip_UART_SetBaudFDR(DEBUG_UART, DEBUG_UART_BAUD_RATE);
   Chip_UART_ConfigData(DEBUG_UART, DEBUG_UART_CONFIG);
   Chip_UART_TXEnable(DEBUG_UART);
}


void Board_UARTPutChar(char ch)
{
   while ( !(Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE));
   Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
}


int Board_UARTGetChar(void)
{
   if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
      return (int) Chip_UART_ReadByte(DEBUG_UART);
   }
   return EOF;
}


void Board_UARTPutSTR(const char *str)
{
   while (*str != '\0') {
      Board_UARTPutChar(*str++);
   }
}


void Board_LED_Set(uint8_t LEDNumber, bool On)
{
   if (LEDNumber >= GPIO_LEDS_SIZE) {
      return;
   }

   const struct gpio_t *io = &GpioLeds[LEDNumber];
   Chip_GPIO_SetPinState(LPC_GPIO_PORT, io->port, io->pin, On);
}


bool Board_LED_Test(uint8_t LEDNumber)
{
   if (LEDNumber >= GPIO_LEDS_SIZE) {
      return false;
   }

   const struct gpio_t *io = &GpioLeds[LEDNumber];
   return Chip_GPIO_GetPinState(LPC_GPIO_PORT, io->port, io->pin);
}


void Board_LED_Toggle(uint8_t LEDNumber)
{
   Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
}


void Board_Init(void)
{
   DEBUGINIT();
   Chip_GPIO_Init (LPC_GPIO_PORT);

   Board_GPIO_Init();
   Board_ADC_Init();
   Board_SPI_Init();
   Board_I2C_Init();

   Board_LED_Init();
   Board_TEC_Init();

#ifdef USE_RMII
   /* PHY_nRESET Signal in GPIO0 */
   Chip_SCU_PinMuxSet(0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC0));			/* P6_1 GPIO0 */
   Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);							/* GPIO3[0] = PHY_NRESET */
   /*** Reset PHY_NRESET ***/
   Chip_GPIO_SetPinState(LPC_GPIO_PORT, 3, 0, false);						/* GPIO3[0] output low */
   Chip_ENET_RMIIEnable(LPC_ETHERNET);
#endif

   // Read clock settings and update SystemCoreClock variable
   SystemCoreClockUpdate(); // @Eric
}


bool Board_TEC_GetStatus(uint8_t button)
{
   if (button >= GPIO_BUTTONS_SIZE) {
      return false;
   }

   return Chip_GPIO_GetPinState(LPC_GPIO_PORT, GpioButtons[button].port,
                                GpioButtons[button].pin);
}


void Board_ADC_ReadBegin(ADC_CHANNEL_T channel)
{
   if (channel < ADC_CH0 || channel > ADC_CH3) {
      return;
   }

   if (curADCChannel >= ADC_CH0 || curADCChannel <= ADC_CH3) {
      Chip_ADC_EnableChannel (LPC_ADC0, curADCChannel, DISABLE);
   }

   curADCChannel = channel;

   Chip_SCU_ADC_Channel_Config(0, channel);
   Chip_ADC_EnableChannel(LPC_ADC0, channel, ENABLE);
   Chip_ADC_SetBurstCmd(LPC_ADC0, DISABLE);
   Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
}


bool Board_ADC_ReadWait()
{
   return (Chip_ADC_ReadStatus(LPC_ADC0, curADCChannel, ADC_DR_DONE_STAT)
           == RESET);
}


uint16_t Board_ADC_ReadEnd()
{
   uint16_t data;

   if (Chip_ADC_ReadValue(LPC_ADC0, curADCChannel, &data) != SUCCESS) {
      data = 0xFFFF;
   }

   Chip_ADC_EnableChannel(LPC_ADC0, curADCChannel, DISABLE);
   curADCChannel = 0xFF;
   return data;
}


/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}


void __stdio_putchar(int c)
{
   Board_UARTPutChar(c);
}

int __stdio_getchar()
{
   return Board_UARTGetChar();;
}

void __stdio_init()
{
   Board_Debug_Init();
}
