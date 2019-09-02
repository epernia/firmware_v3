/*
 * adc.c
 *
 *  Created on: Sep 4, 2013
 *      Author: Pablo
 */

#include "board.h"
#include "fir_q31.h"
#include "main.h"

int adcFlag=0;

#ifdef lpc4337_m4
#define LPC_ADC LPC_ADC0
#define ADC_IRQn ADC0_IRQn
#endif

/* P0.23 -> AD0 */
void adc_init(void)
{
	ADC_CLOCK_SETUP_T adc;

	Chip_ADC_Init(LPC_ADC, &adc);
	Chip_ADC_SetSampleRate(LPC_ADC, &adc, 88000);

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH1, ENABLE);
	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH1, ENABLE);
	Chip_ADC_SetBurstCmd(LPC_ADC, ENABLE);

	NVIC_EnableIRQ(ADC_IRQn);
}

#ifdef lpc4337_m4
void ADC0_IRQHandler(void)
#else
void ADC_IRQHandler(void)
#endif
{
	static uint16_t data;
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH1, &data);

#ifdef lpc4337_m4
	static uint32_t cont;
	cont++;
	if (cont == 4) {
		cont = 0;
#if defined(USAR_FUNCIONES_ASSEMBLER)
		asm_fir_q31_put(&filtro, data);
#else
		fir_q31_put(&filtro, data);
#endif
	}
#else
#if defined(USAR_FUNCIONES_ASSEMBLER)
	asm_fir_q31_put(&filtro, data>>2);
#else
	fir_q31_put(&filtro, data>>2);
#endif
#endif
	adcFlag=1;
}
