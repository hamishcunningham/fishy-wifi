#!/bin/bash

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-c command]\n
\n
Factory defaults:\n
MA0=0x55, MA1=0xAA, MAE=0x77, SL0=0x56, SL1=0xAB, SLE=0x78, FC=9600, CN=0xFE\n
\n
Command format:\n
MA0 MA1 BC CC Data… Data CS MAE\n
\n
Manual:\n
http://smarthardware.eu/manual/str2do14din_doc.pdf
"
DBG=:
OPTIONSTRING=hdnc:

# specific locals
COMM=":"
PORT='/dev/ttyUSB0'
BC13=0D
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
    c)	COMM="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

calculate-check-sum() {
  SUM="2 "
  for h in $*
  do
    SUM="${SUM} + 0x${h}"
  done
  SUM="\$((${SUM}))"
  bash -c "printf '%x\n' ${SUM}"
}

form-command() {
  C="\x55\xAA\x${BC13}"
  for h in $*
  do
    C="${C}\x${h}"
  done
  CHECKSUM=`calculate-check-sum ${BC13} $*`
  C="${C}\x${CHECKSUM}\x77"
  echo $C
}

run-command() {
  $DBG "echo -e "`form-command 10 00 07 00 00 00 00 00 00 00`" > ${PORT}"
  echo -e "`form-command 10 00 07 00 00 00 00 00 00 00`" > ${PORT}
}

test-relay-on() {
  run-command 10 00 07 00 00 00 00 00 00 00
  sleep 2
  run-command 10 00 00 00 00 00 00 00 00 00
}

doit() {
  echo turn it on...
  echo -e "\x55\xAA\x0D\x10\x00\x01\x00\x00\x00\x00\x00\x00\x00\x20\x77" \
    > ${PORT} 
  sleep 2
  echo turn it off
  echo -e "\x55\xAA\x0D\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1F\x77" \
    > ${PORT}
  sleep 2
}

doit2() {
  SUM=$((2 + 0x0D + 0x10 + 0x00 + 0x01 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00))
  printf "SUM: %d; checksum: 0x%X\n" $SUM $SUM
  SUM=$((2 + 0x0D + 0x10 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00 + 0x00))
  printf "SUM: %d; checksum: 0x%X\n" $SUM $SUM

  SUM=0
  for h in 0D 10 00 01 00 00 00 00 00 00 00
  do
    SUM="$((0x${SUM} + 0x${h}))"
  done
  #SUM="$((0x${SUM} + 2))"
  printf "SUM: %d; checksum: 0x%X\n" $SUM $SUM
}

init() {
  echo init
  stty -F ${PORT} sane
  stty -F ${PORT} 9600 cs8 -cstopb -parenb raw -echo
}

tellme() {
  echo tellme
  od -t x1 -N13 < ${PORT} &2>od-out.txt &
  sleep 1
  echo -e '\x55\xAA\x05\x0F\xFE\x14\x77' >${PORT}
  sleep 1
  cat od-out.txt
}

#set -x
# init; doit; doit; tellme; doit
echo running $COMM $*
$COMM $*
