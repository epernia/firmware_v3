/*=====[Nombre del modulo]=====================================================
 * Copyright YYYY Nombre completo del autor <author@mail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Fecha de creacion: YYYY/MM/DD
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "nombreModulo.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "dependencia.h"
#include <dependencia.h>

/*=====[Macros de definicion de constantes privadas]=========================*/

#define QMC5883L_REG_Y_LSB       0x02
#define MAX_AMOUNT_OF_RGB_LEDS   9

/*=====[Macros estilo funcion privadas]======================================*/

#define rtcConfig rtcInit

/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico
typedef uint32_t gpioRegister_t;

// Tipo de datos de puntero a funcion
typedef void (*FuncPtrPrivado_t)(void *);

// Tipo de datos enumerado
typedef enum {
   GPIO_INPUT_PULLUP, 
   GPIO_INPUT_PULLDOWN,
   GPIO_REPEATER
} gpioLPC4337Modes_t;

// Tipo de datos estructua, union o campo de bits
typedef struct {
   uint8_t scuPort;
   uint8_t scuPin;
   uint8_t func;
   uint8_t gpioPort;
   uint8_t gpioPin;
} gpioLpc4337_t;

/*=====[Definiciones de Variables globales publicas externas]================*/

extern int32_t varGlobalExterna;

/*=====[Definiciones de Variables globales publicas]=========================*/

int32_t varGlobalPublica = 0;

/*=====[Definiciones de Variables globales privadas]=========================*/

static int32_t varGlobalPrivada = 0;

/*=====[Prototipos de funciones privadas]====================================*/

static void funPrivada(void);

/*=====[Implementaciones de funciones publicas]==============================*/

bool_t rtcInit( rtc_t* rtc )
{
   // ...
}

/*=====[Implementaciones de funciones de interrupcion publicas]==============*/

void UART0_IRQHandler(void)
{
   // ...
}

/*=====[Implementaciones de funciones privadas]==============================*/

static void funPrivada(void)
{
   // ...
}

