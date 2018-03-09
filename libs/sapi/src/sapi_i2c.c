/* Copyright 2016, Eric Pernia
 * Copyright 2016, Alejandro Permingeat.
 * Copyright 2016, Eric Pernia
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

 /*
  * Date:
  * 2016-05-02 Eric Pernia - Only define API
  * 2016-06-23 Alejandro Permingeat - First functional version
  * 2016-08-07 Eric Pernia - Improve names
  * 2016-09-10 Eric Pernia - Add unlimited buffer transfer
  * 2016-11-20 Eric Pernia - Software I2C
  */

/*==================[inclusions]=============================================*/

#include "sapi_i2c.h"
#include "sapi_gpio.h"
#include "sapi_delay.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

#if( I2C_SOFTWARE == 1 )

   static bool_t i2cSoftwareConfig( i2cMap_t i2cNumber, uint32_t clockRateHz );

   static bool_t i2cSoftwareRead( i2cMap_t  i2cNumber,
                                  uint8_t  i2cSlaveAddress,
                                  uint8_t* dataToReadBuffer,
                                  uint16_t dataToReadBufferSize,
                                  bool_t   sendWriteStop,
                                  uint8_t* receiveDataBuffer,
                                  uint16_t receiveDataBufferSize,
                                  bool_t   sendReadStop );

   static bool_t i2cSoftwareWrite( i2cMap_t  i2cNumber,
                                   uint8_t  i2cSlaveAddress,
                                   uint8_t* transmitDataBuffer,
                                   uint16_t transmitDataBufferSize,
                                   bool_t   sendWriteStop );

   static void i2cSoftwarePinConfig( gpioMap_t pin, uint8_t mode );
   static void i2cSoftwarePinWrite( gpioMap_t pin, bool_t value );
   static bool_t i2cSoftwarePinRead( gpioMap_t pin );

#else

   static bool_t i2cHardwareConfig( i2cMap_t i2cNumber, uint32_t clockRateHz );

   static bool_t i2cHardwareRead( i2cMap_t  i2cNumber,
                                  uint8_t  i2cSlaveAddress,
                                  uint8_t* dataToReadBuffer,
                                  uint16_t dataToReadBufferSize,
                                  bool_t   sendWriteStop,
                                  uint8_t* receiveDataBuffer,
                                  uint16_t receiveDataBufferSize,
                                  bool_t   sendReadStop );

   static bool_t i2cHardwareWrite( i2cMap_t  i2cNumber,
                                   uint8_t  i2cSlaveAddress,
                                   uint8_t* transmitDataBuffer,
                                   uint16_t transmitDataBufferSize,
                                   bool_t   sendWriteStop );

#endif

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

