#!/bin/sh
SB=$(dirname $(readlink -f $0))
B=$(readlink -f ${SB}/../)

#set -x

NAME=$(zenity --text="Project name" --entry) || exit 0

echo "Creating project ${NAME}"

PROJECTDIR=00-projects
PB=${B}/${PROJECTDIR}/${NAME}

mkdir -p ${PB}/src
mkdir -p ${PB}/inc
cat <<EOF > ${PB}/config.mk
# Compile options

VERBOSE=n
OPT=g
USE_NANO=y
SEMIHOST=n
USE_FPU=y

# Libraries

USE_LPCOPEN=y
USE_SAPI=y
LOAD_INRAM=n
EOF

cat <<EOF > ${PB}/src/${NAME}.c
#include <${NAME}.h>

int main()
{
   return 0;
}
EOF

cat <<EOF > ${PB}/inc/${NAME}.h
#ifndef __${NAME}_H__
#define __${NAME}_H__

#include <stdint.h>
#include <stddef.h>

#endif /* __${NAME}_H__ */
EOF

cat <<EOF > ${B}/project.mk.tmp
PROJECT_PATH = ${PROJECTDIR}
PROJECT_NAME = ${NAME}
EOF
mv -b ${B}/project.mk.tmp ${B}/project.mk
