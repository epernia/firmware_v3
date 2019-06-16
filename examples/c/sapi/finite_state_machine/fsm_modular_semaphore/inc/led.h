// led.h
#ifndef _LED_H_
#define _LED_H_

// Inclusion de dependencias
#include "sapi.h"

// Macros publicas

#define LED_ROJO     LED2
#define LED_AMARILLO LED1
#define LED_VERDE    LEDG

// Declaraciones de funciones publicas

void ledsInit( void );

void ledOn( int32_t pin );
void ledOff( int32_t pin );

#endif // _LED_H_
