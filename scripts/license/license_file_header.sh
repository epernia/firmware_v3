#!/bin/sh
TODAY=$(date +'%Y/%m/%d')
YEAR="$(echo $TODAY | cut -d '/' -f 1)"

#$LICENSE_PATH

#$PROGRAMS_FOLDER
#$AUTHOR_FULL_NAME
#$AUTHOR_EMAIL
#$LICENSE

#$PROGRAM_NAME
#$PROGRAM_VERSION
#$PROGRAM_TEMPLATE

if [ "$AUTHOR_FULL_NAME" != "" ]
then
	AUTHOR_LINE=$AUTHOR_FULL_NAME
	if [ "$AUTHOR_EMAIL" != "" ]
	then
		AUTHOR_LINE="$AUTHOR_FULL_NAME <$AUTHOR_EMAIL>"
	fi
fi

FILE_HEADER="/*============================================================================="

if [ "$AUTHOR_LINE" != "" ]
then
if [ "$LICENSE" != "none" ]
then
FILE_HEADER="$FILE_HEADER
 * Copyright (c) $YEAR, $AUTHOR_LINE
 * All rights reserved.
 * License: $LICENSE (see LICENSE.txt)"
else
FILE_HEADER="$FILE_HEADER
 * Author: $AUTHOR_LINE"
fi
else
FILE_HEADER="$FILE_HEADER
 * Program: $PROGRAM_NAME"
fi

FILE_HEADER="$FILE_HEADER
 * Date: $TODAY"

if [ "$PROGRAM_VERSION" != "" ]
then
FILE_HEADER="$FILE_HEADER
 * Version: $PROGRAM_VERSION"
fi

FILE_HEADER="$FILE_HEADER
 *===========================================================================*/"


# Si no eligieron autor
#  
# /*=============================================================================
#  * Program: [programname]
#  * Date: [date]
#  * Version: [version]
#  *===========================================================================*/
#
# Si eligieron licencia:
# 
# /*=============================================================================
#  * Copyright (c) [year], [authorfullname] <[authormail]>
#  * All rights reserved.
#  * License: [license]
#  * Date: [date]
#  * Version: [version]
#  *===========================================================================*/
#  
# Si no eligieron (None)
#  
# /*=============================================================================
#  * Author: [authorfullname] <[authormail]>
#  * Date: [date]
#  * Version: [version]
#  *===========================================================================*/
