#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

mkdir -p ${PB}/src
mkdir -p ${PB}/inc

PROGRAM_NAME_UPPER=$(echo $PROGRAM_NAME | tr '[:lower:]' '[:upper:]')

#$LICENSE_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

cat <<EOF > ${PB}/config.mk
# Compile options
VERBOSE=n
OPT=g
USE_NANO=y
SEMIHOST=n
USE_FPU=y
EOF

if [ -f "$LICENSE_PATH/license_file_header.sh" ]
then
	. "$LICENSE_PATH/license_file_header.sh"
else
	echo "Error, license_file_header.sh not found."
	FILE_HEADER=""
fi

cat <<EOF > ${PB}/src/${PROGRAM_NAME}.c
$FILE_HEADER

/*=====[Inclusions of function dependencies]=================================*/

#include "${PROGRAM_NAME}.h"
#include "asm_module.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
/*=====[Definitions of private local variables]=============================*/
	volatile  uint32_t aValue = 20,
			 otherValue = 30,
			 sumResult_Asm,sumResult_C;
			 
// ----- Setup -----------------------------------
   	boardInit();
	
	sumResult_Asm = asmSum(aValue, otherValue);

// ----- Repeat for ever -------------------------
   while( true ) {
		__WFI(); //wfi
	}

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
EOF

cat <<EOF > ${PB}/src/asm_module.S
$FILE_HEADER

/**
 * Directiva al ensablador que permite indicar que se encarga de buscar
 * la instruccion mas apropiada entre ARM y thumb2
 */
.syntax unified

/**
 * .text permite indicar una seccion de codigo.
 */
.text

/**
 * .global permite definir un simbolo exportable,
 * es decir que podemos verlo desde otros modulos (equivalente a extern).
 * Definimos la rutina como global para que sea visible desde otros modulos.
 */
.global asmSum

/**
 * Indicamos que la siguiente subrutina debe ser ensamblada en modo thumb,
 * entonces en las direcciones en el ultimo bit tendran el 1 para que se reconozcan como en modo thumb.
 * Siempre hay que ponerla antes de la primer instruccion.
 */
.thumb_func


/*=====[Definition macros of public constants]===============================*/

#define op1 r0
#define op2 r1

/*=====[Implementations of public assembly functions]=================================*/

/**
 *	prototipo de la funcion en C
 *
 *	uint32_t asmSum(uint32_t firstOperand, uint32_t secondOperand);
 *
 *	En r0 se cargara firstOperand y en r1 se carga secondOperand. Luego el valor devuelto estara en r0 (si son 32 bits).
 *	Si el resultado que retorna es en 64 bits, usa r0 y r1.
*/
asmSum:
    push {lr}  /* guardamos la direccion de retorno en la pila */
	add op1, op2 /* r0 = r0 + r1 */
	pop {pc}   /* retorno */

	/* otras alternativas para el retorno */
	/* 1. mov pc,lr
	/  2. bx lr */
	/* pop {pc} */
EOF

cat <<EOF > ${PB}/inc/${PROGRAM_NAME}.h
$FILE_HEADER

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __${PROGRAM_NAME_UPPER}_H__
#define __${PROGRAM_NAME_UPPER}_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __${PROGRAM_NAME_UPPER}_H__ */
EOF

cat <<EOF > ${PB}/inc/asm_module.h
$FILE_HEADER

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __ASM_MODULE_H__
#define __ASM_MODULE_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[External data declaration]===========================================*/

/*=====[external functions declaration]======================================*/

/**
 * Funcion que suma dos enteros de 32 bits y devuelve el resultado
 * @param firstOperand primer sumando
 * @param secondOperand segundo sumando
 * @return suma
 */
extern uint32_t asmSum(uint32_t firstOperand, uint32_t secondOperand);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __ASM_MODULE__ */
EOF