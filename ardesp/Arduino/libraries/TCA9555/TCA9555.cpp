/*
* TCA9555 Library
* Kerry D. Wong
* http://www.kerrywong.com
* 3/2011
*/

#include "TCA9555.h"
#include <Wire.h>

//a2, a1 and a0 are the address pin states
//they can be either 0 or 1 (low or high)
//the actual I2C address is calculated using 
//the pin settings.
TCA9555::TCA9555(byte a2, byte a1, byte a0)
{
  A2 = a2;
  A1 = a1;
  A0 = a0;
	
  I2CAddr = 0x20 | (A2 << 2 | A1 << 1 | A0);	
}

//set port directions for both ports
//dir: DIR_OUTPUT or DIR_INPUT
void TCA9555::setPortDirection(byte dir)
{
  Wire.beginTransmission(I2CAddr);
  Wire.write(CR_CFG0);
  Wire.write(dir);
  Wire.write(dir);
  Wire.endTransmission();	
}

//portNum: PORT_0 or PORT_1
void TCA9555::setPortDirection(byte portNum, byte dir)
{
  Wire.beginTransmission(I2CAddr);
  if (portNum == PORT_0) Wire.write(CR_CFG0);
  else Wire.write(CR_CFG1);
  Wire.write(dir);
  Wire.endTransmission();
}

//set port polarity for both ports
//polarity: POLARITY_NORMAL or POLARITY_INV
void TCA9555::setPortPolarity(byte polarity)
{
  Wire.beginTransmission(I2CAddr);
  Wire.write(CR_INV0);
  Wire.write(polarity);
  Wire.write(polarity);
  Wire.endTransmission();
}

void TCA9555::setPortPolarity(byte portNum, byte polarity)
{
  Wire.beginTransmission(I2CAddr);
  if (portNum == PORT_0) Wire.write(CR_INV0);
  else Wire.write(CR_INV1);
  Wire.write(polarity);
  Wire.endTransmission();
}

//set output states when both ports are used as output ports
//w contains all 16 ports' states.
void TCA9555::setOutputStates(word w)
{
  byte low_byte = w & 0xff;
  byte high_byte = (w & 0xff00) >> 8;

  Wire.beginTransmission(I2CAddr);
  Wire.write(CR_OUT0);
  Wire.write(low_byte);
  Wire.write(high_byte);
  Wire.endTransmission();
}

void TCA9555::setOutputStates(byte portNum, byte b)
{
  Wire.beginTransmission(I2CAddr);
  if (portNum == PORT_0) Wire.write(CR_OUT0);
  else Wire.write(CR_OUT1);
  Wire.write(b);
  Wire.endTransmission();
}

//returns the input states of both ports when ports
//are used as inputs
word TCA9555::getInputStates()
{
  byte low_byte, high_byte;

  Wire.beginTransmission(I2CAddr);
  Wire.requestFrom(I2CAddr, 2u);

  while (!Wire.available()) {};
  low_byte = Wire.read();
  while (!Wire.available()) {};
  high_byte = Wire.read();

  Wire.endTransmission();
  
  word w = low_byte | (high_byte << 8);
  
  return w;
}

byte TCA9555::getInputStates(byte portNum)
{
  word w = getInputStates();
  
  if (portNum == PORT_0) return w & 0xff;
  else return (w & 0xff00) >> 8;
}

