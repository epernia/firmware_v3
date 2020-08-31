/* Copyright 2019, Eric Pernia.
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

/* Date: 2019/06/14 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _SAPI_BUTTON_H_
#define _SAPI_BUTTON_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi_datatypes.h"
#include "sapi_gpio.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definitions of public data types]====================================*/
   
// Button events names
typedef enum{
   BUTTON_NO_EVENT,            
   BUTTON_PRESSED,       // BUTTON_UP --> BUTTON_FALLING --> BUTTON_DOWN (falling edge)
   BUTTON_RELEASED,      // BUTTON_DOWN --> BUTTON_RISING --> BUTTON_UP (rising edge)
   BUTTON_HOLD_PRESED,   // BUTTON_PRESSED and elapsed certain time
   BUTTON_EVENT_HANDLED, // A previous event was handled
} buttonEvent_t;

// Button events names
typedef enum{
   BUTTON_ZERO_IS_UP,
   BUTTON_ONE_IS_UP,
} buttonLogic_t;

// FSM state names
typedef enum{
   BUTTON_UP,
   BUTTON_DOWN,
   BUTTON_FALLING,
   BUTTON_RISING
} buttonFsmState_t;

// Button object structure
typedef struct{

   // Pin and electrical connection

   int32_t gpio;
   bool_t logic;

   // Button scan time

   tick_t refreshTime; // Time [ms] that refresh buttons (update fsm)
                       //Must be more than bouncing time! i.e. 50ms

   // Button FSM

   buttonFsmState_t state;
   bool_t flagUp;
   bool_t flagDown;
   bool_t flagFalling;
   bool_t flagRising;
   tick_t timeInSate; // In [ms]

   // Button event

   tick_t event;

   bool_t checkPressedEvent;
   bool_t checkReleasedEvent;
   bool_t checkHoldPressedEvent;

   tick_t holdPressedTime; // In [ms]

   callBackFuncPtr_t pressedCallback;
   callBackFuncPtr_t releasedCallback;
   callBackFuncPtr_t holdPressedCallback;
} button_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

// Button initialization
void buttonInit( button_t* button,           // Button structure (object)                
                 int32_t gpio, bool_t logic, // Pin and electrical connection               
                 tick_t refreshTime,         // Button scan time
                 // Button event
                 bool_t checkPressedEvent,
                 bool_t checkReleasedEvent,
                 bool_t checkHoldPressedEvent,
                 tick_t holdPressedTime,
                 callBackFuncPtr_t pressedCallback,
                 callBackFuncPtr_t releasedCallback,
                 callBackFuncPtr_t holdPressedCallback );

// EVENT FUNCTIOS --------------------------------------------

// Get Button last event
buttonFsmState_t buttonEventGet( button_t* button );

// Event was handled
void buttonEventHandled( button_t* button );

// FSM FUNCTIOS ----------------------------------------------

// Get button FSM state
buttonFsmState_t buttonStateGet( button_t* button );

// Get button FSM time in that state
tick_t buttonTimeInStateGet( button_t* button );

// Init button FSM
void buttonFsmInit( button_t* button );

// Handle FSM errors
void buttonFsmError( button_t* button );

// FSM Update Sate Function
void buttonFsmUpdate( button_t* button );

// LOW LEVEL FUNCTIONS --------------------------------------

// Return true if button is up
bool_t buttonIsUp( button_t* button );

// Return true if button is down
bool_t buttonIsDown( button_t* button );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_BUTTON_H_ */
