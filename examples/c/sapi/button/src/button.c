/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE file)
 * Date: 2019/06/14
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

#define BUTTON_LOGIC BUTTON_ONE_IS_UP

#if BOARD==ciaa_nxp
   #define BUTTON0 DI0
   #define BUTTON1 DI1
   #define BUTTON2 DI2
   #define BUTTON3 DI3
#elif BOARD==edu_ciaa_nxp
   #define BUTTON0 TEC1
   #define BUTTON1 TEC2
   #define BUTTON2 TEC3
   #define BUTTON3 TEC4
#else
   #error You must select a valid board!
#endif

/*=====[Definitions of public functions]=====================================*/

// Callbacks for Button 0 events

void myButton1PressedCallback(void* param)
{
   printf("Boton 0 presionado\r\n");
}
void myButton1ReleasedCallback(void* param)
{
   printf("Boton 0 liberado\r\n");
}
void myButton1HoldPressedCallback(void* param)
{
   printf("Boton 0 presionado durante mas de 3 segundos\r\n");
}

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------

   boardInit();

   // Temporization
   delay_t refreshButton;
   delay_t refreshButtonEvents;
   delayInit( &refreshButton, 50 );
   delayInit( &refreshButtonEvents, 10 );

   // Button objects
   button_t myButton0;
   button_t myButton1;
   button_t myButton2;
   button_t myButton3;

   // Button 0 is handled with callbacks
   buttonInit( &myButton0,                  // Button structure (object)
               BUTTON0, BUTTON_LOGIC,       // Pin and electrical connection
               50,                          // Button scan time [ms]
               TRUE,                        // checkPressedEvent
               TRUE,                        // checkReleasedEvent
               TRUE,                        // checkHoldPressedEvent
               3000,                        // holdPressedTime [ms]
               myButton1PressedCallback,    // pressedCallback
               myButton1ReleasedCallback,   // releasedCallback
               myButton1HoldPressedCallback // holdPressedCallback
             );

   // Rest of buttons handled by pooling
   
   // Buttton 1 not check released event
   buttonInit( &myButton1,                  // Button structure (object)
               BUTTON1, BUTTON_LOGIC,       // Pin and electrical connection
               50,                          // Button scan time [ms]
               TRUE,                        // checkPressedEvent
               FALSE,                       // checkReleasedEvent
               TRUE,                        // checkHoldPressedEvent
               5000,                        // holdPressedTime [ms]
               0,                           // pressedCallback
               0,                           // releasedCallback
               0                            // holdPressedCallback
             );

   buttonInit( &myButton2,                  // Button structure (object)
               BUTTON2, BUTTON_LOGIC,       // Pin and electrical connection
               50,                          // Button scan time [ms]
               TRUE,                        // checkPressedEvent
               TRUE,                        // checkReleasedEvent
               TRUE,                        // checkHoldPressedEvent
               2000,                        // holdPressedTime [ms]
               0,                           // pressedCallback
               0,                           // releasedCallback
               0                            // holdPressedCallback
             );

   buttonInit( &myButton3,                  // Button structure (object)
               BUTTON3, BUTTON_LOGIC,       // Pin and electrical connection
               50,                          // Button scan time [ms]
               TRUE,                        // checkPressedEvent
               TRUE,                        // checkReleasedEvent
               TRUE,                        // checkHoldPressedEvent
               2000,                        // holdPressedTime [ms]
               0,                           // pressedCallback
               0,                           // releasedCallback
               0                            // holdPressedCallback
             );

   // ----- Repeat for ever -------------------------
   while(true) {

      // Refrehs button FSMs every 50 ms
      if( delayRead(&refreshButton) ) {
         buttonFsmUpdate( &myButton0 );
         buttonFsmUpdate( &myButton1 );
         buttonFsmUpdate( &myButton2 );
         buttonFsmUpdate( &myButton3 );
      }

      // Poll buttons events every 10 ms
      if( delayRead(&refreshButtonEvents) ) {
         // button 1 will print 5 times beacause difference of temporization not mark evend as handled
         if( (buttonEventGet( &myButton1 ) == BUTTON_PRESSED) ) {
            printf("Se presiono boton 1\r\n");
         }
         // Never execute because not check released event
         if( (buttonEventGet( &myButton1 ) == BUTTON_RELEASED) ) {
            printf("Se libero boton 1\r\n");
         }
         // Rest of buttons will print only 1 time because mark events as handled
         if( (buttonEventGet( &myButton2 ) == BUTTON_PRESSED) ) {
            printf("Se presiono boton 2\r\n");
            buttonEventHandled( &myButton2 );
         }
         if( (buttonEventGet( &myButton2 ) == BUTTON_RELEASED) ) {
            printf("Se libero boton 2\r\n");
            buttonEventHandled( &myButton2 );
         }
         if( (buttonEventGet( &myButton3 ) == BUTTON_PRESSED) ) {
            printf("Se presiono boton 3\r\n");
            buttonEventHandled( &myButton3 );
         }
         if( (buttonEventGet( &myButton3 ) == BUTTON_RELEASED) ) {
            printf("Se libero boton 3\r\n");
            buttonEventHandled( &myButton3 );
         }
         // Button 2 and 3 in combination (hold together)
         if( (buttonEventGet( &myButton2 ) == BUTTON_HOLD_PRESED) &&
             (buttonEventGet( &myButton3 ) == BUTTON_HOLD_PRESED) ) {
            printf("Se presionaron ambos botones juntos (2 y 3) durante mas de 2 segundos\r\n");
            buttonEventHandled( &myButton2 ); // Avisar que el ultimo evento fue manejado
            buttonEventHandled( &myButton3 );
         }
      }

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
