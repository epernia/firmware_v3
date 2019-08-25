/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/24
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __AXIS_H__
#define __AXIS_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "sapi.h"
#include "gpio_port.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

// CLOCK ---------------------------------

#define m_clk_o     T_FIL1 // T_FIL1 LEDB
#define s_clk_i     T_COL2 // T_COL2
#define T_CLK       10     // ms 5 10 1000
   
// MASTER --------------------------------

// m_data_o
#define M_DATA_O7   T_COL0
#define M_DATA_O6   T_FIL2
#define M_DATA_O5   T_FIL3
#define M_DATA_O4   T_FIL0
#define M_DATA_O3   T_COL1
#define M_DATA_O2   CAN_TD
#define M_DATA_O1   CAN_RD
#define M_DATA_O0   RS232_TXD

// m_valid_o
#define m_valid_o   LCD4 // LCD4 LED1

// m_last_o
#define m_last_o    LCDRS // LCDRS LED2

// m_ready_i
#define m_ready_i   GPIO0

// SLAVE ---------------------------------

// s_data_i
#define S_DATA_I7   GPIO8
#define S_DATA_I6   GPIO7
#define S_DATA_I5   GPIO5
#define S_DATA_I4   GPIO3
#define S_DATA_I3   GPIO1
#define S_DATA_I2   LCD1
#define S_DATA_I1   LCD2
#define S_DATA_I0   LCD3

// s_valid_i
#define s_valid_i   GPIO2

// s_last_i
#define s_last_i    GPIO4

// s_ready_o
#define s_ready_o   GPIO6

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

void axisInit( uint8_t* txBuff, uint32_t txBuffLen, 
               uint8_t* rxBuff, uint32_t rxBuffLen );

void axisUpdate( void );

// ----------------------------------------------------------------

void axisStartToSendData( void );
void axisStopSendingDataAndReset( void );

void axisPauseSendingData( void );
void axisResumeSendingData( void );

// ----------------------------------------------------------------

void axisStartToReceiveData( void );
void axisStopReceiveDataAndReset( void );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __AXIS_H__ */
