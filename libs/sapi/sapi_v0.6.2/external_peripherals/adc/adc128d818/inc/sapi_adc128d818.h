/* Copyright 2019, Gustavo Ramoscelli.
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

/*
 * Date: 2019-02-04
 */

// ----------------------------------------------------------
// Library for the ADC128D818 12 BIT ADC Sigma-Delta
// ----------------------------------------------------------
// Pinout ADC128D818
//
//                         +-------+
//               VREF   1 =|       |= 16  IN0
//                SDA   2 =|       |= 15  IN1
//                SCL   3 =|       |= 14  IN2
//                GND   4 =|       |= 13  IN3
//                 V+   5 =|       |= 12  IN4
//               ~INT   6 =|       |= 11  IN5
//                 A0   7 =|       |= 10  IN6
//                 A1   8 =|       |= 9   IN7
//                         +-------+
//


#ifndef _SAPI_ADC128D818_H_
#define _SAPI_ADC128D818_H_
/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

typedef enum {
    ADC128D818_OPERATION_MODE_0 = 0x00, // Single ended with temp.: IN0..IN6 plus temp 
    ADC128D818_OPERATION_MODE_1 = 0x01, // Single ended: IN0..IN7
    ADC128D818_OPERATION_MODE_2 = 0x02, // Differential: IN0-IN1, IN3-IN2, IN4-IN5, IN7-IN6
    ADC128D818_OPERATION_MODE_3 = 0x03  // Mixed: IN0..IN3, IN4-IN5, IN7-IN6
} adc128d818_operation_mode_t;

// i2c address => Tri-state levels on pins A0:A1
typedef enum {
    ADC128D818_ADDRESS_LOW_LOW   = 0x1D,
    ADC128D818_ADDRESS_LOW_MID   = 0x1E,
    ADC128D818_ADDRESS_LOW_HIGH  = 0x1F,
    ADC128D818_ADDRESS_MID_LOW   = 0x2D,
    ADC128D818_ADDRESS_MID_MID   = 0x2E,
    ADC128D818_ADDRESS_MID_HIGH  = 0x2F,
    ADC128D818_ADDRESS_HIGH_LOW  = 0x35,
    ADC128D818_ADDRESS_HIGH_MID  = 0x36,
    ADC128D818_ADDRESS_HIGH_HIGH = 0x37
}  adc128d818_address_t;

typedef enum {
    ADC128D818_VREF_INT = 0x00,
    ADC128D818_VREF_EXT = 0x01
}  adc128d818_vref_t;

typedef enum {
    ADC128D818_RATE_LOW_POWER   = 0x00,
    ADC128D818_RATE_CONTINUOUS  = 0x01,
    ADC128D818_RATE_ONE_SHOT    = 0x03
} adc128d818_rate_t;

typedef enum {
    ADC128D818_LIMIT_HIGH  = 0x00,
    ADC128D818_LIMIT_LOW   = 0x01
}  adc128d818_limit_t;

typedef enum {
    ADC128D818_CHANNEL_IN0  = 0x00,
    ADC128D818_CHANNEL_IN1  = 0x01,
    ADC128D818_CHANNEL_IN2  = 0x02,
    ADC128D818_CHANNEL_IN3  = 0x03,
    ADC128D818_CHANNEL_IN4  = 0x04,
    ADC128D818_CHANNEL_IN5  = 0x05,
    ADC128D818_CHANNEL_IN6  = 0x06,
    ADC128D818_CHANNEL_IN7  = 0x07,
    ADC128D818_CHANNEL_TEMP = 0x07
}  arc128d818_channel_t;

