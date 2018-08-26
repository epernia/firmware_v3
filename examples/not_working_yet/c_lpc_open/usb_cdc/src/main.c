/* Copyright 2015, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of lpc1769_template.
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

/** @brief Ejemplo con FreeRTOS, USB, LCD 16x2.
 **
 **/

/** \addtogroup usb_cdc CDC+LCD+FreeRTOS example
 ** @{ */

/*==================[inclusions]=============================================*/

#include "board.h"
#include "main.h"
#include "cdc_vcom.h"
#include "lcd.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/** @brief cola para recibir caracteres desde el CDC y enviarlos al display */
xQueueHandle qLCD;

/** @brief semáforo para sincronizar el envío de un mensaje por el CDC desde
 * la interrupción de un pulsador.
 */
xSemaphoreHandle sem;

/*==================[internal functions definition]==========================*/

/** @brief Inicialización general del hardware
 *
 */
static void initHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, false);

    Chip_GPIOINT_Init(LPC_GPIOINT);
    Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, GPIOINT_PORT0, 1 << 18);
    NVIC_EnableIRQ(EINT3_IRQn);

    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 2, 9);
    Chip_GPIO_SetPinOutLow(LPC_GPIO, 2, 9);
}

/** @brief Tarea que destella el led del stick cada 500ms y prueba STDOUT
 *
 * @param p no utilizado
 */
static void ledTask(void * p)
{
	while (1)
	{
		Board_LED_Toggle(0);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

/** @brief Tarea que inicializa el LCD de 16x2 caracteres y espera
 * caracteres por una cola ajustando el contenido al tamaño del display.
 *
 * @param p no utilizado
 */
static void lcdTask(void * p)
{
	int count = 0;
	char c;

	vTaskDelay(100);
	LCD_Init();
	LCD_GoToxy(0, 0);
	LCD_Print("Hola!");
	LCD_GoToxy(0, 0);
	while(1)
	{
		xQueueReceive(qLCD, &c, portMAX_DELAY);
		if (c == 0x08) {
			LCD_Init();
			count = 0;
		}
		else if ((c == '\n') || (c == '\r')) {
			if (count < 16) {
				LCD_GoToxy(0, 1);
				count = 16;
			}
			else {
				LCD_GoToxy(0, 0);
				count = 0;
			}
		}
		else {
			LCD_SendChar(c);
			count++;
			if (count == 16) {
				LCD_GoToxy(0, 1);
			}
			if (count == 32) {
				LCD_GoToxy(0, 0);
				count = 0;
			}
		}
	}
}

/*==================[external functions definition]==========================*/

/** @brief entry point */
int main(void)
{
	initHardware();

	xTaskCreate(ledTask, (signed const char *)"led", 2048, 0, tskIDLE_PRIORITY+1, 0);
	xTaskCreate(cdcTask, (signed const char *)"cdc", 1024, 0, tskIDLE_PRIORITY+1, 0);
	xTaskCreate(lcdTask, (signed const char *)"lcd", 1024, 0, tskIDLE_PRIORITY+1, 0);

	qLCD = xQueueCreate(32, sizeof(char));
	vSemaphoreCreateBinary(sem);

	vTaskStartScheduler();

	while(1);
}

/** @brief interrupción externa del pulsador SW2 */
void EINT3_IRQHandler(void)
{
	static portBASE_TYPE xSwitchRequired;

	if (Chip_GPIOINT_GetStatusFalling(LPC_GPIOINT, GPIOINT_PORT0) & (1<<18)) {
		Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIOINT_PORT0, 1<<18);
		xSemaphoreGiveFromISR(sem, &xSwitchRequired);
	}
	portEND_SWITCHING_ISR(xSwitchRequired);
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
