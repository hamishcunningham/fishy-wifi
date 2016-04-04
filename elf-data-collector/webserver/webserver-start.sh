#!/bin/bash
#
# Start the webserver.
#
WsBase=~/www/logs/webserver-$(hostname)-8080
PidFile=$WsBase.pid
if [ -f $PidFile ] ; then
    Pid=$(cat $PidFile)
    if kill -0 $Pid 2>&1 >/dev/null ; then
	echo "ERROR: webserver is already running, PID=$Pid"
	exit 1
    else
	# The process does not exist, remove the pid file.
	rm -f $PidFile
    fi
fi
webserver.py -H $(hostname) -p 8080 -l warning --no-dirlist -r ~/www -d ~/www/logs
echo "started"
