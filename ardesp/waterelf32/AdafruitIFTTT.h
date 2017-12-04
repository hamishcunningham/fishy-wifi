// Ex20.h

#ifndef AdafruitIFTTT
#define AdafruitIFTTT

#define IO_USERNAME    "andrisole"
#define IO_KEY         "63cc3971e9a044c0b7c54dae5a2ffaff"

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056

#define WIFI_SSID       "<WIFI_SSID>"
#define WIFI_PASS       "<WIFI_PASS>"
#define IFTTT_KEY       "<IFTTT_KEY>"
#define IFTTT_EVENT_NAME       "<IFTTT_EVENT_NAME>"
#include "AdafruitIO_WiFi.h"
#include <WiFi.h>
#include <AdafruitIO_Feed.h>
#include <ArduinoHttpClient.h>
// Connecting to Adafruit.IO APIs
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
WiFiClient wifi;
//Adafruit and IFTTT clients
HttpClient ioClient = HttpClient(wifi, "io.adafruit.com", 80);
HttpClient IFTTTClient = HttpClient(wifi, "maker.ifttt.com", 80);

// Value for triggering something
int UltraSonicTriggerValue = 20;

// Creates Adafruit.io feed. Returns true if feed exists or successfuly created.
bool createAdafruitFeed(String);

//Checks if the feed with a given name already exists on Adafruit. Returns true if true.
bool feedExists(String);

//Triggers IFTTT url. First parameter - url, Seconde param - url params e.g 'value1=abc&value2=cde'
bool triggerIFTTWebHook(String,String);

//Creates AdafruitIO trigger. Takes feedID, operator and trigger value.
bool createAdafruitIOTrigger(String, String = "gt", int = UltraSonicTriggerValue);

//Returns IFTTT WebHook url bas
String genrateIFTTTWebHookURL(String,String);
String generateSensorFeedName(String);
String getAdafruitIOFeedId(String = "");
#endif
