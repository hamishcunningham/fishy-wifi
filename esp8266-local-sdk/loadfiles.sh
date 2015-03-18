#!/bin/bash


# load source files onto chip

cd ../wegrow
for f in `ls *.lua`
do
	echo "Loading $f"
	if [ -n "$1" ] 
	then
		PORT=$1
	else
		PORT=/dev/ttyUSB0
	fi	
	../esp8266-local-sdk/luatool/luatool/luatool.py -p $PORT -f $f -t $f
done

