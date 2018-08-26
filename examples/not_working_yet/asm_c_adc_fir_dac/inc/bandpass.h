/*
 * bandpass.h
 *
 *  Created on: Sep 7, 2013
 *      Author: Pablo
 */

#ifndef BANDPASS_H_
#define BANDPASS_H_

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 22000 Hz

fixed point precision: 31 bits

* 0 Hz - 500 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -50.91 dB

* 900 Hz - 1100 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 1.17 dB

* 1500 Hz - 11000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -50.91 dB

*/

#define BANDPASS_TAP_NUM 95
extern const int bandpass_taps[BANDPASS_TAP_NUM];

#endif /* BANDPASS_H_ */
