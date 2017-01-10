#!/bin/bash
# tui -- text mode (curses) UI for ponic walls

# standard locals
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-v(ersion)] [-l(og entries)]"
OPTIONSTRING=hdvl
alias cd='builtin cd'
DBG=:

# specific locals
INST_DIR=`dirname ${P}`
CLI=${INST_DIR}/rs485-api.sh
LOG_STRING=tui
VERSION=0.000001

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# write a log; get recent log entries
log() { logger "${LOG_STRING}: $*"; }
log_grep() { grep -i $LOG_STRING /var/log/syslog |tail -15; }

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
read_board() {
  # TODO set SOLENOIDS_A state according to board state
  # e.g. set_solenoid 3 on
  :
}

# menu actions etc.
do_about() {
  whiptail --title "About" --msgbox "\
    This is a control tool for Aquaponic Green Walls.

    Version ${VERSION}.
    " $WT_HEIGHT $(( $WT_WIDTH / 2 )) $WT_MENU_HEIGHT
}
do_water_control() {
  TITLE='Control Water Supply'
  C="whiptail --title \"${TITLE}\" \
       --checklist \"Specify carts to water\" \
       $WT_HEIGHT $(( $WT_WIDTH / 2 + 9 )) $WT_MENU_HEIGHT \
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
      cli_command -c on ${SOLENOIDS}
      clear_solenoid_state
      for s in ${SOLENOIDS}; do set_solenoid $s on; done
      log "wrote <<${SOLENOIDS}>> to board"
      MESS="Command sent to wall. Good luck!"
    else
      MESS="Oops! Internal error, RET was ${RET}"
    fi
    whiptail --msgbox "${MESS}" 20 60 1
    
    return 0
  fi
}

# initialisation
clear
cli_command -c init  # configure USB port
read_board           # read board & init internal model of solenoid state
log initialised

# main loop
while true; do
  SEL=$(whiptail --title "Text mode wall UI (tui)" \
    --menu "\n" \
      $WT_HEIGHT $WT_WIDTH $WT_MENU_HEIGHT \
    --cancel-button Finish --ok-button Select \
      "1 Watering"              "Control water supply to the wall" \
      "2 Status"                "Show current status from the wall" \
      "3 Show Log Entries"      "Show the most recent log entries" \
      "4 About"                 "Information about this tool" \
    3>&1 1>&2 2>&3)
  RET=$?
  if [ $RET -eq 1 ]; then
    exit 0
  elif [ $RET -eq 0 ]; then
    case "$SEL" in
      1\ *) do_water_control ;;
      2\ *) whiptail --title "Status" --msgbox \
              "`print_solenoid_state |pr -e -t2 -w76 |expand`" \
              $WT_HEIGHT 78 1 ;;
      3\ *) whiptail --title "Recent Log Entries" --msgbox \
              "`log_grep`" $WT_HEIGHT $WT_WIDTH 1 ;;
      4\ *) do_about ;;
      *)    whiptail --msgbox "Error: unrecognized option" 20 60 1 ;;
    esac || whiptail --msgbox "There was an error running option $SEL" 20 60 1
  else
    exit 5
  fi
done
