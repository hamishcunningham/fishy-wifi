#!/bin/bash
# CLI API for the STR2DO14DIN RS-485 controller

### standard locals #########################################################
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-B base] [-c command]\n
\n
A CLI API for the STR2DO14DIN RS-485 controller.\n
Commands: init, on, off, ...\n
\n
Base (00,01,02 or 03) is for use with command 10, as exposed in e.g. 'on'\n
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
OPTIONSTRING=hdc:B:

### specific locals ##########################################################
MA0='55'
MA1='AA'
MAE='77'
BC13='0D'
COMM=":"
PORT='/dev/ttyUSB0'
BASE=00

### message & exit if exit num present ######################################
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

### process options #########################################################
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    c)	COMM="${OPTARG}" ;;
    B)	BASE="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

### procedural interface ####################################################
init() {
  stty -F ${PORT} sane
  stty -F ${PORT} 9600 cs8 -cstopb -parenb raw -echo
}
read_status() { # TODO what's going on with the temp file?!
# TODO interpret results
# $DBG "od -t x1 -N15 < ${PORT} &2>od-out.txt &"
# od -t x1 -N13 < ${PORT} &2>od-out.txt &
# sleep 1
  #$DBG "echo -e "\x${MA0}\x${MA1}\x06\x11\x01\x00\x1A\x${MAE}" >${PORT}"
  #echo -e "\x${MA0}\x${MA1}\x06\x11\x01\x00\x1A\x${MAE}" >${PORT}

#<0x11> Read Digital Outputs ...
#-> 55 AA 06 11 01 00 1A 77
#<- 56 AB 0D 01 00 00 00 00 00 00 00 00 00 10 78
#<0x11> Read Digital Outputs ...
#-> 55 AA 06 11 02 00 1B 77
#<- 56 AB 0D 02 00 00 00 00 00 00 00 00 00 11 78

echo read from controller 1, then 2
set -x
init
sleep 1
od -t x1 -N14 </dev/ttyUSB0 &
echo -e '\x55\xAA\x06\x11\x01\x00\x1A\x77' >/dev/ttyUSB0
sleep 1
init
sleep 1
od -t x1 -N14 </dev/ttyUSB0 &
echo -e '\x55\xAA\x06\x11\x02\x00\x1B\x77' >/dev/ttyUSB0 
sleep 1
return

  # get status of outputs
  od -t x1 -N1 <${PORT}
  od -t x1 -N1 < ${PORT} &2>od-out.txt &
  #echo -e "\x55\xAA\x06\x11\x01\x00\x1A\x77" >${PORT}
  sleep 1

  # get type etc. from controller 01
# CN=01
# od -t x1 -N13 < ${PORT} &2>od-out.txt &
# sleep 1
# echo -e "\x${MA0}\x${MA1}\x05\x0F\x${CN}\x17\x${MAE}" >${PORT}
# sleep 1
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
  BITS1=""  # first byte:   relays up to the 8th
  BITS2=""  # second byte:  relays 9th to 16th
  BITS3=""  # third byte:   17th to 24th
  BITS4=""  # fourth byte:  25th to 32nd
  BITS5=""  # fifth byte:   33rd to 40th
  BITS6=""  # sixth byte:   41st to 48th
  BITS7=""  # seventh byte: 49th to 56th
  BITS8=""  # eighth byte:  57th to 64th

  for r in `sort <<< $*`
  do
    if [ $r -le 8 ]
    then
      [ ! -z "$BITS1" ] && BITS1+=" | "
      BITS1+=2#`bfi2bin $r`
    elif [ $r -le 16 ]
    then
      r=$((r - 8))
      [ ! -z "$BITS2" ] && BITS2+=" | "
      BITS2+=2#`bfi2bin $r`
    elif [ $r -le 24 ]
    then
      r=$((r - 16))
      [ ! -z "$BITS3" ] && BITS3+=" | "
      BITS3+=2#`bfi2bin $r`
    elif [ $r -le 32 ]
    then
      r=$((r - 24))
      [ ! -z "$BITS4" ] && BITS4+=" | "
      BITS4+=2#`bfi2bin $r`
    elif [ $r -le 40 ]
    then
      r=$((r - 32))
      [ ! -z "$BITS5" ] && BITS5+=" | "
      BITS5+=2#`bfi2bin $r`
    elif [ $r -le 48 ]
    then
      r=$((r - 40))
      [ ! -z "$BITS6" ] && BITS6+=" | "
      BITS6+=2#`bfi2bin $r`
    elif [ $r -le 56 ]
    then
      r=$((r - 48))
      [ ! -z "$BITS7" ] && BITS7+=" | "
      BITS7+=2#`bfi2bin $r`
    elif [ $r -le 64 ]
    then
      r=$((r - 56))
      [ ! -z "$BITS8" ] && BITS8+=" | "
      BITS8+=2#`bfi2bin $r`
    fi
  done
  $DBG echo $* >&2
  $DBG echo $BITS1 - $BITS2 - $BITS3 - $BITS4 - \
    $BITS5 - $BITS6 - $BITS7 - $BITS8 >&2
  BIN1=$(( $BITS1 )); BIN2=$(( $BITS2 ))
  BIN3=$(( $BITS3 )); BIN4=$(( $BITS4 ))
  BIN5=$(( $BITS5 )); BIN6=$(( $BITS6 ))
  BIN7=$(( $BITS7 )); BIN8=$(( $BITS8 ))
  $DBG printf '%02X %02X %02X %02X %02X %02X %02X %02X\n' \
    $BIN1 $BIN2 $BIN3 $BIN4 $BIN5 $BIN6 $BIN7 $BIN8 >&2
  printf '%02X %02X %02X %02X %02X %02X %02X %02X\n' \
    $BIN1 $BIN2 $BIN3 $BIN4 $BIN5 $BIN6 $BIN7 $BIN8
}
calculate_check_sum() {
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
form_command() {
  C="\x${MA0}\x${MA1}\x${BC13}"
  for h in $*
  do
    C="${C}\x${h}"
  done
  CHECKSUM=`calculate_check_sum ${BC13} $*`
  C="${C}\x${CHECKSUM}\x${MAE}"
  echo $C
}
run_command() {
  $DBG "echo -e "`form_command $*`" > ${PORT}" >&2
  echo -e "`form_command $*`" > ${PORT}
}
on() {
  run_command 10 ${BASE} `ris2hex $*`
}
off() {
  run_command 10 ${BASE} 00 00 00 00 00 00 00 00
}
hpr() { # print hex number in decimal and binary
  printf 'hex %X in decimal is %d and in base 2 is ' 0x$1 0x$1
  bc <<< "ibase=16; obase=2; $1"
}

### CLI access to procedures ################################################
echo running $COMM $* >&2
$COMM $*

### test code and docs ######################################################
#
# relay on command examples:
# R6 / 7th;  \x55\xAA\x0D\x10\x00\x40\x00\x00\x00\x00\x00\x00\x00\x5F\x77
# R7 / 8th;  \x55\xAA\x0D\x10\x00\x80\x00\x00\x00\x00\x00\x00\x00\x9F\x77
# R8 / 9th;  \x55\xAA\x0D\x10\x00\x00\x01\x00\x00\x00\x00\x00\x00\x20\x77
# R9 / 10th; \x55\xAA\x0D\x10\x00\x00\x02\x00\x00\x00\x00\x00\x00\x21\x77
#
test_relay_on() {
  run_command 10 00 07 00 00 00 00 00 00 00
  sleep 2
  run_command 10 00 00 00 00 00 00 00 00 00
}
doit() {
  echo turn it on... >&2
  echo -e "\x55\xAA\x0D\x10\x00\x01\x00\x00\x00\x00\x00\x00\x00\x20\x77" \
    > ${PORT} 
  sleep 2
  echo turn it off >&2
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
