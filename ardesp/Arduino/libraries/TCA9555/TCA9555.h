#ifndef TCA9555_H
#define TCA9555_H

#include <inttypes.h>
#include <Arduino.h>

/*
* TCA9555 Library
* Kerry D. Wong
* http://www.kerrywong.com
* 3/2011
*/
class TCA9555 
{
public:
	static const byte PORT_0 = 0;
	static const byte PORT_1 = 1;

	static const byte DIR_OUTPUT = 0;
	static const byte DIR_INPUT = 1;
	
	static const byte POLARITY_NORMAL = 0;
	static const byte POLARITY_INV = 1;

	static const byte CR_IN0 = 0x0;
	static const byte CR_IN1 = 0x1;
	static const byte CR_OUT0 = 0x2;
	static const byte CR_OUT1 = 0x3;
	static const byte CR_INV0 = 0x4;
	static const byte CR_INV1 = 0x5;
	static const byte CR_CFG0 = 0x6;
	static const byte CR_CFG1 = 0x7;

	TCA9555(): A2(0), A1(0), A0(0) {};
	TCA9555(byte a2, byte a1, byte a0);
	void setPortDirection(byte dir);
	void setPortDirection(byte portNum, byte dir);
        void setPortPolarity(byte polarity);
        void setPortPolarity(byte portNum, byte polarity);
	void setOutputStates(word w);
	void setOutputStates(byte portNum, byte b);
	word getInputStates();
	byte getInputStates(byte portNum);

	void pinMode(byte pinNum, byte dir);
        byte getI2CAddr();

private:
	byte A2, A1, A0;
	byte I2CAddr;
};

#endif
