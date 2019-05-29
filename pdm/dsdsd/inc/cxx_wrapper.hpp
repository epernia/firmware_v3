#ifndef __CXX_WRAPPER_H__
#define __CXX_WRAPPER_H__

#include "sapi.h"

namespace sapi
{

class GpioOut
{
private:
   gpioMap_t _pin;
public:
   GpioOut(gpioMap_t pin): _pin(pin)
   {
      gpioConfig(_pin, GPIO_OUTPUT);
   }
   GpioOut& operator=(int v)
   {
      gpioWrite(_pin, v);
      return *this;
   }
   inline operator int() const
   {
      return gpioRead(_pin);
   }
   GpioOut& toggle()
   {
      *this = inverted();
      return *this;
   }
   bool isOn() const
   {
      return int(*this);
   }
   bool isOff() const
   {
      return !isOn();
   }
   int inverted() const
   {
      return !int(*this);
   }
};

class GpioIn
{
private:
   gpioMap_t _pin;
public:
   GpioIn(gpioMap_t pin) : _pin(pin)
   {
      gpioConfig(_pin, GPIO_INPUT);
   }
   operator int() const
   {
      return gpioRead(_pin);
   }
   bool isOn() const
   {
      return int(*this);
   }
   bool isOff() const
   {
      return !isOn();
   }
   int inverted() const
   {
      return !int(*this);
   }
};

class Board_t
{
public:
   Board_t();

   float frec() const
   {
      SystemCoreClockUpdate();
      return SystemCoreClock / 1000.0f;
   }
};

extern Board_t Board;

template<typename T, int N> int arraySize(T(&)[N])
{
   return N;
}

}

#endif
