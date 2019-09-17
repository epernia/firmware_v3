/*=====[sapi_timer_modified]===================================================
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 2.0.0
 * Creation Date: Date: 2017-02-10

/*
 * For more information about the Timer peripheral, refer to the Chapter 32 of
 * the LPC43xx user manual
 */

/*=====[Inclusions of private function dependencies]=========================*/

#include "sapi_timer_modified.h"

/*=====[Definition macros of private constants]==============================*/

#define LPC4337_MAX_FREC 204000000UL /* Microcontroller frequency */
#define MAX_SYSCALL_INTERRUPT_PRIORITY 5

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// ----- NEW TIMER - begin ----------------------------------------------------

volatile sAPI_FuncPtr_t timer0CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer0MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer1CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer1MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer2CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer2MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };

volatile sAPI_FuncPtr_t timer3CaptureHookFunction[4] = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
volatile sAPI_FuncPtr_t timer3MatchHookFunction[4]   = { sAPI_NullFuncPtr, sAPI_NullFuncPtr,
                                                         sAPI_NullFuncPtr, sAPI_NullFuncPtr };
                                                         
uint32_t timerGetClock( timerMap_t timer ){
   return ( LPC4337_MAX_FREC / timerGetPrescale( timer ) );
}

                                                         
uint32_t timerGetPrescale( timerMap_t timer ){
   switch( timer ){
      case TIMER0:
         return LPC_TIMER0->PR;
      break;      
      case TIMER1:
         return LPC_TIMER1->PR;
      break;      
      case TIMER2:      
         return LPC_TIMER2->PR;
      break;      
      case TIMER3:
         return LPC_TIMER3->PR;
      break;
      default:
         return 0;
   }
}


uint32_t timerReadCounter(timerMap_t timer ){
   switch( timer ){
      case TIMER0:
         return Chip_TIMER_ReadCount( LPC_TIMER0 );
      break;      
      case TIMER1:
         return Chip_TIMER_ReadCount( LPC_TIMER1 );
      break;      
      case TIMER2:      
         return Chip_TIMER_ReadCount( LPC_TIMER2 );   
      break;      
      case TIMER3:
         return Chip_TIMER_ReadCount( LPC_TIMER3 );
      break;
      default:
         return 0;
   }
}

uint32_t timerReadCapture(timerMap_t timer, uint8_t captureNumber ){
   switch( timer ){
      case TIMER0:
         return Chip_TIMER_ReadCapture( LPC_TIMER0, captureNumber);
      break;      
      case TIMER1:
         return Chip_TIMER_ReadCapture( LPC_TIMER1, captureNumber);
      break;      
      case TIMER2:      
         return Chip_TIMER_ReadCapture( LPC_TIMER2, captureNumber);   
      break;      
      case TIMER3:
         return Chip_TIMER_ReadCapture( LPC_TIMER3, captureNumber);
      break;
      default:
         return 0;
   }
}

// TIMER_TICKER

