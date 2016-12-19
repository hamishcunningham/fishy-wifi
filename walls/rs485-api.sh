#!/bin/bash

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-n(no xyz)] [-i [1|2|3|...]"
DBG=:
OPTIONSTRING=hdni:

# specific locals
ABC=0
USEXYZ="1"

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    n)	USEXYZ="" ;;
    i)	ABC="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

# TODO
doit() {
  while :
  do
    echo turn it on...
    echo -e '\x55\xaa\x0B\x24\xFE\x00\xFF\x03\xFF\x01\x00\x2F\x77' > /dev/ttyUSB0
    sleep 2
    echo turn it off
    echo -e '\x55\xaa\x0B\x24\xFE\x00\x00\x00\xFF\x01\x00\x2D\x77' > /dev/ttyUSB0
    sleep 2
  done
}

# doit
#echo doing "cat </dev/ttyUSB0"; cat </dev/ttyUSB0
echo sending request
echo "echo -e '\x55\xaa\x05\x10\xFE\x13\x77' >/dev/ttyUSB0"
echo -e '\x55\xaa\x05\x10\xFE\x13\x77' >/dev/ttyUSB0
#sleep 1
#echo doing "cat </dev/ttyUSB0"; cat </dev/ttyUSB0
#echo doing tail -f
#tail -f /dev/ttyUSB0
