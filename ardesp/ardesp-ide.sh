#!/bin/bash
#
# ardesp-ide.sh -- run the ESP8266 Arduino IDE

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-S(taging)] [-H(EAD)]"
DBG=:
OPTIONSTRING=hdSHx:

# specific locals
PREFSDIR=~/.arduino15
S_PREFSDIR=~/.arduino15-staging
H_PREFSDIR=~/.arduino15-head
X=
STAGING=
HEAD=
IDEBASE=~/esp-arduino-ide
STAGINGDIR=${IDEBASE}/arduino-1.6.5
HEADDIR=${IDEBASE}/arduino-1.6.6

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    S)	STAGING=yes ;;
    H)	HEAD=yes ;;
    x)	X="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

# need to specify a version to run
if [ x$STAGING == x -a x$HEAD == x ]
then
  echo you must choose either staging or head
  usage 2
fi

# choose version
if [ x$STAGING == xyes ]
then
  USE_PREFS=${S_PREFSDIR}
  USE_IDE=${STAGING_DIR}
elif [ x$HEAD == xyes ]
  USE_PREFS=${H_PREFSDIR}
  USE_IDE=${HEAD_DIR}
else
  echo you must choose either staging or head
  usage 3
fi

# the prefs dir should be a link to a version-specific dir
if [ ! -L $PREFSDIR ]
then
  echo $PREFSDIR should be a link
  ls -l $PREFSDIR
  usage 4
fi

# switch the link to the required version
rm $PREFSDIR
ln -s $PREFSDIR $USE_PREFS
echo using $PREFSDIR: `ls -ld $PREFSDIR`

# run the IDE
cd $USE_IDE
echo running arduino IDE from `pwd`
./arduino
