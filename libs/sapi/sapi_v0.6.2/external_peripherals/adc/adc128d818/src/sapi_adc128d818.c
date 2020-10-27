/* Copyright 2019, Gustavo Ramoscelli.
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
 * Date: 2020-10-19
 */

/*==================[inclusions]=============================================*/

#include "sapi_adc128d818.h"
#include "sapi.h"            /* <= sAPI header */
#include <string.h>
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

const int8_t ADC128D818_ADDR[] = {0x1d, 0x1e, 0x1f, 0x2d, 0x2e, 0x2f, 0x35, 0x36, 0x37};
double my_ref_voltage[] = {2.56f, 2.56f, 2.56f, 2.56f, 2.56f, 2.56f, 2.56f, 2.56f, 2.56f};

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

// private methods
//

/*
From sapi_i2c.h: 

ool_t i2cInit( i2cMap_t i2cNumber, uint32_t clockRateHz );

bool_t i2cRead( i2cMap_t i2cNumber,
                uint8_t  i2cSlaveAddress,
                uint8_t* receiveDataBuffer,
                uint16_t receiveDataBufferSize,
                bool_t   sendReadStop );

bool_t i2cWrite( i2cMap_t i2cNumber,
                 uint8_t  i2cSlaveAddress,
                 uint8_t* transmitDataBuffer,
                 uint16_t transmitDataBufferSize,
                 bool_t   sendWriteStop );

bool_t i2cWriteRead( i2cMap_t i2cNumber,
                     uint8_t  i2cSlaveAddress,
                     uint8_t* dataToReadBuffer,
                     uint16_t dataToReadBufferSize,
                     bool_t   sendWriteStop,
                     uint8_t* receiveDataBuffer,
                     uint16_t receiveDataBufferSize,
                     bool_t   sendReadStop );

*/

bool_t adc128d818_setRegisterAddress(uint8_t address, uint8_t reg_addr) 
{
    return i2cWrite(I2C0, address, &reg_addr, 1, 1);
}

/** @brief read the channel in "raw" format
*	@return none
*/
bool_t adc128d818_setRegister(uint8_t address, uint8_t reg_addr, uint8_t value) 
{
    int8_t data[2];
    data[0] = reg_addr;
    data[1] = value;
    return i2cWrite(I2C0, address, data, 2, 1);
}

/** @brief read one register
*	@return none
*/
uint8_t adc128d818_readRegister(uint8_t address, uint8_t reg_addr) 
{
    uint8_t val;
    i2cWriteRead(I2C0, address, &reg_addr, 1, 0, &val, 1, 1); 
    return val;
}

/** @brief read the channel in "raw" format and continuous mode
*	@return success
*/
bool_t adc128d818_readChannel(uint8_t address, uint8_t channel, uint16_t * value)
{
    
    uint8_t busy_reg;
    uint16_t count;
    bool_t shutdown_mode;
    uint8_t buff[2];
    uint8_t adr_channel;
    uint16_t result;
    
    if (channel>7) { // there are only 7 channels
        return FALSE;
    }
    adr_channel = ADC128D818_REG_Channel_Readings_Registers + channel;

    // test shutdown mode
    if ((adc128d818_readRegister(address, ADC128D818_REG_Configuration_Register)&ADC128D818_CONFIGURATION_BIT_Start) == 0)
    {
        shutdown_mode = TRUE;
    } else {
        shutdown_mode = FALSE;
    }

    if (shutdown_mode  == TRUE) {
#if DEBUG == 1
        printf("\r\nError: device in shutdown mode\r\n");
#endif
        return FALSE; // Error! Continuous mode needs that the device is nos in shutdown mode (or deep shutdown mode)
    }
    
    i2cWriteRead(I2C0, address, &adr_channel, 1, 0, buff, 2, 1);
    
    result = buff[0];
    result = (result << 8) | buff[1];
    *value = (result >> 4);
    return TRUE;
}

/** @brief read the channel in "raw" format
*	@return success
*/
bool_t adc128d818_readOneShotChannel(uint8_t address, uint8_t channel, uint16_t * value)
{
    
    uint8_t busy_reg;
    uint16_t count;
    uint8_t buff[2];
    uint8_t adr_channel;
    uint16_t result;
    bool_t shutdown_mode;
    
    if (channel>7) { // there are only 7 channels
        return FALSE;
    }
    adr_channel = ADC128D818_REG_Channel_Readings_Registers + channel;
    
    // test shutdown mode
    if ((adc128d818_readRegister(address, ADC128D818_REG_Configuration_Register)&ADC128D818_CONFIGURATION_BIT_Start) == 0)
    {
        shutdown_mode = TRUE;
    } else {
        shutdown_mode = FALSE;
    }

    if (shutdown_mode  == FALSE) {
        return FALSE; // Error! One shot only works when device is in shutdown mode (or deep shutdown mode)
    }


    // shot to start conversion
    adc128d818_setRegister(address, ADC128D818_REG_One_Shot_Register, 0x01);

    // Wait for conversion ready ...
    delay(13); // 12.2 ms is the reate time by channel in continuous mode
    busy_reg = adc128d818_readRegister(address, ADC128D818_REG_Busy_Status_Register);
    count = 0;
    while (busy_reg&( ADC128D818_STATUS_BUSY_BIT ))
    {
        count++;
        if (count>MAX_MS_WAIT_FOR_NOT_BUSY) {
            return FALSE; // Error!
        }
        delay(1);
        busy_reg = adc128d818_readRegister(address, ADC128D818_REG_Busy_Status_Register);
    }
    i2cWriteRead(I2C0, address, &adr_channel, 1, 0, buff, 2, 1);
    
    result = buff[0];
    result = (result << 8) | buff[1];
    *value = (result >> 4);
    return TRUE;
}

