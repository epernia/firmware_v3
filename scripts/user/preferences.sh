#!/bin/sh

# BSD 3-Clause License
# Copyright (c) 2019, Eric Pernia and Martin Ribelotta
# All rights reserved.

USER_PREFERENCES_FILE="user_preferences.cfg"
USER_PREFERENCES_PATH=$B/scripts/user/$USER_PREFERENCES_FILE

if [ -f $USER_PREFERENCES_PATH ]
then
	echo "User preferences found in file: $USER_PREFERENCES_PATH"
	. $USER_PREFERENCES_PATH
else
	echo "User preferences not found."
	echo "Please complete your information. Starting window..."

#Github licenses, downloaded from:
#https://github.com/github/choosealicense.com/tree/gh-pages/_licenses
   LICENSES_LIST="none"
	GITHUB_LICENSES_FILE_NAMES=$(ls $GITHUB_LICENSES_PATH)
	for eachfile in $GITHUB_LICENSES_FILE_NAMES
	do
		LICENSES_LIST="$LICENSES_LIST|$(echo $(basename $eachfile .txt))"
	done

	FORM_USER_PREFERENCES=$(zenity --forms --title="Author" \
		--text="Configure author for your programs" \
		--separator="|" \
		--add-entry="Programs folder (no spaces!)" \
		--add-entry="Author full name" \
		--add-entry="Author e-mail" \
		--add-list="License" --list-values=$LICENSES_LIST )
      
	case $? in
		0)
			PROGRAMS_FOLDER=$(echo $FORM_USER_PREFERENCES | cut -d '|' -f 1)
			AUTHOR_FULL_NAME=$(echo $FORM_USER_PREFERENCES | cut -d '|' -f 2)
			AUTHOR_EMAIL=$(echo $FORM_USER_PREFERENCES | cut -d '|' -f 3)
			LICENSE=$(echo $FORM_USER_PREFERENCES | cut -d '|' -f 4)

			if [ "$PROGRAMS_FOLDER" == "" ]
			then
				echo "Programs will save in $SB"
			fi

			if [ "$LICENSE" == "" ]
			then
				LICENSE="none"
			fi

cat <<EOF > $USER_PREFERENCES_PATH
# Folder to save your programs
PROGRAMS_FOLDER="$PROGRAMS_FOLDER"

# Author information:
AUTHOR_FULL_NAME="$AUTHOR_FULL_NAME"
AUTHOR_EMAIL="$AUTHOR_EMAIL"

# License for your programs:
LICENSE="$LICENSE"
EOF
			;;
		1)
			echo "Aborted by user."
			exit 0;;
		-1)
			echo "Unexpected error.";;
	esac

fi

echo " - Programs folder: $PROGRAMS_FOLDER"
echo " - Author full name: $AUTHOR_FULL_NAME"
echo " - Author e-mail: $AUTHOR_EMAIL"
echo " - License: $LICENSE"
echo ""
