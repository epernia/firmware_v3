/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
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

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 2016-05-02   v0.0.1   ENP   First version
 */

/*==================[inclusions]=============================================*/

#include "sapi_spi.h"
#include "chip.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

bool_t spiConfig( spiMap_t spi ){

   bool_t retVal = TRUE;

   if( spi == SPI0 ){

  		/* Set up clock and power for SSP1 module */
		// Configure SSP SSP1 pins
		Chip_SCU_PinMuxSet(0xf, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // CLK0
		Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); // MISO1
		Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5)); // MOSI1

		Chip_SCU_PinMuxSet(0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // CS1 configured as GPIO
		Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);

      // Initialize SSP Peripheral
      Chip_SSP_Init( LPC_SSP1 );
      Chip_SSP_Enable( LPC_SSP1 );

   } else{
      retVal = FALSE;
   }

   return retVal;
}


bool_t spiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize ){

   bool_t retVal = TRUE;

   Chip_SSP_DATA_SETUP_T xferConfig;

	xferConfig.tx_data = NULL;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = buffer;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = bufferSize;

   if( spi == SPI0 ){
      Chip_SSP_RWFrames_Blocking( LPC_SSP1, &xferConfig );
   } else{
      retVal = FALSE;
   }

   return retVal;
}


bool_t spiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize){

   bool_t retVal = TRUE;

   Chip_SSP_DATA_SETUP_T xferConfig;

	xferConfig.tx_data = buffer;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = NULL;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = bufferSize;

   if( spi == SPI0 ){
      Chip_SSP_RWFrames_Blocking( LPC_SSP1, &xferConfig );
   } else{
      retVal = FALSE;
   }

   return retVal;
}


/*==================[ISR external functions definition]======================*/



/** @} doxygen end group definition */
/*==================[end of file]============================================*/
