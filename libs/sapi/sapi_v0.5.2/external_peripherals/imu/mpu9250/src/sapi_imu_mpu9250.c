/* Copyright 2017 Bolder Flight Systems <brian.taylor@bolderflight.com>.
 * Copyright 2018, Sergio Renato De Jesus Melean <sergiordj@gmail.com>.
 * Copyright 2018, Eric Pernia.
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
 
/* Date: 2018-07-06 */

/*==================[inclusions]=============================================*/

#include "sapi_imu_mpu9250.h"   /* <= sAPI HMC5883L header */
#include "sapi_i2c.h"           /* <= sAPI I2C header */
#include "sapi_delay.h"         /* <= sAPI Delay header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

static int8_t mpu9250InitializeControlStructure( void );
static int8_t mpu9250WriteRegister( uint8_t subAddress, uint8_t data );
static int8_t mpu9250ReadRegisters( uint8_t subAddress, uint8_t count );
static int8_t mpu9250WriteAK8963Register( uint8_t subAddress, uint8_t data );
static int8_t mpu9250WhoAmI( void );
static int8_t mpu9250WhoAmIAK8963( void );
static int8_t mpu9250ReadAK8963Registers( uint8_t subAddress, uint8_t count );
static int8_t mpu9250CalibrateGyro( void );
static int8_t mpu9250SetGyroRange( MPU9250_GyroRange_t range );
static int8_t mpu9250SetDlpfBandwidth( MPU9250_DlpfBandwidth_t bandwidth );
static int8_t mpu9250SetSrd( uint8_t srd );

/*==================[internal data definition]===============================*/

//MPU control structure
static MPU9250_control_t control;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static int8_t mpu9250InitializeControlStructure( void )
{
	control._tempScale = 333.87f;
	control._tempOffset = 21.0f;
	control._numSamples = 100;
	control._axs = 1.0f;
	control._ays = 1.0f;
	control._azs = 1.0f;
	control._maxCounts = 1000;
	control._deltaThresh = 0.3f;
	control._coeff = 8;
	control._hxs = 1.0f;
	control._hys = 1.0f;
	control._hzs = 1.0f;
	control.tX[0] = 0;
	control.tX[1] = 1;
	control.tX[2] = 0;
	control.tY[0] = 1;
	control.tY[1] = 0;
	control.tY[2] = 0;
	control.tZ[0] = 0;
	control.tZ[1] = 0;
	control.tZ[2] = -1;
}

static int8_t mpu9250WriteRegister( uint8_t subAddress, uint8_t data )
{
	uint8_t transmitDataBuffer[2];
	transmitDataBuffer[0] = subAddress;
	transmitDataBuffer[1] = data;
	i2cWrite(I2C0, control.address, transmitDataBuffer, 2, TRUE);

	delay(10);

	/* read back the register */
	mpu9250ReadRegisters(subAddress,1);
	/* check the read back register against the written register */
	if(control._buffer[0] == data) {
      return 1;
	}
	else{
      return -1;
	}
}

static int8_t mpu9250ReadRegisters( uint8_t subAddress, uint8_t count )
{
	if( i2cWriteRead( I2C0, control.address,
                     &subAddress, 1, TRUE,
                     control._buffer, count, TRUE ) ){
		return 1;
	} else {
		return -1;
	}
}

static int8_t mpu9250WriteAK8963Register( uint8_t subAddress, uint8_t data )
{
	// set slave 0 to the AK8963 and set for write
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_ADDR, MPU9250_AK8963_I2C_ADDR) < 0) {
		return -1;
	}
	// set the register to the desired AK8963 sub address
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_REG, subAddress) < 0) {
		return -2;
	}
	// store the data for write
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_DO, data) < 0) {
		return -3;
	}
	// enable I2C and send 1 byte
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_CTRL, MPU9250_I2C_SLV0_EN | (uint8_t)1) < 0) {
		return -4;
	}
	// read the register and confirm
	if (mpu9250ReadAK8963Registers(subAddress,1) < 0) {
		return -5;
	}
	if(control._buffer[0] == data) {
		return 1;
	} else{
		return -6;
	}
}

static int8_t mpu9250WhoAmI( void )
{
	// read the WHO AM I register
	if (mpu9250ReadRegisters(MPU9250_WHO_AM_I,1) < 0) {
		return -1;
	}
	// return the register value
	return control._buffer[0];
}