#if( I2C_SOFTWARE == 1 )

   static bool_t i2cSoftwareConfig( i2cMap_t i2cNumber, uint32_t clockRateHz ){

      bool_t retVal = TRUE;

      i2cSoftwarePinConfig( I2C_SOFTWARE_SDA_DIR, GPIO_INPUT_PULLUP );
      i2cSoftwarePinConfig( I2C_SOFTWARE_SCL_DIR, GPIO_INPUT_PULLUP );

      return retVal;
   }

   static bool_t i2cSoftwareRead( i2cMap_t  i2cNumber,
                                  uint8_t  i2cSlaveAddress,
                                  uint8_t* dataToReadBuffer,
                                  uint16_t dataToReadBufferSize,
                                  bool_t   sendWriteStop,
                                  uint8_t* receiveDataBuffer,
                                  uint16_t receiveDataBufferSize,
                                  bool_t   sendReadStop ){

      bool_t retVal = TRUE;
      uint16_t i = 0;

      // Check Errors
      if( (dataToReadBuffer == NULL)  || (dataToReadBufferSize < 0) ||
          (receiveDataBuffer == NULL) || (receiveDataBufferSize <= 0) ){
         return FALSE;
      }

      // First Write

      if( dataToReadBufferSize > 0 ){
         retVal &= i2cSoftwareWrite( i2cNumber,
                                     i2cSlaveAddress,
                                     dataToReadBuffer,
                                     dataToReadBufferSize,
                                     sendWriteStop );
      }

      // Then Read

      // Start condition
      i2cSoftwareMasterWriteStart();
      // 7 bit address + Read = 1
      i2cSoftwareMasterWriteAddress( i2cSlaveAddress, I2C_SOFTWARE_READ );
      // Write all data buffer
      for( i=0; i<receiveDataBufferSize; i++ ){
         receiveDataBuffer[i] = i2cSoftwareMasterReadByte( TRUE ); // TRUE send ACK, FALSE not
      }
      // Send Stop condition
      if( sendReadStop ){
         i2cSoftwareMasterWriteStop();
      }
      return retVal;
   }

   static bool_t i2cSoftwareWrite( i2cMap_t  i2cNumber,
                                   uint8_t  i2cSlaveAddress,
                                   uint8_t* transmitDataBuffer,
                                   uint16_t transmitDataBufferSize,
                                   bool_t   sendWriteStop ){

      bool_t retVal = TRUE;
      uint16_t i = 0;

      // Check Errors
      if( (transmitDataBuffer == NULL) || (transmitDataBufferSize <= 0) ){
         return FALSE;
      }
      // Start condition
      i2cSoftwareMasterWriteStart();
      // 7 bit address + Write = 0
      i2cSoftwareMasterWriteAddress( i2cSlaveAddress, I2C_SOFTWARE_WRITE );
      // Write all data buffer
      for( i=0; i<transmitDataBufferSize; i++ ){
         i2cSoftwareMasterWriteByte( transmitDataBuffer[i] );
      }
      // Send Stop condition
      if(sendWriteStop){
         i2cSoftwareMasterWriteStop();
      }

      return retVal;
   }


   // Point of contact with sapi_gpio module

   static void i2cSoftwarePinConfig( uint8_t pin, uint8_t mode ){

      if( pin == I2C_SOFTWARE_SDA_DIR ){
         if( mode == GPIO_OUTPUT ){
            //IO_DIR_PORT_PIN( OCM_DATA_PORT, OCM_DATA_PIN, IO_OUT );
            gpioConfig( I2C_SOFTWARE_SDA_DIR, GPIO_OUTPUT );
         }else if( mode == GPIO_INPUT_PULLUP ){
            // Seteo de pines como ENTRADA
            //IO_DIR_PORT_PIN( OCM_DATA_PORT, OCM_DATA_PIN, IO_IN );
            // Seteo de pines con pull-up
            //IO_PUD_PORT( OCM_DATA_PORT, IO_PUP );
            gpioConfig( I2C_SOFTWARE_SDA_DIR, GPIO_INPUT );
         }
      }else if( pin == I2C_SOFTWARE_SCL_DIR ){
         if( mode == GPIO_OUTPUT ){
            //IO_DIR_PORT_PIN( OCM_CLK_PORT, OCM_CLK_PIN, IO_OUT );
            gpioConfig( I2C_SOFTWARE_SCL_DIR, GPIO_OUTPUT );
         }else if( mode == GPIO_INPUT_PULLUP ){
            // Seteo de pines como ENTRADA
            //IO_DIR_PORT_PIN( OCM_CLK_PORT, OCM_CLK_PIN, IO_IN );
            // Seteo de pines con pull-up
            //IO_PUD_PORT( OCM_CLK_PORT, IO_PUP );
            gpioConfig( I2C_SOFTWARE_SCL_DIR, GPIO_INPUT );
         }
      }

   }
   static void i2cSoftwarePinWrite( uint8_t pin, bool_t value ){

      if( pin == I2C_SOFTWARE_SDA_OUT ){
         gpioWrite( I2C_SOFTWARE_SDA_OUT, value );
      }else if( pin == I2C_SOFTWARE_SCL_OUT ){
         if(value){
            //IO_DIR_PORT_PIN( OCM_CLK_PORT, OCM_CLK_PIN, IO_IN );
            gpioConfig( I2C_SOFTWARE_SCL_DIR, GPIO_INPUT );
            while( !gpioRead( I2C_SOFTWARE_SCL_IN ) );   // Espera hasta que el clock este en alto
            i2cSoftwareDelay(1); // 1 clock time delay
         }else{
            //IO_DIR_PORT_PIN( OCM_CLK_PORT, OCM_CLK_PIN, IO_OUT );
            //OCM_SCL = 0;                //Setea el clock a LOW
            gpioConfig( I2C_SOFTWARE_SCL_DIR, GPIO_OUTPUT );
            gpioWrite( I2C_SOFTWARE_SCL_OUT, LOW );
         }
         // 1 clock time delay
         i2cSoftwareDelay(1);
      }
   }


   static bool_t i2cSoftwarePinRead( uint8_t pin ){

      bool_t retVal = 0;

      retVal = gpioRead( (int8_t)pin );
      return retVal;
   }
