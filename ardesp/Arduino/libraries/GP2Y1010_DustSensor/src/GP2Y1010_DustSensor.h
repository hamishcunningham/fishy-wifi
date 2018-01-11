/*
  GP2Y1010_DustSensor.cpp - SHARP GP2Y1010AU0F Dust sensor library for ESP-WROOM-02/32(esp8266/ESP32) or Arduino
  version 0.4
  
  License MIT
*/

#ifndef GP2Y1010_DustSensor_h_
#define GP2Y1010_DustSensor_h_

#include "Arduino.h"
#include <math.h>

class GP2Y1010_DustSensor
{
public:
	GP2Y1010_DustSensor();
	GP2Y1010_DustSensor(int ledPin, int measurePin);

	~GP2Y1010_DustSensor();

	void begin(int ledPin, int measurePin);
	void setADCbit(int bit);
	int  getADCbit();
	void setInputVolts(float volts);
	float getInputVolts();
	
	float getDustDensity();

protected:

private:
	int led_pin = 2 , measure_pin = A0;

	static const int SAMPLINGTIME = 280;
	static const int DELTATIME = 40;
	static const int SLEEPTIME = 9680;
	
	int analog_bit = 10;
	double analog_bit_num = pow(2., (double)analog_bit);
	float inputvolts = 5.;
};

#endif
