#include "chip.h"
#include "board.h"

#define TIME_INTERVAL   (1000)
static volatile bool On;

void RIT_IRQHandler(void)
{
	/* Clearn interrupt */
	Chip_RIT_ClearInt(LPC_RITIMER);

	/* Toggle LED */
	On = (bool) !On;
	Board_LED_Set(0, On);
}

int main( void )
{
	SystemCoreClockUpdate();

	On = true;
	Board_LED_Set(0, On);

	/* Initialize RITimer */
	Chip_RIT_Init(LPC_RITIMER);

	/* Configure RIT for a 1s interrupt tick rate */
	Chip_RIT_SetTimerInterval(LPC_RITIMER, TIME_INTERVAL);

	NVIC_EnableIRQ(RITIMER_IRQn);
    while (1) {
    	__WFI();
   	}
   	return 0;
}
