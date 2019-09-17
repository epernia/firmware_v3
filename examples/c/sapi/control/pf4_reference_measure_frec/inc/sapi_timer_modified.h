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

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef SAPI_TIMER_MODIFIED_H_
#define SAPI_TIMER_MODIFIED_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

typedef void (*voidFunctionPointer_t)(void);

typedef enum{
   TIMER_TICKER,
   TIMER_OVERFLOW,
   TIMER_MATCH,
   TIMER_MATCH_OUTPUT,
   TIMER_PWM,
   TIMER_INPUT_CAPTURE
} timerMode_t;

typedef enum{
   TIMER_CAPTURE_IN_RISING_EDGE,
   TIMER_CAPTURE_IN_FALLING_EDGE,
   TIMER_CAPTURE_IN_BOTH_EDGES
} timerInputCaptureMode_t;

typedef enum{
   TIMER_SET_OUTPUT_ON_MATCH,      // (Set high on match)
   TIMER_CLEAR_OUTPUT_ON_MATCH,    // (Set low on match)
   TIMER_TOGGLE_OUTPUT_ON_MATCH    // (Toggle on match)
} timerMatchOutputMode_t;

typedef enum{
   TIMER_MATCH0,
   TIMER_MATCH1,
   TIMER_MATCH2,
   TIMER_MATCH3
} timerMatch_t;

typedef enum{
   TIMER_CAPTURE0,
   TIMER_CAPTURE1,
   TIMER_CAPTURE2,
   TIMER_CAPTURE3
} timerCapture_t;

typedef enum{
   CLOCK_SOURCE_INTERNAL_MCU_CLK,
   CLOCK_SOURCE_EXTERNAL_CLK
} clockSource_t;

typedef enum{
   PRESCALER_1    = 1,
   PRESCALER_8    = 8,
   PRESCALER_16   = 16,
   PRESCALER_32   = 32,
   PRESCALER_64   = 64,
   PRESCALER_128  = 128,
   PRESCALER_256  = 256,
   PRESCALER_512  = 515,
   PRESCALER_1024 = 1024
} prescaler_t;

uint32_t timerReadCounter( timerMap_t timer );
uint32_t timerReadCapture( timerMap_t timer, uint8_t captureNumber );
uint32_t timerGetClock( timerMap_t timer );
uint32_t timerGetPrescale( timerMap_t timer );

// TIMER_TICKER

bool_t timerTickerConfig( timerMap_t timer, uint32_t tickRateHz );

void timerTickerSetTickEvent( timerMap_t timer, sAPI_FuncPtr_t eventHook );


// TIMER_INPUT_CAPTURE

bool_t timerInputCaptureConfig( timerMap_t timer,
                                timerCapture_t capture,
                                uint32_t prescale,
                                bool_t captureRising,
                                bool_t captureFalling );

void timerInputCaptureSetCaptureEvent( timerMap_t timer,
                                       timerCapture_t capture,
                                       sAPI_FuncPtr_t eventHook );

// TIMER POWER
void timerSetPower( timerMap_t timer, bool_t power );

// TIMER INTERRUPT
void timerInterruptEnable( timerMap_t timer, bool_t enabeling );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/**
 * @Brief:   Executes the functions passed by parameter in the Timer_init,
 *   at the chosen frequencies
 */
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void TIMER3_IRQHandler(void);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* SAPI_TIMER_MODIFIED_H_ */
