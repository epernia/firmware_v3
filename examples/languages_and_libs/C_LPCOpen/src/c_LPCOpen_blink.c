#include "board.h"

#define TICKRATE_HZ (1000)

static volatile uint32_t tick_ct = 0;

void SysTick_Handler(void)
{
   tick_ct++;
}

void delay(uint32_t tk)
{
   uint32_t end = tick_ct + tk;
   while(tick_ct < end)
      __WFI();
}

int main(void)
{
   SystemCoreClockUpdate();
   Board_Init();
   SysTick_Config(SystemCoreClock / TICKRATE_HZ);

   while (1) {
      Board_LED_Toggle(LED_2);
      delay(100);
      printf("Hola mundo at %d\r\n", tick_ct);
   }
}
