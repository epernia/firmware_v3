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
USE_NANO=n
SEMIHOST=n
USE_FPU=y

# Libraries

USE_LPCOPEN=y
USE_SAPI=y
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
#include "sapi.h"

/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

int main( void )
{
   // ----- Setup -----------------------------------
   boardInit();

   // ----- Repeat for ever -------------------------
   while( true ) {
      gpioToggle(LED);
      delay(500);
   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
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