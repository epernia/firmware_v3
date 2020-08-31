/* Copyright 2017 Bolder Flight Systems <brian.taylor@bolderflight.com>.
 * Copyright 2018, Sergio Renato De Jesus Melean <sergiordj@gmail.com>.
 * Copyright 2018, Eric Pernia.
 * Copyright 2019, Facundo Adrian Lucianna <facundolucianna@gmail.com>.
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

/* Date: 2019-03-10 */

#ifndef _SAPI_IMU_MPU60X0_H_
#define _SAPI_IMU_MPU60X0_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
// physical constants
#define MPU60X0_G                     9.807f
#define MPU60X0_D2R                   3.14159265359f/180.0f

// MPU60X0 registers directions
#define MPU60X0_SELF_TEST_X           0x0D
#define MPU60X0_SELF_TEST_Y           0x0E
#define MPU60X0_SELF_TEST_Z           0x0F
#define MPU60X0_SELF_TEST_A           0x10                //These registers are used for gyroscope and accelerometer self-tests
#define MPU60X0_SMPRT_DIV             0x19                //Sample Rate Divider
#define MPU60X0_CONFIG                0x1A
#define MPU60X0_GYRO_CONFIG           0x1B                // Gyroscope Configuration
#define MPU60X0_ACCEL_CONFIG          0x1C                // Accelerometer Configuration
#define MPU60X0_FIFO_EN               0x23
#define MPU60X0_I2C_MST_CTRL          0x24
#define MPU60X0_I2C_SLV0_ADDR         0x25
#define MPU60X0_I2C_SLV0_REG          0x26
#define MPU60X0_I2C_SLV0_CTRL         0x27
#define MPU60X0_I2C_SLV1_ADDR         0x28
#define MPU60X0_I2C_SLV1_REG          0x29
#define MPU60X0_I2C_SLV1_CTRL         0x2A
#define MPU60X0_I2C_SLV2_ADDR         0x2B
#define MPU60X0_I2C_SLV2_REG          0x2C
#define MPU60X0_I2C_SLV2_CTRL         0x2D
#define MPU60X0_I2C_SLV3_ADDR         0x2E
#define MPU60X0_I2C_SLV3_REG          0x2F
#define MPU60X0_I2C_SLV3_CTRL         0x30
#define MPU60X0_I2C_SLV4_ADDR         0x31
#define MPU60X0_I2C_SLV4_REG          0x32
#define MPU60X0_I2C_SLV4_DO           0x33
#define MPU60X0_I2C_SLV4_CTRL         0x34
#define MPU60X0_I2C_SLV4_DI           0x35
#define MPU60X0_I2C_MST_STATUS        0x36
#define MPU60X0_INT_PIN_CFG           0x37
#define MPU60X0_INT_ENABLE            0x38
#define MPU60X0_INT_STATUS            0x3A
#define MPU60X0_ACCEL_OUT             0x3B
//#define MPU60X0_ACCEL_XOUT_H          0x3B
//#define MPU60X0_ACCEL_XOUT_L          0x3C
//#define MPU60X0_ACCEL_YOUT_H          0x3D
//#define MPU60X0_ACCEL_YOUT_L          0x3E
//#define MPU60X0_ACCEL_ZOUT_H          0x3F
//#define MPU60X0_ACCEL_ZOUT_L          0x40
#define MPU60X0_TEMP_OUT_H            0x41
#define MPU60X0_TEMP_OUT_L            0x42
#define MPU60X0_GYRO_XOUT_H           0x43
#define MPU60X0_GYRO_XOUT_L           0x44
#define MPU60X0_GYRO_YOUT_H           0x45
#define MPU60X0_GYRO_YOUT_L           0x46
#define MPU60X0_GYRO_ZOUT_H           0x47
#define MPU60X0_GYRO_ZOUT_L           0x48
#define MPU60X0_EXT_SENS_DATA_00      0x49
#define MPU60X0_EXT_SENS_DATA_01      0x4A
#define MPU60X0_EXT_SENS_DATA_02      0x4B
#define MPU60X0_EXT_SENS_DATA_03      0x4C
#define MPU60X0_EXT_SENS_DATA_04      0x4D
#define MPU60X0_EXT_SENS_DATA_05      0x4E
#define MPU60X0_EXT_SENS_DATA_06      0x4F
#define MPU60X0_EXT_SENS_DATA_07      0x50
#define MPU60X0_EXT_SENS_DATA_08      0x51
#define MPU60X0_EXT_SENS_DATA_09      0x52
#define MPU60X0_EXT_SENS_DATA_10      0x53
#define MPU60X0_EXT_SENS_DATA_11      0x54
#define MPU60X0_EXT_SENS_DATA_12      0x55
#define MPU60X0_EXT_SENS_DATA_13      0x56
#define MPU60X0_EXT_SENS_DATA_14      0x57
#define MPU60X0_EXT_SENS_DATA_15      0x58
#define MPU60X0_EXT_SENS_DATA_16      0x59
#define MPU60X0_EXT_SENS_DATA_17      0x5A
#define MPU60X0_EXT_SENS_DATA_18      0x5B
#define MPU60X0_EXT_SENS_DATA_19      0x5C
#define MPU60X0_EXT_SENS_DATA_20      0x5D
#define MPU60X0_EXT_SENS_DATA_21      0x5E
#define MPU60X0_EXT_SENS_DATA_22      0x5F
#define MPU60X0_EXT_SENS_DATA_23      0x60
#define MPU60X0_I2C_SLV0_DO           0x63
#define MPU60X0_I2C_SLV1_DO           0x64
#define MPU60X0_I2C_SLV2_DO           0x65
#define MPU60X0_I2C_SLV3_DO           0x66
#define MPU60X0_I2C_MST_DELAY_CTRL    0x67
#define MPU60X0_SIGNAL_PATH_RESET     0x68
#define MPU60X0_USER_CTRL             0x6A
#define MPU60X0_PWR_MGMT_1            0x6B
#define MPU60X0_PWR_MGMT_2            0x6C
#define MPU60X0_FIFO_COUNT_H          0x72
#define MPU60X0_FIFO_COUNT_L          0x73
#define MPU60X0_FIFO_R_W              0x74
#define MPU60X0_WHO_AM_I              0x75

