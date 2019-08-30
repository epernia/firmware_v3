/* Copyright 2017, Rodrigo Furlani
 * Copyright 2017, Alejandro Permingeat
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

/* Date: 2017-11-14 */

#include "sapi_magnetometer_qmc5883l.h"         /* <= sAPI QMC5883L header */
#include "sapi_i2c.h"         	   /* <= sAPI I2C header */


bool_t qmc5883lPrepareDefaultInit( QMC5883L_config_t * config )
{

   config->samples = QMC5883L_DEFAULT_sample;
   config->gain = QMC5883L_DEFAULT_gain;
   config->rate = QMC5883L_DEFAULT_rate;

   config->mode = QMC5883L_DEFAULT_mode;

   return (TRUE);
}


bool_t qmc5883lInit( QMC5883L_config_t config )
{

   uint8_t register1;//, register2, registerMode;

   uint8_t transmitDataBuffer[2];

   register1 = 0;

   /*OSR bit 7 and bit 6 */
   register1 |= ((uint8_t)config.samples) << 6;

   /*RNG bit 5 and bit 4*/
   register1 |= ((uint8_t)config.gain) << 4;

   /*ODR bit 3 and bit 2*/
   register1 |= ((uint8_t)config.rate) << 2;

   /*MODE bit 1 and bit 0*/
   register1 |= config.mode;

   i2cInit( I2C0, 100000 );

   transmitDataBuffer[0] = QMC5883L_SET_RESET_PERIOD;
   transmitDataBuffer[1] = 1; /* value recomeended in datasheet for this register*/
   i2cWrite( I2C0, QMC5883L_ADD, transmitDataBuffer, 2, TRUE );

   transmitDataBuffer[0] = QMC5883L_REG_CTRL_1;
   transmitDataBuffer[1] = register1;
   i2cWrite( I2C0, QMC5883L_ADD, transmitDataBuffer, 2, TRUE );

   /** TODO: Implement the configuration of register QMC5883L_REG_CTRL_2: possition 0x0A*/

   return ( TRUE);
}


bool_t qmc5883lRead( int16_t * x, int16_t * y, int16_t * z )
{

   bool_t result = TRUE;

   uint8_t x_MSB, x_LSB;
   uint8_t y_MSB, y_LSB;
   uint8_t z_MSB, z_LSB;

   uint8_t dataToReadBuffer;


   dataToReadBuffer = QMC5883L_REG_X_LSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &x_LSB, 1, TRUE );

   dataToReadBuffer = QMC5883L_REG_X_MSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &x_MSB, 1, TRUE );

   dataToReadBuffer = QMC5883L_REG_Y_LSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &y_LSB, 1, TRUE );

   dataToReadBuffer = QMC5883L_REG_Y_MSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &y_MSB, 1, TRUE );


   dataToReadBuffer = QMC5883L_REG_Z_LSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &z_LSB, 1, TRUE );

   dataToReadBuffer = QMC5883L_REG_Z_MSB;
   i2cRead( I2C0, QMC5883L_ADD,
            &dataToReadBuffer, 1, TRUE,
            &z_MSB, 1, TRUE );

   *x = x_MSB;
   *x = (*x << 8)|x_LSB;

   *y = y_MSB;
   *y = (*y << 8)|y_LSB;

   *z = z_MSB;
   *z = (*z << 8)|z_LSB;

   return(result); /** TODO: return value must reflect the result of the operation */
}


/** Calculo de angulo en el plano X Y (con eje Z coincidiendo con el cenit)
  El angulo se calcula con respecto al eje Y es decir, 
  un valor de    0 grados indica que el norte coincide con el eje  Y, 
  un valor de   90 grados indica que el norte coincide con el eje  X,
  un valor de  180 grados indica que el norte coincide con el eje -Y, 
  un valor de  -90 grados indica que el norte coincide con el eje -X. 
*/
/*
angulo = atan2( qmc5883l_y_raw,qmc5883l_x_raw ); //valor en radianes

angulo = angulo * 180 / 3.14; //Convertir a grados

consolePrintString("Angulo: ");
consolePrintlnInt(angulo);
consolePrintEnter();
*/