static int8_t mpu9250WhoAmIAK8963( void )
{
	// read the WHO AM I register
	if (mpu9250ReadAK8963Registers(MPU9250_AK8963_WHO_AM_I,1) < 0) {
		return -1;
	}
	// return the register value
	return control._buffer[0];
}

static int8_t mpu9250ReadAK8963Registers( uint8_t subAddress, uint8_t count )
{
	// set slave 0 to the AK8963 and set for read
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_ADDR, MPU9250_AK8963_I2C_ADDR | MPU9250_I2C_READ_FLAG) < 0) {
		return -1;
	}
	// set the register to the desired AK8963 sub address
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_REG, subAddress) < 0) {
		return -2;
	}
	// enable I2C and request the bytes
	if (mpu9250WriteRegister( MPU9250_I2C_SLV0_CTRL, MPU9250_I2C_SLV0_EN | count) < 0) {
		return -3;
	}
	delay(1); // takes some time for these registers to fill
	// read the bytes off the MPU9250 EXT_SENS_DATA registers
	control._status = mpu9250ReadRegisters(MPU9250_EXT_SENS_DATA_00,count);
	return control._status;
}

static int8_t mpu9250CalibrateGyro( void )
{
	// set the range, bandwidth, and srd
	if (mpu9250SetGyroRange(MPU9250_GYRO_RANGE_250DPS) < 0) {
		return -1;
	}
	if (mpu9250SetDlpfBandwidth(MPU9250_DLPF_BANDWIDTH_20HZ) < 0) {
		return -2;
	}
	if (mpu9250SetSrd(19) < 0) {
		return -3;
	}

	// take samples and find bias
	control._gxbD = 0;
	control._gybD = 0;
	control._gzbD = 0;
	for (uint8_t i=0; i < control._numSamples; i++) {
		mpu9250Read();
		control._gxbD += ((mpu9250GetGyroX_rads() + control._gxb)/control._numSamples);
		control._gybD += ((mpu9250GetGyroY_rads() + control._gyb)/control._numSamples);
		control._gzbD += ((mpu9250GetGyroZ_rads() + control._gzb)/control._numSamples);
		delay(20);
	}
	control._gxb = (float)control._gxbD;
	control._gyb = (float)control._gybD;
	control._gzb = (float)control._gzbD;

	// set the range, bandwidth, and srd back to what they were
	if (mpu9250SetGyroRange(control._gyroRange) < 0) {
		return -4;
	}
	if (mpu9250SetDlpfBandwidth(control._bandwidth) < 0) {
		return -5;
	}
	if (mpu9250SetSrd(control._srd) < 0) {
		return -6;
	}
	return 1;
}

static int8_t mpu9250SetGyroRange( MPU9250_GyroRange_t range )
{
	switch(range) {
		case MPU9250_GYRO_RANGE_250DPS: {
		  // setting the gyro range to 250DPS
		  if(mpu9250WriteRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_250DPS) < 0){
			return -1;
		  }
        // setting the gyro scale to 250DPS
		  control._gyroScale = 250.0f/32767.5f * MPU9250_D2R; 
		  break;
		}
		case MPU9250_GYRO_RANGE_500DPS: {
		  // setting the gyro range to 500DPS
		  if(mpu9250WriteRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_500DPS) < 0){
			return -1;
		  }
        // setting the gyro scale to 500DPS
		  control._gyroScale = 500.0f/32767.5f * MPU9250_D2R; 
		  break;
		}
		case MPU9250_GYRO_RANGE_1000DPS: {
		  // setting the gyro range to 1000DPS
		  if(mpu9250WriteRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_1000DPS) < 0){
			return -1;
		  }
        // setting the gyro scale to 1000DPS
		  control._gyroScale = 1000.0f/32767.5f * MPU9250_D2R; 
		  break;
		}
		case MPU9250_GYRO_RANGE_2000DPS: {
		  // setting the gyro range to 2000DPS
		  if(mpu9250WriteRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_2000DPS) < 0){
			return -1;
		  }
        // setting the gyro scale to 2000DPS
		  control._gyroScale = 2000.0f/32767.5f * MPU9250_D2R; 
		  break;
		}
	}
	control._gyroRange = range;
	return 1;
}

