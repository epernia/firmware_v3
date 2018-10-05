#ifndef CDC_UART_ENDPOINTS_H_
#define CDC_UART_ENDPOINTS_H_

/* Manifest constants defining interface numbers and endpoints used by a
   particular interface in this application.
 */
#define USB_CDC_CIF_NUM         0
#define USB_CDC_DIF_NUM         1
#define USB_CDC_IN_EP           0x81
#define USB_CDC_OUT_EP          0x01
#define USB_CDC_INT_EP          0x82

#endif // CDC_UART_ENDPOINTS_H_