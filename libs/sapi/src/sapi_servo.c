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

/*TODO: make a graphic and explanation of the timer ramp and compare match values
 to make everything clear */

/*==================[inclusions]=============================================*/

#include "sapi_gpio.h"
#include "sapi_servo.h"
#include "sapi_timer.h"

/*==================[macros and definitions]=================================*/

#define EMPTY_POSITION   255
#define SERVO_TOTALNUMBER   9
#define SERVO_COMPLETECYCLE_PERIOD   20000  /*value in uSec */
#define SERVO_MAXUPTIME_PERIOD   2000  /*value in uSec */
#define SERVO_MINUPTIME_PERIOD   500 /*value in uSec */

/*==================[internal data declaration]==============================*/

typedef struct
{
   uint8_t servo; /*Servo number. It's mapped to the DIOMap_t in the servoMap*/
   uint8_t value; /*Value of the servo*/
   /*To manage each servo more efficiently, every one of them has a default
   * timer, match number and function associated depending of their position in the list
   * This can be done because all timers initialize with the same period (20ms). So,  if
   * you need different frequencies for different timers you will have to change that since
   * it won't be the same to attach a servo in one position or another in the list*/
   uint8_t associatedTimer;
   uint8_t associatedCompareMatch;
   voidFunctionPointer_t associatedFunction;

}attachedServo_t;

/*==================[internal functions declaration]=========================*/

static uint32_t valueToMicroseconds(uint8_t );

static void timer1CompareMatch0func(void);
static void timer1CompareMatch1func(void);
static void timer1CompareMatch2func(void);
static void timer1CompareMatch3func(void);

static void timer2CompareMatch0func(void);
static void timer2CompareMatch1func(void);
static void timer2CompareMatch2func(void);
static void timer2CompareMatch3func(void);

static void timer3CompareMatch0func(void);
static void timer3CompareMatch1func(void);
static void timer3CompareMatch2func(void);
static void timer3CompareMatch3func(void);

static void servoInitTimers( void );
static bool_t servoAttach( servoMap_t servoNumber );
static bool_t servoDetach( servoMap_t servoNumber );

/*==================[internal data definition]===============================*/

/* Enter a servo number, get a Gpio number
 * Since this module works with servo numbers, but uses gpio pins to generate
 * the signal, its necessary to connect servo number with the DIOMap_t (sAPI_PeripheralMap.h).
 * This way the user sets "servos", while using gpio outputs internally so the gpioWrite()
 * function can be easily used*/
static const uint8_t servoMap[SERVO_TOTALNUMBER] =
{
   /* Servo name | DIOMap name | Name in the board*/
   /* SERVO0 */ T_FIL1, /* T_FIL1 */
   /* SERVO1 */ T_COL0, /* T_COL0 */
   /* SERVO2 */ T_FIL2, /* T_FIL2 */
   /* SERVO3 */ T_FIL3, /* T_FIL3 */
   /* SERVO4 */ GPIO8,  /* GPIO8  */
   /* SERVO5 */ LCD1,   /* LCD1   */
   /* SERVO6 */ LCD2,   /* LCD2   */
   /* SERVO7 */ LCD3,   /* LCD3   */
   /* SERVO8 */ GPIO2   /* GPIO2  */
};

