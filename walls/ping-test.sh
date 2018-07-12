#!/bin/bash
waterelf1='192.168.207.191'
waterelf2='192.168.207.192'
waterelf3='192.168.207.193'

echo `date` &>>  /home/pi/pingtest.txt
ping -c1 $waterelf1 | tail -n2 1>> /home/pi/pingtest.txt
ping -c1 $waterelf2 | tail -n2 1>> /home/pi/pingtest.txt
ping -c1 $waterelf3 | tail -n2 1>> /home/pi/pingtest.txt
