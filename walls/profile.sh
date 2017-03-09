# stuff this in /home/pi/.profile and light blue touch paper
# (pi should be configured to autologin to console)

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