/** @brief add new ADC128D818 device
*	@return TRUE = Success
*/
bool_t adc128d818_init(adc128d818_init_t *init)
{
    // test alive: read the Manufacturer ID register (must be != 0)

    if (adc128d818_readRegister(init->address, ADC128D818_REG_Manufacturer_ID_Register) == 0) {
        // Error!
        return FALSE;
    }
    /* HINT:
    *
    * Quick Start
    * 1. Power on the device, then wait for at least 33ms.
    * 2. Set the 'START' bit of the Configuration Register (address 00h, bit 0) to 0 (shutdown mode).
    * 3. Read the Busy Status Register (address 0Ch). If the 'Not Ready' bit = 1, then increase the wait time until 'Not Ready' bit = 0 before proceeding to the next step.
    * 4. Program the Advanced Configuration Register -- Address 0Bh:
    * - a. Choose to use the internal or external VREF (bit 0).
    * - b. Choose the mode of operation (bits [2:1]).
    * 5. Program the Conversion Rate Register (address 07h).
    * 6. Choose to enable or disable the channels using the Channel Disable Register (address 08h).
    * 7. Using the Interrupt Mask Register (address 03h), choose to mask or not to mask the interrupt status from propagating to the interrupt output pin, INT.
    * 8. Program the Limit Registers (addresses 2Ah - 39h).
    * 9. Set the 'START' bit of the Configuration Register (address 00h, bit 0) to 1.
    * 10. Set the 'INT_Clear' bit (address 00h, bit 3) to 0. If needed, program the 'INT_Enable' bit (address 00h, bit 1) to 1 to enable the INT output.
    */

#if DEBUG==1
    printf("Start ADC128D818 on address 0x%x\r\n", init->address);
#endif

    // Set shutdown mode => bit 0 to 0 in 
    adc128d818_setRegister(init->address, ADC128D818_REG_Configuration_Register, 0);

    uint8_t busy_reg = 0x00;
    int cont = 0;
    do {
        cont++;
        if (cont >= MAX_ITER_NOT_READY) {
#if DEBUG==1
            printf("Wait for not busy ADC128D818 timeout\r\n");
#endif
            return FALSE; 
        }
        
#if DEBUG==1
        printf("> Wait for device ready. Trial #%u\r\n", cont);
#endif

        delay(33);
        busy_reg = adc128d818_readRegister(init->address, ADC128D818_REG_Busy_Status_Register);
#if DEBUG==1
        printf(">>> Busy Status Register value: 0x%x\r\n", busy_reg);
#endif

    } while (busy_reg&( ADC128D818_STATUS_NOT_READY_BIT ));

    
    // program advanced config reg
#if DEBUG==1
    printf("> Setting Advanced Configuration Register\r\n");
    switch (init->op_mode) {
        case ADC128D818_OPERATION_MODE_0: printf("    Single ended with temp.: IN0..IN6 plus temp\r\n"); break;
        case ADC128D818_OPERATION_MODE_1: printf("    Single ended: IN0..IN7\r\n"); break;
        case ADC128D818_OPERATION_MODE_2: printf("    Differential: IN0-IN1, IN3-IN2, IN4-IN5, IN7-IN6\r\n"); break;
        case ADC128D818_OPERATION_MODE_3: printf("    Mixed: IN0..IN3, IN4-IN5, IN7-IN6\r\n"); break;
    }
    if (init->ref_mode == ADC128D818_VREF_INT) {
        printf("    Internal Voltage Reference\r\n");
    } else {
        printf("    External Voltage Reference\r\n");
    }
#endif
    adc128d818_setRegister(init->address, ADC128D818_REG_Advanced_Configuration_Register, init->ref_mode | (init->op_mode << 1));

    // program conversion rate regster
    printf("> Setting Conversion Rate\r\n");
    switch (init->rate) {
        case ADC128D818_RATE_LOW_POWER:              
#if DEBUG==1
            printf("    Low Power\r\n"); 
#endif
            adc128d818_setRegister(init->address, ADC128D818_REG_Conversion_Rate_Register, ADC128D818_RATE_LOW_POWER);
            break;
        case ADC128D818_RATE_CONTINUOUS:             
#if DEBUG==1
            printf("    Continuous\r\n"); break;
#endif
            adc128d818_setRegister(init->address, ADC128D818_REG_Conversion_Rate_Register, ADC128D818_RATE_CONTINUOUS);
            break;
        case ADC128D818_RATE_ONE_SHOT_SHUTDOWN:      
#if DEBUG==1
            printf("    One Shot\r\n"); 
#endif
            adc128d818_setRegister(init->address, ADC128D818_REG_Conversion_Rate_Register, ADC128D818_RATE_CONTINUOUS);
            break;
        case ADC128D818_RATE_ONE_SHOT_DEEP_SHUTDOWN: 
#if DEBUG==1
            printf("    One Shot w/deep shutdown\r\n"); 
#endif
            adc128d818_setRegister(init->address, ADC128D818_REG_Conversion_Rate_Register, ADC128D818_RATE_LOW_POWER);
            break;
    }

    // program enabled channels
    adc128d818_setRegister(init->address, ADC128D818_REG_Channel_Disable_Register, init->enabled_mask);

    // program limit regs
    // currently noop!

    // set start bit in configuration (interrupts disabled)
    if ((init->rate == ADC128D818_RATE_CONTINUOUS) || (init->rate == ADC128D818_RATE_ONE_SHOT_SHUTDOWN))
    {
        adc128d818_setRegister(init->address, ADC128D818_REG_Configuration_Register, 1);
    }    
    return TRUE;
}

