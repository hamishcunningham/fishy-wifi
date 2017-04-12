#!/bin/bash
# tui -- text mode (curses) UI for ponic walls

# standard locals
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-v(ersion)] [-l(og entries)]"
OPTIONSTRING=hdvl
alias cd='builtin cd'
DBG=:
RED='\033[0;31m'   # red
GR='\033[0;32m'    # green
BLUE='\033[1;34m'  # blue
NC='\033[0m'       # no color

# specific locals
INST_DIR=`dirname ${P}`
CLI=${INST_DIR}/rs485-api.sh
AREA_DIR=${INST_DIR}/areas
LOG_STRING=rs485
VERSION=0.6
NUM_CONTROLLERS=14
NUM_SOLENOIDS=196
ALL_SOLENOIDS=`seq 1 $NUM_SOLENOIDS`
DBG_LOG=/tmp/rs485-dbg.txt

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# write a log; get recent log entries
log() { logger "${LOG_STRING}: $*"; }
log_grep() { grep -i $LOG_STRING /var/log/syslog |tac; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    v)	echo $P is at version $VERSION; exit 0 ;;
    d)	DBG=echo; CLI=echo ;;
    l)  log_grep; exit 0 ;;
    *)	usage 3 ;;
  esac
done 
shift `expr $OPTIND - 1`

# does CLI work?
${CLI} -h >/dev/null || {
  COUT=`${CLI} -h 2>&1`
  echo "${P}: ${CLI} not working; installation problem? (${COUT})" >&2
  usage 4
}

# figure out optimal size for whiptail
calc_wt_size() {
  WT_HEIGHT=24
  WT_WIDTH=$(tput cols)

  if [ -z "$WT_WIDTH" ] || [ "$WT_WIDTH" -lt 60 ]; then
    WT_WIDTH=80
  fi
  if [ "$WT_WIDTH" -gt 178 ]; then
    WT_WIDTH=120
  fi
  WT_MENU_HEIGHT=$(($WT_HEIGHT-8))
}
calc_wt_size

# helper functions
cli_command() { $CLI $*; }

