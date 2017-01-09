#!/bin/bash
# CLI API for the STR2DO14DIN RS-485 controller

### standard locals #########################################################
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-c command]\n
\n
Commands: init, on, off, ...\n
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
OPTIONSTRING=hdc:

### specific locals ##########################################################
COMM=":"
PORT='/dev/ttyUSB0'
BC13='0D'

### message & exit if exit num present ######################################
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

### process options #########################################################
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    c)	COMM="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

### procedural interface ####################################################
init() {
  stty -F ${PORT} sane
  stty -F ${PORT} 9600 cs8 -cstopb -parenb raw -echo
}
readstatus() { # TODO what's going on with the temp file?!
  od -t x1 -N13 < ${PORT} &2>od-out.txt &
  sleep 1
  echo -e '\x55\xAA\x05\x0F\xFE\x14\x77' >${PORT}
  sleep 1
  cat od-out.txt
}
bfi2bin() { # bit field index to binary
  if [ $1 -eq 1 ]
  then
    echo 1
  else
    printf "1%0$((${1}-1))d " 0
  fi
}
ris2hex() { # convert relay index set to hex; counts from R1
  BITS1="" # first byte: relays up to the 8th
  BITS2="" # second byte: relays 9th to 14th
  for r in $*
  do
    if [ $r -le 8 ]
    then
      [ ! -z "$BITS1" ] && BITS1+=" | "
      BITS1+=2#`bfi2bin $r`
    else
      r=$((r - 8))
      [ ! -z "$BITS2" ] && BITS2+=" | "
      BITS2+=2#`bfi2bin $r`
    fi
  done
  $DBG echo $* >&2
  $DBG echo $BITS1 $BITS2 >&2
  BIN1=$(( $BITS1 ))
  BIN2=$(( $BITS2 ))
  $DBG printf '%02X %02X\n' $BIN1 $BIN2 >&2
  printf '%02X %02X\n' $BIN1 $BIN2
}
calculate-check-sum() {
  SUM="2 "
  for h in $*
  do
    SUM="${SUM} + 0x${h}"
  done
  SUM="\$((${SUM}))"
  $DBG SUM = $SUM >&2
  S=`bash -c "printf '%X\n' ${SUM}"`
  $DBG S = $S >&2
  echo ${S: -2}
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
  $DBG "echo -e "`form-command $*`" > ${PORT}"
  echo -e "`form-command $*`" > ${PORT}
}
on() {
  run-command 10 00 `ris2hex $*` 00 00 00 00 00 00
}
off() {
  run-command 10 00 00 00 00 00 00 00 00 00
}

### CLI access to procedures ################################################
echo running $COMM $*
$COMM $*

### test code and docs ######################################################
#
# relay on command examples:
# R6 / 7th;  \x55\xAA\x0D\x10\x00\x40\x00\x00\x00\x00\x00\x00\x00\x5F\x77
# R7 / 8th;  \x55\xAA\x0D\x10\x00\x80\x00\x00\x00\x00\x00\x00\x00\x9F\x77
# R8 / 9th;  \x55\xAA\x0D\x10\x00\x00\x01\x00\x00\x00\x00\x00\x00\x20\x77
# R9 / 10th; \x55\xAA\x0D\x10\x00\x00\x02\x00\x00\x00\x00\x00\x00\x21\x77
#
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
