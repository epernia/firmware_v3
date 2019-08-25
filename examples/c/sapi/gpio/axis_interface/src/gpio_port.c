/*=============================================================================
 * Copyright (c) 2019, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/08/24
 * Version: 1.0.0
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#include "gpio_port.h"

void gpioPortInit( const int32_t* port, int32_t direction )
{
   uint8_t i = 0;
   for( i=0; i<PORT_SIZE; i++ ) {
      gpioInit( port[i], direction );
   }
}

void gpioPortWrite( const int32_t* port, uint64_t value )
{
   uint8_t i = 0;
   for( i=0; i<PORT_SIZE; i++ ) {
      gpioWrite( port[i], value & (1<<i) );
   }
}

uint64_t gpioPortRead( const int32_t* port )
{
   uint8_t i = 0;
   uint64_t value = 0;
   for( i=0; i<PORT_SIZE; i++ ) {
      if( gpioRead( port[i] ) )
         value |= (1<<i);
   }
   return value;
}

void gpioPortToggle( const int32_t* port )
{
   uint64_t value = gpioPortRead( port );
   gpioPortWrite( port, ~value );
}

//gpioPort8BitsWrite( m_data_o, 0x01 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x02 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x04 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x08 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x10 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x20 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x40 );
//delay(100);
//gpioPort8BitsWrite( m_data_o, 0x80 );
//delay(100);

//gpioWrite( M_DATA_O7, !gpioRead(S_DATA_I7) );
//gpioWrite( M_DATA_O6, !gpioRead(S_DATA_I6) );
//gpioWrite( M_DATA_O5, !gpioRead(S_DATA_I5) );
//gpioWrite( M_DATA_O4, !gpioRead(S_DATA_I4) );
//gpioWrite( M_DATA_O3, !gpioRead(S_DATA_I3) );
//gpioWrite( M_DATA_O2, !gpioRead(S_DATA_I2) );
//gpioWrite( M_DATA_O1, !gpioRead(S_DATA_I1) );
//gpioWrite( M_DATA_O0, !gpioRead(S_DATA_I0) );
