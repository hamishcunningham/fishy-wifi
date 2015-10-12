/*
  Example for outlets which are configured with two rotary/sliding switches.
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  // Transmitter is connected to Arduino Pin #13  
  mySwitch.enableTransmit(13);
  
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the setting of the upper rotary switch. 
  // In this example it's switched to "IV". 
  // 
  // The second parameter represents the setting of the lower rotary switch. 
  // In this example it's switched to "2". 
  mySwitch.switchOn(4, 2);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff(4, 2);
  
  // Wait another second
  delay(1000);
  
}
