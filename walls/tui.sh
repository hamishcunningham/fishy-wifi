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

# get recent log entries
do_log_grep() { grep -i $LOG_STRING /var/log/syslog |tail -${WT_HEIGHT}; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    v)	echo $P is at version $VERSION; exit 0 ;;
    d)	DBG=echo; CLI=echo ;;
    l)  do_log_grep; exit 0 ;;
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
  WT_MENU_HEIGHT=$(($WT_HEIGHT-10))
}
calc_wt_size

# menu actions etc.
do_about() {
  whiptail --title "About" --msgbox "\
    This is a control tool for Aquaponic Green Walls.

    Version ${VERSION}.
    " $WT_HEIGHT $(( $WT_WIDTH / 2 )) $WT_MENU_HEIGHT
}

# main loop
clear
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
              "`${CLI} -e |pr -e -t2 -w76 |expand`" $WT_HEIGHT 78 1 ;;
      3\ *) whiptail --title "Log Entries" --msgbox \
              "`do_log_grep`" $WT_HEIGHT $WT_WIDTH 1 ;;
      4\ *) do_about ;;
      *)    whiptail --msgbox "Error: unrecognized option" 20 60 1 ;;
    esac || whiptail --msgbox "There was an error running option $SEL" 20 60 1
  else
    exit 5
  fi
done
