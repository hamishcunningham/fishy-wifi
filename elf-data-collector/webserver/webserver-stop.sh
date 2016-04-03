#!/bin/bash
#
# Stop the webserver.
#
WsBase=~/www/logs/webserver-$(hostname)-8080
PidFile=$WsBase.pid
if [ -f $PidFile ] ; then
    Pid=$(cat $PidFile)
    if kill -0 $Pid 2>&1 >/dev/null ; then
	echo  "stopping $Pid"
	kill -9 $Pid
    fi
    rm -f $PidFile
fi
echo "stopped"
