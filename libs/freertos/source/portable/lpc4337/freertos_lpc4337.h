// LPC4337

#ifndef __FREERTOS_LPC4337_H__
#define __FREERTOS_LPC4337_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <chip.h>

/* The interrupt number to use for the software interrupt generation.  This
 * could be any unused number.  In this case the first chip level (non system)
 * interrupt is used, which happens to be the DAC on the LPC4337 M4.  DAC_IRQHandler */
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
 * the DAC on the LPC4337 M4.  DAC_IRQHandler */
#define vSoftwareInterruptHandler (DAC_IRQHandler)

#ifdef __cplusplus
}
#endif

#endif // __FREERTOS_LPC4337_H__
