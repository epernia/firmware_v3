/* Copyright 2016, Alejandro Permingeat
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

/* Date: 2015-06-27 */

#include "sapi_hmc5883l.h"         /* <= sAPI HMC5883L header */
#include "sapi_i2c.h"         	   /* <= sAPI I2C header */

bool_t hmc5883lIsAlive( void ){

   uint8_t idRegister[3];

   // i2cRead( I2C0, HMC5883L_ADD, HMC5883L_REG_ID_REG_A, &idRegister, 3 );

   if( (HMC5883L_VALUE_ID_REG_A == idRegister[0]) &&
       (HMC5883L_VALUE_ID_REG_B == idRegister[1]) &&
       (HMC5883L_VALUE_ID_REG_C == idRegister[2])
     ){
      return (TRUE);
   }
   else{
      return (FALSE);
   }
}

bool_t hmc5883lPrepareDefaultConfig( HMC5883L_config_t * config ){

   config->gain = HMC5883L_DEFAULT_gain;
   config->meassurement = HMC5883L_DEFAULT_messurement;
   config->rate = HMC5883L_DEFAULT_rate;
   config->samples = HMC5883L_DEFAULT_sample;
   config->mode = HMC5883L_DEFAULT_mode;

   return (TRUE);
}


bool_t hmc5883lConfig( HMC5883L_config_t config ){

   uint8_t registerA, registerB, registerMode;

   uint8_t transmitDataBuffer[2];

   registerA = config.samples;
   registerA = registerA<<3;
   registerA |= config.rate;
   registerA = registerA<<2;
   registerA |= config.meassurement;

   registerB = config.gain;
   registerB = registerB << 5;

   registerMode = config.mode;

   i2cConfig( I2C0, 100000 );

   transmitDataBuffer[0] = HMC5883L_REG_CONFIG_A;
   transmitDataBuffer[1] = registerA;
   i2cWrite( I2C0, HMC5883L_ADD, transmitDataBuffer, 2, TRUE );

   transmitDataBuffer[0] = HMC5883L_REG_CONFIG_B;
   transmitDataBuffer[1] = registerB;
   i2cWrite( I2C0, HMC5883L_ADD, transmitDataBuffer, 2, TRUE );

   transmitDataBuffer[0] = HMC5883L_REG_MODE;
   transmitDataBuffer[1] = registerMode;
   i2cWrite( I2C0, HMC5883L_ADD, transmitDataBuffer, 2, TRUE );

   return ( hmc5883lIsAlive() );
}


bool_t hmc5883lRead( int16_t * x, int16_t * y, int16_t * z ){

   bool_t result = TRUE;

   uint8_t x_MSB, x_LSB;
   uint8_t y_MSB, y_LSB;
   uint8_t z_MSB, z_LSB;

   uint8_t dataToReadBuffer;

   dataToReadBuffer = HMC5883L_REG_X_MSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &x_MSB, 1, TRUE );

   dataToReadBuffer = HMC5883L_REG_X_LSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &x_LSB, 1, TRUE );

   dataToReadBuffer = HMC5883L_REG_Y_MSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &y_MSB, 1, TRUE );

   dataToReadBuffer = HMC5883L_REG_Y_LSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &y_LSB, 1, TRUE );

   dataToReadBuffer = HMC5883L_REG_Z_MSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &z_MSB, 1, TRUE );

   dataToReadBuffer = HMC5883L_REG_Z_LSB;
   i2cRead( I2C0, HMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &z_LSB, 1, TRUE );

   *x = x_MSB;
   *x = (*x << 8)|x_LSB;

   *y = y_MSB;
   *y = (*y << 8)|y_LSB;

   *z = z_MSB;
   *z = (*z << 8)|z_LSB;

   return(result); /** TODO: return value must reflect the result of the operation */
}
