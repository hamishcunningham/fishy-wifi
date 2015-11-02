
pin=1
pause=16000
readings={}
sum=0
for i=1,1000 do
  gpio.mode(pin, gpio.OUTPUT)
  gpio.write(pin, gpio.LOW) 
  tmr.delay(pause)
  c=0
  gpio.mode(pin, gpio.INPUT)
  while gpio.read(pin) == gpio.LOW do
    c = c + 1
  end
  sum = sum + c
end
return sum/1000

















/*
from http://www.raspberrypi.org/forums/viewtopic.php?t=42358
#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void) {
  int c,x,re[1000],med=0;
  int NUMREADINGS=200;
  printf ("Raspberry Pi wiringPi Capacitor reading \n") ;

  if (wiringPiSetup () == -1)
    exit (1) ;

  for (;;) {

    for (x=0;x<NUMREADINGS;x++) {
        pinMode (10, OUTPUT);
        digitalWrite (10, LOW);
        delay(16);
        c=0;
        pinMode (10, INPUT);
        while (digitalRead(10)==LOW)
          c++;
        re[x]=c;
      }
      med=0;    
      for (x=0;x<NUMREADINGS;x++) 
        med+=re[x];
      printf("%d\n",med/NUMREADINGS);
  }                             
}
*/















