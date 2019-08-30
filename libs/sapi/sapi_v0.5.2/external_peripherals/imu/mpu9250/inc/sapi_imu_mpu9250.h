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

#ifndef _SAPI_IMU_MPU9250_H_
#define _SAPI_IMU_MPU9250_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
// physical constants
#define MPU9250_G                     9.807f
#define MPU9250_D2R                   3.14159265359f/180.0f

// MPU9250 registers
#define MPU9250_ACCEL_OUT             0x3B
#define MPU9250_GYRO_OUT              0x43
#define MPU9250_TEMP_OUT              0x41
#define MPU9250_EXT_SENS_DATA_00      0x49
#define MPU9250_ACCEL_CONFIG 	        0x1C
#define MPU9250_ACCEL_FS_SEL_2G       0x00
#define MPU9250_ACCEL_FS_SEL_4G       0x08
#define MPU9250_ACCEL_FS_SEL_8G       0x10
#define MPU9250_ACCEL_FS_SEL_16G      0x18
#define MPU9250_GYRO_CONFIG           0x1B
#define MPU9250_GYRO_FS_SEL_250DPS    0x00
#define MPU9250_GYRO_FS_SEL_500DPS    0x08
#define MPU9250_GYRO_FS_SEL_1000DPS   0x10
#define MPU9250_GYRO_FS_SEL_2000DPS   0x18

#define MPU9250_ACCEL_CONFIG2         0x1D
#define MPU9250_ACCEL_DLPF_184        0x01
#define MPU9250_ACCEL_DLPF_92         0x02
#define MPU9250_ACCEL_DLPF_41         0x03
#define MPU9250_ACCEL_DLPF_20         0x04
#define MPU9250_ACCEL_DLPF_10         0x05
#define MPU9250_ACCEL_DLPF_5          0x06
#define MPU9250_CONFIG                0x1A
#define MPU9250_GYRO_DLPF_184         0x01
#define MPU9250_GYRO_DLPF_92          0x02
#define MPU9250_GYRO_DLPF_41          0x03
#define MPU9250_GYRO_DLPF_20          0x04
#define MPU9250_GYRO_DLPF_10          0x05
#define MPU9250_GYRO_DLPF_5           0x06
#define MPU9250_SMPDIV                0x19
#define MPU9250_INT_PIN_CFG           0x37
#define MPU9250_INT_ENABLE            0x38
#define MPU9250_INT_DISABLE           0x00
#define MPU9250_INT_PULSE_50US        0x00
#define MPU9250_INT_WOM_EN            0x40
#define MPU9250_INT_RAW_RDY_EN        0x01
#define MPU9250_PWR_MGMNT_1           0x6B
#define MPU9250_PWR_CYCLE             0x20
#define MPU9250_PWR_RESET             0x80
#define MPU9250_CLOCK_SEL_PLL         0x01
#define MPU9250_PWR_MGMNT_2           0x6C
#define MPU9250_SEN_ENABLE            0x00
#define MPU9250_DIS_GYRO              0x07
#define MPU9250_USER_CTRL             0x6A
#define MPU9250_I2C_MST_EN            0x20
#define MPU9250_I2C_MST_CLK           0x0D
#define MPU9250_I2C_MST_CTRL          0x24
#define MPU9250_I2C_SLV0_ADDR         0x25
#define MPU9250_I2C_SLV0_REG          0x26
#define MPU9250_I2C_SLV0_DO           0x63
#define MPU9250_I2C_SLV0_CTRL         0x27
#define MPU9250_I2C_SLV0_EN           0x80
#define MPU9250_I2C_READ_FLAG         0x80
#define MPU9250_MOT_DETECT_CTRL       0x69
#define MPU9250_ACCEL_INTEL_EN        0x80
#define MPU9250_ACCEL_INTEL_MODE      0x40
#define MPU9250_LP_ACCEL_ODR          0x1E
#define MPU9250_WOM_THR               0x1F
#define MPU9250_WHO_AM_I              0x75
#define MPU9250_FIFO_EN               0x23
#define MPU9250_FIFO_TEMP             0x80
#define MPU9250_FIFO_GYRO             0x70
#define MPU9250_FIFO_ACCEL            0x08
#define MPU9250_FIFO_MAG              0x01
#define MPU9250_FIFO_COUNT            0x72
#define MPU9250_FIFO_READ             0x74

// AK8963 registers
#define MPU9250_AK8963_I2C_ADDR       0x0C
#define MPU9250_AK8963_HXL            0x03
#define MPU9250_AK8963_CNTL1          0x0A
#define MPU9250_AK8963_PWR_DOWN       0x00
#define MPU9250_AK8963_CNT_MEAS1      0x12
#define MPU9250_AK8963_CNT_MEAS2      0x16
#define MPU9250_AK8963_FUSE_ROM       0x0F
#define MPU9250_AK8963_CNTL2          0x0B
#define MPU9250_AK8963_RESET          0x01
#define MPU9250_AK8963_ASA            0x10
#define MPU9250_AK8963_WHO_AM_I       0x00

// I2C baudrate
#define MPU9250_I2C_RATE              400000 // 400 kHz

/*==================[typedef]================================================*/

//Different options for basic MPU9250 setting registers

