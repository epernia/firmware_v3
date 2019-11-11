/*
 * swhdl.c
 *
 * Minimal debouncing switch handler.
 * hardware access.
 *
 */

#include "board.h"
#include "swhdl.h"


MUInt rawsw1( void )
{
    return gpioRead( TEC1 );
}

MUInt rawsw2( void )
{
    return gpioRead( TEC2 );
}
