#include "sapi_usb_device.h"


// @ LPC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*
 * @brief This file using USB ROM Drivers.
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

#include <stdio.h>
#include <string.h>

#include "lpc_app_usbd_cfg.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
static USBD_HANDLE_T g_hUsb;

/* Endpoint 0 patch that prevents nested NAK event processing */
static uint32_t g_ep0RxBusy = 0;/* flag indicating whether EP0 OUT/RX buffer is busy. */
static USB_EP_HANDLER_T g_Ep0BaseHdlr; /* variable to store the pointer to base EP0 handler */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const USBD_API_T *g_pUsbApi;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* EP0_patch part of WORKAROUND for artf45032. */
ErrorCode_t EP0_patch(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
   switch (event) {
   case USB_EVT_OUT_NAK:
      if (g_ep0RxBusy) {
         /* we already queued the buffer so ignore this NAK event. */
         return LPC_OK;
      }
      else {
         /* Mark EP0_RX buffer as busy and allow base handler to queue the buffer. */
         g_ep0RxBusy = 1;
      }
      break;

   case USB_EVT_SETUP:  /* reset the flag when new setup sequence starts */
   case USB_EVT_OUT:
      /* we received the packet so clear the flag. */
      g_ep0RxBusy = 0;
      break;
   }
   return g_Ep0BaseHdlr(hUsb, data, event);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief   Handle interrupt from USB
 * @return  Nothing
 */
#ifndef USB_HOST_ONLY // Parche para envitar conflictos con biblioteca host
void USB_IRQHandler(void){
   USBD_API->hw->ISR(g_hUsb);
}
#endif

/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
   USB_COMMON_DESCRIPTOR *pD;
   USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
   uint32_t next_desc_adr;

   pD = (USB_COMMON_DESCRIPTOR *) pDesc;
   next_desc_adr = (uint32_t) pDesc;

   while (pD->bLength) {
      /* is it interface descriptor */
      if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

         pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
         /* did we find the right interface descriptor */
         if (pIntfDesc->bInterfaceClass == intfClass) {
            break;
         }
      }
      pIntfDesc = 0;
      next_desc_adr = (uint32_t) pD + pD->bLength;
      pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
   }

   return pIntfDesc;
}

// @ LPC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// Private function declarations (prototypes) -------------------------------------------------

static ErrorCode_t usbDeviceLpcInit(USB_CORE_DESCS_T* desc,
                                    USBD_API_INIT_PARAM_T* usb_param);

// For now only used by cdc_uart
static void usbDeviceLpcInterruptPrioritySet( uint32_t priority ); 

static void usbDeviceLpcInterruptInit( void ); 


// Public function definitions ----------------------------------------------------------------

bool_t usbDeviceInit( UsbSubClass_t subclass ){
   
   ErrorCode_t ret = LPC_OK;
   USBD_API_INIT_PARAM_T usb_param;
   USB_CORE_DESCS_T desc;
   
   switch(subclass){
      
      case USB_HID_MOUSE:
         printf("USB_HID_MOUSE\r\n");
      break;
      
      
      case USB_HID_KEYBOARD:
         printf("USB_HID_KEYBOARD\r\n");
      
         usb_param.max_num_ep = 2; // Keyboard has 2 endpoints
      
         ret = usbDeviceLpcInit(&desc, &usb_param);

         // Configuration routine for HID USB Keyboard example
         ret = usbDeviceKeyboardInit(
                  g_hUsb,
                  (USB_INTERFACE_DESCRIPTOR *) &USB_FsConfigDescriptor[sizeof(USB_CONFIGURATION_DESCRIPTOR)],
                  &(usb_param.mem_base),
                  &(usb_param.mem_size)
               );
      
         if (ret == LPC_OK) {
            usbDeviceLpcInterruptInit();
            return TRUE;
         }         
      break;
         
         
      case USB_HID_GENERIC:
         printf("USB_HID_GENERIC\r\n");
      break;
      
      
      case USB_CDC_UART:
         printf("USB_CDC_UART\r\n");
      
         usb_param.max_num_ep = 4; // CDC Uart has 4 endpoints
      
         ret = usbDeviceLpcInit(&desc, &usb_param);
      
         /* Init UCOM - USB to UART bridge interface */
         ret = cdcUartLpcInit(g_hUsb, &desc, &usb_param);
      
         if (ret == LPC_OK) {
            /* Make sure USB and UART IRQ priorities are same (5) for this example */
            usbDeviceLpcInterruptPrioritySet(5); // FreeRTOS Requiere prioridad>=5
            usbDeviceLpcInterruptInit();      
            return TRUE;   
         }
      break;
         
         
      case USB_MSC_DRIVE:
         printf("USB_MSC_DRIVE\r\n");
         return TRUE;
      break;     
     
      
      default:
         printf("Error, you must use a valid USB device class\r\n");
         return TRUE;
   }
   
   return FALSE;
}


