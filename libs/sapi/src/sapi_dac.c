/* Copyright 2016, Ian Olivieri
 * Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
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

/* Date: 2016-02-20 */

/*==================[inclusions]=============================================*/

#include "sapi_dac.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @brief:  enable/disable the ADC and DAC peripheral
 * @param:  DAC_ENABLE, DAC_DISABLE
 * @return: none
*/
void dacConfig( dacConfig_t config ){

   switch(config){

      case DAC_ENABLE:
         /* Initialize the DAC peripheral */
         Chip_DAC_Init(LPC_DAC);

         /* Enables the DMA operation and controls DMA timer */
         Chip_DAC_ConfigDAConverterControl(LPC_DAC, DAC_DMA_ENA);
                                                 /* DCAR DMA access */
         /* Update value to DAC buffer*/
         Chip_DAC_UpdateValue(LPC_DAC, 0);
      break;

      case DAC_DISABLE:
         /* Disable DAC peripheral */
         Chip_DAC_DeInit( LPC_DAC );
      break;
   }

}


/*
 * @brief   Write a value in the DAC.
 * @param   analogOutput: AO0 ... AOn
 * @param   value: analog value to be writen in the DAC, from 0 to 1023
 * @return  none
 */
void dacWrite( dacMap_t analogOutput, uint16_t value ){

   if( analogOutput == AO ){
      if( value > 1023 ){
         value = 1023;
      }
      Chip_DAC_UpdateValue( LPC_DAC, value );
   }
}

/*==================[end of file]============================================*/
