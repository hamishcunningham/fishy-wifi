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
  echo 'echo -e '\x55\xAA\x05\x10\xFE\x13\x77' >/dev/ttyUSB0'
  echo -e '\x55\xAA\x05\x10\xFE\x13\x77' >/dev/ttyUSB0
  #sleep 1
  #echo doing "cat </dev/ttyUSB0"; cat </dev/ttyUSB0
  #echo doing tail -f
  #tail -f /dev/ttyUSB0
}

init() {
  echo init
  stty -F /dev/ttyUSB0 sane
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


#The -F switch says to configure the /dev/ttyUSB0 port (I am using a cheap USB RS485 transceiver for testing)
#
#9600 is the baud, cs8 is 8 bits of data, -cstopb is 1 stop bit, -parenb is no parity.
#
#raw tells linux not to process the input per line, and -echo stops it echoing.
#
#Then I can use the command:
#
#od -t x1 -N13 < /dev/ttyUSB0
#
#the N13 says to stop after 13 bytes.
#
#If I run the od command in one terminal and in another:
#
#echo -e "\x55\xAA\x05\x0F\xFE\x14\x77" > /dev/ttyUSB0
