#!/bin/bash
# minify lua

LUA=lua5.2
MINIFYCOM=CommandLineMinify.lua

MINIFYDIR=$1
shift
FILES=$*

WD=`pwd`
cd $MINIFYDIR

for f in $FILES
do
  echo $LUA $MINIFYCOM $WD/$f
  $LUA $MINIFYCOM $WD/$f
done
