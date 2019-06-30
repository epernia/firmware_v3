#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia
# All rights reserved.

#set -x

# Script folder path
SB=$(dirname $(readlink -f $0))

# Repository root folder path
B=$(readlink -f ${SB}/../../)

# Repository root folder name
REPO_FOLDER=$(basename $B)

echo ""
echo "Select target board to compile wizard is starting..."
echo ""

FORM_SELECT_BOARD=$(zenity --forms --title="Select board" \
	--text="Select a target board to compile" \
	--separator="|" \
	--add-list="Board" --list-values="edu_ciaa_nxp|ciaa_nxp" )

RET=$?


case $RET in
	0)
	SELECTED_BOARD=$(echo $FORM_SELECT_BOARD | cut -d '|' -f 1)
	cat <<EOF > ${B}/board.mk
#==============================================================================
# Select a target board to compile. Available Boards:
#
#  - ciaa_sim_ia32        ---> Not supported yet!
#  - ciaa_sim_ia64        ---> Not supported yet!
#  - ciaa_nxp           
#  - edu_ciaa_nxp       
#  - ciaa_fsl             ---> Not supported yet!
#  - ciaa_pic             ---> Not supported yet!
#  - pico_ciaa            ---> Not supported yet!
#  - ciaa_leon3_fpga_nfp  ---> Not supported yet!
#  - ciaa_z3r0            ---> Not supported yet!
#  - ciaa_7st             ---> Not supported yet!
#
# Remember to clean program if change the board!
#==============================================================================

BOARD = ${SELECTED_BOARD}
EOF
	echo "Selected target board to compile:"
 	echo " - $SELECTED_BOARD"
	echo ""
	echo "File changed sucessfull:"
	echo "${B}/board.mk"
	;;
	1)
		echo "Aborted by user."
		exit 0;;
	-1)
		echo "Unexpected error.";;
esac

# See: https://bioinf.comav.upv.es/courses/unix/scripts_bash.html
