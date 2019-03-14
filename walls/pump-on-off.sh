#!/bin/bash
# ask the pump controller Elf to turn the pump on or off

FISH_ELF_IP=192.168.207.191
POWER_SENSOR_ELF_IP=192.168.207.192
PRESSURE_SENSOR_ELF_IP=192.168.207.193

for ELF_IP in $PRESSURE_SENSOR_ELF_IP $FISH_ELF_IP $POWER_SENSOR_ELF_IP
do
  echo; echo sending $1 to $ELF_IP ...
  /usr/bin/curl \
    -v "http://${ELF_IP}/actuate" \
    -H "Origin: http://${ELF_IP}" \
    -H 'Accept-Encoding: gzip, deflate' \
    -H 'Accept-Language: en-GB,en-US;q=0.8,en;q=0.6' \
    -H 'Upgrade-Insecure-Requests: 1' \
    -H 'User-Agent: curl' \
    -H 'Content-Type: application/x-www-form-urlencoded' \
    -H 'Accept: text/html,application/xhtml+xml,application/xml' \
    -H 'Cache-Control: max-age=0' \
    -H "Referer: http://${ELF_IP}/" \
    -H 'Connection: keep-alive' \
    --data "state=$1" --compressed
  echo; echo done
  sleep 1
done
