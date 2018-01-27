#!/bin/bash

rm -rf min
mkdir min
for f in $*
do
  echo $f
  grep -v DEBUG $f |sed -e 's,[   ]*--.*,,' | sed '/^[    ]*$/d' >min/$f
done
