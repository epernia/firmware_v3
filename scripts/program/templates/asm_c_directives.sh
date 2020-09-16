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
#include "c_function.h"
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main(void) {

	/*=====[Definitions of private local variables]=============================*/
	volatile uint32_t A = 10, B = 20, C = 30, D = 40, sumResult_Asm,
			sumResult_C;

// ----- Setup -----------------------------------
	boardInit();

	sumResult_Asm = asmFunction(A, B, C, D);
	sumResult_C = c_function(A, B, C, D);
// ----- Repeat for ever -------------------------
	while ( true) {
		__WFI(); //wfi
	}

	// YOU NEVER REACH HERE, because this program runs directly or on a
	// microcontroller and is not called by any Operating System, as in the
	// case of a PC program.
	return 0;
}
EOF

cat <<EOF > ${PB}/src/c_function.c
$FILE_HEADER

#include "c_function.h"

uint32_t c_function(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3){
	return (arg0+arg1+arg2+arg3);
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
.global asmFunction

/**
 * Indicamos que la siguiente subrutina debe ser ensamblada en modo thumb,
 * entonces en las direcciones en el ultimo bit tendran el 1 para que se reconozcan como en modo thumb.
 * Siempre hay que ponerla antes de la primer instruccion.
 */
.thumb_func


/*=====[Definition macros of public constants]===============================*/

#define op1 r0
#define op2 r1
#define op3 r2
#define op4 r3

/*=====[Implementations of public assembly functions]=================================*/

/**
 *	prototipo de la funcion en C
 *
 *	uint32_t asmFunction(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
*/
asmFunction:
    push {lr}  /* guardamos la direccion de retorno en la pila */
	add op1, op2 /* r0 = r0 + r1 */
	add op1, op3 /* r0 = r0 + r2 */
	add op1, op4 /* r0 = r0 + r3 */
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

cat <<EOF > ${PB}/inc/c_function.h
$FILE_HEADER

#ifndef __${PROGRAM_NAME_UPPER}_C_FUNCTION_H__
#define __${PROGRAM_NAME_UPPER}_C_FUNCTION_H__

#include "${PROGRAM_NAME}.h"

/**
 * c_function do the same of ASM function for compare purpose
 * 
 * @param arg0
 * @param arg1
 * @param arg2
 * @param arg3
 * @return
 */
uint32_t c_function(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif /* __${PROGRAM_NAME_UPPER}_C_FUNCTION_H__ */
EOF

cat <<EOF > ${PB}/inc/asm_module.h
$FILE_HEADER

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __${PROGRAM_NAME_UPPER}_ASM_MODULE_H__
#define __${PROGRAM_NAME_UPPER}_ASM_MODULE_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include "${PROGRAM_NAME}.h"

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
 * @brief C prototype function link with asm_module
 *
 * Automatically when you call and return from function, MCU utilizes these register:
 * 
 * |   C	| CORE    |
 * | ----- 	| ------- |
 * |  r0	|  arg0   |
 * |  r1 	|  arg1   |
 * |  r2 	|  arg2   |
 * |  r3 	|  arg3   |
 * | ----- 	| ------- |
 * |  r0 	| returnL |
 * |  r1 	| returnH |
 *
 * @param arg0
 * @param arg1
 * @param arg2
 * @param arg3
 * @return implicit in r0 when return its 32bits, and r1-r0 when is 64bits
 */
extern uint32_t asmFunction(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __${PROGRAM_NAME_UPPER}_ASM_MODULE__ */
EOF