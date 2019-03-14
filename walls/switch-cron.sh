#!/bin/bash
#
# switch-cron.sh

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` on|off"
DBG=:
OPTIONSTRING=hdni:
RED='\033[0;31m'   # red (use with echo -e)
BLUE='\033[1;34m'  # blue
GREEN='\033[0;32m' # green
NC='\033[0m'       # no color

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

[ x$1 == xon -o x$1 == xoff ] || { echo 'on or off? exiting'; usage 1; }

cd
if [ x$1 == xon ]
then
  echo -e "turning cron on... cron now is:"
  crontab -l
  crontab .crontab
  echo -e "...done; cron now is:"
  crontab -l
elif [ x$1 == xoff ]
then
  echo -e "turning cron off... cron now is:"
  crontab -l
  CT=`crontab -l`
  [ "x${CT}x" == xx ] && { echo "oops: cron off already; exiting"; usage 2; }
  cp .crontab /tmp/crontab-$$
  crontab -l >.crontab
  cat /dev/null |crontab
  echo -e "...done; cron now is:"
  crontab -l
fi