// Private function definitions ---------------------------------------------------------------

// @ LPC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/**
 * @brief Configuration routine USB Device
 * maxNumEP max number of endpoints supported by the USB device
 * controller instance (specified by \em usb_reg_base field)
 * to which this instance of stack is attached.
 */
static ErrorCode_t usbDeviceLpcInit(USB_CORE_DESCS_T* desc,
                                    USBD_API_INIT_PARAM_T* usb_param){

   ErrorCode_t ret = LPC_OK;
   USB_CORE_CTRL_T *pCtrl;

   /* enable clocks and pinmux */
   USB_init_pin_clk();

   /* Init USB API structure */
   g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->usbdApiBase;
   
   // Save max_num_ep that is set outside this function because memset()
   // destroy it.
   uint8_t max_num_ep = usb_param->max_num_ep;
   
   /* initialize call back structures */
   memset((void *) usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));   
   usb_param->usb_reg_base = LPC_USB_BASE;
   usb_param->mem_base = USB_STACK_MEM_BASE;
   usb_param->mem_size = USB_STACK_MEM_SIZE;   
   
   // Restore max_num_ep in usb_param
   usb_param->max_num_ep = max_num_ep;

   /* Set the USB descriptors */
   desc->device_desc = (uint8_t *) USB_DeviceDescriptor;
   desc->string_desc = (uint8_t *) USB_StringDescriptor;

   #ifdef USE_USB0
      desc->high_speed_desc = USB_HsConfigDescriptor;
      desc->full_speed_desc = USB_FsConfigDescriptor;
      desc->device_qualifier = (uint8_t *) USB_DeviceQualifier;
   #else
      /* Note, to pass USBCV test full-speed only devices should have both
       * descriptor arrays point to same location and device_qualifier set
       * to 0.
       */
      desc->high_speed_desc = USB_FsConfigDescriptor;
      desc->full_speed_desc = USB_FsConfigDescriptor;
      desc->device_qualifier = 0;
   #endif

   /* USB Initialization */
   ret = USBD_API->hw->Init(&g_hUsb, desc, usb_param);

   if (ret == LPC_OK) {      
      /* WORKAROUND for artf45032 ROM driver BUG:
         Due to a race condition there is the chance that a second NAK event will
         occur before the default endpoint0 handler has completed its preparation
         of the DMA engine for the first NAK event. This can cause certain fields
         in the DMA descriptors to be in an invalid state when the USB controller
         reads them, thereby causing a hang. */
      pCtrl = (USB_CORE_CTRL_T *) g_hUsb; /* convert the handle to control structure */
      g_Ep0BaseHdlr = pCtrl->ep_event_hdlr[0];/* retrieve the default EP0_OUT handler */
      pCtrl->ep_event_hdlr[0] = EP0_patch;/* set our patch routine as EP0_OUT handler */
   }
   return ret;
}

static void usbDeviceLpcInterruptPrioritySet( uint32_t priority ){
   NVIC_SetPriority( LPC_USB_IRQ, priority );
}

static void usbDeviceLpcInterruptInit( void ){
    /*  enable USB interrupts */
   NVIC_EnableIRQ( LPC_USB_IRQ );
   /* now connect */
   USBD_API->hw->Connect( g_hUsb, 1 );
}

// @ LPC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>