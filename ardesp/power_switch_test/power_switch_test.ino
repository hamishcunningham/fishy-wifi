
/*
  Example for outlets which are configured with two rotary/sliding switches.
  
  http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  // Transmitter is connected to esp Pin #15  
  mySwitch.enableTransmit(15);
  
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

  // Wait half a second
  delay(500);
  
  // Switch off
  mySwitch.switchOff(2, 2);
  
  // Wait another second
  delay(1000);
  
}
