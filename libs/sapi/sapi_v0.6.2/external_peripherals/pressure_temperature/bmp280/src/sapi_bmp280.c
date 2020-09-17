/*=============================================================================
 * Copyright (c) 2020, Luciano Francisco Vittori <lucianovittori99@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/06/16
 * Version: 1.0
 *===========================================================================*/

/*!
 *  @brief Example shows basic setup of sensor which includes following
 *      Initialization of the interface.
 *      performing the sensor initialization.
 */

#include "sapi_bmp280.h"   		/* <= sAPI BMP280 header */
#include "sapi_i2c.h"           /* <= sAPI I2C header */
#include "sapi_delay.h"         /* <= sAPI Delay header */
#include "sapi_spi.h"			/* <= sAPI SPI header */
#include "sapi_gpio.h"


/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs such as "bmg250_soft_reset",
 *      "bmg250_set_foc", "bmg250_perform_self_test"  and so on.
 *
 *  @param[in] period_ms  : the required wait time in milliseconds.
 *  @return void.
 *
 */
void delay_ms(uint32_t period_ms)
{
	delay(period_ms);
}

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose value is to be written.
 *  @param[in] length   : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval < 0 -> Failure Info
 *
 */
int8_t i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length) {
	int8_t status = -1;
	int8_t i2c_ok = 0;
	uint8_t i;

	union {
		uint8_t buff[length + 1];
		struct {
			uint8_t addr;
			uint8_t data[length];
		};
	} i2c_data;

	i2c_data.addr = reg_addr;
	for(i=0;i<length;i++){
		i2c_data.data[i] = *reg_data+i;
	}

	i2c_ok = i2cWrite(I2C0, i2c_addr, i2c_data.buff, length+1, TRUE);

	status += i2c_ok;

	return status;
}

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] i2c_addr : Sensor I2C address.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval < 0 -> Failure Info
 *
 */
int8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length) {
	int8_t status = -1;
	int8_t i2c_ok = 0;

	i2c_ok = i2cWriteRead(I2C0, i2c_addr, &reg_addr, 1, TRUE, reg_data, length, TRUE);

	status += i2c_ok;

	return status;
}

/*!
 *  @brief Function for writing the sensor's registers through SPI bus.
 *
 *  @param[in] cs           : Chip select to enable the sensor.
 *  @param[in] reg_addr     : Register address.
 *  @param[in] reg_data : Pointer to the data buffer whose data has to be written.
 *  @param[in] length       : No of bytes to write.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval < 0 -> Failure Info
 *
 */
int8_t spi_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	int8_t status = -1;
	int8_t spi_ok = 0;
	uint8_t i;

	union {
		uint8_t buff[length + 1];
		struct {
			uint8_t addr;
			uint8_t* data;
		};
	} spi_data;

	spi_data.addr = reg_addr;
	spi_data.data = reg_data;

	BMP280_CS_LOW();
	spi_ok = spiWrite( SPI0, &spi_data.addr, 1);
	spi_ok = spiWrite( SPI0, spi_data.data, length);
	delayInaccurateUs(10);
	BMP280_CS_HIGH();

	status += spi_ok;

	return status;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       : Chip select to enable the sensor.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval < 0 -> Failure Info
 *
 */
int8_t spi_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	int8_t status = -1;
	int8_t spi_ok = 0;
	uint8_t i;

	union {
		uint8_t buff[length + 1];
		struct {
			uint8_t addr;
			uint8_t* data;
		};
	} spi_data;

	spi_data.addr = reg_addr;
	spi_data.data = reg_data;

	BMP280_CS_LOW();
	spi_ok = spiWrite( SPI0, &spi_data.addr, 1);
	spi_ok = spiRead( SPI0, spi_data.data, length);
	delayInaccurateUs(10);
	BMP280_CS_HIGH();

	status += spi_ok;

	return status;
}


/*!
 *  @brief Prints the execution status of the APIs.
 *
 *  @param[in] api_name : name of the API whose execution status has to be printed.
 *  @param[in] rslt     : error code returned by the API whose execution status has to be printed.
 *
 *  @return void.
 */
void print_rslt(const char api_name[], int8_t rslt)
{
    if (rslt != BMP280_OK)
    {
        printf("%s\t", api_name);
        if (rslt == BMP280_E_NULL_PTR)
        {
            printf("Error [%d] : Null pointer error\r\n", rslt);
        }
        else if (rslt == BMP280_E_COMM_FAIL)
        {
            printf("Error [%d] : Bus communication failed\r\n", rslt);
        }
        else if (rslt == BMP280_E_IMPLAUS_TEMP)
        {
            printf("Error [%d] : Invalid Temperature\r\n", rslt);
        }
        else if (rslt == BMP280_E_DEV_NOT_FOUND)
        {
            printf("Error [%d] : Device not found\r\n", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            printf("Error [%d] : Unknown error code\r\n", rslt);
        }
    }
}
