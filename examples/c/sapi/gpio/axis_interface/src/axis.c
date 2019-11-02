/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/24
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "axis.h"

// ----------------------------------------------------------------
// PRIVATE VARIABLE DECLARATIONS
// ----------------------------------------------------------------

static const int32_t m_data_o[PORT_SIZE] = {
   M_DATA_O7,
   M_DATA_O6,
   M_DATA_O5,
   M_DATA_O4,
   M_DATA_O3,
   M_DATA_O2,
   M_DATA_O1,
   M_DATA_O0,
};

static const int32_t s_data_i[PORT_SIZE] = {
   S_DATA_I7,
   S_DATA_I6,
   S_DATA_I5,
   S_DATA_I4,
   S_DATA_I3,
   S_DATA_I2,
   S_DATA_I1,
   S_DATA_I0,
};

static bool_t clk_i = LOW;
static bool_t clk_i_old = LOW;

static bool_t clk_o = LOW;
static bool_t clk_o_old = LOW;

static delay_t m_clk_delay;

// Send ----------------------------------

static uint8_t* tx_data = NULL;
static uint32_t tx_data_len = 0;

static bool_t flagSendData = false;
static bool_t flagSendInNextClock = false;
static uint32_t dataSendIndex = 0;

// Receive -------------------------------

static uint8_t* rx_data = NULL;
static uint32_t rx_data_len = 0;

static bool_t flagReceiveData = false;
static uint32_t dataReceiveIndex = 0;

// ----------------------------------------------------------------
// PRIVATE FUNCTION DECLARATIONS
// ----------------------------------------------------------------
            
static void axisPutDataToSend( void );
static void axisOnRisingSendUpdate( void );
static void axisOnFallingSendUpdate( void );
// ----------------------------------------------------------------

static void axisReceiveData( void );
static void axisOnRisingReceiveUpdate( void );
static void axisOnFallingReceiveUpdate( void );

// ----------------------------------------------------------------

static void axisClockToggleAndSave( void );
static void axisClockOnRisingEdgeEvent( void );
static void axisClockOnFallingEdgeEvent( void );
static void axisClockSaveLasValue( void );

// ----------------------------------------------------------------

static void axisCompareData( void );

// ----------------------------------------------------------------
// PUBLIC FUNCTION IMPLEMENTATIONS
// ----------------------------------------------------------------

void axisInit( uint8_t* txBuff, uint32_t txBuffLen, 
               uint8_t* rxBuff, uint32_t rxBuffLen )
{
   // Pointers ------------------------------
   tx_data = txBuff;
   tx_data_len = txBuffLen;   
   rx_data = rxBuff;
   rx_data_len = rxBuffLen;   
   
   // MASTER --------------------------------
   gpioPortInit( m_data_o, GPIO_OUTPUT );
   gpioInit( m_valid_o, GPIO_OUTPUT );
   gpioInit( m_last_o,  GPIO_OUTPUT );
   gpioInit( m_ready_i, GPIO_INPUT_PULLUP );

   // SLAVE ---------------------------------
   gpioPortInit( s_data_i, GPIO_INPUT_PULLUP );
   gpioInit( s_valid_i, GPIO_INPUT_PULLUP );
   gpioInit( s_last_i,  GPIO_INPUT_PULLUP );
   gpioInit( s_ready_o, GPIO_OUTPUT );

   // CLOCK ---------------------------------
   gpioInit( s_clk_i, GPIO_INPUT_PULLUP );
   gpioInit( m_clk_o, GPIO_OUTPUT );
   delayInit( &m_clk_delay, T_CLK );
   
   // Rest
   axisStopSendingDataAndReset();
}

void axisUpdate( void )
{
   axisClockToggleAndSave();      
   axisClockOnRisingEdgeEvent();
   axisClockOnFallingEdgeEvent();
   axisClockSaveLasValue();
}
// ----------------------------------------------------------------

void axisStartToSendData( void )
{
   flagSendData = true;
   printf( "Start to send data...\r\n" );
}

void axisStopSendingDataAndReset( void )
{
   flagSendData = false;
   flagSendInNextClock = false;
   dataSendIndex = 0;
   gpioWrite( m_last_o, LOW );
   gpioWrite( m_valid_o, LOW );
   gpioPortWrite( m_data_o, 0 );   
}

void axisPauseSendingData( void )
{
   gpioWrite( m_valid_o, LOW );
}

void axisResumeSendingData( void )
{
   gpioWrite( m_valid_o, HIGH );
}

// ----------------------------------------------------------------