static int8_t mpu9250SetDlpfBandwidth( MPU9250_DlpfBandwidth_t bandwidth )
{
	switch (bandwidth) {
		case MPU9250_DLPF_BANDWIDTH_184HZ: {
         // setting accel bandwidth to 184Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_184) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 184Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_184) < 0) { 
				return -2;
			}
			break;
		}
		case MPU9250_DLPF_BANDWIDTH_92HZ: {
         // setting accel bandwidth to 92Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_92) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 92Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_92) < 0) { 
				return -2;
			}
			break;
		}
		case MPU9250_DLPF_BANDWIDTH_41HZ: {
         // setting accel bandwidth to 41Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_41) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 41Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_41) < 0) { 
				return -2;
			}
			break;
		}
		case MPU9250_DLPF_BANDWIDTH_20HZ: {
         // setting accel bandwidth to 20Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_20) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 20Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_20) < 0) { 
				return -2;
			}
			break;
		}
		case MPU9250_DLPF_BANDWIDTH_10HZ: {
         // setting accel bandwidth to 10Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_10) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 10Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_10) < 0) { 
				return -2;
			}
			break;
		}
		case MPU9250_DLPF_BANDWIDTH_5HZ: {
         // setting accel bandwidth to 5Hz
			if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_5) < 0) { 
				return -1;
			}
         // setting gyro bandwidth to 5Hz
			if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_5) < 0) { 
				return -2;
			}
			break;
		}
	}
	control._bandwidth = bandwidth;
	return 1;
}

static int8_t mpu9250SetSrd( uint8_t srd )
{
	/* setting the sample rate divider to 19 to facilitate setting up 
      magnetometer */
   // setting the sample rate divider
	if (mpu9250WriteRegister(MPU9250_SMPDIV, 19) < 0) {
		return -1;
	}
	if (srd > 9) {
		// set AK8963 to Power Down
		if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_PWR_DOWN) < 0) {
			return -2;
		}
		delay(100); // long wait between AK8963 mode changes
		// set AK8963 to 16 bit resolution, 8 Hz update rate
		if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_CNT_MEAS1) < 0) {
			return -3;
		}
		delay(100); // long wait between AK8963 mode changes
		// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
		mpu9250ReadAK8963Registers(MPU9250_AK8963_HXL, 7);
	} else {
		// set AK8963 to Power Down
		if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_PWR_DOWN) < 0) {
			return -2;
		}
		delay(100); // long wait between AK8963 mode changes
		// set AK8963 to 16 bit resolution, 100 Hz update rate
		if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_CNT_MEAS2) < 0) {
			return -3;
		}
		delay(100); // long wait between AK8963 mode changes
		// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
		mpu9250ReadAK8963Registers(MPU9250_AK8963_HXL, 7);
	}
	/* setting the sample rate divider */
	if (mpu9250WriteRegister(MPU9250_SMPDIV, srd) < 0) { // setting the sample rate divider
		return -4;
	}
	control._srd = srd;
	return 1;
}

/*==================[external functions definition]==========================*/

