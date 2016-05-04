/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/
#include "Adafruit_MCP23008.h"
#include "Wire.h"

#define trigPin1 12
#define echoPin1 13
#define trigPin2 0
#define echoPin2 14

Adafruit_MCP23008 mcp; // Create object for MCP23008

void setup() {
  Serial.begin (115200);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
      
  mcp.begin();      // use default address 0 for mcp23008
  mcp.pinMode(trigPin2, OUTPUT);
}

void loop() {
  long duration1, distance1, duration2, distance2;
  digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH,20000);
  distance1 = (duration1/2) / 29.1;
  
  mcp.digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  mcp.digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  mcp.digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH,20000);
  distance2 = (duration2/2) / 29.1;

  Serial.print(distance1);
  Serial.print(" cm (1)  ");
  Serial.print(distance2);
  Serial.println(" cm (2)");
    
  delay(250);
}
