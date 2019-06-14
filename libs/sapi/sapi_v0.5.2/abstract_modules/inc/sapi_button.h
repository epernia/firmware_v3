/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/06/14
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __SAPI_BUTTON_H__
#define __SAPI_BUTTON_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi_datatypes.h"
#include "sapi_gpio.h"

/*=====[C++ - begin]=========================================================*/

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

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SAPI_BUTTON_H__ */