//Initialize MPU9250 (TODO: include SPI communication)
int8_t mpu9250Init( MPU9250_address_t address )
{
	mpu9250InitializeControlStructure();

	control.address = address;

	// using I2C for communication
	// starting the I2C bus
	i2cInit(I2C0, MPU9250_I2C_RATE);

	// select clock source to gyro
	if (mpu9250WriteRegister(MPU9250_PWR_MGMNT_1, MPU9250_CLOCK_SEL_PLL) < 0) {
		return -1;
	}
	// enable I2C master mode
	if (mpu9250WriteRegister(MPU9250_USER_CTRL, MPU9250_I2C_MST_EN) < 0) {
		return -2;
	}
	// set the I2C bus speed to 400 kHz
	if (mpu9250WriteRegister(MPU9250_I2C_MST_CTRL, MPU9250_I2C_MST_CLK) < 0) {
		return -3;
	}
	// set AK8963 to Power Down
	mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_PWR_DOWN);
	// reset the MPU9250
	mpu9250WriteRegister(MPU9250_PWR_MGMNT_1, MPU9250_PWR_RESET);
	// wait for MPU-9250 to come back up
	delay(1);
	// reset the AK8963
	mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL2, MPU9250_AK8963_RESET);
	// select clock source to gyro
	if (mpu9250WriteRegister(MPU9250_PWR_MGMNT_1, MPU9250_CLOCK_SEL_PLL) < 0) {
		return -4;
	}
	// check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
	if ((mpu9250WhoAmI() != 113) && (mpu9250WhoAmI() != 115)) {
		return -5;
	}
	// enable accelerometer and gyro
	if (mpu9250WriteRegister(MPU9250_PWR_MGMNT_2, MPU9250_SEN_ENABLE) < 0) {
		return -6;
	}
	// setting accel range to 16G as default
	if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG, MPU9250_ACCEL_FS_SEL_16G) < 0) {
		return -7;
	}
	control._accelScale = MPU9250_G * 16.0f / 32767.5f; // setting the accel scale to 16G
	control._accelRange = MPU9250_ACCEL_RANGE_16G;
	// setting the gyro range to 2000DPS as default
	if (mpu9250WriteRegister(MPU9250_GYRO_CONFIG, MPU9250_GYRO_FS_SEL_2000DPS) < 0) {
		return -8;
	}
   // setting the gyro scale to 2000DPS
	control._gyroScale = 2000.0f / 32767.5f * MPU9250_D2R; 
	control._gyroRange = MPU9250_GYRO_RANGE_2000DPS;
	// setting bandwidth to 184Hz as default
	if (mpu9250WriteRegister(MPU9250_ACCEL_CONFIG2, MPU9250_ACCEL_DLPF_184) < 0) {
		return -9;
	}
   // setting gyro bandwidth to 184Hz
	if (mpu9250WriteRegister(MPU9250_CONFIG, MPU9250_GYRO_DLPF_184) < 0) { 
		return -10;
	}
	control._bandwidth = MPU9250_DLPF_BANDWIDTH_184HZ;
	// setting the sample rate divider to 0 as default
	if (mpu9250WriteRegister(MPU9250_SMPDIV, 0x00) < 0) {
		return -11;
	}
	control._srd = 0;
	// enable I2C master mode
	if (mpu9250WriteRegister(MPU9250_USER_CTRL, MPU9250_I2C_MST_EN) < 0) {
		return -12;
	}
	// set the I2C bus speed to 400 kHz
	if (mpu9250WriteRegister(MPU9250_I2C_MST_CTRL, MPU9250_I2C_MST_CLK) < 0) {
		return -13;
	}
	// check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
	if (mpu9250WhoAmIAK8963() != 72) {
		return -14;
	}
	/* get the magnetometer calibration */
	// set AK8963 to Power Down
	if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_PWR_DOWN) < 0) {
		return -15;
	}
	delay(100); // long wait between AK8963 mode changes
	// set AK8963 to FUSE ROM access
	if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_FUSE_ROM) < 0) {
		return -16;
	}
	delay(100); // long wait between AK8963 mode changes
	// read the AK8963 ASA registers and compute magnetometer scale factors
	mpu9250ReadAK8963Registers(MPU9250_AK8963_ASA, 3);
	control._magScaleX = ((((float) control._buffer[0]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f
			/ 32760.0f; // micro Tesla
	control._magScaleY = ((((float) control._buffer[1]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f
			/ 32760.0f; // micro Tesla
	control._magScaleZ = ((((float) control._buffer[2]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f
			/ 32760.0f; // micro Tesla
	// set AK8963 to Power Down
	if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_PWR_DOWN) < 0) {
		return -17;
	}
	delay(100); // long wait between AK8963 mode changes
	// set AK8963 to 16 bit resolution, 100 Hz update rate
	if (mpu9250WriteAK8963Register(MPU9250_AK8963_CNTL1, MPU9250_AK8963_CNT_MEAS2) < 0) {
		return -18;
	}
	delay(100); // long wait between AK8963 mode changes
	// select clock source to gyro
	if (mpu9250WriteRegister(MPU9250_PWR_MGMNT_1, MPU9250_CLOCK_SEL_PLL) < 0) {
		return -19;
	}
	// instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
	mpu9250ReadAK8963Registers(MPU9250_AK8963_HXL, 7);
	// estimate gyro bias
	if (mpu9250CalibrateGyro() < 0) {
		return -20;
	}
	// successful init, return 1
	return 1;
}

//Read sensor registers and store data at control structure
bool_t mpu9250Read(void)
{
	// grab the data from the MPU9250
	if( !mpu9250ReadRegisters(MPU9250_ACCEL_OUT, 21) ){
		return 0;
	}
	// combine into 16 bit values
	control._axcounts = (((int16_t)control._buffer[0]) << 8)  | control._buffer[1];
	control._aycounts = (((int16_t)control._buffer[2]) << 8)  | control._buffer[3];
	control._azcounts = (((int16_t)control._buffer[4]) << 8)  | control._buffer[5];
	control._tcounts  = (((int16_t)control._buffer[6]) << 8)  | control._buffer[7];
	control._gxcounts = (((int16_t)control._buffer[8]) << 8)  | control._buffer[9];
	control._gycounts = (((int16_t)control._buffer[10]) << 8) | control._buffer[11];
	control._gzcounts = (((int16_t)control._buffer[12]) << 8) | control._buffer[13];
	control._hxcounts = (((int16_t)control._buffer[15]) << 8) | control._buffer[14];
	control._hycounts = (((int16_t)control._buffer[17]) << 8) | control._buffer[16];
	control._hzcounts = (((int16_t)control._buffer[19]) << 8) | control._buffer[18];
	// transform and convert to float values
	control._ax = (((float)(control.tX[0]*control._axcounts + control.tX[1]*control._aycounts + control.tX[2]*control._azcounts) * control._accelScale) - control._axb)*control._axs;
	control._ay = (((float)(control.tY[0]*control._axcounts + control.tY[1]*control._aycounts + control.tY[2]*control._azcounts) * control._accelScale) - control._ayb)*control._ays;
	control._az = (((float)(control.tZ[0]*control._axcounts + control.tZ[1]*control._aycounts + control.tZ[2]*control._azcounts) * control._accelScale) - control._azb)*control._azs;
	control._gx = ((float) (control.tX[0]*control._gxcounts + control.tX[1]*control._gycounts + control.tX[2]*control._gzcounts) * control._gyroScale) -  control._gxb;
	control._gy = ((float) (control.tY[0]*control._gxcounts + control.tY[1]*control._gycounts + control.tY[2]*control._gzcounts) * control._gyroScale) -  control._gyb;
	control._gz = ((float) (control.tZ[0]*control._gxcounts + control.tZ[1]*control._gycounts + control.tZ[2]*control._gzcounts) * control._gyroScale) -  control._gzb;
	control._hx = (((float)(control._hxcounts) * control._magScaleX) - control._hxb)*control._hxs;
	control._hy = (((float)(control._hycounts) * control._magScaleY) - control._hyb)*control._hys;
	control._hz = (((float)(control._hzcounts) * control._magScaleZ) - control._hzb)*control._hzs;
	control._t = ((((float) control._tcounts)  - control._tempOffset)/ control._tempScale) + control._tempOffset;
	return 1;
}

// Returns the accelerometer measurement in the x direction, m/s/s
float mpu9250GetAccelX_mss( void )
{
	return control._ax;
}

// Returns the accelerometer measurement in the y direction, m/s/s
float mpu9250GetAccelY_mss( void )
{
	return control._ay;
}

// Returns the accelerometer measurement in the z direction, m/s/s
float mpu9250GetAccelZ_mss( void )
{
	return control._az;
}

// Returns the gyroscope measurement in the x direction, rad/s
float mpu9250GetGyroX_rads( void )
{
	return control._gx;
}

// Returns the gyroscope measurement in the y direction, rad/s
float mpu9250GetGyroY_rads( void )
{
	return control._gy;
}

// Returns the gyroscope measurement in the z direction, rad/s
float mpu9250GetGyroZ_rads( void )
{
	return control._gz;
}

// Returns the magnetometer measurement in the x direction, uT
float mpu9250GetMagX_uT( void )
{
  return control._hx;
}

// Returns the magnetometer measurement in the y direction, uT
float mpu9250GetMagY_uT( void )
{
  return control._hy;
}

// Returns the magnetometer measurement in the z direction, uT
float mpu9250GetMagZ_uT( void )
{
  return control._hz;
}

// Returns the die temperature, C
float mpu9250GetTemperature_C( void )
{
  return control._t;
}

/*==================[end of file]============================================*/
