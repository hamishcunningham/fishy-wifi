/////////////////////////////////////////////////////////////////////////////
// waterelf.ino /////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <RCSwitch.h>

/////////////////////////////////////////////////////////////////////////////
// resource management stuff ////////////////////////////////////////////////
int loopCounter = 0;
const int LOOP_ROLLOVER = 1000; // how many loops per action slice
const byte TICK_MONITOR = 0;
const byte TICK_WIFI_DEBUG = 500;
const byte TICK_POST_DEBUG = 200;
const byte TICK_HEAP_DEBUG = 1000;

/////////////////////////////////////////////////////////////////////////////
// wifi management stuff ////////////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
String apSSIDStr = "WaterElf-" + String(ESP.getChipId());
const char* apSSID = apSSIDStr.c_str();
String svrAddr = ""; // address of a local server

/////////////////////////////////////////////////////////////////////////////
// page generation stuff ////////////////////////////////////////////////////
String pageTopStr = String(
  "<html><head><title>WaterElf Aquaponics Helper [ID: " + apSSIDStr + "]"
);
const char* pageTop = pageTopStr.c_str();
const char* pageTop2 = "</title>\n"
  "<meta charset=\"utf-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
  "</head><body>\n";
const char* pageDefault =
  "<h2>Welcome to WaterElf</h2>\n"
  "<h2>Control</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifi'>Join a wifi network</a></li>\n"
  "<li><a href='/serverconf'>Configure server location</a></li>\n"
  "<li>\n"
    "<form method='POST' action='actuate'>\n"
    "Operate actuator: "
    "on <input type='radio' name='state' value='on'>\n"
    "off <input type='radio' name='state' value='off' checked>\n"
    "<input type='submit' value='Submit'></form>\n"
  "</li>\n"
  "</ul></p>\n"
  "<h2>Monitor</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifistatus'>Wifi status</a></li>\n"
  "<li><a href='/data'>Sensor data</a></li>\n"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>WaterElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='https://www.wegrow.social/'>CleanFood</a></p></body></html>";

/////////////////////////////////////////////////////////////////////////////
// data monitoring stuff ////////////////////////////////////////////////////
const boolean SEND_DATA = true;  // turn off posting of data if required here
const int MONITOR_POINTS = 60; // number of data points to store
struct monitor_t {
  unsigned long timestamp;
  float waterCelsius;
  float airCelsius;
  float airHumid;
  uint16_t lux;
};
monitor_t monitorData[MONITOR_POINTS];
int monitorCursor = 0;
int monitorSize = 0;
const int DATA_ENTRIES = 30; // size of /data rpt; must be <= MONITOR_POINTS
void updateSensorData(monitor_t *monitorData);
void postSensorData(monitor_t *monitorData);
void printMonitorEntry(monitor_t m, String* buf);
void jsonMonitorEntry(monitor_t *m, String* buf);

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
OneWire ds(2); // DS1820 on pin 2 (a 4.7K resistor is necessary)
DallasTemperature tempSensor(&ds);  // pass through reference to library
void getTemperature(float* waterCelsius);
boolean GOT_TEMP_SENSOR = false; // we'll change later if we detect sensor
DeviceAddress tempAddr; // array to hold device address

/////////////////////////////////////////////////////////////////////////////
// humidity sensor stuff ////////////////////////////////////////////////////
DHT dht(12, DHT22); // what digital pin we're on, plus type DHT22 aka AM2302
boolean GOT_HUMID_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// Light sensor stuff ///////////////////////////////////////////////////////
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // sensor id
boolean GOT_LIGHT_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// pH sensor stuff //////////////////////////////////////////////////////////
boolean GOT_PH_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// RC switch stuff //////////////////////////////////////////////////////////
RCSwitch mySwitch = RCSwitch();

/////////////////////////////////////////////////////////////////////////////
// misc utils ///////////////////////////////////////////////////////////////
void ledOn();
void ledOff();
String ip2str(IPAddress address);

/////////////////////////////////////////////////////////////////////////////
// setup ////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT); // turn built-in LED on
  blink(3); // signal we're starting config

  startPeripherals();
  startAP();
  printIPs();
  startDNS();

  startWebServer();

  if(WiFi.hostname("waterelf"))
    Serial.println("set hostname succeeded");
  else
    Serial.println("set hostname failed");

  delay(300); blink(3); // signal we've finished config
}

