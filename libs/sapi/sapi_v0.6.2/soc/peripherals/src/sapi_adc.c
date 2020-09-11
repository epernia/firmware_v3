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
void adcInit( adcInit_t config )
{
   /*
   Pines ADC EDU-CIAA-NXP
   
               pin  func
   ADC_CH1 ---- 2   ADC0_1/ADC1_1
   ADC_CH2 ---- 143 ADC0_2/ADC1_2
   ADC_CH3 ---- 139 ADC0_3/ADC1_3
   DAC     ---- 6   ADC0_0/ADC1_0/DAC

   T_FIL1  ---- 3   ADC0_1 (ANALOG_SEL)
   T_COL2  ---- 133 ADC0_3 (ANALOG_SEL)

   LCD1    ---- 9   DAC (ANALOG_SEL)

   T_FIL3  ---- 7   ADC0_0 (ANALOG_SEL)
   T_COL1  ---- 132 ADC0_4 (ANALOG_SEL)
   ENET_MDC --- 140 ADC1_6 (ANALOG_SEL)
   */
   
   switch(config) {

      case ADC_ENABLE: {

         /* Config ADC0 sample mode */      
         ADC_CLOCK_SETUP_T ADCSetup = {
            ADC_MAX_SAMPLE_RATE,   // ADC Sample rate:ADC_MAX_SAMPLE_RATE = 400KHz
            10,                    // ADC resolution: ADC_10BITS = 10
            0                      // ADC Burst Mode: (true or false)
         };

         Chip_ADC_Init( LPC_ADC0, &ADCSetup );
         /* Disable burst mode */
         Chip_ADC_SetBurstCmd( LPC_ADC0, DISABLE );
         /* Set sample rate to 200KHz */
         Chip_ADC_SetSampleRate( LPC_ADC0, &ADCSetup, ADC_MAX_SAMPLE_RATE/2 );
         /* Disable all channels */
         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH1, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH1, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH2, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH2, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH3, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH3, DISABLE );

         Chip_ADC_EnableChannel( LPC_ADC0, ADC_CH4, DISABLE );
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH4, DISABLE );

         // For aditional ADC Inputs (Pablo Gomez)
         #if BOARD==edu_ciaa_nxp
         Chip_SCU_ADC_Channel_Config( 0, 4 );                      // Revisar codigo
         Chip_ADC_Int_SetChannelCmd( LPC_ADC0, ADC_CH5, DISABLE ); // Revisar codigo
         #endif
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
uint16_t adcRead( adcMap_t analogInput )
{
   uint8_t lpcAdcChannel = analogInput + 1;
   uint16_t analogValue = 0;

   // Enable channel
   Chip_ADC_EnableChannel(LPC_ADC0, lpcAdcChannel, ENABLE);

   // Start conversion
   Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

   // Wait for conversion complete
   while(
      (Chip_ADC_ReadStatus(LPC_ADC0, lpcAdcChannel, ADC_DR_DONE_STAT) != SET)
   );

   // Enable Read value
   Chip_ADC_ReadValue( LPC_ADC0, lpcAdcChannel, &analogValue );

   // Disable channel
   Chip_ADC_EnableChannel( LPC_ADC0, lpcAdcChannel, DISABLE );

   return analogValue;
}

/*==================[end of file]============================================*/
