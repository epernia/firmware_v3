#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

#set -x

SB=$(dirname $(readlink -f $0))
# Script folder 

B=$(readlink -f ${SB}/../../)
# Repository root folder

echo ""

USER_PREFERENCES_FILE="user_preferences.cfg"
USER_PREFERENCES_PATH=$B/scripts/user/$USER_PREFERENCES_FILE

if [ -f $USER_PREFERENCES_PATH ]
then
	echo "User preferences found in file: $USER_PREFERENCES_PATH"
	. $USER_PREFERENCES_PATH
	PROGRAM_FOLDER_FOR_FORM="$B/$PROGRAMS_FOLDER/"
else
	echo "User preferences not found."
	PROGRAM_FOLDER_FOR_FORM="$B"
fi

PROG_LIST=$(find ${B} -name config.mk -exec dirname '{}' \; | sed "s|${B}/||")

echo ""
echo "Select program to compile wizard is starting..."
echo ""
#FORM_SELECT_PROGRAM=${B}/$(zenity --list --title="Select program" --column="Project path" $PROG_LIST)
FORM_SELECT_PROGRAM=$(zenity --file-selection --directory \
                        --title="Select a program folder" \
                        --filename="$B/$PROGRAMS_FOLDER/" )

RET=$?

FORM_SELECT_PROGRAM=$(readlink -f ${FORM_SELECT_PROGRAM})

case $RET in
	0)
		#if is a real program folder (includes a config.mk file)
		if [ -f $FORM_SELECT_PROGRAM/config.mk ]
		then
      
      #check if path contains repository folder
      REPO_FOLDER=$(basename $B)
      case "$FORM_SELECT_PROGRAM" in
          *"$REPO_FOLDER"*)
            PROGRAM="$(basename $FORM_SELECT_PROGRAM)"
            PROGRAM_PATH=$(dirname $FORM_SELECT_PROGRAM)
            PROGRAMS_FOLDER=${PROGRAM_PATH#${B}/}
            cat <<EOF > ${B}/program.mk
#==============================================================================
# This file select a program to compile with make
#==============================================================================

# ---------- Example of path -------------------------------------------
# Program path
#(relative to this folder, leave void if the program is in this folder)
#PROGRAM_PATH = examples_c/sapi/gpio
# Program name
#PROGRAM_NAME = blinky

# ---------- Your paths: -----------------------------------------------

PROGRAM_PATH = ${PROGRAMS_FOLDER}
PROGRAM_NAME = ${PROGRAM}
EOF
#mv -b ${B}/program.mk.tmp ${B}/program.mk

            echo "Selected program to compile:"
            echo " - $PROGRAM"
            echo " - Located at: $PROGRAMS_FOLDER"
            echo ""
            echo "File changed sucessfull:"
            echo "${B}/program.mk"
            ;;
          *)
            echo "Error: this script only work with paths inside repository folder."
            echo "If you want to use a path outside repository folder please change program.mk manualy."
            ;;
      esac

		else
			echo "Error: This is an invalid program folder. Tip: The folder must include a config.mk file"
			echo "Please re-run and select another one."
		fi
		;;
	1)
		echo "Aborted by user."
		exit 0;;
	-1)
		echo "Unexpected error.";;
esac

# See: https://bioinf.comav.upv.es/courses/unix/scripts_bash.html
