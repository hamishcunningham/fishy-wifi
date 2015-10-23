/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =	"UnionSt Coworking 4G1";		// cannot be longer than 32 characters!
const char *pass =	"welcometounionst4g1";		//

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 17);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server);

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

  if (client.connect("arduinoClient")) {
    client.publish("outTopic","hello world!!");
    client.subscribe("inTopic");
  }
}

void loop()
{
  client.loop();
}

