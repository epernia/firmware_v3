/* Copyright 2016, Ian Olivieri
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

/* Date: 2016-02-10 */

/* TODO: hacer una forma de buscar las funciones que tocan el modulo siguiente
 * All functions relative to the microcontroller */

/*==================[inclusions]=============================================*/

#include "sapi_pwm.h"
#include "sapi_sct.h"

/*==================[macros and definitions]=================================*/

#ifndef EMPTY_POSITION
   #define EMPTY_POSITION 255
#endif

#define PWM_TOTALNUMBER   11   /* From PWM0 to PWM10 */

#define PWM_FREC          1000 /* 1Khz */
#define PWM_PERIOD        1000 /* 1000uS = 1ms*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*
 * @Brief: Initializes the pwm timers.
 * @param  none
 * @return nothing
 */
static void pwmInitTimers(void);

/*
 * @brief:   adds pwm to the the list of working pwms
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:   True if pwm was successfully attached, False if not.
 */
static bool_t pwmAttach( pwmMap_t pwmNumber );

/*
 * @brief:   removes pwm (attached to pwmNumber) from the list
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:    True if pwm was successfully detached, False if not.
 */
static bool_t pwmDetach( pwmMap_t pwmNumber );

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/* Enter a pwm number, get a sct number
 * Since this module works with pwm numbers, but uses sct channels to generate
 * the signal, its necessary to connect pwm number with the SctMap_t (sAPI_PeripheralMap.h).
 * This way the user sets "pwms", while using the sct peripheral internally*/
static const uint8_t pwmMap[PWM_TOTALNUMBER] = {
   /* PWM0 */  CTOUT1,  /* T_FIL1 */
   /* PWM1 */  CTOUT12, /* T_COL2 */
   /* PWM2 */  CTOUT10, /* T_COL0 */
   /* PWM3 */  CTOUT0,  /* T_FIL2 */
   /* PWM4 */  CTOUT3,  /* T_FIL3 */
   /* PWM5 */  CTOUT13, /* T_COL1 */
   /* PWM6 */  CTOUT7,  /* GPIO8  */
   /* PWM7 */  CTOUT2,  /* LED1   */
   /* PWM8 */  CTOUT5,  /* LED2   */
   /* PWM9 */  CTOUT4,  /* LED3   */
   /* PWM10 */ CTOUT6   /* GPIO2  */
};

/*when the user adds a pwm with pwmAttach the list updates with the pin number of the element*/
static uint8_t AttachedPWMList[PWM_TOTALNUMBER] = {
/*Position | Pwm Number*/
   /*0*/  EMPTY_POSITION,
   /*1*/  EMPTY_POSITION,
   /*2*/  EMPTY_POSITION,
   /*3*/  EMPTY_POSITION,
   /*4*/  EMPTY_POSITION,
   /*5*/  EMPTY_POSITION,
   /*6*/  EMPTY_POSITION,
   /*7*/  EMPTY_POSITION,
   /*8*/  EMPTY_POSITION,
   /*9*/	EMPTY_POSITION,
   /*10*/ EMPTY_POSITION,
};

/*==================[internal functions definition]==========================*/

/*
 * @Brief:   Initializes the pwm timers.
 * @param   none
 * @return   nothing
 */
static void pwmInitTimers(void){
   Sct_Init(PWM_FREC);
}

/*
 * @brief:   adds pwm to the the list of working pwms
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:   True if pwm was successfully attached, False if not.
 */
static bool_t pwmAttach( pwmMap_t pwmNumber){

   bool_t success = FALSE;
   uint8_t position = 0;

   position = pwmIsAttached(pwmNumber);
   if(position==0){
      position = pwmIsAttached(EMPTY_POSITION); /* Searches for the first empty position */
      if(position){ /* if position==0 => there is no room in the list for another pwm */
         AttachedPWMList[position-1] = pwmNumber;
         Sct_EnablePwmFor(pwmMap[pwmNumber]);
         success = TRUE;
      }
   }
   return success;
}

/*
 * @brief:   removes pwm (attached to pwmNumber) from the list
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:    True if pwm was successfully detached, False if not.
 */
static bool_t pwmDetach( pwmMap_t pwmNumber ){

   bool_t success = FALSE;
   uint8_t position = 0;

   position = pwmIsAttached(pwmNumber);

   if(position){
      AttachedPWMList[position-1] = EMPTY_POSITION;
      success = TRUE;
   }
   return success;
}

/*==================[external functions definition]==========================*/

/*
 * @brief:   change the value of the pwm at the selected pin
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @param:   value:   8bit value, from 0 to 255
 * @return:   True if the value was successfully changed, False if not.
 */
bool_t pwmWrite( pwmMap_t pwmNumber, uint8_t value ){

   bool_t success = FALSE;
   uint8_t position = 0;

   position = pwmIsAttached(pwmNumber);

   if(position){
      Sct_SetDutyCycle(pwmMap[pwmNumber], value);
      success = TRUE;
   }

   return success;
}

/*
 * @brief:   read the value of the pwm in the pin
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:   value of the pwm in the pin (0 ~ 255).
 *   If an error ocurred, return = EMPTY_POSITION = 255
 */
uint8_t pwmRead( pwmMap_t pwmNumber ){

   uint8_t position = 0, value = 0;
   position = pwmIsAttached(pwmNumber);

   if(position){
      value = Sct_GetDutyCycle(pwmMap[pwmNumber]);
   }else{
      value = EMPTY_POSITION;
   }

   return value;
}


/*
 * @Brief: Initializes the pwm peripheral.
 * @param  uint8_t pwmNumber
 * @param  uint8_t config
 * @return bool_t true (1) if config it is ok
 */
bool_t pwmConfig( pwmMap_t pwmNumber, pwmConfig_t config){

   bool_t ret_val = 1;

   switch(config){

      case PWM_ENABLE:
         pwmInitTimers();
      break;

      case PWM_DISABLE:
         ret_val = 0;
      break;

      case PWM_ENABLE_OUTPUT:
         ret_val = pwmAttach( pwmNumber );
      break;

      case PWM_DISABLE_OUTPUT:
         ret_val = pwmDetach( pwmNumber );
      break;

      default:
         ret_val = 0;
      break;
   }

   return ret_val;
}

/*
 * @brief:   Tells if the pwm is currently active, and its position
 * @param:   pwmNumber:   ID of the pwm, from 0 to 10
 * @return:   position (1 ~ PWM_TOTALNUMBER), 0 if the element was not found.
 */
uint8_t pwmIsAttached( pwmMap_t pwmNumber )
{
   uint8_t position = 0, positionInList = 0;
   while ( (position < PWM_TOTALNUMBER) &&
           (pwmNumber != AttachedPWMList[position]) ) {
      position++;
   }

   if (position < PWM_TOTALNUMBER){
      positionInList = position + 1;
   } else{
      positionInList = 0;
   }

   return positionInList;
}

/*==================[end of file]============================================*/
