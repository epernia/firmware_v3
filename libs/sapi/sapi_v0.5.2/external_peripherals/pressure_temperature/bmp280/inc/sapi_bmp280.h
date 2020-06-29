/*=============================================================================
 * Copyright (c) 2020, Luciano Francisco Vittori <lucianovittori99@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/06/16
 * Version: 1.0
 *===========================================================================*/


#ifndef _SAPI_BMP280_H_
#define _SAPI_BMP280_H_

#include "sapi_datatypes.h"
#include "bmp280.h"

#ifndef BMP280_CS_PIN
#define BMP280_CS_PIN                    0
#endif

#ifndef BMP280_CS_PORT
#define BMP280_CS_PORT                   3
#endif

#define BMP280_CS_LOW()    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, BMP280_CS_PORT, BMP280_CS_PIN)
#define BMP280_CS_HIGH()   Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, BMP280_CS_PORT, BMP280_CS_PIN)

void delay_ms(uint32_t period_ms);
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);

#endif /*_SAPI_BMP280_H_ */