// Registers configurations
// MPU60X0_PWR_MGMT_1
#define MPU60X0_CLOCK_SEL_INT_CLK     0x00
#define MPU60X0_CLOCK_SEL_PLL_X_GYRO  0x01
#define MPU60X0_CLOCK_SEL_PLL_Y_GYRO  0x02
#define MPU60X0_CLOCK_SEL_PLL_Z_GYRO  0x03
#define MPU60X0_CLOCK_SEL_PLL_EXT_32K 0x04
#define MPU60X0_CLOCK_SEL_PLL_EXT_19M 0x05
#define MPU60X0_PWR_RESET             0x80
// MPU60X0_PWR_MGMT_2
#define MPU60X0_SEN_ENABLE            0x00
// MPU60X0_USER_CTRL
#define MPU60X0_I2C_MST_EN            0x20
// MPU60X0_I2C_MST_CTRL
#define MPU60X0_I2C_MST_CLK           0x0D
// MPU60X0_CONFIG
#define MPU60X0_DLPF_260HZ            0x00  // Accel: 260 Hz (Fs = 1Khz). Gyro: 256 Hz (Fs = 8Khz)
#define MPU60X0_DLPF_184HZ            0x01  // Accel: 184 Hz (Fs = 1Khz). Gyro: 188 Hz (Fs = 1Khz)
#define MPU60X0_DLPF_94HZ             0x02  // Accel: 94 Hz (Fs = 1Khz). Gyro: 98 Hz (Fs = 1Khz)
#define MPU60X0_DLPF_44HZ             0x03  // Accel: 44 Hz (Fs = 1Khz). Gyro: 42 Hz (Fs = 1Khz)
#define MPU60X0_DLPF_21HZ             0x04  // Accel: 21 Hz (Fs = 1Khz). Gyro: 20 Hz (Fs = 1Khz)
#define MPU60X0_DLPF_10HZ             0x05  // Accel: 10 Hz (Fs = 1Khz). Gyro: 10 Hz (Fs = 1Khz)
#define MPU60X0_DLPF_5HZ              0x06  // Accel: 5 Hz (Fs = 1Khz). Gyro: 5 Hz (Fs = 1Khz)
// MPU60X0_GYRO_CONFIG
#define MPU60X0_GYRO_FS_SEL_250DPS    0x00
#define MPU60X0_GYRO_FS_SEL_500DPS    0x08
#define MPU60X0_GYRO_FS_SEL_1000DPS   0x10
#define MPU60X0_GYRO_FS_SEL_2000DPS   0x18
// MPU60X0_ACCEL_CONFIG
#define MPU60X0_ACCEL_FS_SEL_2G       0x00
#define MPU60X0_ACCEL_FS_SEL_4G       0x08
#define MPU60X0_ACCEL_FS_SEL_8G       0x10
#define MPU60X0_ACCEL_FS_SEL_16G      0x18


// I2C baudrate
#define MPU60X0_I2C_RATE              400000 // 400 kHz

/*==================[typedef]================================================*/

//Different options for basic MPU60X0 setting registers

