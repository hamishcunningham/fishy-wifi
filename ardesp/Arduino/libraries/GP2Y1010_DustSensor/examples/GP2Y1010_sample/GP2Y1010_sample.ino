/*----------------------------------------------------------
    SHARP GP2Y1010AU0F Dust Sensor  SAMPLE
  ----------------------------------------------------------*/

#include <GP2Y1010_DustSensor.h>

const int led_pin = 2;
const int analog_pin = A0;

GP2Y1010_DustSensor dustsensor;

/*----------------------------------------------------------
    SHARP GP2Y1010AU0F Dust Sensor  setup
  ----------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  dustsensor.begin(led_pin, analog_pin);
}

/*----------------------------------------------------------
    SHARP GP2Y1010AU0F Dust Sensor  loop
  ----------------------------------------------------------*/
void loop() {
  float dust = dustsensor.getDustDensity();
  Serial.print("Dust Density: "); Serial.print(dust); Serial.println(" ug/m3");
  delay(30000);
}

