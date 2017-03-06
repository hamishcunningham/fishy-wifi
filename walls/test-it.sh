#!/bin/bash
#
# template.sh

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-n(no xyz)] [-i [1|2|3|...]"
DBG=:
OPTIONSTRING=hd1i:
RED='\033[0;31m'   # red (use with echo -e)
BLUE='\033[1;34m'  # blue
GREEN='\033[0;32m' # green
NC='\033[0m'       # no color

# specific locals
INST_DIR=`dirname ${P}`
CLI=${INST_DIR}/rs485-api.sh
ONE_AT_A_TIME=

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    1)	ONE_AT_A_TIME="1" ;;
    i)	ABC="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

# cycle through all relay numbers up to 196, gradually turning them all on
doit() {
  for BASE in 0 1 2 3
  do
    echo; echo BASE ${BASE}...
    ${CLI} -B ${BASE} -c off all
    RELAY_SET=""
    for OUTPUT in `seq 64`
    do
      if [ x$ONE_AT_A_TIME = x ]
      then
        RELAY_SET="${OUTPUT}"
      else
        RELAY_SET="${RELAY_SET} ${OUTPUT}"
      fi
      COMMAND="${CLI} -B ${BASE} -c on ${RELAY_SET}"
      echo $COMMAND
      $COMMAND
      # [ x$ONE_AT_A_TIME = x ] && sleep 1
    done
    sleep 1
  done
}
doit
