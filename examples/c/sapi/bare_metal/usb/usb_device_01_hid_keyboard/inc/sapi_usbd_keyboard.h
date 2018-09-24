/*
 * @brief This file contains USB HID Keyboard example using USB ROM Drivers.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef SAPI_USBD_KEYBOARD_H
#define SAPI_USBD_KEYBOARD_H

#include "lpc_app_usbd_cfg.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @ingroup EXAMPLES_USBDROM_18XX43XX_KEYBOARD
 * @{
 */

#define KEYBOARD_REPORT_SIZE                        8

#define HID_KEYBOARD_CLEAR_REPORT(x)                memset(x, 0, 8);
#define HID_KEYBOARD_REPORT_SET_KEY_PRESS(x, val)   x[2] = (uint8_t) val;


// USB HID Keyboard Scan codes
typedef enum{
   USB_KEYBOARD_KEY_A = 0x04,
   USB_KEYBOARD_KEY_C = 0x06,
   USB_KEYBOARD_KEY_I = 0x0C,
   USB_KEYBOARD_KEY_ENTER = 0x28,
} usb_keyboard_keys_t;

/**
 * @brief	HID keyboard interface init routine.
 * @param	hUsb		: Handle to USB device stack
 * @param	pIntfDesc	: Pointer to HID interface descriptor
 * @param	mem_base	: Pointer to memory address which can be used by HID driver
 * @param	mem_size	: Size of the memory passed
 * @return	On success returns LPC_OK. Params mem_base and mem_size are updated
 *			to point to new base and available size.
 */
extern ErrorCode_t usbDeviceKeyboardInit(
                      USBD_HANDLE_T hUsb,
                      USB_INTERFACE_DESCRIPTOR *pIntfDesc,
                      uint32_t *mem_base,
                      uint32_t *mem_size
                   );

/**
 * @brief	Keyboard tasks.
 * @return	On success returns LPC_OK.
 */
extern void usbDeviceKeyboardTasks(void);


// Keyboard receive from host set callback
bool_t usbDeviceKeyboardReceiveFromHostCallbackSet(
         callBackFuncPtr_t keyboardReceiveCallback
       );
       
       
// Keyboard function to check inmy device for pressed keys
bool_t usbDeviceKeyboardCheckKeysCallbackSet(
         callBackFuncPtr_t checkForPressedKeysCallback
       );

void usbDeviceKeyboardPress( uint8_t key );
void usbDeviceKeyboardPressHold( uint8_t key );
void usbDeviceKeyboardRelease( uint8_t key );


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* SAPI_USBD_KEYBOARD_H */