bool_t timerTickerConfig( timerMap_t timer, uint32_t tickRateHz ){
   bool_t retVal = TRUE;
   uint32_t timerFreq = 0;
   switch( timer ){
      case TIMER0:
      break;      
      case TIMER1:
         // Configurar TIMER1 modo comparacion para generar una interrupcion periodica a TICKRATE_HZ
         // Enable timer 1 clock and reset it
         Chip_TIMER_Init( LPC_TIMER1 );
         Chip_RGU_TriggerReset( RGU_TIMER1_RST );
         while( Chip_RGU_InReset( RGU_TIMER1_RST ) );
         // Get timer 1 peripheral clock rate
         timerFreq = Chip_Clock_GetRate( CLK_MX_TIMER1 );
         // Timer setup for match and interrupt at TICKRATE_HZ
         Chip_TIMER_Reset( LPC_TIMER1 );
         Chip_TIMER_MatchEnableInt( LPC_TIMER1, 1 );
         Chip_TIMER_SetMatch( LPC_TIMER1, 1, (timerFreq/tickRateHz) );
         
         // TODO: separar esta config
         Chip_TIMER_ResetOnMatchEnable( LPC_TIMER1, 1 );
      break;      
      case TIMER2:         
      break;      
      case TIMER3:         
      break;      
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

void timerTickerSetTickEvent( timerMap_t timer, sAPI_FuncPtr_t eventHook ){
   switch( timer ){
      case TIMER0:
         if( eventHook ){
            timer0MatchHookFunction[0] = eventHook;
            // Enable TIMER0 interrupt
            timerInterruptEnable( TIMER0, ON );
         }
      break;
      case TIMER1:
         if( eventHook ){
            timer1MatchHookFunction[0] = eventHook;
            // Enable TIMER1 interrupt
            timerInterruptEnable( TIMER1, ON );
         } 
      break;
      case TIMER2:
         if( eventHook ){
            timer2MatchHookFunction[0] = eventHook;
            // Enable TIMER2 interrupt
            timerInterruptEnable( TIMER2, ON );
         } 
      break;
      case TIMER3:
         if( eventHook ){
            timer3MatchHookFunction[0] = eventHook;
            // Enable TIMER3 interrupt
            timerInterruptEnable( TIMER3, ON );
         } 
      break;
   }
}

// TIMER_OVERFLOW

bool_t timerConfigOverflow( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

// TIMER_MATCH

bool_t timerConfigMatch( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

// TIMER_MATCH_OUTPUT

bool_t timerConfigMatchOutput( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

// TIMER_PWM

bool_t timerConfigPwm( timerMap_t timer ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:         
      break;      
      case TIMER1:         
      break;      
      case TIMER2:         
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}

// TIMER_INPUT_CAPTURE

bool_t timerInputCaptureConfig( timerMap_t timer,
                                timerCapture_t capture,
                                uint32_t prescale,
                                bool_t captureRising,
                                bool_t captureFalling ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:
         // Configurar TIMER0 en modo entrada de captura 1 
         // Configurar pin P1.20 como T0_CAP2
         // ENET_TXD1 --> P1.20, FUNC4 --> T0_CAP2
         Chip_SCU_PinMux( 0x1,20, MD_PLN_FAST, SCU_MODE_FUNC4 ); // P1.20 whith SCU_MODE_FUNC4 is T0_CAP2
                      // T0_CAP2
         LPC_GIMA->CAP0_IN[0][capture]|= 0x20;
         // Enable timer 0 clock and reset it
         Chip_TIMER_Init( LPC_TIMER0 );
         Chip_TIMER_Reset( LPC_TIMER0 );
         Chip_TIMER_TIMER_SetCountClockSrc( LPC_TIMER0, TIMER_CAPSRC_RISING_PCLK, capture );
         Chip_TIMER_PrescaleSet( LPC_TIMER0, prescale );
         Chip_TIMER_ClearCapture( LPC_TIMER0, capture );      
         if(captureRising) {
            Chip_TIMER_CaptureRisingEdgeEnable( LPC_TIMER0, capture );
         } else {
            Chip_TIMER_CaptureRisingEdgeDisable( LPC_TIMER0, capture );
         }
         if(captureFalling) {
            Chip_TIMER_CaptureFallingEdgeEnable( LPC_TIMER0, capture );
         } else {
            Chip_TIMER_CaptureFallingEdgeDisable( LPC_TIMER0, capture );
         }
      break;
      case TIMER1:
      break;
      case TIMER2:
      break;
      case TIMER3:
      break;
      default:
         retVal = FALSE;
      break;
   }
   return retVal;
}
                                          
void timerInputCaptureSetCaptureEvent( timerMap_t timer,
                                       timerCapture_t capture,
                                       sAPI_FuncPtr_t eventHook ){
   bool_t retVal = TRUE;
   switch( timer ){
      case TIMER0:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER0 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER0, capture );
            // Enable TIMER0 interrupt
            timerInterruptEnable( TIMER0, ON );
         }
      break;
      case TIMER1:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER1 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER1, capture );
            // Enable TIMER1 interrupt
            timerInterruptEnable( TIMER1, ON );
         } 
      break;
      case TIMER2:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER2 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER2, capture );
            // Enable TIMER2 interrupt
            timerInterruptEnable( TIMER2, ON );
         } 
      break;
      case TIMER3:
         if( eventHook ){
            timer0CaptureHookFunction[capture] = eventHook;
            // Enable TIMER3 Capture interrupt
            Chip_TIMER_CaptureEnableInt( LPC_TIMER3, capture );
            // Enable TIMER3 interrupt
            timerInterruptEnable( TIMER3, ON );
         } 
      break;
      default:
         retVal = FALSE;
      break;
   }
}

