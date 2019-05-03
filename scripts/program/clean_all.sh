#!/bin/bash

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

BASE=$(readlink -f $(dirname $(readlink -f $0))/../../)

ALL=$(for f in $(find ${BASE} -name config.mk); do dirname $f; done |grep -v deprecated)

N=$(echo ${ALL} | wc -w)

for D in ${ALL}
do
   rm -fR "$D/out"
   echo "clean $D/out"
done
