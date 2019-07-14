/* Copyright 2018, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SAPI_USB_DEVICE_H_
#define _SAPI_USB_DEVICE_H_

// Datatypes
#include "sapi_datatypes.h"

// Devices
#include "usbd_keyboard.h"
#include "cdc_uart.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

// Rename for compatibility
#define usbDeviceConfig usbDeviceInit

// USB Classes
typedef enum{
   USB_HID,    // Human Interface Device (HID). Support for Device and Host
   USB_CDC,    // Communication Class Device (CDC). Support for Device and Host
   USB_MSC,    // Mass Storage Class Device (MSC). Support for Device and Host
   //USB_HUB     // Hub currnetly only support 1 level of hub. Only supprt for Host
} UsbClass_t;

// USB Sub Classes
typedef enum{
   USB_HID_MOUSE,      // Support for Device and Host
   USB_HID_KEYBOARD,   // Support for Device and Host
   USB_HID_GENERIC,    // Support for Device and Host
   USB_CDC_UART,       // Support for Device and Host
   USB_MSC_DRIVE       // Support for Device and Host
} UsbSubClass_t;

// USB Device Initialization
bool_t usbDeviceInit( UsbSubClass_t subclass );

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_USB_DEVICE_H_ */

// See https://github.com/hathach/tinyusb
