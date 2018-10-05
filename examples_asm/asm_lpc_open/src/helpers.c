/*
 * Helper functions replacing cmsis inlines to call from assembler.
 */
#include <chip.h>

void _NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
   NVIC_SetPriorityGrouping(PriorityGroup);
}

uint32_t _SysTick_Config(uint32_t ticks)
{
   return SysTick_Config(ticks);
}

