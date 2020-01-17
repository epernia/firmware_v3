/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "sapi_adc.h"
#include "sapi_dac.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint32_t _readResolution = 10;
static uint32_t _writeResolution = 10;

void analogReadResolution(uint32_t res) {
  _readResolution = res;
}

void analogWriteResolution(uint32_t res) {
  _writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
  if (from == to)
    return value;
  if (from > to)
    return value >> (from-to);
  else
    return value << (to-from);
}

//perform the read operation on the selected analog pin.
//the initialization of the analog PIN is done through this function
uint32_t analogRead(uint32_t ulPin)
{
  uint16_t analogValue = adcRead( (adcMap_t) ulPin);

  return mapResolution(analogValue, 10, _readResolution);
}


void analogOutputInit(void) {
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// variant.cpp file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, uint32_t ulValue) {
  uint16_t analogValue = mapResolution(analogValue, _writeResolution, 10);

  if (ulPin == DAC) // if analog output is the DAC port => use dacWrite()
    dacWrite( (adcMap_t) ulPin, analogValue);
  else { // Use PWM on digital port
  	if (!pwmIsAttached( (pwmMap_t) ulPin)) {
  		
  	}
  	if (pwmIsAttached( (pwmMap_t) ulPin)) {
  		pwmWrite( (pwmMap_t) ulPin, (uint8_t) ulValue);
  	}

  }
}

#ifdef __cplusplus
}
#endif
