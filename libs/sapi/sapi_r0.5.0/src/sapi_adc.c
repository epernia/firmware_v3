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

#include "sapi_adc.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @brief:  enable/disable the ADC and DAC peripheral
 * @param:  ADC_ENABLE, ADC_DISABLE
 * @return: none
*/
void adcConfig( adcConfig_t config ){

   switch(config){

      case ADC_ENABLE: {

         /* Config ADC0 sample mode */
         /*
         ADC_CLOCK_SETUP_T ADCSetup = {
            400000,   // ADC rate
            10,       // ADC bit accuracy
            0         // ADC Burt Mode (true or false)
         };
         */
         ADC_CLOCK_SETUP_T ADCSetup;

         /* Initialized to default values:
		   *   - Sample rate:ADC_MAX_SAMPLE_RATE=400KHz
		   *   - resolution: ADC_10BITS
		   *   - burst mode: DISABLE */
         Chip_ADC_Init( LPC_ADC0, &ADCSetup );
         /* Disable burst mode */
         Chip_ADC_SetBurstCmd( LPC_ADC0, DISABLE );
         /* Set sample rate to 200KHz */
         Chip_ADC_SetSampleRate( LPC_ADC0, &ADCSetup, ADC_MAX_SAMPLE_RATE/2 );
         /* Disable all channels */
         Chip_ADC_EnableChannel( LPC_ADC0,ADC_CH1, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH1, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH2, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH2, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH3, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH3, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH4, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH4, DISABLE );
      }
      break;

      case ADC_DISABLE:
         /* Disable ADC peripheral */
         Chip_ADC_DeInit( LPC_ADC0 );
      break;
   }

}


/*
 * @brief   Get the value of one ADC channel. Mode: BLOCKING
 * @param   AI0 ... AIn
 * @return  analog value
 */
uint16_t adcRead( adcMap_t analogInput ){

   uint8_t lpcAdcChannel = 66 - analogInput;
   uint16_t analogValue = 0;

   Chip_ADC_EnableChannel(LPC_ADC0, lpcAdcChannel, ENABLE);
   Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

   while(
      (Chip_ADC_ReadStatus(LPC_ADC0, lpcAdcChannel, ADC_DR_DONE_STAT) != SET)
   );
   Chip_ADC_ReadValue( LPC_ADC0, lpcAdcChannel, &analogValue );

   Chip_ADC_EnableChannel( LPC_ADC0, lpcAdcChannel, DISABLE );

   return analogValue;
}

/*==================[end of file]============================================*/
