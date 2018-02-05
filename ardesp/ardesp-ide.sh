#!/bin/bash
#
# ardesp-ide.sh -- run the Arduino IDE

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-e(rase flash)] [-S(taging)] [-H(EAD)] [-8 (1.6.8)] [-1 (1.8.1)] [-3 (1.8.3)] [-5 (1.8.5)]"
DBG=:
OPTIONSTRING=hdSHx:8135e

# specific locals
ERASE=
PORT='/dev/ttyUSB0'
IDEBASE=~/esp-arduino-ide
PREFSDIR=~/.arduino15
SPREFSDIR=${IDEBASE}/dot-arduino15-staging
HPREFSDIR=${IDEBASE}/dot-arduino15-head
EIGHTPREFSDIR=${IDEBASE}/dot-arduino15-168
ONE8ONEPREFSDIR=${IDEBASE}/dot-arduino15-181
ONE83PREFSDIR=${IDEBASE}/dot-arduino15-183
ONE85PREFSDIR=${IDEBASE}/dot-arduino15-185
X=
STAGING=
HEAD=
EIGHT=
ONE8ONE=
ONE83=
ONE85=
STAGINGDIR=${IDEBASE}/arduino-1.6.5-r5
HEADDIR=${IDEBASE}/arduino-1.6.5-r5--local-esp
EIGHTDIR=${IDEBASE}/arduino-1.6.8
ONE8ONEDIR=${IDEBASE}/arduino-1.8.1
ONE83DIR=${IDEBASE}/arduino-1.8.3
ONE85DIR=${IDEBASE}/arduino-1.8.5

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    e)	ERASE=yes ;;
    S)	STAGING=yes ;;
    H)	HEAD=yes ;;
    8)	EIGHT=yes ;;
    1)	ONE8ONE=yes ;;
    3)	ONE83=yes ;;
    5)	ONE85=yes ;;
    x)	X="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

if [ x$ERASE == xyes ]
then
  [ -d Arduino ] || { echo "can't find Arduino dir from `pwd`"; exit 10; }
  echo running erase on $PORT ...
  echo Arduino/hardware/espressif/esp32/tools/esptool.py --port "${PORT}" erase_flash
  Arduino/hardware/espressif/esp32/tools/esptool.py --port "${PORT}" erase_flash
  exit 0
fi

# need to specify a version to run
if [ x$STAGING == x -a x$HEAD == x -a x$EIGHT == x -a x$ONE8ONE == x -a x$ONE83 == x -a x$ONE85 == x ]
then
  echo you must choose staging or head or 8 or 1 or 3
  usage 2
fi

# choose version
if [ x$STAGING == xyes ]
then
  USEPREFS=${SPREFSDIR}
  USEIDE=${STAGINGDIR}
elif [ x$HEAD == xyes ]
then
  USEPREFS=${HPREFSDIR}
  USEIDE=${HEADDIR}
elif [ x$EIGHT == xyes ]
then
  USEPREFS=${EIGHTPREFSDIR}
  USEIDE=${EIGHTDIR}
elif [ x$ONE8ONE == xyes ]
then
  USEPREFS=${ONE8ONEPREFSDIR}
  USEIDE=${ONE8ONEDIR}
elif [ x$ONE83 == xyes ]
then
  USEPREFS=${ONE83PREFSDIR}
  USEIDE=${ONE83DIR}
elif [ x$ONE85 == xyes ]
then
  USEPREFS=${ONE85PREFSDIR}
  USEIDE=${ONE85DIR}
else
  echo 'erk!'
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
ln -s $USEPREFS $PREFSDIR
echo using $PREFSDIR: `ls -ld $PREFSDIR`

# run the IDE
cd $USEIDE
echo running arduino IDE from `pwd`
if [ -x ./arduino ] 
then
  #./arduino --pref sketchbook.path="${HOME}/iot/com3505/labs"
  ./arduino
else 
  cd build
  ant run
fi
