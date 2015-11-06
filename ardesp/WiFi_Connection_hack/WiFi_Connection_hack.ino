/*
 *  This sketch just connects to a wifi AP. Change ssid and password below to match your network.
 */

#include <ESP8266WiFi.h>

const char* ssid     = "UnionSt Coworking 4G2";
const char* password = "welcometounionst4g2";

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(5000);
  Serial.println("All done - wifi details saved to EEPROM. looping");  
}