typedef enum {
   MPU60X0_ADDRESS_0 = 0x68,
   MPU60X0_ADDRESS_1 = 0x69
} MPU60X0_address_t;

typedef enum
{
   MPU60X0_ACCEL_RANGE_2G,
   MPU60X0_ACCEL_RANGE_4G,
   MPU60X0_ACCEL_RANGE_8G,
   MPU60X0_ACCEL_RANGE_16G
} MPU60X0_AccelRange_t;

typedef enum
{
   MPU60X0_GYRO_RANGE_250DPS,
   MPU60X0_GYRO_RANGE_500DPS,
   MPU60X0_GYRO_RANGE_1000DPS,
   MPU60X0_GYRO_RANGE_2000DPS
} MPU60X0_GyroRange_t;

typedef enum
{
   MPU60X0_DLPF_BANDWIDTH_260HZ,    // Accel: 260 Hz (Fs = 1Khz). Gyro: 256 Hz (Fs = 8Khz)
   MPU60X0_DLPF_BANDWIDTH_184HZ,    // Accel: 184 Hz (Fs = 1Khz). Gyro: 188 Hz (Fs = 1Khz)
   MPU60X0_DLPF_BANDWIDTH_94HZ,     // Accel: 94 Hz (Fs = 1Khz). Gyro: 98 Hz (Fs = 1Khz)
   MPU60X0_DLPF_BANDWIDTH_44HZ,     // Accel: 44 Hz (Fs = 1Khz). Gyro: 42 Hz (Fs = 1Khz)
   MPU60X0_DLPF_BANDWIDTH_21HZ,     // Accel: 21 Hz (Fs = 1Khz). Gyro: 20 Hz (Fs = 1Khz)
   MPU60X0_DLPF_BANDWIDTH_10HZ,     // Accel: 10 Hz (Fs = 1Khz). Gyro: 10 Hz (Fs = 1Khz)
   MPU60X0_DLPF_BANDWIDTH_5HZ       // Accel: 5 Hz (Fs = 1Khz). Gyro: 5 Hz (Fs = 1Khz)
} MPU60X0_DlpfBandwidth_t;

//Control structure for MPU60X0 operation (only one IMU per project)
typedef struct {
   MPU60X0_address_t address; //MPU60X0 address can be configured through AD0 pin

   // scale factors
   float _accelScale;
   float _gyroScale;

   float _tempScale;
   float _tempOffset;

   // configuration
   MPU60X0_AccelRange_t    _accelRange;
   MPU60X0_GyroRange_t     _gyroRange;
   MPU60X0_DlpfBandwidth_t _bandwidth;
   uint8_t _srd;

   // buffer for reading from sensor
   uint8_t _buffer[21];

   // data buffer
   float _ax, _ay, _az;
   float _gx, _gy, _gz;
   float _t;

   // gyro bias estimation
   uint8_t _numSamples;
   double _gxbD, _gybD, _gzbD;
   float _gxb, _gyb, _gzb;

   // accel bias and scale factor estimation
   double _axbD, _aybD, _azbD;
   float _axmax, _aymax, _azmax;
   float _axmin, _aymin, _azmin;
   float _axb, _ayb, _azb;
   float _axs;
   float _ays;
   float _azs;

   // data counts
   int16_t _axcounts, _aycounts, _azcounts;
   int16_t _gxcounts, _gycounts, _gzcounts;
   int16_t _tcounts;

   // transformation matrix
   /* transform the accel and gyro axes to match the magnetometer axes */
   int16_t tX[3];
   int16_t tY[3];
   int16_t tZ[3];

   // track success of interacting with sensor
   int8_t _status;

} MPU60X0_control_t;

/*==================[external functions declaration]=========================*/

// Initialize mpu60X0 (Only I2C)
int8_t mpu60X0Init( MPU60X0_address_t address );

// Read sensor registers and store data at control structure
bool_t mpu60X0Read(void);

// Returns the accelerometer measurement in the x direction, m/s/s
float mpu60X0GetAccelX_mss( void );

// Returns the accelerometer measurement in the y direction, m/s/s
float mpu60X0GetAccelY_mss( void );

// Returns the accelerometer measurement in the z direction, m/s/s
float mpu60X0GetAccelZ_mss( void );

// Returns the gyroscope measurement in the x direction, rad/s
float mpu60X0GetGyroX_rads( void );

// Returns the gyroscope measurement in the y direction, rad/s
float mpu60X0GetGyroY_rads( void );

// Returns the gyroscope measurement in the z direction, rad/s
float mpu60X0GetGyroZ_rads( void );

// Returns the die temperature, C
float mpu60X0GetTemperature_C( void );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_IMU_MPU60X0_H_ */