typedef enum {
    ADC128D818_INT_IN0 = (char)~(0x01 <<0),
    ADC128D818_INT_IN1 = (char)~(0x01 <<1),
    ADC128D818_INT_IN2 = (char)~(0x01 <<2),
    ADC128D818_INT_IN3 = (char)~(0x01 <<3),
    ADC128D818_INT_IN4 = (char)~(0x01 <<4),
    ADC128D818_INT_IN5 = (char)~(0x01 <<5),
    ADC128D818_INT_IN6 = (char)~(0x01 <<6),
    ADC128D818_INT_IN7 = (char)~(0x01 <<7),
    ADC128D818_INT_TEMP = (char)~(0x01 <<7),
    ADC128D818_INT_ALL = 0x00
}  adc128d818_int_t;

typedef enum {
    ADC128D818_ENABLE_IN0 = (char)~(0x01 <<0),
    ADC128D818_ENABLE_IN1 = (char)~(0x01 <<1),
    ADC128D818_ENABLE_IN2 = (char)~(0x01 <<2),
    ADC128D818_ENABLE_IN3 = (char)~(0x01 <<3),
    ADC128D818_ENABLE_IN4 = (char)~(0x01 <<4),
    ADC128D818_ENABLE_IN5 = (char)~(0x01 <<5),
    ADC128D818_ENABLE_IN6 = (char)~(0x01 <<6),
    ADC128D818_ENABLE_IN7 = (char)~(0x01 <<7),
    ADC128D818_ENABLE_TEMP = ~(0x01 <<7),
    ADC128D818_ENABLE_ALL = 0x00
} adc128d818_enable_t;

typedef enum {
    ADC128D818_REG_Configuration_Register           = 0x00,
    ADC128D818_REG_Interrupt_Status_Register        = 0x01,
    ADC128D818_REG_Interrupt_Mask_Register          = 0x03,
    ADC128D818_REG_Conversion_Rate_Register         = 0x07,
    ADC128D818_REG_Channel_Disable_Register         = 0x08,
    ADC128D818_REG_One_Shot_Register                = 0x09,
    ADC128D818_REG_Deep_Shutdown_Register           = 0x0A,
    ADC128D818_REG_Advanced_Configuration_Register  = 0x0B,
    ADC128D818_REG_Busy_Status_Register             = 0x0C,
    ADC128D818_REG_Channel_Readings_Registers       = 0x20, // in mode 1: 0x20 = IN0, 0x27 = IN7
    ADC128D818_REG_Temperature_Register             = 0x27, // in mode 0, CH7 is not external, is the internal temperature sensor
    ADC128D818_REG_Limit_Registers                  = 0x2A,
    ADC128D818_REG_Manufacturer_ID_Register         = 0x3E,
    ADC128D818_REG_Revision_ID_Register             = 0x3F
} adc128d818_reg_address_t;

// Busy Status Register: Address 0x0C (see above)
#define ADC128D818_STATUS_NOT_READY_BIT  0x01  // e.g. while power up chip
#define ADC128D818_STATUS_BUSY_BIT       0x02  // e.g. STAUS = "Reading"

typedef struct {
    adc128d818_address_t address;
    adc128d818_operation_mode_t op_mode;
    adc128d818_rate_t rate;
    adc128d818_vref_t ref_mode;
    double ref_voltage;
    adc128d818_enable_t enabled_mask;
} adc128d818_init_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** @brief Init new ADC128D818 device
*
*   @param adc128d818_init_t
*
*	@return true = success
*/
bool_t adc128d818_init(adc128d818_init_t *init);

/** 
*   @brief read the channel in "raw" format
*
*   @param address  I2C address of A1, A0 tristate values
*   @param channel  The channel to read
*
*	@return none
*/
uint16_t adc128d818_readChannel(uint8_t address, uint8_t channel);


/**
*   @brief read the channel 
*
*   @param address   I2C address of A1, A0 tristate values
*   @param channel   The channel to read
*
*	@return none
*/
uint8_t adc128d818_readRegister(uint8_t address, uint8_t reg_addr);


/**
*   @brief read temperature in Celsius
*
*   @param address   I2C address of A1, A0 tristate values
*   @param channel   The channel to read from 0 to 7
*
*	@return none
*/
double adc128d818_readTemperatureConverted(int id);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* #ifndef _SAPI_ADC128D818_H_ */

