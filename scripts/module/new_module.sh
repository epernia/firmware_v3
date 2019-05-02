#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia
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

echo "New module wizard starting..."
echo ""

TEMPLATES_LIST_IDX=0
TEMPLATES_PATH=$B/scripts/module/templates
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

FORM_NEW_PROGRAM=$(zenity --forms --title="New module" \
	--text="Create a new module\
	--separator="|" \
	--add-entry="Name" \
	--add-entry="Version" \
	--add-list="Template" --list-values=$TEMPLATES_LIST )

case $? in
	0)
		MODULE_NAME=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 1)
		MODULE_VERSION=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 2)
		MODULE_TEMPLATE=$(echo $FORM_NEW_PROGRAM | cut -d '|' -f 3)

		if [ "$PROGRAMS_FOLDER" != "" ]
		then
			PB=${B}/$PROGRAMS_FOLDER/$MODULE_NAME
		else
			PB=${B}/$MODULE_NAME
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

		if [ -f "$TEMPLATES_PATH/$MODULE_TEMPLATE.sh" ]
		then
			. "$TEMPLATES_PATH/$MODULE_TEMPLATE.sh"

			if [ "$PROGRAMS_FOLDER" != "" ]
			then
				echo "Module created on folder: $PROGRAMS_FOLDER"
			else
				echo "Module created on folder: $B"
			fi         
			echo " - Name: $MODULE_NAME"
			echo " - Version: $MODULE_VERSION"
			echo " - Template: $MODULE_TEMPLATE"
		else
			echo "Error, template not found. Please relaunch and select a template."
		fi
		;;
	1)
		echo "Aborted by user."
		exit 0;;
	-1)
		echo "Unexpected error.";;
esac
