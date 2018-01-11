/*
  GP2Y1010_DustSensor.cpp - SHARP GP2Y1010AU0F Dust sensor library for ESP-WROOM-02/32(esp8266/ESP32) or Arduino
  version 0.4
  
  License MIT
*/

#include "GP2Y1010_DustSensor.h"
#include "Arduino.h"
#include <math.h>

// public

GP2Y1010_DustSensor::GP2Y1010_DustSensor(){
}

GP2Y1010_DustSensor::GP2Y1010_DustSensor(int ledPin, int measurePin){
	begin(ledPin,measurePin);

}

GP2Y1010_DustSensor::~GP2Y1010_DustSensor(){
}

void GP2Y1010_DustSensor::begin(int ledPin, int measurePin){
	led_pin = ledPin;
	measure_pin = measurePin;
	pinMode(led_pin, OUTPUT);
}

void GP2Y1010_DustSensor::setADCbit(int bit){
	analog_bit = bit;
	analog_bit_num = pow(2., (double)analog_bit);
}

int GP2Y1010_DustSensor::getADCbit(){
	return analog_bit;
}

void GP2Y1010_DustSensor::setInputVolts(float volts){
	inputvolts = volts;
}

float GP2Y1010_DustSensor::getInputVolts(){
	return inputvolts;
}

float GP2Y1010_DustSensor::getDustDensity() {

	digitalWrite(led_pin, LOW);
	delayMicroseconds(SAMPLINGTIME);

	float mesured = analogRead(measure_pin);	//read analog pin / Dust value
	delayMicroseconds(DELTATIME);

	digitalWrite(led_pin, HIGH);
	delayMicroseconds(SLEEPTIME);

	// culc dust density
	float dust = (0.17 * (mesured * (inputvolts / analog_bit_num)) - 0.1) * 1000.;
	if( dust<0 )	dust=0.;
	
	return dust;
}