/////////////////////////////////////////////////////////////////////////////
// looooooooooooooooooooop //////////////////////////////////////////////////
void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if(loopCounter == TICK_MONITOR) {
    updateSensorData(monitorData);
    delay(500); // TODO a better way?!
  } 
  if(loopCounter == TICK_WIFI_DEBUG) {
    Serial.print("SSID: "); Serial.print(apSSID);
    Serial.print("; IP address(es): local="); Serial.print(WiFi.localIP());
    Serial.print("; AP="); Serial.println(WiFi.softAPIP());
  }
  if(loopCounter == TICK_HEAP_DEBUG) {
    Serial.print("free heap="); Serial.println(ESP.getFreeHeap());
  }

  if(loopCounter++ == LOOP_ROLLOVER) loopCounter = 0;
}

/////////////////////////////////////////////////////////////////////////////
// wifi and web server management stuff /////////////////////////////////////
void startAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID);
  Serial.println("Soft AP started");
}
void startDNS() {
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("DNS server started");
}
void printIPs() {
  Serial.print("AP SSID: ");
  Serial.print(apSSID);
  Serial.print("; IP address(es): local=");
  Serial.print(WiFi.localIP());
  Serial.print("; AP=");
  Serial.println(WiFi.softAPIP());
}
void startWebServer() {
  webServer.on("/", handle_root);
  webServer.on("/generate_204", handle_root); // Android support
  webServer.on("/L0", handle_root);
  webServer.on("/L2", handle_root);
  webServer.on("/ALL", handle_root);
  webServer.onNotFound(handleNotFound);

  webServer.on("/wifi", handle_wifi);
  webServer.on("/wifistatus", handle_wifistatus);
  webServer.on("/serverconf", handle_serverconf);
  webServer.on("/wfchz", handle_wfchz);
  webServer.on("/svrchz", handle_svrchz);
  webServer.on("/data", handle_data);
  webServer.on("/actuate", handle_actuate);
  webServer.begin();
  Serial.println("HTTP server started");
}
void handleNotFound() {
  Serial.print("URI Not Found: ");
  Serial.println(webServer.uri());
  // TODO send redirect to /? or just use handle_root?
  webServer.send(200, "text/plain", "URI Not Found");
}
void handle_root() {
  Serial.println("serving page notionally at /");
  String toSend = pageTop;
  toSend += pageTop2;
  toSend += pageDefault;
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
void handle_data() {
  Serial.println("serving page at /data");
  String toSend = pageTop;
  toSend += ": Sensor Data";
  toSend += pageTop2;
  toSend += "<pre>\n";

  // Serial.print("monitorCursor="); Serial.print(monitorCursor);
  // Serial.print(" monitorSize=");  Serial.println(monitorSize);
  int mSize = monitorSize;
  for(
    int i = monitorCursor - 1, j = 1;
    j <= DATA_ENTRIES && j <= monitorSize;
    i--, j++
  ) {
    // Serial.print("printMonitorEntry(monitorData["); Serial.print(i); 
    // Serial.println("], &toSend)");
    printMonitorEntry(monitorData[i], &toSend);
    toSend += "\n";
    if(i == 0)
      i = MONITOR_POINTS;
  }

  toSend += "</pre>\n";
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
String genAPForm() {
  String f = pageTop;
  f += ": Wifi Config";
  f += pageTop2;
  f += "<h2>Choose a wifi access point to join</h2><p>\n";

  const char *checked = " checked";

  int n = WiFi.scanNetworks();
  Serial.print("scan done: ");
  if(n == 0) {
    Serial.println("no networks found");
    f += "No wifi access points found :-( ";
    f += "<a href='/'>Back</a><br/><a href='/wifi'>Try again?</a></p>\n";
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    f += "<form method='POST' action='wfchz'> ";
    for(int i = 0; i < n; ++i) {
      // print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");

      f.concat("<input type='radio' name='ssid' value='");
      f.concat(WiFi.SSID(i));
      f.concat("'");
      f.concat(checked);
      f.concat(">");
      f.concat(WiFi.SSID(i));
      f.concat("<br/>\n");
      checked = "";
    }
    f += "<br/>Pass key: <input type='textarea' name='key'><br/><br/> ";
    f += "<input type='submit' value='Submit'></form></p>";
  }

  f += pageFooter;
  return f;
}
void handle_wifi() {
  Serial.println("serving page at /wifi");
  String toSend = genAPForm();
  webServer.send(200, "text/html", toSend);
}
void handle_wifistatus() {
  Serial.println("serving page at /wifistatus");

  String toSend = pageTop;
  toSend += ": Wifi Status";
  toSend += pageTop2;
  toSend += "\n<h2>Wifi Status</h2><p><ul>\n";

  toSend += "\n<li>SSID: ";
  toSend += WiFi.SSID();
  toSend += "</li>";
  toSend += "\n<li>Status: ";
  switch(WiFi.status()) {
    case WL_IDLE_STATUS:
      toSend += "WL_IDLE_STATUS</li>"; break;
    case WL_NO_SSID_AVAIL:
      toSend += "WL_NO_SSID_AVAIL</li>"; break;
    case WL_SCAN_COMPLETED:
      toSend += "WL_SCAN_COMPLETED</li>"; break;
    case WL_CONNECTED:
      toSend += "WL_CONNECTED</li>"; break;
    case WL_CONNECT_FAILED:
      toSend += "WL_CONNECT_FAILED</li>"; break;
    case WL_CONNECTION_LOST:
      toSend += "WL_CONNECTION_LOST</li>"; break;
    case WL_DISCONNECTED:
      toSend += "WL_DISCONNECTED</li>"; break;
    default:
       toSend += "unknown</li>";
  }

  toSend += "\n<li>Local IP: ";   toSend += ip2str(WiFi.localIP());
  toSend += "</li>\n";
  toSend += "\n<li>Soft AP IP: "; toSend += ip2str(WiFi.softAPIP());
  toSend += "</li>\n";
  toSend += "\n<li>AP SSID name: "; toSend += apSSID;
  toSend += "</li>\n";

  toSend += "</ul></p>";

  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
void handle_wfchz() {
  Serial.println("serving page at /wfchz");
  String toSend = pageTop;
  toSend += ": joining wifi network";
  toSend += pageTop2;
  String ssid = "";
  String key = "";

  for(uint8_t i = 0; i < webServer.args(); i++ ) {
    // Serial.println(" " + webServer.argName(i) + ": " + webServer.arg(i));
    if(webServer.argName(i) == "ssid")
      ssid = webServer.arg(i);
    else if(webServer.argName(i) == "key")
      key = webServer.arg(i);
  }

  if(ssid == "") {
    toSend += "<h2>Ooops, no SSID...?</h2>";
    toSend += "<p>Looks like a bug :-(</p>";
  } else {
    toSend += "<h2>Done! Now trying to join network...</h2>";
    toSend += "<p>Check <a href='/wifistatus'>wifi status here</a>.</p>";
    char ssidchars[sizeof(ssid)];
    char keychars[sizeof(key)];
    ssid.toCharArray(ssidchars, sizeof(ssid));
    key.toCharArray(keychars, sizeof(key));
    WiFi.begin(ssidchars, keychars);

    // TODO causes reset in /wifistatus
    /*
    if(WiFi.hostname("waterelf"))
      Serial.println("set hostname succeeded");
    else
      Serial.println("set hostname failed");
    */
  }

  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
String genServerConfForm() {
  String f = pageTop;
  f += ": Server Config";
  f += pageTop2;
  f += "<h2>Configure a server</h2><p>\n";

  f += "<form method='POST' action='svrchz'> ";
  f += "<br/>Local server IP address: ";
  f += "<input type='textarea' name='svraddr'><br/><br/> ";
  f += "Sharing on WeGrow.social: <br/>";
  f += "on <input type='radio' name='wegrow' value='on' checked>\n";
  f += "off <input type='radio' name='wegrow' value='off'>\n";
  f += "<input type='submit' value='Submit'></form></p>";

  f += pageFooter;
  return f;
}
void handle_serverconf() {
  Serial.println("serving page at /serverconf");
  String toSend = genServerConfForm();
  webServer.send(200, "text/html", toSend);
}
void handle_svrchz() {
  Serial.println("serving page at /svrchz");
  String toSend = pageTop;
  toSend += ": server configured";
  toSend += pageTop2;

  for(uint8_t i = 0; i < webServer.args(); i++) {
    if(webServer.argName(i) == "svraddr") {
      svrAddr = webServer.arg(i);
      toSend += "<h2>Added local server config...</h2>";
      toSend += "<p>...at ";
      toSend += svrAddr;
      toSend += "</p>";
    }

    // TODO else if got "wegrow" turn cloud server share on
  }

  // TODO persist the config

  // TODO some way of verifying if server config worked

  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
void handle_actuate() {
  Serial.println("serving page at /actuate");
  String toSend = pageTop;
  toSend += ": Setting Actuator";
  toSend += pageTop2;

  boolean newState = false;
  for(uint8_t i = 0; i < webServer.args(); i++ ) {
    if(webServer.argName(i) == "state") {
      if(webServer.arg(i) == "on")
        newState = true;
    }
  }

  // now we trigger the 433 transmitter
  if(newState == true){
    mySwitch.switchOn(4, 2);
  } else {
    mySwitch.switchOff(4, 2);
  }

  toSend += "<h2>Actuator triggered</h2>\n";
  toSend += "<p>(New state is ";
  toSend += (newState) ? "on" : "off";
  toSend += ".)</p>\n";
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}

/////////////////////////////////////////////////////////////////////////////
// sensor/actuator stuff ////////////////////////////////////////////////////
void startPeripherals() {
  Serial.println("startPeripherals");
  mySwitch.enableTransmit(13);   // RC transmitter is connected to Pin 13

  tempSensor.begin();     // start the onewire temperature sensor
  if(tempSensor.getDeviceCount()==1) {
    GOT_TEMP_SENSOR = true;
    tempSensor.getAddress(tempAddr, 0);
    tempSensor.setResolution(tempAddr, 12); // 12 bit res (DS18B20 does 9-12)
  }
  
  dht.begin();    // Start the humidity and air temperature sensor
  float airHumid = dht.readHumidity();
  float airCelsius = dht.readTemperature();
  if (isnan(airHumid) || isnan(airCelsius)) {
    Serial.println("failed to find Humidity sensor");
  } else {
    GOT_HUMID_SENSOR = true;
  }

  Wire.begin();
  byte error;
  Wire.beginTransmission(0x29);
  error = Wire.endTransmission();
  if(error==0){
    GOT_LIGHT_SENSOR = true;
    tsl.begin();  // startup light sensor
    // can change gain of light sensor on the fly, to adapt 
    // brighter/dimmer light situations
    // tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    tsl.setGain(TSL2591_GAIN_MED);       // 25x gain
    // tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
    // changing the integration time gives you a longer time over which to
    // sense light longer timelines are slower, but are good in very low light
    // situtations!
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS); // shortest (bright)
    tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
    // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); // longest (dim)
  }
  
  Wire.begin();
  Wire.beginTransmission(0x4D);
  error = Wire.endTransmission();
  if(error==0){
    GOT_PH_SENSOR = true;
    Serial.println("Found pH sensor");
  }
}
void updateSensorData(monitor_t *monitorData) {
  // Serial.print("monitorCursor = "); Serial.print(monitorCursor);
  // Serial.print(" monitorSize = ");  Serial.println(monitorSize);

  monitor_t* now = &monitorData[monitorCursor];
  if(monitorSize < MONITOR_POINTS)
    monitorSize++;
  now->timestamp = millis();
  if(GOT_TEMP_SENSOR)
    getTemperature(&(now->waterCelsius));

  if(GOT_HUMID_SENSOR)
    getHumidity(&now->airCelsius, &now->airHumid);
    
  if(GOT_LIGHT_SENSOR)
    getLight(&now->lux);

  if(SEND_DATA) postSensorData(&monitorData[monitorCursor]);
    
  if(++monitorCursor == MONITOR_POINTS)
    monitorCursor = 0;
}
void postSensorData(monitor_t *monitorData) {
  Serial.println("postSensorData");

  // create a JSON form
  String jsonBuf = "";
  jsonMonitorEntry(monitorData, &jsonBuf);
  String envelope = "POST /fishydata HTTP/1.1\n";
  envelope += "Content-Type: application/json\n";
  envelope += "Content-Length: " ;
  envelope += jsonBuf.length();
  envelope += "\nConnection: close\n\n";
  envelope += jsonBuf;
  Serial.println(envelope);

  //IPAddress couchServer(10,0,0,24);
  WiFiClient couchClient;
  if(couchClient.connect(svrAddr.c_str(), 5984)) {
    Serial.print(svrAddr);
    Serial.println(" - connected as couch server");
    couchClient.print(envelope);
  } else {
    Serial.print(svrAddr);
    Serial.println(" - no couch server");
  }

  return;
}
void jsonMonitorEntry(monitor_t *m, String* buf) {
  buf->concat("{ ");
  buf->concat("\"timestamp\": ");
  buf->concat(m->timestamp);
  if(GOT_TEMP_SENSOR){
    buf->concat(", ");
    buf->concat("\"waterTemp\": ");
    buf->concat(m->waterCelsius);
  }
  if(GOT_HUMID_SENSOR){  
    buf->concat(", ");
    buf->concat("\"airTemp\": ");
    buf->concat(m->airCelsius);
    buf->concat(", ");
    buf->concat("\"humidity\": ");
    buf->concat(m->airHumid);
  }
  if(GOT_LIGHT_SENSOR){
    buf->concat(", ");
    buf->concat(", \"lux\": ");
    buf->concat(m->lux);
  }
  buf->concat(" }");
}
void printMonitorEntry(monitor_t m, String* buf) {
  buf->concat("{ ");
  buf->concat("\"timestamp\": ");
  buf->concat(m.timestamp);
  if(GOT_TEMP_SENSOR){
    buf->concat(", Water Temp: ");
    buf->concat(m.waterCelsius);
    buf->concat(" °C");
  }
  if(GOT_HUMID_SENSOR){  
    buf->concat(", Air Temp: ");
    buf->concat(m.airCelsius);
    buf->concat(" °C"); 
    buf->concat(", Humidity: ");
    buf->concat(m.airHumid);
    buf->concat(" %RH");
  }
  if(GOT_LIGHT_SENSOR){
    buf->concat(", Light: ");
    buf->concat(m.lux);
    buf->concat(" lux");
  }
}
void getTemperature(float* waterCelsius) {
  tempSensor.requestTemperatures(); // send command to get temperatures
  (*waterCelsius) = tempSensor.getTempC(tempAddr);
  Serial.print("Temp: ");
  Serial.print(*waterCelsius);
  Serial.println(" C, ");
  return;
}
void getHumidity(float* airCelsius, float* airHumid) {
  float _airCelsius = *airCelsius;
  float _airHumid = *airHumid;
  _airCelsius = dht.readTemperature();
  _airHumid = dht.readHumidity();
  Serial.print("Air Temp: ");
  Serial.print(_airCelsius);
  Serial.print(" C, ");
  Serial.print("Humidity: ");
  Serial.print(_airHumid);
  Serial.println(" %RH, ");
  *airCelsius = _airCelsius;
  *airHumid = _airHumid;
  return;
}
void getLight(uint16_t* lux) {
  uint16_t _lux = *lux;
  sensors_event_t event;
  tsl.getEvent(&event);
  _lux = event.light; 
  Serial.print("Light: ");
  Serial.print(_lux);
  Serial.println(" Lux");
  *lux = _lux;
  return;
}

/////////////////////////////////////////////////////////////////////////////
// misc utils ///////////////////////////////////////////////////////////////
void ledOn() { digitalWrite(BUILTIN_LED, LOW); }
void ledOff() { digitalWrite(BUILTIN_LED, HIGH); }
void blink(int times) {
  ledOff();
  for(int i=0; i<times; i++) {
    ledOn(); delay(300); ledOff(); delay(300);
  }
  ledOff();
}
String ip2str(IPAddress address) {
  return
    String(address[0]) + "." + String(address[1]) + "." + 
    String(address[2]) + "." + String(address[3]);
}
