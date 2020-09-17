#!/bin/bash

# BSD 3-Clause License
# Copyright (c) 2018, Martin Ribelotta
# All rights reserved.

BASE=$(readlink -f $(dirname $(readlink -f $0))/../../)
OUT=${BASE}/.build-log

#rm -fr ${OUT}
mkdir -p ${OUT}
OUT_GLOBAL_PASS=${OUT}/PASS.log
OUT_GLOBAL_FAIL=${OUT}/FAIL.log

ALL=$(for f in $(find ${BASE} -name config.mk); do dirname $f; done |grep -v deprecated)

N=$(echo ${ALL} | wc -w)
COUNT=1
for D in ${ALL}
do
   P_NAME=$(basename ${D})
   P_PATH=$(dirname ${D})
   OUT_STDOUT=${OUT}/${P_NAME}.stdout
   OUT_STDERR=${OUT}/${P_NAME}.stderr
   printf "[%02d of %02d] \e[36m%-32s\e[0m BUILD " ${COUNT} ${N} [${P_NAME}]
   make -C ${BASE} \
      PROJECT_PATH=${P_PATH} \
      PROJECT_NAME=${P_NAME} \
      CROSS="ccache arm-none-eabi-" \
      all clean > ${OUT_STDOUT} 2>${OUT_STDERR} \
         && (echo -e "[\e[32mPASS\e[0m]"; echo "${D}" >> ${OUT_GLOBAL_PASS}) \
         || (echo -e "[\e[31mFAIL\e[0m]"; echo "${D}" >> ${OUT_GLOBAL_FAIL})
   COUNT=$((${COUNT} + 1))
done
echo
echo "************** PASS Projects **************"
cat ${OUT_GLOBAL_PASS}
echo
echo "************** FAIL Projects **************"
cat ${OUT_GLOBAL_FAIL}
echo
echo "*************** FAIL Logs *****************"
FAIL_NUM=0
for D in $(cat ${OUT_GLOBAL_FAIL})
do
   P_NAME=$(basename ${D})
   OUT_STDOUT=${OUT}/${P_NAME}.stdout
   OUT_STDERR=${OUT}/${P_NAME}.stderr
   echo
   echo "STDOUT for ${P_NAME}"
   cat ${OUT_STDOUT}
   echo
   echo "STDERR for ${P_NAME}"
   cat ${OUT_STDERR}
   FAIL_NUM=$((${FAIL_NUM} + 1))
done
echo "Failed ${FAIL_NUM} projects"
exit ${FAIL_NUM}