# control water supply solenoids
SOLENOIDS_A=(
#  n=desciption=========================================default
   1 "\" \""    off
   2 '" "'      off
   3 '" "'      off
   4 '" "'      off
   5 '" "'      off
   6 '" "'      off
   7 '" "'      off
   8 '" "'      off
   9 '" "'      off
  10 '" "'      off
  11 '" "'      off
  12 '" "'      off
  13 '" "'      off
  14 '" "'      off
  15 '" "'      off
  16 '" "'      off
  17 '" "'      off
  18 '" "'      off
  19 '" "'      off
  20 '" "'      off
  21 '" "'      off
  22 '" "'      off
  23 '" "'      off
  24 '" "'      off
  25 '" "'      off
  26 '" "'      off
  27 '" "'      off
  28 '" "'      off
  29 '" "'      off
  30 '" "'      off
  31 '" "'      off
  32 '" "'      off
  33 '" "'      off
  34 '" "'      off
  35 '" "'      off
  36 '" "'      off
  37 '" "'      off
  38 '" "'      off
  39 '" "'      off
  40 '" "'      off
  41 '" "'      off
  42 '" "'      off
  43 '" "'      off
  44 '" "'      off
  45 '" "'      off
  46 '" "'      off
  47 '" "'      off
  48 '" "'      off
  49 '" "'      off
  50 '" "'      off
  51 '" "'      off
  52 '" "'      off
  53 '" "'      off
  54 '" "'      off
  55 '" "'      off
  56 '" "'      off
  57 '" "'      off
  58 '" "'      off
  59 '" "'      off
  60 '" "'      off
  61 '" "'      off
  62 '" "'      off
  63 '" "'      off
  64 '" "'      off
  65 '" "'      off
  66 '" "'      off
  67 '" "'      off
  68 '" "'      off
  69 '" "'      off
  70 '" "'      off
  71 '" "'      off
  72 '" "'      off
  73 '" "'      off
  74 '" "'      off
  75 '" "'      off
  76 '" "'      off
  77 '" "'      off
  78 '" "'      off
  79 '" "'      off
  80 '" "'      off
  81 '" "'      off
  82 '" "'      off
  83 '" "'      off
  84 '" "'      off
  85 '" "'      off
  86 '" "'      off
  87 '" "'      off
  88 '" "'      off
  89 '" "'      off
  90 '" "'      off
  91 '" "'      off
  92 '" "'      off
  93 '" "'      off
  94 '" "'      off
  95 '" "'      off
  96 '" "'      off
  97 '" "'      off
  98 '" "'      off
  99 '" "'      off
 100 '" "'      off
 101 '" "'      off
 102 '" "'      off
 103 '" "'      off
 104 '" "'      off
 105 '" "'      off
 106 '" "'      off
 107 '" "'      off
 108 '" "'      off
 109 '" "'      off
 110 '" "'      off
 111 '" "'      off
 112 '" "'      off
 113 '" "'      off
 114 '" "'      off
 115 '" "'      off
 116 '" "'      off
 117 '" "'      off
 118 '" "'      off
 119 '" "'      off
 120 '" "'      off
 121 '" "'      off
 122 '" "'      off
 123 '" "'      off
 124 '" "'      off
 125 '" "'      off
 126 '" "'      off
 127 '" "'      off
 128 '" "'      off
 129 '" "'      off
 130 '" "'      off
 131 '" "'      off
 132 '" "'      off
 133 '" "'      off
 134 '" "'      off
 135 '" "'      off
 136 '" "'      off
 137 '" "'      off
 138 '" "'      off
 139 '" "'      off
 140 '" "'      off
 141 '" "'      off
 142 '" "'      off
 143 '" "'      off
 144 '" "'      off
 145 '" "'      off
 146 '" "'      off
 147 '" "'      off
 148 '" "'      off
 149 '" "'      off
 150 '" "'      off
 151 '" "'      off
 152 '" "'      off
 153 '" "'      off
 154 '" "'      off
 155 '" "'      off
 156 '" "'      off
 157 '" "'      off
 158 '" "'      off
 159 '" "'      off
 160 '" "'      off
 161 '" "'      off
 162 '" "'      off
 163 '" "'      off
 164 '" "'      off
 165 '" "'      off
 166 '" "'      off
 167 '" "'      off
 168 '" "'      off
 169 '" "'      off
 170 '" "'      off
 171 '" "'      off
 172 '" "'      off
 173 '" "'      off
 174 '" "'      off
 175 '" "'      off
 176 '" "'      off
 177 '" "'      off
 178 '" "'      off
 179 '" "'      off
 180 '" "'      off
 181 '" "'      off
 182 '" "'      off
 183 '" "'      off
 184 '" "'      off
 185 '" "'      off
 186 '" "'      off
 187 '" "'      off
 188 '" "'      off
 189 '" "'      off
 190 '" "'      off
 191 '" "'      off
 192 '" "'      off
 193 '" "'      off
 194 '" "'      off
 195 '" "'      off
 196 '" "'      off
)
get_solenoid() { echo ${SOLENOIDS_A[$(( ($1 * 3) - 1 ))]}; } # get $1 state
set_solenoid() { SOLENOIDS_A[$(( ($1 * 3) - 1 ))]=$2; } # set $1 to state $2
clear_solenoid_state() {
  for i in `seq 1 $(( ${#SOLENOIDS_A[@]} / 3 ))`; do set_solenoid $i off; done
}
print_solenoid_state() {
  slen=$(( ${#SOLENOIDS_A[@]} / 3 ))
  for i in `seq 1 ${slen}`
  do
    printf "%2d: " ${i}
    get_solenoid $i
  done
}
read_board() { # grungey late-night code: enter at your peril!
  clear_solenoid_state
  for CN in `seq 1 $NUM_CONTROLLERS`
  do
    STAT=`cli_command -C $CN -c read_status`
    if [ $? != 0 -o "x${STAT}" = x ]
    then
      echo -e "${RED}failure on read_status for ${CN} $*${NC}" >&2
      echo -e "${BLUE}hit return to continue or Cntrl&C for exit${NC}" >&2
      read
      continue
    fi

    set $STAT
    shift 5
    echo -e "${GR}solenoid data from read_status: $*${NC}" >&2
    HEX_BITS=""
    # cycle over all the data bytes returned, which are hex pairs
    for h in $1 $2 $3 $4 $5 $6 $7 $8
    do
      # each pair specifies lower number solenoids first, then higher, so we
      # need to reverse each pair in the composite hex string we build up
      HEX_BITS="$HEX_BITS`rev <<< $h |tr '[a-z]' '[A-Z]'`"
    done
    # the composite hex is reversed to put the lower-number bits last
    HEX_BITS=`rev <<< $HEX_BITS`
    echo -e "${GR}HEX_BITS: $HEX_BITS${NC}" >&2
    [ x$HEX_BITS = x ] && continue
    cli_command -C $CN -c hpr $HEX_BITS >&2

    BIN_BITS=`bc <<< "ibase=16; obase=2; ${HEX_BITS}"` # convert to binary
    echo -ne "${GR}" >&2
    for r in `seq $(( (${CN} - 1) * 14 + 1 )) $(( ${CN} * 14 ))`
    do
      # check if least sig bit is on
      if [ `bc <<< "obase=2; $(( 2#${BIN_BITS} & 2#1 ))"` = 1 ]
      then
        set_solenoid $r on
        echo -n "$r is on; " >&2
      else
        set_solenoid $r off
        echo -n "$r is off; " >&2
      fi
      BIN_BITS=`bc <<< "obase=2; $(( 2#${BIN_BITS} >> 1 ))"` # shift by 1 bit
    done
    echo -e "${NC}" >&2
  done
}

# menu actions etc.
do_ping() {
  sudo ping -qw 2 -c 1 $1 |tail -2 |head -1 |sed 's,^[^,]*\, ,,'
}
do_about() {
  echo; echo calculating....; echo
  whiptail --title "About" --msgbox "\
    This is a control tool for Aquaponic Green Walls.

    Version ${VERSION} running at local time `date +%b-%d-%Y-%T`.

    IP address (wlan0): `ifconfig wlan0 | sed -n '2s/[^:]*:\([^ ]*\).*/\1/p'`
    IP address (eth0) : `ifconfig eth0 | sed -n '2s/[^:]*:\([^ ]*\).*/\1/p'`

    Pings:
    - 8.8.8.8:           `do_ping 8.8.8.8`
    - gripplewall.local: `do_ping gripplewall.local`
    - google.co.uk:      `do_ping google.co.uk`
    " $WT_HEIGHT $(( ( $WT_WIDTH / 2 ) + 35 )) $WT_MENU_HEIGHT
}
do_reboot() {
  echo rebooting greenwall, sleeping 2 then rebooting grippletui...
  ( ssh pi@greenwall.local sudo reboot; )&
  sleep 2
  sudo reboot
}
do_halt() {
  echo shutting down greenwall, sleeping 2 then shutting down grippletui...
  ( ssh pi@greenwall.local sudo halt; )&
  sleep 2
  sudo halt
}
do_area_pulsing() {
  # get a whiptail friendly list of wall areas
  CLIST=""
  for f in ${AREA_DIR}/*
  do
    CLIST="${CLIST} \"`basename $f`\" \" \" off"
  done

  # get a list of areas to water
  TITLE='Pulsed Watering by Area'
  C="whiptail --title \"${TITLE}\" \
       --checklist \"Specify areas to water (with 5 one second pulses per m2)\" \
       $(( $WT_HEIGHT + 10 )) $(( $WT_WIDTH / 2 + 9 )) \
       $(( $WT_MENU_HEIGHT + 10 )) \
       --cancel-button \"Cancel\" --ok-button \"Next\" \
       "${CLIST}" "
  AREA_LIST="`bash -c \"${C} 3>&1 1>&2 2>&3\"`"
  [ "x${AREA_LIST}" = x ] || set ${AREA_LIST}
  AREA_FILES="`for a in "$@"; do \
    echo ${AREA_DIR}/$(echo $a |sed 's,",,g'); done`"

  # for each area do pulsed watering
  for AREA in $AREA_FILES
  do
    echo "area: ${AREA}"
    cli_command -c pulse ${AREA}
  done
  read -p "pulse watering complete... hit return to continue"
}
do_water_control() {
  TITLE='Control Water Supply'
  C="whiptail --title \"${TITLE}\" \
       --checklist \"Specify carts to water\" \
       $(( $WT_HEIGHT + 10 )) $(( $WT_WIDTH / 2 + 9 )) \
       $(( $WT_MENU_HEIGHT + 10 )) \
       --cancel-button \"Cancel\" --ok-button \"Next\" \
       "${SOLENOIDS_A[@]}" "
  SOLENOIDS=`bash -c "${C} 3>&1 1>&2 2>&3"`
  RET=$?
  SOLENOIDS=`echo $SOLENOIDS |sed 's,",,g'`
  if [ $RET -eq 1 ] 
  then
    return 0
  elif [ $RET -eq 0 ]
  then
    whiptail --yesno \
    "Preparing to water cartridges\n  ${SOLENOIDS}\n(get yer wellies on!)" \
      --title "Watering" \
      --no-button "Cancel" --yes-button "Go for it!" \
      $WT_HEIGHT $(( $WT_WIDTH / 2 + 10 )) $WT_MENU_HEIGHT
    RET=$?

    if [ $RET -eq 1 ]
    then 
      MESS="Cancelled"
    elif [ $RET -eq 0 ]
    then 
      echo "cli_command -c on ${SOLENOIDS}" >>${DBG_LOG}
      cli_command -c on ${SOLENOIDS}
      log "wrote <<${SOLENOIDS}>> to board"
      clear_solenoid_state
      for s in ${SOLENOIDS}; do set_solenoid $s on; done
      MESS="Command sent to wall. Good luck!"
    else
      MESS="Oops! Internal error, RET was ${RET}"
    fi
    whiptail --msgbox "${MESS}" 20 60 1
    
    return 0
  fi
}

# initialisation
cli_command -c init  # configure USB port
read_board           # read board & init internal model of solenoid state
log initialised
[ x$1 = xquit ] && exit 0

# main loop
while true; do
  SEL=$(whiptail --title "Text mode wall UI (tui)" \
    --menu "\n" \
      $WT_HEIGHT $WT_WIDTH $WT_MENU_HEIGHT \
    --cancel-button Finish --ok-button Select \
      "1 Water by area"         "Pulse watering to specific areas" \
      "2 Water by cartridge"    "Control water supply to each cart" \
      "3 All off"               "Turn all relays off" \
      "4 Status"                "Show current status from the wall" \
      "5 Show Log Entries"      "Show the most recent log entries" \
      "6 Reboot"                "Reboot both controller machines" \
      "7 Shutdown"              "Shutdown both controller machines" \
      "8 About"                 "Data about this tool, IP addresses etc." \
    3>&1 1>&2 2>&3)
  RET=$?
  if [ $RET -eq 1 ]; then
    exit 0
  elif [ $RET -eq 0 ]; then
    case "$SEL" in
      1\ *) do_area_pulsing ;;
      2\ *) do_water_control ;;
      3\ *) cli_command -c clear; clear_solenoid_state ;;
      4\ *) read_board; whiptail --title "Status" --msgbox \
              "`print_solenoid_state |pr -e -t7 -w78 |expand`" \
              $(( $WT_HEIGHT + 10 )) 78 1 ;;
      5\ *) ( echo "space for more; q to quit"; echo; log_grep; ) \
            |more; read -p "hit return to continue"; ;;
      6\ *) do_reboot ;;
      7\ *) do_halt ;;
      8\ *) do_about ;;
      *)    whiptail --msgbox "Error: unrecognized option" 20 60 1 ;;
    esac || whiptail --msgbox "There was an error running option $SEL" 20 60 1
  else
    exit 5
  fi
done
