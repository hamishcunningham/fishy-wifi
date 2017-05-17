# stuff this in /home/pi/.profile and light blue touch paper
# (pi should be configured to autologin to console)

PS1='(to restart type "sudo reboot")  '"${PS1}"

if [ `tty` = "/dev/tty1" ]
then
  /home/pi/fishy-wifi/walls/rs485-api.sh -c trap_leaks_and_kill_pump \
    >>$DBG_LOG 2>&1 &
  /home/pi/fishy-wifi/walls/tui.sh
  echo -n "Shutdown? (s) Reboot? (r) Shell? (any) "
  read ANS
  if [ x$ANS = xs ]
  then
    echo halting...
    sudo halt
  elif  [ x$ANS = xr ]
  then
    echo rebooting...
    sudo reboot
  fi
fi

