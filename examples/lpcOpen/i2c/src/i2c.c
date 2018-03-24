/* Copyright 2015, Pablo Ridolfi
* All rights reserved.
*
 * This file is part of Workspace.
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

/** @brief This is a simple I2C example.
*
* Read and write to an I2C memory like the 24LC64 on LPCXpresso stick.
*/

/** \addtogroup i2c I2C Example
** @{ */

/*==================[inclusions]=============================================*/

#include "board.h"
#include "i2c.h"

/*==================[macros and definitions]=================================*/

/** i2c port used */
#ifdef lpc4337_m4
#define I2C_PORT I2C0
#else
#define I2C_PORT I2C1
#endif

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
*	@return none
*/
static void initHardware(void);

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
   // Read clock settings and update SystemCoreClock variable
   SystemCoreClockUpdate();
   // Set up and initialize all required blocks and
   // functions related to the board hardware
   Board_Init();
   // Set the LED to the state of "Off"
   Board_LED_Set(0, false);

   Board_I2C_Init(I2C_PORT);
   Chip_I2C_SetClockRate(I2C_PORT, 100000);
   Chip_I2C_SetMasterEventHandler(I2C_PORT, Chip_I2C_EventHandlerPolling);
}

/*==================[external functions definition]==========================*/

int main(void)
{
   uint8_t wbuf[3] = {0,0,0xAA};
   uint8_t rbuf[3] = {0,0,0};
   uint32_t i;
   I2C_XFER_T xfer;

   initHardware();

   /* Transferencia de escritura (escribo addr y datos, ojo, tamaño de página 32 bytes) */
   xfer.rxBuff = 0;
   xfer.rxSz = 0;
   xfer.slaveAddr = 0x50;
   xfer.status = 0;
   xfer.txBuff = wbuf;
   xfer.txSz = 3;

   Chip_I2C_MasterTransfer(I2C_PORT, &xfer);

   //delay por software calculado con el teorema de los cinco dedos oscilantes
   for(i=0; i<0xFFFF; i++);

   /* Transferencia de lectura (escribo addr, leo datos) */
   xfer.rxBuff = rbuf;
   xfer.rxSz = 1;
   xfer.slaveAddr = 0x50;
   xfer.status = 0;
   xfer.txBuff = wbuf;
   xfer.txSz = 2;

   Chip_I2C_MasterTransfer(I2C_PORT, &xfer);

   //delay por software calculado con el teorema de los cinco dedos oscilantes
   for(i=0; i<0xFFFF; i++);

   while(1);
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
