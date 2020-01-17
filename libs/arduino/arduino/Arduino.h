
#ifndef __ARADUINO_H__
#define __ARADUINO_H__

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "binary.h"
#include "itoa.h"
#include "sapi.h"         /* <= sAPI header */

#ifdef __cplusplus
extern "C"{
#endif

void yield(void);

#ifndef HIGH
  #define HIGH 0x1
#endif
#ifndef LOW
  #define LOW  0x0
#endif

#define LED_BUILTIN LED1
#define F_CPU 204000000L

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#if defined(__LPC43XX__)
  #define DEFAULT 0               // 0 ... 3.5v?
  #define EXTERNAL 1             // VDDA?
  #define INTERNAL INTERNAL1V1   // bandgap?
#endif

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void init(void);
void initVariant(void);

int atexit(void (*func)()) __attribute__((weak));

void pinMode(uint32_t, uint32_t);
void digitalWrite(uint32_t, uint8_t);
uint8_t digitalRead(uint32_t);
uint32_t analogRead(uint32_t);
void analogReference(uint32_t mode);
void analogWrite(uint32_t, uint32_t);
void analogReadResolution(uint32_t);
void analogWriteResolution(uint32_t);

unsigned long millis(void);
unsigned long micros(void);
void delayMicroseconds(unsigned int us);

void shiftOut( uint32_t ulDataPin, uint32_t ulClockPin, uint32_t ulBitOrder, uint32_t ulVal );
uint32_t shiftIn( uint32_t ulDataPin, uint32_t ulClockPin, uint32_t ulBitOrder );

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void setup(void);
void loop(void);

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "WMath.h"
#include "HardwareSerial.h"



void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

uint32_t pulseIn( uint32_t ulPin, uint32_t ulState, uint32_t ulTimeout = 1000000L ) ;
uint32_t pulseInLong( uint8_t pin, uint8_t state, unsigned long timeout = 1000000L ) ;

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

#endif


#endif // __ARADUINO_H__
