/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _MODULE_NAME_H_
#define _MODULE_NAME_H_

/*=====[Inclusions of public function dependencies]==========================*/

#include "dependency.h"
#include <dependency.h>

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

#define PI 3.14

/*=====[Public function-like macros]=========================================*/

#define printInt(printer,number) printIntFormat((printer),(number),(DEC_FORMAT))

#define printlnString(printer,string);   {printString((printer),(string));\
                                          printEnter((printer));}

/*=====[Definitions of public data types]====================================*/

// Data type that renames an elementary data type
typedef uint8_t bool_t;

// Function pointer data type
typedef void (*callBackFuncPtr_t)(void *);

// Enumerated data type
typedef enum {
   GPIO_INPUT, 
   GPIO_OUTPUT
} gpioMode_t;

typedef enum {
   HMC5883L_REG_X_MSB = 0x00, 
   HMC5883L_REG_X_LSB = 0x01,
   HMC5883L_REG_Y_MSB = 0x02,
   HMC5883L_REG_Y_LSB = 0x03,
   HMC5883L_REG_Z_MSB = 0x04,
   HMC5883L_REG_Z_LSB = 0x05
} hmc5883lRegisters_t;

// Structured, union or bit field data type
typedef struct {
   uint16_t year;	 /* 1 to 4095 */
   uint8_t  month; /* 1 to 12   */
   uint8_t  mday;	 /* 1 to 31   */
   uint8_t  wday;	 /* 1 to 7    */
   uint8_t  hour;	 /* 0 to 23   */
   uint8_t  min;	 /* 0 to 59   */
   uint8_t  sec;	 /* 0 to 59   */
} rtc_t;

/*=====[Prototypes (declarations) of public functions]=======================*/

bool_t rtcInit( rtc_t* rtc );

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

void UART0_IRQHandler(void);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _MODULE_NAME_H_ */
