/* Copyright 2011, ChaN.
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

/* Date: 2016-03-07 */

#ifndef _SAPI_RTC_H_
#define _SAPI_RTC_H_

/*==================[inclusions]=============================================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and definitions]=================================*/

/*==================[typedef]================================================*/

typedef struct {
   uint16_t year;	 /* 1 to 4095 */
   uint8_t  month; /* 1 to 12   */
   uint8_t  mday;	 /* 1 to 31   */
   uint8_t  wday;	 /* 1 to 7    */
   uint8_t  hour;	 /* 0 to 23   */
   uint8_t  min;	 /* 0 to 59   */
   uint8_t  sec;	 /* 0 to 59   */
} rtc_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*
 * @Brief: Configure RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcConfig( rtc_t * rtc );

/*
 * @Brief: Get time from RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcRead( rtc_t * rtc );

/*
 * @Brief: Set time on RTC peripheral.
 * @param  RTC_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcWrite( rtc_t * rtc );

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_RTC_H_ */
