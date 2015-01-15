#!/bin/bash

# if we generate with TRIGGER = 100, it will load on the ESP;
# if we do it with TRIGGER = 300, then we get a reboot when loading
TRIGGER=100
TRIGGER=300

echo 'sizebug = {}' >sizebug.lua
echo 'function sizebug.trigger()' >>sizebug.lua
i=0
while [ $i -lt $TRIGGER ]
do
  i=$((i+1))
  printf "  print(\"%015d\")\n" $i >>sizebug.lua
done
echo end >>sizebug.lua
echo 'return sizebug' >>sizebug.lua
