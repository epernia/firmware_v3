#ifndef SAPI_USB_DEVICE_H_
#define SAPI_USB_DEVICE_H_

// Datatypes
#include "sapi_datatypes.h"

// Devices
#include "usbd_keyboard.h"
#include "cdc_uart.h"



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

#endif // SAPI_USB_DEVICE_H_

// See https://github.com/hathach/tinyusb
