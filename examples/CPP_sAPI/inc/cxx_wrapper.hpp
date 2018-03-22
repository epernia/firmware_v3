#ifndef __CXX_WRAPPER_H__
#define __CXX_WRAPPER_H__

#include "sapi.h"

namespace sapi
{

class DigitalOut
{
private:
   gpioMap_t _pin;
public:
   DigitalOut(gpioMap_t pin): _pin(pin)
   {
      gpioConfig(_pin, GPIO_OUTPUT);
   }
   DigitalOut& operator=(int v)
   {
      gpioWrite(_pin, v);
      return *this;
   }
   inline operator int() const
   {
      return gpioRead(_pin);
   }
};

class DigitalIn
{
private:
   gpioMap_t _pin;
public:
   DigitalIn(gpioMap_t pin) : _pin(pin)
   {
      gpioConfig(_pin, GPIO_INPUT);
   }
   operator int() const
   {
      return gpioRead(_pin);
   }
};

class Board_t
{
public:
   Board_t();
};

extern Board_t Board;

template<typename T, int N> int arraySize(T(&)[N])
{
   return N;
}

}

#endif
