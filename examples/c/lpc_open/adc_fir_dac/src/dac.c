/*
 * dac.c
 *
 *  Created on: Sep 4, 2013
 *      Author: Pablo
 */


#include "board.h"

/* P0.26 -> AOUT */
void dac_init(void)
{
	Chip_DAC_Init(LPC_DAC);
#ifdef lpc4337_m4
	Chip_DAC_ConfigDAConverterControl(LPC_DAC, DAC_DMA_ENA);
#endif
}

void dac_write(uint32_t v)
{
	Chip_DAC_UpdateValue(LPC_DAC, v);
}