void axisStartToReceiveData( void )
{
   flagReceiveData = true;
   gpioWrite( s_ready_o, HIGH ); 
   gpioWrite( LEDR, OFF ); 
   gpioWrite( LEDG, OFF ); 
   printf( "Receiving data...\r\n" );
}

void axisStopReceiveDataAndReset( void )
{
   flagReceiveData = false;
   uint32_t i = 0;
   /*
   for( i=0; i<dataReceiveIndex; i++ ){      
      printf( "%s\r\n", uintToAsciiHexGlobal( rx_data[i], 8 ) );
   }*/
   dataReceiveIndex = 0;
   gpioWrite( s_ready_o, LOW ); 
   axisCompareData();
}

// ----------------------------------------------------------------
// PRIVATE FUNCTION IMPLEMENTATIONS
// ----------------------------------------------------------------

static void axisClockToggleAndSave( void )
{
   if( delayRead(&m_clk_delay) ) {
      gpioToggle(m_clk_o);
   }
   clk_o = gpioRead(m_clk_o);
   clk_i = gpioRead(s_clk_i);
}

static void axisClockOnRisingEdgeEvent( void )
{
   // Clock output rising edge ?
   if( clk_o_old == LOW && clk_o == HIGH ) {     
      axisOnRisingSendUpdate();
   }
   // Clock input rising edge ?
   if( clk_i_old == LOW && clk_i == HIGH ) {     
      axisOnRisingReceiveUpdate();
   }
}

static void axisClockOnFallingEdgeEvent( void )
{
   // Clock output falling edge ?
   if( clk_o_old == HIGH && clk_o == LOW ) {  
      axisOnFallingSendUpdate();
   }
   // Clock input falling edge ?
   if( clk_i_old == HIGH && clk_i == LOW ) {  
      axisOnFallingReceiveUpdate();
   }
}

static void axisClockSaveLasValue( void )
{
   clk_o_old = clk_o;
   clk_i_old = clk_i;
}

// ----------------------------------------------------------------

static void axisOnRisingSendUpdate( void )
{
   if( flagSendData ) {        
      if( gpioRead(m_ready_i) ) {
         flagSendInNextClock = true;
      } else {
         flagSendInNextClock = false;
      }
   }   
}

static void axisOnFallingSendUpdate( void )
{
   if( flagSendInNextClock ) {
      if( dataSendIndex >= tx_data_len ) {
         axisStopSendingDataAndReset();
      } else {
         if( dataSendIndex == tx_data_len - 1 ) {
            gpioWrite( m_last_o, HIGH );
         }
         axisPutDataToSend();
      }
   }
}

// ----------------------------------------------------------------

static void axisPutDataToSend( void )
{
   gpioPortWrite( m_data_o, tx_data[dataSendIndex] );
   gpioWrite( m_valid_o, HIGH );
   dataSendIndex++; 
}

// ----------------------------------------------------------------

static void axisOnRisingReceiveUpdate( void )
{
   if( flagReceiveData ) {        
      if( gpioRead( s_valid_i ) ) {
         if( dataReceiveIndex >= tx_data_len  ){
            dataReceiveIndex = tx_data_len;
            axisStopReceiveDataAndReset();
         }
         axisReceiveData();
         if( gpioRead( s_last_i ) ){
            axisStopReceiveDataAndReset();
         }
      }   
   }   
}

static void axisOnFallingReceiveUpdate( void )
{
}

// ----------------------------------------------------------------

static void axisReceiveData( void )
{
   gpioWrite( s_ready_o, HIGH );
   rx_data[dataReceiveIndex] = gpioPortRead( s_data_i );
   printf( "%s\r\n", uintToAsciiHexGlobal( rx_data[dataReceiveIndex], 8 ) );
   dataReceiveIndex++;
}

// ----------------------------------------------------------------

static void axisCompareData( void )
{
   bool_t equals = true;
   uint32_t i = 0;
   if( tx_data_len != rx_data_len ){}
   for( i=0; i<rx_data_len; i++ ){
      equals = equals && ( tx_data[i] == rx_data[i] );      
      if( tx_data[i] != rx_data[i] ) {
         printf( "Error at i=%d, received: %s \r\n",
                 i, uintToAsciiHexGlobal( rx_data[i], 8 ) );
         gpioWrite( LEDR, ON );     
      }
   }
   if( equals ) {
      printf( "Data received OK.\r\n\r\n" ); 
      gpioWrite( LEDG, ON );      
   } else{
      printf( "\r\n" ); 
   }
   delay(1000);
   gpioWrite( LEDR, OFF ); 
   gpioWrite( LEDG, OFF );    
}
