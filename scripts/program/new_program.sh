#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Martin Ribelotta and Eric Pernia
# All rights reserved.

SB=$(dirname $(readlink -f $0))
# Script folder 

B=$(readlink -f ${SB}/../../)
# Repository root folder

#------------------------------------------------------------------------------
# User preferences
#------------------------------------------------------------------------------

echo ""

PREFERENCES_FILE="preferences.sh"
PREFERENCES_PATH=$B/scripts/user/$PREFERENCES_FILE

LICENSE_PATH=$B/scripts/license
GITHUB_LICENSES_PATH=$LICENSE_PATH/licenses

if [ -f $PREFERENCES_PATH ]
then
	. $PREFERENCES_PATH
else
	echo "$PREFERENCES_FILE script not found."
fi

#------------------------------------------------------------------------------
# New program
#------------------------------------------------------------------------------

echo "New program wizard starting..."
echo ""

TEMPLATES_LIST_IDX=0
TEMPLATES_PATH=$B/scripts/program/templates
TEMPLATES_FILE_NAMES=$(ls $TEMPLATES_PATH)
for eachfile in $TEMPLATES_FILE_NAMES
do
	if [ $TEMPLATES_LIST_IDX -ne 0 ]
	then
		TEMPLATES_LIST="$TEMPLATES_LIST|$(echo $(echo $eachfile | cut -d '.' -f 1))"
	else
		TEMPLATES_LIST="$(echo $(echo $eachfile | cut -d '.' -f 1))"
	fi
	TEMPLATES_LIST_IDX=`expr $TEMPLATES_LIST_IDX + 1`
done

FORM_NEW_PROGRAM=$(zenity --forms --title="New program" \
	--text="Create a new program in \"$PROGRAMS_FOLDER\" folder" \
	--separator="|" \
	--add-entry="Name" \
	--add-entry="Version" \
	--add-list="Template" --list-values=$TEMPLATES_LIST )

case $? in
	0)
		PROGRAM_NAME=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 1)
		PROGRAM_VERSION=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 2)
		PROGRAM_TEMPLATE=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 3)

		if [ "$PROGRAMS_FOLDER" != "" ]
		then
			PB=${B}/$PROGRAMS_FOLDER/$PROGRAM_NAME
		else
			PB=${B}/$PROGRAM_NAME
		fi		
      mkdir -p ${PB}

		if [ "$LICENSE" != "none" ]
		then
			if [ -f "$LICENSE_PATH/license_file.sh" ]
			then
				. "$LICENSE_PATH/license_file.sh"
			else
				echo "Error, license_file.sh not found."
			fi
		fi

		if [ -f "$TEMPLATES_PATH/$PROGRAM_TEMPLATE.sh" ]
		then
			. "$TEMPLATES_PATH/$PROGRAM_TEMPLATE.sh"

			if [ "$PROGRAMS_FOLDER" != "" ]
			then
				echo "Program created on folder: $PROGRAMS_FOLDER"
			else
				echo "Program created on folder: $B"
			fi         
			echo " - Name: $PROGRAM_NAME"
			echo " - Version: $PROGRAM_VERSION"
			echo " - Template: $PROGRAM_TEMPLATE"
		else
			echo "Error, template not found. Please relaunch and select a template."
		fi

      # Append to program.mk file
		#echo "" >> ${B}/program.mk
		#echo "PROGRAM_PATH = ${PROGRAMS_FOLDER}" >> ${B}/program.mk
		#echo "PROGRAM_NAME = ${PROGRAM_NAME}" >> ${B}/program.mk

cat <<EOF > ${B}/program.mk
#==============================================================================
# This file select a program to compile with make
#==============================================================================

# ---------- Example of path inside this file folder -------------------
# Program path
#(relative to this folder, leave void if the program is in this folder)
#PROGRAM_PATH = examples_c/sapi/gpio
# Program name
#PROGRAM_NAME = blinky

# ---------- Example of path outside this file folder ------------------
#PROGRAM_PATH = ../examples
#PROGRAM_NAME = blinky

# ---------- Your paths: -----------------------------------------------

PROGRAM_PATH = ${PROGRAMS_FOLDER}
PROGRAM_NAME = ${PROGRAM_NAME}
EOF
#mv -b ${B}/program.mk.tmp ${B}/program.mk

		echo "Selected as program to compile in program.mk"
		;;
	1)
		echo "Aborted by user."
		exit 0;;
	-1)
		echo "Unexpected error.";;
esac
