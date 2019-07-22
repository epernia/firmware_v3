/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */
 
/*=====[Inclusion of own header]=============================================*/

#include "moduleName.h"
 
/*=====[Inclusions of private function dependencies]=========================*/

#include "dependency.h"
#include <dependency.h>

/*=====[Definition macros of private constants]==============================*/

#define QMC5883L_REG_Y_LSB       0x02
#define MAX_AMOUNT_OF_RGB_LEDS   9

/*=====[Private function-like macros]========================================*/

#define rtcConfig rtcInit

/*=====[Definitions of private data types]===================================*/

// Data type that renames an elementary data type
typedef uint32_t gpioRegister_t;

// Function pointer data type
typedef void (*FuncPtrPrivado_t)(void *);

// Enumerated data type
typedef enum {
   GPIO_INPUT_PULLUP, 
   GPIO_INPUT_PULLDOWN,
   GPIO_REPEATER
} gpioLPC4337Modes_t;

// Structured, union or bit field data type
typedef struct {
   uint8_t scuPort;
   uint8_t scuPin;
   uint8_t func;
   uint8_t gpioPort;
   uint8_t gpioPin;
} gpioLpc4337_t;

/*=====[Definitions of external public global variables]=====================*/

extern int32_t varGlobalExterna;

/*=====[Definitions of public global variables]==============================*/

int32_t varGlobalPublica = 0;

/*=====[Definitions of private global variables]=============================*/

static int32_t varGlobalPrivada = 0;

/*=====[Prototypes (declarations) of private functions]======================*/

static void funPrivada(void);

/*=====[Implementations of public functions]=================================*/

bool_t rtcInit( rtc_t* rtc )
{
   // ...
}

/*=====[Implementations of interrupt functions]==============================*/

void UART0_IRQHandler(void)
{
   // ...
}

/*=====[Implementations of private functions]================================*/

static void funPrivada(void)
{
   // ...
}

