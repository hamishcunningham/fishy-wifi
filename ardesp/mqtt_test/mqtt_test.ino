/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *ssid =	"UnionSt Coworking 4G1";		// cannot be longer than 32 characters!
const char *pass =	"welcometounionst4g1";		//

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 17);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server);

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
OneWire ds(2); // DS1820 on pin 2 (a 4.7K resistor is necessary)
DallasTemperature tempSensor(&ds);  // pass through reference to library
float waterCelsius;
boolean GOT_TEMP_SENSOR = false; // we'll change later if we detect sensor
DeviceAddress tempAddr; // array to hold device address

void setup()
{
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  client.set_callback(callback);

  WiFi.begin(ssid, pass);

  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
  }
  
  tempSensor.begin();     // start the onewire temperature sensor
  if(tempSensor.getDeviceCount()==1) {
    GOT_TEMP_SENSOR = true;
    tempSensor.getAddress(tempAddr, 0);
    tempSensor.setResolution(tempAddr, 12); // 12 bit res (DS18B20 does 9-12)
  }
}

void loop()
{
  client.loop();
    tempSensor.requestTemperatures(); // send command to get temperatures
  waterCelsius = tempSensor.getTempC(tempAddr);
  Serial.print("Temp: ");
  Serial.print(waterCelsius);
  Serial.println(" C, ");
  
  if (client.connect("arduinoClient")) {
    char BUFFER[10];
    char *wc;
    wc = dtostrf(waterCelsius,4,2,BUFFER);
  client.publish("outTopic",wc);
  }
  delay(500);
}

