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
 */

/* Date: 2017-11-14 */

#ifndef _SAPI_QMC5883L_H_
#define _SAPI_QMC5883L_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define QMC5883L_ADD              0x0D

#define QMC5883L_REG_MODE         0x02

#define QMC5883L_REG_X_LSB        0x00
#define QMC5883L_REG_X_MSB        0x01

#define QMC5883L_REG_Y_LSB        0x02
#define QMC5883L_REG_Y_MSB        0x03

#define QMC5883L_REG_Z_LSB        0x04
#define QMC5883L_REG_Z_MSB        0x05

#define QMC5883L_REG_STATUS       0x06

#define QMC5883L_REG_TEMP_LSB     0x07
#define QMC5883L_REG_TEMP_MSB     0x08

#define QMC5883L_REG_CTRL_1       0x09
#define QMC5883L_REG_CTRL_2       0x0A

#define QMC5883L_SET_RESET_PERIOD 0x0B

#define QMC5883LConfig QMC5883LInit

/*==================[typedef]================================================*/
/** The following type represents
    all possible values of the MODE
    */
typedef enum {
   QMC5883L_standby = 0,
   QMC5883L_continuous_measurement = 1,
   QMC5883L_DEFAULT_mode = QMC5883L_standby
} QMC5883L_mode_t;


/** The following type represents
    all possible values of the ODR
    (Output Data Update Rate)*/
typedef enum {
   QMC5883L_10_Hz  = 0,
   QMC5883L_50_Hz  = 1,
   QMC5883L_100_Hz = 2,
   QMC5883L_500_Hz = 3,
   QMC5883L_DEFAULT_rate = QMC5883L_10_Hz
} QMC5883L_rate_t;

/** The following type represents
    all possible values of the RNG
    (magnetic field measurement range
     or sensitivity of the sensors)
    */
typedef enum {
   QMC5883L_2_Gauss = 0, /* ± 2 Ga */
   QMC5883L_8_Gauss = 1, /* ± 8 Ga  */
   QMC5883L_DEFAULT_gain = QMC5883L_8_Gauss
} QMC5883L_gain_t;

/** The following type represents
    all possible values of the OSR
    (Over sample Rate)
    */
typedef enum {
   QMC5883L_512_sample = 0,
   QMC5883L_256_sample = 1,
   QMC5883L_126_sample = 2,
   QMC5883L_64_sample = 3,
   QMC5883L_DEFAULT_sample = QMC5883L_512_sample
} QMC5883L_samples_t;


typedef struct {
   QMC5883L_samples_t samples; /*number of over samples averaged per measurement output.*/

   QMC5883L_gain_t gain; /* Gain Configuration Bits. These bits configure the gain for
                          * the device. The gain configuration is common for all
                          * channels.*/

   QMC5883L_rate_t rate; /* Data Output Rate Bits. These bits set the rate at which data
                          * is written to all three data output registers.*/
   QMC5883L_mode_t mode;


} QMC5883L_config_t;

/*==================[external functions declaration]=========================*/

bool_t qmc5883lPrepareDefaultInit( QMC5883L_config_t * config );
bool_t qmc5883lInit( QMC5883L_config_t config );
bool_t qmc5883lRead( int16_t * x, int16_t * y, int16_t * z );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_QMC5883L_H_ */
