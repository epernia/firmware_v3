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

#ifndef _SAPI_USBD_KEYBOARD_H_
#define _SAPI_USBD_KEYBOARD_H_

#include "lpc_app_usbd_cfg.h"
#include "sapi_datatypes.h"
#include "sapi_peripheral_map.h"

/*==================[c++]====================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup EXAMPLES_USBDROM_18XX43XX_KEYBOARD
 * @{
 */

#define KEYBOARD_REPORT_SIZE                        8

#define HID_KEYBOARD_CLEAR_REPORT(x)                memset(x, 0, 8);
#define HID_KEYBOARD_REPORT_SET_KEY_PRESS(x, val)   x[2] = (uint8_t) val;


// USB HID Keyboard Scan codes
typedef enum{
   USB_RESERVED            = 0x00,
   USB_ERROR_ROLL_OVER     = 0x01,
   USB_POST_FAIL           = 0x02,
   USB_ERROR_UNDEFINED     = 0x00,
   
   USB_KEY_A               = 0x04,
   USB_KEY_B               = 0x05,
   USB_KEY_C               = 0x06,
   USB_KEY_D               = 0x07,
   USB_KEY_E               = 0x08,
   USB_KEY_F               = 0x09,
   USB_KEY_G               = 0x0A,
   USB_KEY_H               = 0x0B,
   USB_KEY_I               = 0x0C,
   USB_KEY_J               = 0x0D,
   USB_KEY_K               = 0x0E,
   USB_KEY_L               = 0x0F,
   USB_KEY_M               = 0x10,
   USB_KEY_N               = 0x11,
   USB_KEY_O               = 0x12,
   USB_KEY_P               = 0x13,
   USB_KEY_Q               = 0x14,
   USB_KEY_R               = 0x15,
   USB_KEY_S               = 0x16,
   USB_KEY_T               = 0x17,
   USB_KEY_U               = 0x18,
   USB_KEY_V               = 0x19,
   USB_KEY_W               = 0x1A,
   USB_KEY_X               = 0x1B,
   USB_KEY_Y               = 0x1C,
   USB_KEY_Z               = 0x1D,
   
   USB_KEY_1               = 0x1E,  // 1 and !
   USB_KEY_2               = 0x1F,  // 2 and @
   USB_KEY_3               = 0x20,  // 3 and #
   USB_KEY_4               = 0x21,  // 4 and $
   USB_KEY_5               = 0x22,  // 5 and %
   USB_KEY_6               = 0x23,  // 6 and ^
   USB_KEY_7               = 0x24,  // 7 and &
   USB_KEY_8               = 0x25,  // 8 and *
   USB_KEY_9               = 0x26,  // 9 and (
   USB_KEY_0               = 0x27,  // 9 and )
                                
   USB_KEY_ENTER           = 0x28,
   USB_KEY_ESCAPE          = 0x29,
   USB_KEY_BACKSPACE       = 0x2A,
   USB_KEY_TAB             = 0x2B,
   USB_KEY_SPACEBAR        = 0x2C,
   
   USB_KEY_MINUS           = 0x2D, // - and _
   USB_KEY_EQUAL           = 0x2E, // = and +
   
   
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

/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _SAPI_USBD_KEYBOARD_H_ */
