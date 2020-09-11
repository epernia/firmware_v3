/*
 * @brief Common SystemInit function for LPC18xx/LPC43xx chips
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include <board.h>

void SystemInit(void)
{
   extern void * const g_pfnVectors[];
#if __CORTEX_M == 0U
   // ******************************
   // Modify CREG->M0APPMAP so that M0 looks in correct place
   // for its vector table when an exception is triggered.
   // Note that we do not use the CMSIS register access mechanism,
   // as there is no guarantee that the project has been configured
   // to use CMSIS.
   volatile unsigned int *pCREG_M0APPMAP = (volatile unsigned int *) 0x40043404;
   // CMSIS : CREG->M0APPMAP = <address of vector table>
   *pCREG_M0APPMAP = (unsigned int)g_pfnVectors;
#else
   SCB->VTOR = (unsigned int) &g_pfnVectors;

   if (SCB_GetFPUType() > 0)
      SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */

   /* Board specific SystemInit only in M4 */
   Board_SystemInit();
   Board_Init();
#endif
}
