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

/*==================[inclusions]=============================================*/

#include "sapi_rtc.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/*
 * @Brief: Configure RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcConfig( rtc_t * rtc ){

   bool_t ret_val = 1;

   static bool_t init;
   RTC_TIME_T rtcTime;

   if( init ){
      /* Already initialized */
      ret_val = 0;
   } else {

      /* RTC Block section ------------------------- */
      Chip_RTC_Init(LPC_RTC);

      /* Set current time for RTC */
      /* Current time is 22:00:00 , 2016-07-02 */
      /*
      rtcTime.time[RTC_TIMETYPE_SECOND]     = 0;
      rtcTime.time[RTC_TIMETYPE_MINUTE]     = 0;
      rtcTime.time[RTC_TIMETYPE_HOUR]       = 22;
      rtcTime.time[RTC_TIMETYPE_DAYOFMONTH] = 2;
      rtcTime.time[RTC_TIMETYPE_MONTH]      = 7;
      rtcTime.time[RTC_TIMETYPE_YEAR]       = 2016;
      Chip_RTC_SetFullAlarmTime(LPC_RTC, &rtcTime);
      */
      rtcWrite( rtc );

      /* Enable rtc (starts increase the tick counter
         and second counter register) */
      Chip_RTC_Enable(LPC_RTC, ENABLE);

      init = 1;
   }

   return ret_val;
}

/*
 * @Brief: Get time from RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcRead( rtc_t * rtc ){

   bool_t ret_val = 1;

   RTC_TIME_T rtcTime;

   Chip_RTC_GetFullTime(LPC_RTC, &rtcTime);

   rtc->sec = rtcTime.time[RTC_TIMETYPE_SECOND];
   rtc->min = rtcTime.time[RTC_TIMETYPE_MINUTE];
   rtc->hour = rtcTime.time[RTC_TIMETYPE_HOUR];
   rtc->wday = rtcTime.time[RTC_TIMETYPE_DAYOFWEEK];
   rtc->mday = rtcTime.time[RTC_TIMETYPE_DAYOFMONTH];
   rtc->month = rtcTime.time[RTC_TIMETYPE_MONTH];
   rtc->year = rtcTime.time[RTC_TIMETYPE_YEAR];

   return ret_val;
}

/*
 * @Brief: Set time on RTC peripheral.
 * @param  rtc_t rtc: RTC structure
 * @return bool_t true (1) if config it is ok
 */
bool_t rtcWrite( rtc_t * rtc ){

   bool_t ret_val = 1;

   RTC_TIME_T rtcTime;

   rtcTime.time[RTC_TIMETYPE_SECOND]     = rtc->sec;
   rtcTime.time[RTC_TIMETYPE_MINUTE]     = rtc->min;
   rtcTime.time[RTC_TIMETYPE_HOUR]       = rtc->hour;
   rtcTime.time[RTC_TIMETYPE_DAYOFMONTH] = rtc->wday;
   rtcTime.time[RTC_TIMETYPE_DAYOFMONTH] = rtc->mday;
   rtcTime.time[RTC_TIMETYPE_MONTH]      = rtc->month;
   rtcTime.time[RTC_TIMETYPE_YEAR]	     = rtc->year;

   Chip_RTC_SetFullTime(LPC_RTC, &rtcTime);

   return ret_val;
}

/*==================[end of file]============================================*/