void timerSetPower( timerMap_t timer, bool_t power ){
   switch( timer ){
      case TIMER0:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER0 );
         else
            Chip_TIMER_Disable( LPC_TIMER0 );
      break;
      case TIMER1:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER1 );
         else
            Chip_TIMER_Disable( LPC_TIMER1 );
      break;
      case TIMER2:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER2 );
         else
            Chip_TIMER_Disable( LPC_TIMER2 );
      break;
      case TIMER3:
         if( power )
            Chip_TIMER_Enable( LPC_TIMER3 );
         else
            Chip_TIMER_Disable( LPC_TIMER3 );
      break;
   }
}

void timerInterruptEnable( timerMap_t timer, bool_t enabeling ){
   switch( timer ){
      case TIMER0:
         if( enabeling ){
            // Enable TIMER0 interrupt
            NVIC_EnableIRQ( TIMER0_IRQn );
            NVIC_ClearPendingIRQ( TIMER0_IRQn );
         }else{
            // Disable TIMER0 interrupt
            NVIC_DisableIRQ( TIMER0_IRQn );
         }
      break;
      case TIMER1:
         if( enabeling ){
            // Enable TIMER1 interrupt
            NVIC_EnableIRQ( TIMER1_IRQn );
            NVIC_ClearPendingIRQ( TIMER1_IRQn );
         }else{
            // Disable TIMER1 interrupt
            NVIC_DisableIRQ( TIMER1_IRQn );
         }
      break;
      case TIMER2:
         if( enabeling ){
            // Enable TIMER2 interrupt
            NVIC_EnableIRQ( TIMER2_IRQn );
            NVIC_ClearPendingIRQ( TIMER2_IRQn );
         }else{
            // Disable TIMER2 interrupt
            NVIC_DisableIRQ( TIMER2_IRQn );
         }
      break;
      case TIMER3:
         if( enabeling ){
            // Enable TIMER3 interrupt
            NVIC_EnableIRQ( TIMER3_IRQn );
            NVIC_ClearPendingIRQ( TIMER3_IRQn );
         }else{
            // Disable TIMER3 interrupt
            NVIC_DisableIRQ( TIMER3_IRQn );
         }
      break;
   }
}

/*=====[Implementations of interrupt functions]==============================*/

//fixme __attribute__ ((section(".after_vectors")))

void TIMER0_IRQHandler(void){
   
   uint8_t i = 0;

//   // Input Capture
//   for( i=0; i<4; i++){
//      if( Chip_TIMER_CapturePending( LPC_TIMER0, i ) ){
//         // Execute Timer1 Match Hook function
//         (* timer0CaptureHookFunction[i] )( 0 );
//         // Clear flag
//         Chip_TIMER_ClearCapture( LPC_TIMER0, i );
//      }
//   }

   // Input Capture
   if( Chip_TIMER_CapturePending( LPC_TIMER0, 2 ) ){  
      // Execute Timer1 Match Hook function
      (* timer0CaptureHookFunction[2] )( 0 );
      // Clear flag
      Chip_TIMER_ClearCapture( LPC_TIMER0, 2 );   
   }  

   // Match
   for( i=0; i<4; i++){
      if( Chip_TIMER_MatchPending(LPC_TIMER0, i) ){
         //Run the functions saved in the timer dynamic data structure
//         (*timer_dd[TIMER0].timerCompareMatchFunctionPointer[i])();
         // Clear flag
         Chip_TIMER_ClearMatch(LPC_TIMER0, i);
      }
   }
}

void TIMER1_IRQHandler( void ){
   
   uint8_t i = 0;
   
   // Match   
   for( i=0; i<4; i++){
      if( Chip_TIMER_MatchPending( LPC_TIMER1, i ) ){
         // Execute Timer1 Match Hook function
         (* timer1MatchHookFunction[0] )( 0 );
         // Run the functions saved in the timer dynamic data structure
         // (*timer_dd[TIMER1].timerCompareMatchFunctionPointer[i])();
         Chip_TIMER_ClearMatch( LPC_TIMER1, i );
      } 
   }
   
}

/*=====[Implementations of private functions]================================*/