#else

   static bool_t i2cHardwareConfig( i2cMap_t i2cNumber, uint32_t clockRateHz ){

      // Configuracion de las lineas de SDA y SCL de la placa
      Chip_SCU_I2C0PinConfig( I2C0_STANDARD_FAST_MODE );

      // Inicializacion del periferico
      Chip_I2C_Init( i2cNumber );
      // Seleccion de velocidad del bus
      Chip_I2C_SetClockRate( i2cNumber, clockRateHz );
      // Configuracion para que los eventos se resuelvan por polliong
      // (la otra opcion es por interrupcion)
      Chip_I2C_SetMasterEventHandler( i2cNumber, Chip_I2C_EventHandlerPolling );

      return TRUE;
   }

   static bool_t i2cHardwareRead( i2cMap_t  i2cNumber,
                                  uint8_t  i2cSlaveAddress,
                                  uint8_t* dataToReadBuffer,
                                  uint16_t dataToReadBufferSize,
                                  bool_t   sendWriteStop,
                                  uint8_t* receiveDataBuffer,
                                  uint16_t receiveDataBufferSize,
                                  bool_t   sendReadStop ){

   //TODO: ver i2cData.options si se puede poner la condicion opcional de stop

      I2CM_XFER_T i2cData;

      i2cData.slaveAddr = i2cSlaveAddress;
      i2cData.options   = 0;
      i2cData.status    = 0;
      i2cData.txBuff    = dataToReadBuffer;
      i2cData.txSz      = dataToReadBufferSize;
      i2cData.rxBuff    = receiveDataBuffer;
      i2cData.rxSz      = receiveDataBufferSize;

      if( Chip_I2CM_XferBlocking( LPC_I2C0, &i2cData ) == 0 ) {
         return FALSE;
      }

      return TRUE;
   }

   static bool_t i2cHardwareWrite( i2cMap_t  i2cNumber,
                                   uint8_t  i2cSlaveAddress,
                                   uint8_t* transmitDataBuffer,
                                   uint16_t transmitDataBufferSize,
                                   bool_t   sendWriteStop ){

      //TODO: ver i2cData.options si se puede poner la condicion opcional de stop

      I2CM_XFER_T i2cData;

      if( i2cNumber != I2C0 ){
         return FALSE;
      }

      // Prepare the i2cData register
      i2cData.slaveAddr = i2cSlaveAddress;
      i2cData.options   = 0;
      i2cData.status    = 0;
      i2cData.txBuff    = transmitDataBuffer;
      i2cData.txSz      = transmitDataBufferSize;
      i2cData.rxBuff    = 0;
      i2cData.rxSz      = 0;

      /* Send the i2c data */
      if( Chip_I2CM_XferBlocking( LPC_I2C0, &i2cData ) == 0 ){
         return FALSE;
      }

      /* *** TEST I2C Response ***

      Chip_I2CM_XferBlocking( LPC_I2C0, &i2cData );

      if( i2cData.status == I2CM_STATUS_OK){
         while(1){
            gpioWrite( LEDB, ON );
            delay(100);
            gpioWrite( LEDB, OFF );
            delay(100);
         }
      }

    *** END - TEST I2C Response *** */

      return TRUE;
   }

#endif


/*==================[external functions definition]==========================*/

bool_t i2cConfig( i2cMap_t i2cNumber, uint32_t clockRateHz ){

   bool_t retVal = FALSE;

   if( i2cNumber != I2C0 ){
      return FALSE;
   }

   #if( I2C_SOFTWARE == 1 )
      retVal = i2cSoftwareConfig( i2cNumber, clockRateHz );
   #else
      retVal = i2cHardwareConfig( i2cNumber, clockRateHz );
   #endif

   return retVal;
}


