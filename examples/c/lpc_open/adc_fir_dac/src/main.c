#include "adc.h"
#include "dac.h"
#include "lowpass.h"
#include "bandpass.h"
#include "fir_q31.h"

volatile uint32_t * DWT_CTRL = (uint32_t *)0xE0001000;
volatile uint32_t * DWT_CYCCNT = (uint32_t *)0xE0001004;

fir_q31_t filtro;

#if defined(FILTRO_PASA_BANDA)
int history[BANDPASS_TAP_NUM];
#elif defined(FILTRO_PASA_BAJOS)
int history[LOWPASS_TAP_NUM];
#else
   #error Se debe definir un filtro: FILTRO_PASA_BANDA o FILTRO_PASA_BAJOS
#endif

int y[500];

static void initHardware(void)
{
#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "Off"
    Board_LED_Set(0, false);
#endif
#endif
   adc_init();
   dac_init();
}

int main(void)
{
	int i=0, offset;


#if defined(FILTRO_PASA_BANDA)
	fir_q31_init(&filtro, history, bandpass_taps, BANDPASS_TAP_NUM);
	offset=500;
#elif defined(FILTRO_PASA_BAJOS)
	fir_q31_init(&filtro, history, lowpass_taps, LOWPASS_TAP_NUM);
	offset=-100;
#else
   #error Se debe definir un filtro: FILTRO_PASA_BANDA o FILTRO_PASA_BAJOS
#endif

	initHardware();

	*DWT_CTRL |= 1;

	while(1)
	{
		if(adcFlag)
		{
			adcFlag=0;
			*DWT_CYCCNT=0; /* para medir tiempos de ejecucion */
#if defined(USAR_FUNCIONES_ASSEMBLER)
			y[i] = asm_fir_q31_get(&filtro)+offset;
			y[i] = __USAT(y[i],10);
#else
			y[i] = fir_q31_get(&filtro)+offset;
			if(y[i]>0x3FF) y[i] = 0x3FF;
			else if(y[i]<0) y[i] = 0;
#endif
			dac_write(y[i]);
			i++;
			if(i==500)
				i=0;
		}
	}
}
