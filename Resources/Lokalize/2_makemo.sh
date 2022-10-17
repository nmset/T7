#!/bin/bash
if [ ! -x 2_makemo.sh ]
then
    echo "Wrong working directory. Please cd to the directory containing this script."
    exit 0
fi

APPNAME=T7
DOMAIN=fr

msgfmt $DOMAIN/$APPNAME.po -o $DOMAIN/$APPNAME.mo

# cp $DOMAIN/$APPNAME.mo /usr/local/share/locale/$DOMAIN/LC_MESSAGES/

exit 0