bool_t i2cRead( i2cMap_t  i2cNumber,
                uint8_t  i2cSlaveAddress,
                uint8_t* dataToReadBuffer,
                uint16_t dataToReadBufferSize,
                bool_t   sendWriteStop,
                uint8_t* receiveDataBuffer,
                uint16_t receiveDataBufferSize,
                bool_t   sendReadStop ){

   bool_t retVal = FALSE;

   if( i2cNumber != I2C0 ){
      return FALSE;
   }

   #if( I2C_SOFTWARE == 1 )
      retVal = i2cSoftwareRead( i2cNumber,
                                i2cSlaveAddress,
                                dataToReadBuffer,
                                dataToReadBufferSize,
                                sendWriteStop,
                                receiveDataBuffer,
                                receiveDataBufferSize,
                                sendReadStop );
   #else
      retVal = i2cHardwareRead( i2cNumber,
                                i2cSlaveAddress,
                                dataToReadBuffer,
                                dataToReadBufferSize,
                                sendWriteStop,
                                receiveDataBuffer,
                                receiveDataBufferSize,
                                sendReadStop );
   #endif

   return retVal;
}


bool_t i2cWrite( i2cMap_t  i2cNumber,
                 uint8_t  i2cSlaveAddress,
                 uint8_t* transmitDataBuffer,
                 uint16_t transmitDataBufferSize,
                 bool_t   sendWriteStop ){

   bool_t retVal = FALSE;

   if( i2cNumber != I2C0 ){
      return FALSE;
   }

   #if( I2C_SOFTWARE == 1 )
      retVal = i2cSoftwareWrite( i2cNumber,
                                 i2cSlaveAddress,
                                 transmitDataBuffer,
                                 transmitDataBufferSize,
                                 sendWriteStop );
   #else
      retVal = i2cHardwareWrite( i2cNumber,
                                 i2cSlaveAddress,
                                 transmitDataBuffer,
                                 transmitDataBufferSize,
                                 sendWriteStop );
   #endif

   return retVal;
}


