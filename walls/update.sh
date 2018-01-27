#!/bin/bash

cd
cd new
echo DIFFS NEW TO EXISTING:
for f in `find . -type f`
do
  echo $f
  diff $f ~/fishy-wifi/walls/$f
  echo
done |more
echo
echo 'INSTALL? (y/n)'
read ANS
if [ x$ANS == xy ]
then
for f in `find . -type f`
do
  echo cp $f ~/fishy-wifi/walls/$f
  cp $f ~/fishy-wifi/walls/$f
done
else
  echo not this time...
fi