typedef enum {
   MPU9250_ADDRESS_0 = 0x68,
   MPU9250_ADDRESS_1 = 0x69
} MPU9250_address_t;

typedef enum
{
   MPU9250_ACCEL_RANGE_2G,
   MPU9250_ACCEL_RANGE_4G,
   MPU9250_ACCEL_RANGE_8G,
   MPU9250_ACCEL_RANGE_16G
} MPU9250_AccelRange_t;

typedef enum
{
   MPU9250_GYRO_RANGE_250DPS,
   MPU9250_GYRO_RANGE_500DPS,
   MPU9250_GYRO_RANGE_1000DPS,
   MPU9250_GYRO_RANGE_2000DPS
} MPU9250_GyroRange_t;

typedef enum
{
   MPU9250_DLPF_BANDWIDTH_184HZ,
   MPU9250_DLPF_BANDWIDTH_92HZ,
   MPU9250_DLPF_BANDWIDTH_41HZ,
   MPU9250_DLPF_BANDWIDTH_20HZ,
   MPU9250_DLPF_BANDWIDTH_10HZ,
   MPU9250_DLPF_BANDWIDTH_5HZ
} MPU9250_DlpfBandwidth_t;

typedef enum
{
   MPU9250_LP_ACCEL_ODR_0_24HZ  = 0,
   MPU9250_LP_ACCEL_ODR_0_49HZ  = 1,
   MPU9250_LP_ACCEL_ODR_0_98HZ  = 2,
   MPU9250_LP_ACCEL_ODR_1_95HZ  = 3,
   MPU9250_LP_ACCEL_ODR_3_91HZ  = 4,
   MPU9250_LP_ACCEL_ODR_7_81HZ  = 5,
   MPU9250_LP_ACCEL_ODR_15_63HZ = 6,
   MPU9250_LP_ACCEL_ODR_31_25HZ = 7,
   MPU9250_LP_ACCEL_ODR_62_50HZ = 8,
   MPU9250_LP_ACCEL_ODR_125HZ   = 9,
   MPU9250_LP_ACCEL_ODR_250HZ   = 10,
   MPU9250_LP_ACCEL_ODR_500HZ   = 11
} MPU9250_LpAccelOdr_t;

//Control structure for MPU9250 operation (only one IMU per project)
typedef struct {
   MPU9250_address_t address; //MPU9250 address can be configured through AD0 pin
   
   // scale factors
   float _accelScale;
   float _gyroScale;
   float _magScaleX;
   float _magScaleY;
   float _magScaleZ;
   float _tempScale;
   float _tempOffset;
   
   // configuration
   MPU9250_AccelRange_t    _accelRange;
   MPU9250_GyroRange_t     _gyroRange;
   MPU9250_DlpfBandwidth_t _bandwidth;
   uint8_t _srd;

   // buffer for reading from sensor
   uint8_t _buffer[21];

   // data buffer
   float _ax, _ay, _az;
   float _gx, _gy, _gz;
   float _hx, _hy, _hz;
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

   // magnetometer bias and scale factor estimation
   uint16_t _maxCounts;
   float _deltaThresh;
   uint8_t _coeff;
   uint16_t _counter;
   float _framedelta, _delta;
   float _hxfilt, _hyfilt, _hzfilt;
   float _hxmax, _hymax, _hzmax;
   float _hxmin, _hymin, _hzmin;
   float _hxb, _hyb, _hzb;
   float _hxs;
   float _hys;
   float _hzs;
   float _avgs;

   // data counts
   int16_t _axcounts, _aycounts, _azcounts;
   int16_t _gxcounts, _gycounts, _gzcounts;
   int16_t _hxcounts, _hycounts, _hzcounts;
   int16_t _tcounts;

   // transformation matrix
   /* transform the accel and gyro axes to match the magnetometer axes */
   int16_t tX[3];
   int16_t tY[3];
   int16_t tZ[3];

   // track success of interacting with sensor
   int8_t _status;

} MPU9250_control_t;

/*==================[external functions declaration]=========================*/

// Initialize MPU9250 (Only I2C)
int8_t mpu9250Init( MPU9250_address_t address );

// Read sensor registers and store data at control structure
bool_t mpu9250Read(void);


// Returns the accelerometer measurement in the x direction, m/s/s
float mpu9250GetAccelX_mss( void );

// Returns the accelerometer measurement in the y direction, m/s/s
float mpu9250GetAccelY_mss( void );

// Returns the accelerometer measurement in the z direction, m/s/s
float mpu9250GetAccelZ_mss( void );

// Returns the gyroscope measurement in the x direction, rad/s
float mpu9250GetGyroX_rads( void );

// Returns the gyroscope measurement in the y direction, rad/s
float mpu9250GetGyroY_rads( void );

// Returns the gyroscope measurement in the z direction, rad/s
float mpu9250GetGyroZ_rads( void );

// Returns the magnetometer measurement in the x direction, uT
float mpu9250GetMagX_uT( void );

// Returns the magnetometer measurement in the y direction, uT
float mpu9250GetMagY_uT( void );

// Returns the magnetometer measurement in the z direction, uT
float mpu9250GetMagZ_uT( void );

// Returns the die temperature, C
float mpu9250GetTemperature_C( void );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_IMU_MPU9250_H_ */
