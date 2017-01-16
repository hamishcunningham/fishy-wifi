#!/bin/bash
# CLI API for the STR2DO14DIN RS-485 controller

### standard locals #########################################################
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-B base] [-C(controller num)] [-c command]\n
\n
A CLI API for the STR2DO14DIN RS-485 controller.\n
Commands: init, on, off, ...\n
\n
Controller (00,01,...) is for use with command 11, as exposed in e.g. 'read_status'\n
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
RED='\033[0;31m'   # red
GR='\033[0;32m'    # green
BLUE='\033[1;34m'  # blue
NC='\033[0m'       # no color
OPTIONSTRING=hdc:B:C:

### specific locals ##########################################################
MA0='55'
MA1='AA'
MAE='77'
BC06='06'
BC13='0D'
COMM=":"
PORT='/dev/ttyUSB0'
CN=00
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
    C)	CN="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

### procedural interface ####################################################
init() {
  stty -F ${PORT} sane
  stty -F ${PORT} 9600 cs8 -cstopb -parenb raw -echo
}
read_status() { # TODO interpret results
  run_command -b ${BC06} 11 ${CN} 00 &
  hd -n 15 <${PORT} |cut -c 11- |cut -d '|' -f 1
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
  $DBG -e "${RED}$*${NC}" >&2
  $DBG -e "${RED}$BITS1 - $BITS2 - $BITS3 - $BITS4 - \
    $BITS5 - $BITS6 - $BITS7 - $BITS8${NC}" >&2
  BIN1=$(( $BITS1 )); BIN2=$(( $BITS2 ))
  BIN3=$(( $BITS3 )); BIN4=$(( $BITS4 ))
  BIN5=$(( $BITS5 )); BIN6=$(( $BITS6 ))
  BIN7=$(( $BITS7 )); BIN8=$(( $BITS8 ))
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
  $DBG -e "${RED}SUM = $SUM${NC}" >&2
  S=`bash -c "printf '%X\n' ${SUM}"`
  $DBG -e "${RED}S = $S${NC}" >&2
  echo ${S: -2}
}
form_command() {
  BC=${BC13}
  [ x$1 = x-b ] && { BC=$2; shift; shift; }
  C="\x${MA0}\x${MA1}\x${BC}"
  for h in $*
  do
    C="${C}\x${h}"
  done
  CHECKSUM=`calculate_check_sum ${BC} $*`
  C="${C}\x${CHECKSUM}\x${MAE}"
  echo -e "${RED}`echo ${C} |sed 's,\\\x, ,g'`${NC}" >&2
  echo $C
}
run_command() {
  C="`form_command $*`"
  $DBG -ne "$RED" >&2; $DBG -n "echo -ne ${C} > ${PORT}" >&2; $DBG -e "$NC"
  echo -ne "${C}" > ${PORT}
}
on() {
  if [ x$1 = xall ]
  then
    run_command 10 ${BASE} `ris2hex \`seq 1 $2\``
  else
    run_command 10 ${BASE} `ris2hex $*`
  fi
}
off() {
  run_command 10 ${BASE} 00 00 00 00 00 00 00 00
}
hpr() { # print hex number in decimal and binary
  echo -ne "${BLUE} \
    `printf 'hex %X in decimal is %d and in base 2 is ' 0x$1 0x$1`"
  echo -ne "${BLUE} \
    `bc <<< \"ibase=16; obase=2; \`echo $1 |tr '[a-z]' '[A-Z]'\`\"`"
  echo -e "${NC}"
}

### CLI access to procedures ################################################
echo -e "${RED}running $COMM $*${NC}" >&2
$COMM $*

### test code and docs ######################################################
#
# relay on command examples:
# R6 / 7th;  \x55\xAA\x0D\x10\x00\x40\x00\x00\x00\x00\x00\x00\x00\x5F\x77
# R7 / 8th;  \x55\xAA\x0D\x10\x00\x80\x00\x00\x00\x00\x00\x00\x00\x9F\x77
# R8 / 9th;  \x55\xAA\x0D\x10\x00\x00\x01\x00\x00\x00\x00\x00\x00\x20\x77
# R9 / 10th; \x55\xAA\x0D\x10\x00\x00\x02\x00\x00\x00\x00\x00\x00\x21\x77
#
test_relay_on1() {
  run_command 10 00 07 00 00 00 00 00 00 00
  sleep 2
  run_command 10 00 00 00 00 00 00 00 00 00
}
test_relay_on2() {
  echo turn it on... >&2
  echo -ne "\x55\xAA\x0D\x10\x00\x01\x00\x00\x00\x00\x00\x00\x00\x20\x77" \
    > ${PORT} 
  sleep 2
  echo turn it off >&2
  echo -ne "\x55\xAA\x0D\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x1F\x77" \
    > ${PORT}
  sleep 2
}
test_relay_on3() {
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
