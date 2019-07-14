/* Copyright 2017, Danilo Zecchin.
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

/*
 * Ultrasonic sensor HC-SR04 API
 * Date: 2017-11-17
 */

#ifndef _SAPI_ULTRASONICSENSOR_H_
#define _SAPI_ULTRASONICSENSOR_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define ultrasonicSensorConfig ultrasonicSensorInit

/*==================[typedef]================================================*/

typedef enum {
	ULTRASONIC_SENSOR_0, ULTRASONIC_SENSOR_1, ULTRASONIC_SENSOR_2
} ultrasonicSensorMap_t;

typedef enum {
	ULTRASONIC_SENSOR_ENABLE, ULTRASONIC_SENSOR_DISABLE
} ultrasonicSensorInit_t;

typedef enum {
	CM, INCH
} unitMap_t;

/*==================[external functions declaration]=========================*/

/*
 * @Brief   Configure an ultrasonic sensor
 * @param   aSensor:   sensor number (0 to 2)
 * @param   aConfig:   enable or disable sensor
 * @return   nothing
 */
void ultrasonicSensorInit( ultrasonicSensorMap_t aSensor, ultrasonicSensorInit_t aConfig );

/*
 * @Brief   retrieves sensor actual distance in the specified unit
 * @param   aSensor:	sensor number (0 to 2)
 * @param   anUnit:		returned value unit (centimeters, inch, etc)
 * @return  float value with measured distance
 */
float ultrasonicSensorGetDistance( ultrasonicSensorMap_t aSensor, unitMap_t anUnit );

/*==================[ISR external functions declaration]=====================*/
/*
 * @Brief:   GPIO Echo interrupt handler for each sensor
 */
void GPIO0_IRQHandler(void);
void GPIO1_IRQHandler(void);
void GPIO2_IRQHandler(void);

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_ULTRASONICSENSOR_H_ */
