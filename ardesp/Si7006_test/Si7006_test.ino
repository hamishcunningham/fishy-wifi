// Hacky sketch to try to read from new humidity/temp sensor Si7006
//I2C Library
#include <Wire.h>

const int Si7006i2cAddress =                0x40;
const int Si7006_FIRMWARE_0 =               0x84;
const int Si7006_FIRMWARE_1 =               0xB8;
// Master mode works.  No Master mode does not work.
const int Si7006_HUMIDITY_MASTER_MODE =     0xE5;
const int Si7006_TEMPERATURE  =             0xE0;

void setup(){
  Wire.begin(); //initialize I2C
  Serial.begin(115200);
  Serial.println("********Si7006 example sketch********");
}

void loop(){
  float humidity, temperature;
  //We'll assign a 2 BYTES variables to capture the LSB and MSB(or Hi Low in this case)
  byte data_high;
  byte data_low;
  //We'll assemble the 2 in this variable
  int data_value, errCode;
  
  Wire.beginTransmission(Si7006i2cAddress);
  Wire.write(Si7006_HUMIDITY_MASTER_MODE);
  errCode = Wire.endTransmission();
  Serial.print("Return code: ");
  Serial.println(errCode);
  
  Wire.requestFrom(Si7006i2cAddress, 2);
  while(Wire.available() < 2); // while two bytes to receive
  data_high = Wire.read();
  data_low = Wire.read();
  data_value = word(data_high, data_low);
  //From datasheet: A humidity measurement will always return XXXXXX10 in the LSB field.
  if (data_value & 0xFFFE) {
    humidity = ((125 * (float)data_value ) / 65536) - 6;
    Serial.print("Humidity: ");
    Serial.println(humidity);    
  } else {
    Serial.print("Something went wrong!");
  }
  
  //onto the temperature.
  Wire.beginTransmission(Si7006i2cAddress);
  Wire.write(Si7006_TEMPERATURE);
  errCode = Wire.endTransmission();
  Serial.print("Return code 2: ");
  Serial.println(errCode);
  
  Wire.requestFrom(Si7006i2cAddress, 2);
  while(Wire.available() < 2); // while two bytes to receive
  data_high = Wire.read();
  data_low = Wire.read();
  data_value = word(data_high, data_low);
  // A temperature measurement will always return XXXXXX00 in the LSB field.
  if (data_value & 0xFFFC) {
    temperature = (172.72 * (float)data_value) / 65536 - 46.85;
    Serial.print("Temperature: ");
    Serial.println(temperature);
  } else {
    Serial.print("Something went wrong!");
  } 
   delay(2000);
}
