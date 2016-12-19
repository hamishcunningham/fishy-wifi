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

# TODO
doit() {
  while :
  do
    echo turn it on...
    echo
    sleep 2
    echo turn it off
    echo
    sleep 2
  done
}

doit