/*when the user adds a servo with servoAttach the list updates with the servo number*/
static attachedServo_t AttachedServoList[SERVO_TOTALNUMBER] =
{
/*position |Servo number | value | asociatedTimer | associatedCompareMatch | associatedFunction*/
   /*0*/	{EMPTY_POSITION , 0,  TIMER1 , TIMERCOMPAREMATCH1 , timer1CompareMatch1func},
   /*1*/	{EMPTY_POSITION , 0 , TIMER1 , TIMERCOMPAREMATCH2 , timer1CompareMatch2func},
   /*2*/	{EMPTY_POSITION , 0 , TIMER1 , TIMERCOMPAREMATCH3 , timer1CompareMatch3func},
   /*3*/	{EMPTY_POSITION , 0 , TIMER2 , TIMERCOMPAREMATCH1 , timer2CompareMatch1func},
   /*4*/	{EMPTY_POSITION , 0 , TIMER2 , TIMERCOMPAREMATCH2 , timer2CompareMatch2func},
   /*5*/	{EMPTY_POSITION , 0 , TIMER2 , TIMERCOMPAREMATCH3 , timer2CompareMatch3func},
   /*6*/	{EMPTY_POSITION , 0 , TIMER3 , TIMERCOMPAREMATCH1 , timer3CompareMatch1func},
   /*7*/	{EMPTY_POSITION , 0 , TIMER3 , TIMERCOMPAREMATCH2 , timer3CompareMatch2func},
   /*8*/	{EMPTY_POSITION , 0 , TIMER3 , TIMERCOMPAREMATCH3 , timer3CompareMatch3func}
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*
 * @brief   Converts a value in value to micro seconds for a specific type of servo (see the defines)
 * @param   value:   value of the servo, from 0 to 180
 * @return   Equivalent in microseconds for a specific type of servo (see the defines)
 * @note   Should be used with Timer_microsecondsToTicks to use some of
 *   the functions in the sAPI_Timer that requires ticks as a parameter
*/
static uint32_t valueToMicroseconds(uint8_t value){

   return (SERVO_MINUPTIME_PERIOD+(value*SERVO_MAXUPTIME_PERIOD)/180);
}

/*
 * @brief:	compare match 0 function. The one that is executed when the cycle ends
 * (to visualize it, think about the 'timer ramp')
 * @note:   this function can't be generalized because sAPI_Timer functions expect void-void function pointers
 */
static void timer1CompareMatch0func(void)
{
   uint8_t servoListPosition= 0;

   for(servoListPosition=0;servoListPosition<3;servoListPosition++)
   {
      if(AttachedServoList[servoListPosition].servo != EMPTY_POSITION)
      {
         gpioWrite(servoMap[AttachedServoList[servoListPosition].servo],TRUE);
         Timer_SetCompareMatch( 	AttachedServoList[servoListPosition].associatedTimer,
                                    AttachedServoList[servoListPosition].associatedCompareMatch,
                                    Timer_microsecondsToTicks(valueToMicroseconds(AttachedServoList[servoListPosition].value)));
      }
   }
}

static void timer1CompareMatch1func(void)
{
   gpioWrite(servoMap[AttachedServoList[0].servo],FALSE);
}

static void timer1CompareMatch2func(void)
{
   gpioWrite(servoMap[AttachedServoList[1].servo],FALSE);
}

static void timer1CompareMatch3func(void)
{
   gpioWrite(servoMap[AttachedServoList[2].servo],FALSE);
}

static void timer2CompareMatch0func(void)
{
   uint8_t servoListPosition= 3;

   for(servoListPosition=3;servoListPosition<6;servoListPosition++)
   {
      if(AttachedServoList[servoListPosition].servo != EMPTY_POSITION)
      {
         gpioWrite(servoMap[AttachedServoList[servoListPosition].servo],TRUE);
         Timer_SetCompareMatch( AttachedServoList[servoListPosition].associatedTimer,
                                 AttachedServoList[servoListPosition].associatedCompareMatch,
                                 Timer_microsecondsToTicks(valueToMicroseconds(AttachedServoList[servoListPosition].value)));
		}
	}
}

static void timer2CompareMatch1func(void){
   gpioWrite(servoMap[AttachedServoList[3].servo],FALSE);
}

static void timer2CompareMatch2func(void){
   gpioWrite(servoMap[AttachedServoList[4].servo],FALSE);
}

static void timer2CompareMatch3func(void){
   gpioWrite(servoMap[AttachedServoList[5].servo],FALSE);
}

static void timer3CompareMatch0func(void){

   uint8_t servoListPosition= 6;

   for(servoListPosition=6;servoListPosition<9;servoListPosition++)
   {
      if(AttachedServoList[servoListPosition].servo != EMPTY_POSITION)
      {
         gpioWrite(servoMap[AttachedServoList[servoListPosition].servo],TRUE);
         Timer_SetCompareMatch( AttachedServoList[servoListPosition].associatedTimer,
                                 AttachedServoList[servoListPosition].associatedCompareMatch,
                                 Timer_microsecondsToTicks(valueToMicroseconds(AttachedServoList[servoListPosition].value)));
      }
   }
}

static void timer3CompareMatch1func(void){
   gpioWrite(servoMap[AttachedServoList[6].servo],FALSE);
}

static void timer3CompareMatch2func(void){
   gpioWrite(servoMap[AttachedServoList[7].servo],FALSE);
}

static void timer3CompareMatch3func(void){
   gpioWrite(servoMap[AttachedServoList[8].servo],FALSE);
}


/*
 * @Brief: Initializes the servo peripheral
 * @param   none
 * @return   nothing
 * @IMPORTANT:   this function uses Timer 1, 2 and 3 to generate the servo signals, so
 *   they won't be available to use.
 */
static void servoInitTimers(void){
   Timer_Init( TIMER1,
               Timer_microsecondsToTicks(SERVO_COMPLETECYCLE_PERIOD),
               timer1CompareMatch0func
              );
   Timer_Init( TIMER2,
               Timer_microsecondsToTicks(SERVO_COMPLETECYCLE_PERIOD),
               timer2CompareMatch0func
             );
   Timer_Init( TIMER3,
               Timer_microsecondsToTicks(SERVO_COMPLETECYCLE_PERIOD),
               timer3CompareMatch0func
             );
}

/*
 * @brief: adds a servo to the active servos list
 * @param   servoNumber:   ID of the servo, from 0 to 8
 * @return: True if servo was successfully attached, False if not.
 */
static bool_t servoAttach( servoMap_t servoNumber)
{
   bool_t success = FALSE;
   uint8_t position = 0;

   /* Pin must b config as Output */
   gpioConfig( (gpioMap_t)servoNumber, GPIO_OUTPUT );

   position = servoIsAttached(servoNumber);
   if(position==0)
   {
      position = servoIsAttached(EMPTY_POSITION); /* Searches for the first empty position */
      if(position) /* if position==0 => there is no room in the list for another servo */
      {
         AttachedServoList[position-1].servo = servoNumber;
         /* Enables the compare match interrupt */
         Timer_EnableCompareMatch( AttachedServoList[position-1].associatedTimer,
                                    AttachedServoList[position-1].associatedCompareMatch,
                                    Timer_microsecondsToTicks(valueToMicroseconds(AttachedServoList[position-1].value)),
                                    AttachedServoList[position-1].associatedFunction
                                 );
         success = TRUE;
      }
   }

   return success;
}

/*
 * @brief: removes a servo from the active servos list
 * @param   servoNumber:   ID of the servo, from 0 to 8
 * @return: True if servo was successfully detached, False if not.
 */
static bool_t servoDetach( servoMap_t servoNumber )
{
   bool_t success = FALSE;
   uint8_t position = 0;

   position = servoIsAttached(servoNumber);

   if(position)
   {
      AttachedServoList[position-1].servo = EMPTY_POSITION;
      AttachedServoList[position-1].value = 0;
      Timer_DisableCompareMatch( AttachedServoList[position-1].associatedTimer,
                                 AttachedServoList[position-1].associatedCompareMatch);
      success = TRUE;
   }
   return success;
}

/*==================[external functions definition]==========================*/

/*
 * @Brief: Initializes the servo peripheral
 * @param  uint8_t servoNumber
 * @param  uint8_t config
 * @return   nothing
 * @IMPORTANT:   this function uses Timer 1, 2 and 3 to generate the servo signals, so
 *   they won't be available to use.
 */
bool_t servoConfig( servoMap_t servoNumber, servoConfig_t config ){

   bool_t ret_val = 1;

   switch(config){

      case SERVO_ENABLE:
         servoInitTimers();
      break;

      case SERVO_DISABLE:
         ret_val = 0;
      break;

      case SERVO_ENABLE_OUTPUT:
         ret_val = servoAttach( servoNumber );
      break;

      case SERVO_DISABLE_OUTPUT:
         ret_val = servoDetach( servoNumber );
      break;

      default:
         ret_val = 0;
      break;
   }

   return ret_val;
}

/*
 * @brief:   Tells if the servo is currently active, and its position
 * @param:   servoNumber:   ID of the servo, from 0 to 8
 * @param:   value:   value of the servo, from 0 to 180
 * @return:   position (1 ~ SERVO_TOTALNUMBER), 0 if the element was not found.
 */
uint8_t servoIsAttached( servoMap_t servoNumber ){

   uint8_t position = 0, positionInList = 0;
   while ( (position < SERVO_TOTALNUMBER) &&
           (servoNumber != AttachedServoList[position].servo) ){
      position++;
   }

   if (position < SERVO_TOTALNUMBER){
      positionInList = position + 1;
   } else{
      positionInList = 0;
   }

   return positionInList;
}

/*
 * @brief: read the value of the servo
 * @param:   servoNumber:   ID of the servo, from 0 to 8
 * @return: value of the servo (0 ~ 180).
 *   If an error ocurred, return = EMPTY_POSITION = 255
 */
uint16_t servoRead( servoMap_t servoNumber){

   uint8_t position = 0, value = 0;
   position = servoIsAttached(servoNumber);

   if(position){
      value = AttachedServoList[position-1].value;
   }
   else{
      value = EMPTY_POSITION;
   }
   return value;
}

/*
 * @brief: change the value of the servo
 * @param:   servoNumber:   ID of the servo, from 0 to 8
 * @param:   value:   value of the servo, from 0 to 180
 * @return: True if the value was successfully changed, False if not.
 */
bool_t servoWrite( servoMap_t servoNumber, uint16_t angle ){

   bool_t success = FALSE;
   uint8_t position = 0;

   position = servoIsAttached(servoNumber);

   if(position && (angle>=0 && angle<=180)){
      AttachedServoList[position-1].value = angle;
      success = TRUE;
   }

   return success;
}

/*==================[end of file]============================================*/
