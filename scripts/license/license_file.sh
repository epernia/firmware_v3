#!/bin/sh
TODAY=$(date +'%Y/%m/%d')
YEAR="$(echo $TODAY | cut -d '/' -f 1)"

#$LICENSE_PATH
#$GITHUB_LICENSES_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

GITHUB_LICENSE_FILE="$GITHUB_LICENSES_PATH/$LICENSE.txt"

GITHUB_LICENSE_FILE=$(tail -n +$(($(echo $(cat $GITHUB_LICENSE_FILE| grep -n -E -e '^---$'|cut -f 1 -d ':'|tail -1)+2))) $GITHUB_LICENSE_FILE)

cat <<EOF > ${PB}/LICENSE.txt.tmp
$GITHUB_LICENSE_FILE
EOF

if [ "$AUTHOR_FULL_NAME" != "" ]
then
	AUTHOR_LINE=$AUTHOR_FULL_NAME
	if [ "$AUTHOR_EMAIL" != "" ]
	then
		AUTHOR_LINE="$AUTHOR_FULL_NAME <$AUTHOR_EMAIL>"
	fi
fi

cat ${PB}/LICENSE.txt.tmp|sed "s/\[year\]/$YEAR/g"|sed "s/\[fullname\]/$AUTHOR_LINE/g" > ${PB}/LICENSE.txt

rm -f ${PB}/LICENSE.txt.tmp
