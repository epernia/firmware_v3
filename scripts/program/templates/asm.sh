#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

mkdir -p ${PB}/src

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

cat <<EOF > ${PB}/src/${PROGRAM_NAME}.s
   .syntax unified
   .cpu cortex-m4
   .fpu softvfp
   .thumb

.equ BUTTON, 0
.equ LED, 1
.equ DELAY, 100

   .section .text
   .type  main, %function
   .global  main
main:
   bl boardInit                  // boardInit();
main_loop:                       
   movs r0, #LED             
   bl gpioToggle                 // gpioToggle( LED );
   movs r0, #DELAY & 0xFFFFFFFF  // <-- r0 DELAY[31:0] 
   movs r1, #DELAY >> 31 //32    // <-- r1 DELAY[63:32]
   bl delay // delay( 100 );     // tick_t is a 64-bits data type!
   b main_loop                   // Infinite loop on stop
   .size main, . - main

/*
// Same example in C
int main( void )
{
   boardInit();
   while( true ) {
      gpioToggle( LED );
      delay( 100 );
   }
   return 0;
}
*/
EOF
