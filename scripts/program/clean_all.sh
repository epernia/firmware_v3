#!/bin/bash

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

BASE=$(readlink -f $(dirname $(readlink -f $0))/../../)

ALL=$(for f in $(find ${BASE} -name config.mk); do dirname $f; done |grep -v deprecated)

N=$(echo ${ALL} | wc -w)

PROG_IDX=0

for D in ${ALL}
do
   rm -fR "$D/out"
   echo "clean $D/out"
	PROG_IDX=`expr $PROG_IDX + 1`
done

echo "Clean builds for $PROG_IDX programas."
