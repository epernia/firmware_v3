// led.c
#include "led.h"

// Implementaciones de funciones publicas

void ledsInit( void )
{
   gpioInit( LED_ROJO, GPIO_OUTPUT );
   gpioInit( LED_AMARILLO, GPIO_OUTPUT );
   gpioInit( LED_VERDE, GPIO_OUTPUT );
}

void ledOn( int32_t pin )
{
   gpioWrite( pin, ON );
}

void ledOff( int32_t pin )
{
   gpioWrite( pin, OFF );
}
