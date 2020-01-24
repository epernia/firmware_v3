#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

mkdir -p ${PB}/src
mkdir -p ${PB}/inc

PROGRAM_NAME_UPPER=$(echo $PROGRAM_NAME | tr '[:lower:]' '[:upper:]')

#$LICENSE_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

cat <<EOF > ${PB}/config.mk
# Compile options

VERBOSE=n
OPT=g
USE_NANO=n
SEMIHOST=n
USE_FPU=y

# Libraries

USE_LPCOPEN=y
USE_SAPI=y
EOF

if [ -f "$LICENSE_PATH/license_file_header.sh" ]
then
	. "$LICENSE_PATH/license_file_header.sh"
else
	echo "Error, license_file_header.sh not found."
	FILE_HEADER=""
fi

cat <<EOF > ${PB}/src/${PROGRAM_NAME}.cpp
$FILE_HEADER

/*=====[Inclusions of function dependencies]=================================*/

#include "sapi.h"
#include "cxx_wrapper.hpp"

/*=====[Definition macros of private constants]==============================*/

using namespace sapi;

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

struct {
   GpioIn button;
   GpioOut led;
} io[] = {
   { { BUTTON }, { LED } },
};

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   GpioOut blinkLed(LED);

   blinkLed = ON;
   delay( 1000 );
   blinkLed = OFF;
   
   // ----- Repeat for ever -------------------------
   while( true ) {
      // Buttons --> Leds (with not because push = 0)
      for( auto& e: io )
         e.led = not e.button;
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
EOF

cat <<EOF > ${PB}/src/cxx_wrapper.cpp
#include "cxx_wrapper.hpp"

using namespace sapi;

Board_t Board;

Board_t::Board_t()
{
   boardInit();
}
EOF

cat <<EOF > ${PB}/inc/cxx_wrapper.hpp
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
EOF