#if( I2C_SOFTWARE == 1 )
   // Software Master I2C

   void i2cSoftwareDelay( tick_t duration ){
      volatile tick_t i;

      duration = 13 * duration;
      for( i=duration; i>0; i-- );
   }

   // Ver!!!
   // communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
   //       _____________________________________________________         ________
   // DATA:                                                      |_______|
   //          _    _    _    _    _    _    _    _    _        ___     ___
   // SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______


   // Generates a transmission start bit sequence
   //      ________
   // SCL:         |_
   //      _____
   // SDA:      |____
   //
   void i2cSoftwareMasterWriteStart( void ){

      // Clock (SCL) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
      // Data (SDA) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, HIGH );
      // 1 clock time delay
      i2cSoftwareDelay(10);

      // Data (SDA) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, LOW );
      // 1/2 clock time delay
      i2cSoftwareDelay(5);

      // Clock (SCL) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
      // 3/10 clock time delay
      i2cSoftwareDelay(3);
   }

   // Generates a transmission stop bit sequence
   //        ________
   // SCL: _|
   //           _____
   // SDA: ____|

   void i2cSoftwareMasterWriteStop( void ){
      // Data (SDA) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, LOW );
      // Clock (SCL) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
      // 1/5 clock time delay
      i2cSoftwareDelay(2);

      // Clock (SCL) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
      // 1/2 clock time delay
      i2cSoftwareDelay(5);

      // Data (SDA) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, HIGH );
      // 1 clock time delay
      i2cSoftwareDelay(10);
   }

   // Write data byte
   //          ___     ___     ___     ___     ___     ___     ___     ___     ___
   // SCL: ___| 1 |___| 2 |___| 3 |___| 4 |___| 5 |___| 6 |___| 7 |___| 8 |___| 9 |___
   //         _______ _______ _______ _______ _______ _______ _______ _______
   // SDA: __|   D7  |   D6  |   D5  |   D4  |   D3  |   D2  |   D1  |   D0  |__ACK?__
   //
   bool_t i2cSoftwareMasterWriteByte( uint8_t dataByte ){

      uint8_t i;
      static bool_t ackOrNack;

      for( i=8; i>0; i-- ) {

         // Clock (SCL) pin LOW
         i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
         // Data (SDA) pin with MSB bit value of dataByte
         i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, (dataByte & 0x80) );
         // 1/5 clock time delay
         i2cSoftwareDelay(2);

         // Clock (SCL) pin HIGH
         i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
         // 1/2 clock time delay
         i2cSoftwareDelay(5);

         // Clock (SCL) pin LOW
         i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
         // 3/10 clock time delay
         i2cSoftwareDelay(3);

         // left shift dataByte
         dataByte <<= 1;
     }

      // Maintain SCL LOW for 1/10 clock time delay
      i2cSoftwareDelay(1);
      // Configure SDA pin as input
      i2cSoftwarePinConfig( I2C_SOFTWARE_SDA_DIR, GPIO_INPUT_PULLUP );
      // Maintain SCL LOW for 1/10 clock time delay more
      i2cSoftwareDelay(1);

      // Clock (SCL) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
      // 1/10 clock time delay
      i2cSoftwareDelay(1);
      // Read Data (SDA) pin for possible ACK bit
      ackOrNack = i2cSoftwarePinRead( I2C_SOFTWARE_SDA_IN );
      // 2/5 clock time delay
      i2cSoftwareDelay(4);

      // Clock (SCL) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
      // 1/5 clock time delay
      i2cSoftwareDelay(2);
      // Configure SDA pin as output. This prevent that SCL master, the
      // microcontroller, and SCL Slave, device, both OUTPUT at the same time.
      // This Output-Output condition can damage devices.
      i2cSoftwarePinConfig( I2C_SOFTWARE_SDA_DIR, GPIO_OUTPUT );
      // 1/10 clock time delay
      i2cSoftwareDelay(1);

     return ackOrNack;
   }

   // Read data byte
   //          ___     ___     ___     ___     ___     ___     ___     ___     ___
   // SCL: ___| 1 |___| 2 |___| 3 |___| 4 |___| 5 |___| 6 |___| 7 |___| 8 |___| 9 |___
   //         _______ _______ _______ _______ _______ _______ _______ _______
   // SDA: __|   D7  |   D6  |   D5  |   D4  |   D3  |   D2  |   D1  |   D0  |__ACK?__
   //
   uint8_t i2cSoftwareMasterReadByte( bool_t ack ){

      uint8_t i, receivedData = 0;
      bool_t receivedBit = 0;

      // Configure SDA pin as input
      i2cSoftwarePinConfig( I2C_SOFTWARE_SDA_DIR, GPIO_INPUT_PULLUP );

      for( i=8; i>0; i-- ) {

         // Clock (SCL) pin LOW
         i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
         // 1/5 clock time delay
         i2cSoftwareDelay(2);

         //do{
            // Clock (SCL) pin HIGH
            i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
         //}while( SCL_IN == 0 );    // wait for any SCL clock stretching
         // 1/10 clock time delay
         i2cSoftwareDelay(1);
         // Read Data (SDA) pin
         receivedBit = i2cSoftwarePinRead( I2C_SOFTWARE_SDA_IN );
         // 2/5 clock time delay
         i2cSoftwareDelay(4);

         // Shift left receivedData
         receivedData <<= 1;

         if( receivedBit ){
            receivedData |= 0x01;
         }

         // Clock (SCL) pin LOW
         i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
         // 3/10 clock time delay
         i2cSoftwareDelay(3);
      }

      // Maintain SCL LOW for 1/10 clock time delay
      i2cSoftwareDelay(1);
      // Configure SDA pin as output
      i2cSoftwarePinConfig( I2C_SOFTWARE_SDA_DIR, GPIO_OUTPUT );

      // send (N)ACK bit (ACK=LOW, NACK=HIGH)
      if( ack ){
         // Data (SDA) pin LOW
         i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, LOW );
      }else{
         // Data (SDA) pin HIGH
         i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, HIGH );
      }
      // Maintain SCL LOW for 1/10 clock time delay more
      i2cSoftwareDelay(1);

      // Clock (SCL) pin HIGH
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, HIGH );
      // 1/2 clock time delay
      i2cSoftwareDelay(5);

      // Clock (SCL) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, LOW );
      // 1/10 clock time delay
      i2cSoftwareDelay(1);
      // Data (SDA) pin LOW
      i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, LOW );
      // 1/5 clock time delay
      i2cSoftwareDelay(2);

      return receivedData;
   }
   /* That's almost it for simple I2C communications, but there is one more
    * complication. When the master is reading from the slave, its the slave that
    * places the data on the SDA line, but its the master that controls the clock.
    * What if the slave is not ready to send the data! With devices such as
    * EEPROMs this is not a problem, but when the slave device is actually a
    * microprocessor with other things to do, it can be a problem. The
    * microprocessor on the slave device will need to go to an interrupt routine,
    * save its working registers, find out what address the master wants to read
    * from, get the data and place it in its transmission register. This can take
    * many uS to happen, meanwhile the master is blissfully sending out clock
    * pulses on the SCL line that the slave cannot respond to. The I2C protocol
    * provides a solution to this: the slave is allowed to hold the SCL line low!
    * This is called clock stretching. When the slave gets the read command from
    * the master it holds the clock line low. The microprocessor then gets the
    * requested data, places it in the transmission register and releases the
    * clock line allowing the pull-up resistor to finally pull it high. From the
    * masters point of view, it will issue the first clock pulse of the read by
    * making SCL high and then check to see if it really has gone high. If its
    * still low then its the slave that holding it low and the master should wait
    * until it goes high before continuing. Luckily the hardware I2C ports on
    * most microprocessors will handle this automatically.
    *
    * Sometimes however, the master I2C is just a collection of subroutines and
    * there are a few implementations out there that completely ignore clock
    * stretching. They work with things like EEPROM's but not with microprocessor
    * slaves that use clock stretching. The result is that erroneous data is read
    * from the slave. Beware!
    *
    * http://www.robot-electronics.co.uk/i2c-tutorial
    */


   // Write 7 bit address + R or W bit
   //              ___
   // SDA: _______|
   //          _______
   // SCL: ___|
   //
   bool_t i2cSoftwareMasterWriteAddress( uint8_t i2cSlaveAddress,
                                         I2C_Software_rw_t readOrWrite ){

      bool_t ackOrNack = FALSE;

      if( readOrWrite == I2C_SOFTWARE_WRITE ){
         // 7 bit address + Write = 0
         i2cSlaveAddress <<= 1;
         ackOrNack = i2cSoftwareMasterWriteByte( i2cSlaveAddress );

      } else if( readOrWrite == I2C_SOFTWARE_READ ){
         // 7 bit address + Read = 1
         i2cSlaveAddress <<= 1;
         i2cSlaveAddress |= 0x01;
         ackOrNack = i2cSoftwareMasterWriteByte( i2cSlaveAddress );
      }

      return ackOrNack;
   }
