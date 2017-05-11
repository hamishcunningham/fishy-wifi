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
If base is not specified, relay numbers may be >64, else they must be <64\n
\n
Factory defaults:\n
MA0=0x55, MA1=0xAA, MAE=0x77, SL0=0x56, SL1=0xAB, SLE=0x78, FC=9600, CN=0xFE\n
\n
Command format:\n
MA0 MA1 BC CC Data… Data CS MAE\n
\n
Each controller must be programmed with a CN in ascending order from 1,\n
and each relay must be programmed with a digital output number in ascending\n
order from 1. The API then allows addressing of the relays in groups of 64\n
spread across the 4 bases (0-3) -- i.e. the 1st relay on controller 6 is\n
base 1 / output 70, but via the API is addressed as base 1 relay 7, turned\n
on by:\n
rs485-api.sh -B 1 -c on 7\n
\n
Manual:\n
http://smarthardware.eu/manual/str2do14din_doc.pdf\n
\n
The read_analog_sensor command reads sensor data from WaterElves, e.g.:\n
rs485-api.sh -c read_analog_sensor 192.168.22.73\n
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
LOG_STRING=rs485
DBG_LOG=/tmp/rs485-dbg.txt

### message & exit if exit num present ######################################
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

### logging #################################################################
log() { logger "${LOG_STRING}: $*"; }
log $0 $*

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
  ( echo; echo; date; echo; ) >${DBG_LOG}
  stty -F ${PORT} sane
  stty -F ${PORT} 9600 cs8 -cstopb -parenb raw -echo
}
read_status() {
  run_command -b ${BC06} 11 ${CN} 00 &
  timeout 0.5 bash -c "hd -n 15 <${PORT} |cut -c 11- |cut -d '|' -f 1"
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
    else
      echo -e "oops! relay bigger than 64: ${RED}$*${NC}" >&2
    fi
  done
  $DBG -e "${BLUE}$*${NC}" >&2
  $DBG -e "${BLUE}$BITS1 - $BITS2 - $BITS3 - $BITS4 - \
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
  $DBG -e "${BLUE}SUM = $SUM${NC}" >&2
  S=`bash -c "printf '%X\n' ${SUM}"`
  $DBG -e "${BLUE}S = $S${NC}" >&2
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
  echo -e "${BLUE}`echo ${C} |sed 's,\\\x, ,g'`${NC}" >&2
  echo $C
}
run_command() {
  echo "run_command $*" >>${DBG_LOG}
  C="`form_command $*`"
  $DBG -ne "$BLUE" >&2; $DBG -n "echo -ne ${C} > ${PORT}" >&2; $DBG -e "$NC"
  echo "echo -ne \"${C}\" > ${PORT}" >>${DBG_LOG}
  echo -ne "${C}" > ${PORT}
}
on() {
  # if BASE unset assume that we're running on 1-196 numbering
  if [ "x${BASE}" = "x00" ]
  then
    B0SOLS=
    B1SOLS=
    B2SOLS=
    B3SOLS=

    for r in `sort <<< $*`
    do
      if [ $r -le 64 ]
      then
        B0SOLS="$B0SOLS $r"
      elif [ $r -le 128 ]
      then
        r=$((r - 64))
        B1SOLS="$B1SOLS $r"
      elif [ $r -le 192 ]
      then
        r=$((r - 128))
        B2SOLS="$B2SOLS $r"
      else
        r=$((r - 192))
        B3SOLS="$B3SOLS $r"
      fi
    done
    
    BASE=0
    for LIST in "$B0SOLS" "$B1SOLS" "$B2SOLS" "$B3SOLS"
    do
      run_command 10 ${BASE} `ris2hex $LIST`
      BASE=$(( $BASE + 1 ))
    done
  # BASE is set, so assume that numbering is 1-64 (and allow "all")
  else
    if [ x$1 = xall ]
    then
      run_command 10 ${BASE} `ris2hex \`seq 1 $2\``
    else
      run_command 10 ${BASE} `ris2hex $*`
    fi
  fi
}
clear() {
  run_command 10 0 00 00 00 00 00 00 00 00
  run_command 10 1 00 00 00 00 00 00 00 00
  run_command 10 2 00 00 00 00 00 00 00 00
  run_command 10 3 00 00 00 00 00 00 00 00
}
hpr() { # print hex number in decimal and binary
  echo -ne "${BLUE} \
    `printf 'hex %X in decimal is %d and in base 2 is ' 0x$1 0x$1`"
  echo -ne "${BLUE} \
    `bc <<< \"ibase=16; obase=2; \`echo $1 |tr '[a-z]' '[A-Z]'\`\"`"
  echo -e "${NC}"
}
pulse() { 
  AREA=$1
  [ -f "$AREA" ] || \
    { echo -e "${RED}oops: ${AREA} doesn't exist :(${NC}"; return; }
  log "running pulse watering from file at ${AREA}..."
  echo -e "${BLUE}running pulse watering from file at ${AREA}...${NC}"

  while read SOL_SET
  do
    echo -e "${GREEN}pulsing ${SOL_SET}...${NC}"
    for i in `seq 1 5`
    do
      echo "  "on ${SOL_SET}...
      log "  "on ${SOL_SET}...
      BASE="00" on $SOL_SET && sleep 1
      echo "  "off...
      log "  "off...
      clear; sleep 1; clear
      sleep 9
      echo
    done
  done < ${AREA}
}
read_analog_sensor() {
  wget -O - "http://$1/data" 2>/dev/null |grep analog |head -1 |cut -f 2 |xargs
}
run_test_routine() {
  # TODO check pressure and current usage here
  # e.g.:
  # PRESSURE=`read_analog_sensor <ip address of pressure sensor elf>`
  # CURRENT=`read_analog_sensor <ip address of mains current sensor elf>`
  # ...
  for s in `cat $( dirname $P )/areas/all-planted`
  do
    echo cycling ${s}...
    BASE="00" on $s
    # TODO check pressure and current usage here; calculate if there's flow
    sleep 2
    clear
    # TODO check pressure and current usage here; calculate if there's flow
    sleep 2
  done
  clear
}

### CLI access to procedures ################################################
log            "running $COMM $*"
echo -e "${BLUE}running $COMM $*${NC}" >&2
$COMM $*
