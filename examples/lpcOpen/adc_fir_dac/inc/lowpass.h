/*
 * lowpass.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Pablo
 */

#ifndef LOWPASS_H_
#define LOWPASS_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 22000 Hz

fixed point precision: 31 bits

* 0 Hz - 1500 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = n/a

* 2000 Hz - 11000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = n/a

*/

#define LOWPASS_TAP_NUM 49
extern const int lowpass_taps[];

#endif /* LOWPASS_H_ */