#endif


#if( SOFTWARE_I2C_DEBUG == 1 )

   /*
    * Conexión:
    *
    * Se debe conectar un led al pin elegido como SCL con una R de 470ohm.
    * Otro led al pin elegido como SDA con una R de 470ohm.
    * Una R de pull-up de 4K7 entre VDD=+3.3V y SDA.
    * Un pulsador entre GND y SDA.
    *
    * Funcionamiento:
    *
    * Cada 10 segundos toglea el modo del pin SDA entre Input y Output.
    * Mientras SDA es input escribe el valor del pulsador en el pin SCL.
    * Cada 500ms se toglea una variable llamada clockStatus.
    * Mientras el pin SDA es Output se escribe en el led conectado a SDA
    * el valor de la variable clockStatus.
    */

    //#include "sapi_delay.h"

    // Test vars
    bool_t clockStatus = FALSE;
    delay_t clockDelay;
    bool_t direction = FALSE;
    delay_t delayDir;

    void i2cSoftwareMasterPinTestConfig( void ){
       delayConfig( &clockDelay, 500 );
       delayConfig( &delayDir, 10000 );
    }

    void i2cSoftwareMasterPinTest( void ){

       if( delayRead( &delayDir ) ){
          if( direction ){
             direction = FALSE;
          } else{
             direction = TRUE;
          }
//          I2C_SOFTWARE_SDA_DIR = direction;
       }

       if( delayRead( &clockDelay ) ){
          if( clockStatus ){
             clockStatus = FALSE;
          } else{
             clockStatus = TRUE;
          }
          //I2C_SOFTWARE_SCL_OUT = clockStatus;
       }

       if( direction ){ // Input
          //I2C_SOFTWARE_SCL_OUT = I2C_SOFTWARE_SDA_IN;
          i2cSoftwarePinWrite( I2C_SOFTWARE_SCL_OUT, I2C_SOFTWARE_SDA_IN );
       } else{          // Output
          //I2C_SOFTWARE_SDA_OUT = clockStatus;
          i2cSoftwarePinWrite( I2C_SOFTWARE_SDA_OUT, clockStatus );
       }
    }
#endif

/*==================[ISR external functions definition]======================*/

/*==================[end of file]============================================*/
