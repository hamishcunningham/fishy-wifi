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

doit() {
  echo turn it on...
  echo -e "\x55\xAA\x0D\x10\x00\x01\x00\x00\x00\x00\x00\x00\x00\x20\x77" > /dev/ttyUSB0 
  sleep 2
  echo turn it off
  echo -e "\x55\xAA\x0D\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1F\x77" > /dev/ttyUSB0
  sleep 2
}

doit2() {
  echo sending request
  echo -e '\x55\xAA\x05\x10\xFE\x13\x77' >/dev/ttyUSB0
  #sleep 1
  #echo doing "cat </dev/ttyUSB0"; cat </dev/ttyUSB0
  #echo doing tail -f
  #tail -f /dev/ttyUSB0
}

init() {
  echo init
  # stty -F /dev/ttyUSB0 sane
  stty -F /dev/ttyUSB0 9600 cs8 -cstopb -parenb raw -echo
}

tellme() {
  echo tellme
  od -t x1 -N13 < /dev/ttyUSB0 &2>od-out.txt &
  sleep 1
  echo -e '\x55\xAA\x05\x0F\xFE\x14\x77' >/dev/ttyUSB0
  sleep 1
  cat od-out.txt
}

#set -x
init
doit
doit
tellme
doit
