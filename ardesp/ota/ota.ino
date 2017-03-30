// basic OTA example, from
// github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA/examples/BasicOTA
// 23/3/17

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "...";
const char* password = "...";

void setup() {
  Serial.begin(115200);
  Serial.println("Booting (OTA'd version!)");
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() { Serial.println("Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  delay(5000);
  Serial.println("Ready (OTA's version!)");
  Serial.print("my IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(2, HIGH); digitalWrite(0, LOW); delay(250);
  digitalWrite(2, LOW); digitalWrite(0, HIGH); delay(500);

  ArduinoOTA.handle();
  Serial.print("the ESP's IP address is: "); Serial.println(WiFi.localIP());
}
