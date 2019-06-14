/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/06/14
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi_button.h"

/*=====[Private functions declarations]======================================*/

static buttonFsmState_t buttonInitState( button_t* button );

/*=====[Public functions definitions]========================================*/

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
                 callBackFuncPtr_t holdPressedCallback )
{
   // Pin and electrical connection
   button->gpio = gpio;
   button->logic = logic;

   // Button scan time
   button->refreshTime = refreshTime;

   // Button FSM
   buttonFsmInit( button );

   // Button events
   button->event = BUTTON_NO_EVENT;

   button->checkPressedEvent = checkPressedEvent;
   button->checkReleasedEvent = checkReleasedEvent;
   button->checkHoldPressedEvent = checkHoldPressedEvent;

   button->holdPressedTime = holdPressedTime;

   button->pressedCallback = pressedCallback;
   button->releasedCallback = releasedCallback;
   button->holdPressedCallback = holdPressedCallback;
}

// EVENT FUNCTIOS --------------------------------------------

// Get Button last event
buttonFsmState_t buttonEventGet( button_t* button )
{
   return button->event;
}

// Event was handled
void buttonEventHandled( button_t* button )
{
   button->event = BUTTON_EVENT_HANDLED;
}

// FSM FUNCTIOS ----------------------------------------------

// Get button FSM state
buttonFsmState_t buttonStateGet( button_t* button )
{
   return button->state;
}

// Get button FSM time in that state
tick_t buttonTimeInStateGet( button_t* button )
{
   return button->timeInSate;
}

// Init button FSM
void buttonFsmInit( button_t* button )
{
   button->state = buttonInitState( button );
   button->timeInSate = 0;
   button->flagUp = FALSE;
   button->flagDown = FALSE;
   button->flagFalling = FALSE;
   button->flagRising = FALSE;
}

// Handle FSM errors
void buttonFsmError( button_t* button )
{
   // TODO
}

// FSM Update Sate Function
void buttonFsmUpdate( button_t* button )
{   
   button->timeInSate += button->refreshTime;
   switch( button->state ){

      case BUTTON_UP: 
         // ENTRY
         if( button->flagUp == FALSE ){
            button->flagUp = TRUE;
            button->event = BUTTON_NO_EVENT;
         }
         // CHECK TRANSITION CONDITIONS
         if( buttonIsDown(button) ){
            button->state = BUTTON_FALLING;
            button->timeInSate = 0;
         }
         // EXIT
         if( button->state != BUTTON_UP ){
            button->flagUp = FALSE;
            button->timeInSate = 0;
            button->event = BUTTON_NO_EVENT;
         }
      break;

      case BUTTON_DOWN:
         // ENTRY
         if( button->flagDown == FALSE ){
            button->flagDown = TRUE;
            button->event = BUTTON_NO_EVENT;
         }
         // CHECK TRANSITION CONDITIONS
         if( buttonIsUp(button) ){
            button->state = BUTTON_RISING;
         }
         // Check and execute if correspond hold pressed event
         if( (button->checkHoldPressedEvent) && 
             (button->timeInSate >= button->holdPressedTime) ){
            if( button->event == BUTTON_NO_EVENT ){   
               button->event = BUTTON_HOLD_PRESED;           
               // Execute callback function if check event and pointer is not NULL
               if( (button->holdPressedCallback != NULL) ){
                  (*(button->holdPressedCallback))( 0 );
               }
            }
         }
         // EXIT
         if( button->state != BUTTON_DOWN ){
            button->flagDown = FALSE;
            button->timeInSate = 0;
            button->event = BUTTON_NO_EVENT;
         }
      break;

      case BUTTON_FALLING:
         // ENTRY
         if( button->flagFalling == FALSE ){
            button->flagFalling = TRUE;
            //gpioWrite(DO6, ON);
         }      
         // CHECK TRANSITION CONDITIONS
         if( buttonIsDown(button) ){
            button->state = BUTTON_DOWN;
            // Check and execute if correspond pressed event
            if( (button->checkPressedEvent) ){
               button->event = BUTTON_PRESSED;
               // Execute callback function if check event and pointer is not NULL
               if( (button->pressedCallback != NULL) ){
                  (*(button->pressedCallback))( 0 );
               }
            }
         } else{
            button->state = BUTTON_UP;
         }         
         // EXIT
         if( button->state != BUTTON_FALLING ){
            button->flagFalling = FALSE;
            button->timeInSate = 0;
            //gpioWrite(DO6, OFF);
         }
      break;

      case BUTTON_RISING:
         // ENTRY
         if( button->flagRising == FALSE ){
            button->flagRising = TRUE;
            //gpioWrite(DO7, ON);
         }    
         // CHECK TRANSITION CONDITIONS
         if( buttonIsUp(button) ){
            button->state = BUTTON_UP;
            // Check and execute if correspond released event
            if( (button->checkReleasedEvent) ){
               button->event = BUTTON_RELEASED;
               // Execute callback function if check event and pointer is not NULL
               if( (button->releasedCallback != NULL) ){
                  (*(button->releasedCallback))( 0 );
               }
            }
         } else{
            button->state = BUTTON_DOWN;
         }

         // EXIT
         if( button->state != BUTTON_RISING ){
            button->flagRising = FALSE;
            button->timeInSate = 0;
            //gpioWrite(DO7, OFF);
         }
      break;

      default:
         buttonFsmError(button);
      break;
   }
}

// LOW LEVEL FUNCTIONS --------------------------------------

// Return true if button is up
bool_t buttonIsUp( button_t* button )
{
   bool_t gpioStatus = gpioRead( button->gpio );
   if( button->logic == BUTTON_ONE_IS_UP ){
      return gpioStatus;
   } else{
      return !gpioStatus;
   }
}

// Return true if button is down
bool_t buttonIsDown( button_t* button )
{
   return !buttonIsUp( button );
}

/*=====[Private functions definitions]=======================================*/

static buttonFsmState_t buttonInitState( button_t* button )
{
   if( buttonIsDown(button) ){
      return BUTTON_DOWN;
   }else{
      return BUTTON_UP;
   }
}