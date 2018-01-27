#!/bin/bash
 
# check we've some input
INFILE=$1
[ -f $INFILE ] || { echo oops: no input file?; exit 1; }

# the name of the target file on the ESP
TARGET=`basename $INFILE`

# init temp file
T=/tmp/$$.txt
>$T

# tell the world
echo "writing temp file ($T) to overwrite $TARGET on the ESP..."

# lua to write the file to disk on the ESP
echo "file.open(\"${TARGET}\", \"w\")"          >>$T
echo "file.write([["                            >>$T
cat $INFILE                                     >>$T
echo "]])"                                      >>$T
echo "file.close()"                             >>$T

# copy to clipboard
xclip -selection clipboard $T

# tell the world
echo "...done, and in the clipboard:"
echo
head -5 $T
echo;echo ...;echo
tail -5 $T
echo
