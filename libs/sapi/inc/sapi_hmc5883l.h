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

#ifndef _SAPI_HMC5883L_H_
#define _SAPI_HMC5883L_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define HMC5883L_ADD              0x1E

#define HMC5883L_REG_CONFIG_A     0x00
#define HMC5883L_REG_CONFIG_B     0x01

#define HMC5883L_REG_MODE         0x02

#define HMC5883L_REG_X_MSB        0x03
#define HMC5883L_REG_X_LSB        0x04

#define HMC5883L_REG_Z_MSB        0x05
#define HMC5883L_REG_Z_LSB        0x06

#define HMC5883L_REG_Y_MSB        0x07
#define HMC5883L_REG_Y_LSB        0x08

#define HMC5883L_REG_STATUS       0x09

#define HMC5883L_REG_ID_REG_A     0x0A
#define HMC5883L_REG_ID_REG_B     0x0B
#define HMC5883L_REG_ID_REG_C     0x0C

#define HMC5883L_VALUE_ID_REG_A   0x48
#define HMC5883L_VALUE_ID_REG_B   0x34
#define HMC5883L_VALUE_ID_REG_C   0x33

/*==================[typedef]================================================*/

typedef enum {
   HMC5883L_1_sample = 0,
   HMC5883L_2_sample = 1,
   HMC5883L_4_sample = 2,
   HMC5883L_8_sample = 3,
   HMC5883L_DEFAULT_sample = HMC5883L_1_sample
} HMC5883L_samples_t;

typedef enum {
   HMC5883L_0_75_Hz = 0,
   HMC5883L_1_50_Hz = 1,
   HMC5883L_3_Hz    = 2,
   HMC5883L_7_50_Hz = 3,
   HMC5883L_15_Hz   = 4,
   HMC5883L_30_Hz   = 5,
   HMC5883L_75_Hz   = 6,
   HMC5883L_DEFAULT_rate = HMC5883L_15_Hz
} HMC5883L_rate_t;

typedef enum {
   HMC5883L_normal   = 0,
   HMC5883L_positive = 1,
   HMC5883L_regative = 2,
   HMC5883L_DEFAULT_messurement = HMC5883L_normal
} HMC5883L_messurement_t;

typedef enum {
   HMC5883L_1370 = 0, /* ± 0.88 Ga */
   HMC5883L_1090 = 1, /* ± 1.3 Ga  */
   HMC5883L_820  = 2, /* ± 1.9 Ga  */
   HMC5883L_660  = 3, /* ± 2.5 Ga  */
   HMC5883L_440  = 4, /* ± 4.0 Ga  */
   HMC5883L_390  = 5, /* ± 4.7 Ga  */
   HMC5883L_330  = 6, /* ± 5.6 Ga  */
   HMC5883L_230  = 7, /* ± 8.1 Ga  */
   HMC5883L_DEFAULT_gain = HMC5883L_1090
} HMC5883L_gain_t;

typedef enum {
   HMC5883L_continuous_measurement = 0,
   HMC5883L_single_measurement = 1,
   HMC5883L_idle = 2,
   HMC5883L_DEFAULT_mode = HMC5883L_single_measurement
} HMC5883L_mode_t;

typedef struct{
   HMC5883L_samples_t samples; /*number of samples averaged (1 to 8) per measurement output.*/
   HMC5883L_rate_t    rate;    /* Data Output Rate Bits. These bits set the rate at which data
                                * is written to all three data output registers.*/
   HMC5883L_messurement_t meassurement; /*Measurement Configuration Bits. These bits define the
                                         * measurement flow of the device, specifically whether or not
                                         * to incorporate an applied bias into the measurement.*/
   HMC5883L_gain_t gain; /* Gain Configuration Bits. These bits configure the gain for
                          * the device. The gain configuration is common for all
                          * channels.*/
   HMC5883L_mode_t mode;
} HMC5883L_config_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

bool_t hmc5883lIsAlive(void);
bool_t hmc5883lPrepareDefaultConfig( HMC5883L_config_t * config );
bool_t hmc5883lConfig( HMC5883L_config_t config );
bool_t hmc5883lRead( int16_t * x, int16_t * y, int16_t * z );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_HMC5883L_H_ */
