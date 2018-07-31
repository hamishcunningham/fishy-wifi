/*
* TCA9555 Library, code sample
* Kerry D. Wong
* http://www.kerrywong.com
* 3/2011
*/
#include <Wire.h>
#include <TCA9555.h>

// intializaing TCA9555, A2 A1 A0 are set to 000.
TCA9555 tca9555(0,0,0);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  //set the pin directions for both ports to 
  //output
  tca9555.setPortDirection(TCA9555::DIR_OUTPUT);
  
  //change P00 (pin 4), P01 (pin 5) and P17 (pin 20) to high.
  tca9555.setOutputStates(1<<8 | 3);
  
  //read back the pin states
  Serial.print(tca9555.getInputStates());
}

void loop()
{
}
