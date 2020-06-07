#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia
# All rights reserved.

mkdir -p ${PB}/src
mkdir -p ${PB}/inc

MODULE_NAME_UPPER=$(echo $MODULE_NAME | tr '[:lower:]' '[:upper:]')

#$LICENSE_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

if [ -f "$LICENSE_PATH/license_file_header.sh" ]
then
	. "$LICENSE_PATH/license_file_header.sh"
else
	echo "Error, license_file_header.sh not found."
	FILE_HEADER=""
fi
echo $PB
cat <<EOF > ${PB}/src/${MODULE_NAME}.c
/*=====${MODULE_NAME}===========================================================
 
$FILE_HEADER

/*=====[Inclusion of own header]=============================================*/

#include "${MODULE_NAME}.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of external public functions]==============*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

EOF

cat <<EOF > ${PB}/inc/${MODULE_NAME}.h
$FILE_HEADER

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _${MODULE_NAME_UPPER}_H_
#define _${MODULE_NAME_UPPER}_H_

/*=====[Inclusions of public function dependencies]==========================*/

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

#endif /* _${MODULE_NAME_UPPER}_H_ */

EOF


echo "MODULES+=${PB}/" >> $SELECTED_PROGRAM/config.mk
