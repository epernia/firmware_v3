// LPC1769

#ifndef __FREERTOS_LPC1769_H__
#define __FREERTOS_LPC1769_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <chip.h>

/* The interrupt number to use for the software interrupt generation.  This
 * could be any unused number.  In this case the first chip level (non system)
 * interrupt is used, which happens to be the watchdog on the LPC1768.  WDT_IRQHandler */
#define mainSW_INTERRUPT_ID	(0)

/* The priority of the software interrupt.  The interrupt service routine uses
 * an (interrupt safe) FreeRTOS API function, so the priority of the interrupt must
 * be equal to or lower than the priority set by
 * configMAX_SYSCALL_INTERRUPT_PRIORITY - remembering that on the Cortex-M3 high
 * numeric values represent low priority values, which can be confusing as it is
 * counter intuitive. */
#define mainSOFTWARE_INTERRUPT_PRIORITY	(5)

/* The service routine for the interrupt.  This is the interrupt that the
 * task will be synchronized with.  void vSoftwareInterruptHandler(void);
 * the watchdog on the LPC1768 => WDT_IRQHandler */
#define vSoftwareInterruptHandler (WDT_IRQHandler)

#ifdef __cplusplus
}
#endif

#endif // __FREERTOS_LPC1769_H__
