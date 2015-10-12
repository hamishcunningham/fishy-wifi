/////////////////////////////////////////////////////////////////////////////
// waterelf.ino /////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <OneWire.h>

/////////////////////////////////////////////////////////////////////////////
// misc /////////////////////////////////////////////////////////////////////
const boolean GOT_TEMP_SENSOR = false;

/////////////////////////////////////////////////////////////////////////////
// resource management stuff ////////////////////////////////////////////////
byte loopCounter = 0;
const byte TICK_WIFI_DEBUG = 0;
const byte TICK_HEAP_DEBUG = 0;
const byte TICK_MONITOR = 100;

/////////////////////////////////////////////////////////////////////////////
// wifi management stuff ////////////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
const char* ssid = "WaterElf";

IPAddress couchServer(192,168,1,151);
WiFiClient client;
IPAddress googleServer(216,58,210,78);
WiFiClient googleClient;

/////////////////////////////////////////////////////////////////////////////
// page generation stuff ////////////////////////////////////////////////////
const char* pageTop =
  "<html><head><title>WaterElf Aquaponics Helper version 0000000001";
const char* pageTop2 = "</title>\n"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
  "</head><body>\n";
const char* pageDefault =
  "<h2>Welcome to WaterElf</h2>\n"
  "<h2>Control</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifi'>Join a wifi network</a></li>\n"
  "<li><a href='/wifistatus'>Wifi status</a></li>\n"
  "<li>\n"
    "<form method='POST' action='actuate'>\n"
    "Actuator: "
    "on <input type='radio' name='state' value='on' checked>\n"
    "off <input type='radio' name='state' value='off'>\n"
    "<input type='submit' value='Submit'></form></p>\n"
  "</li>\n"
  "</ul></p>\n"
  "<h2>Monitor</h2>\n"
  "<p><ul>\n"
  "<li><a href='/data'>Sensor data</a></li>\n"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>WaterElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='https://www.fish4tea.net/'>Fish4Tea</a></p></body></html>";

/////////////////////////////////////////////////////////////////////////////
// data monitoring stuff
const int MONITOR_POINTS = 60; // number of data points to store
struct monitor_t {
  unsigned long timestamp;
  float celsius;
  float fahrenheit;
};
monitor_t monitorData[MONITOR_POINTS];
int monitorCursor = 0;
int monitorSize = 0;
const int DATA_ENTRIES = 30; // size of /data report; must be <= MONITOR_POINTS
void updateSensorData(monitor_t *monitorData);
void postSensorData(monitor_t *monitorData);
void printMonitorEntry(monitor_t m, String* buf);

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
OneWire ds(2); // DS1820 on pin 2 (a 4.7K resistor is necessary)
void getTemperature(float* celsius, float* fahrenheit);

/////////////////////////////////////////////////////////////////////////////
// misc utils ///////////////////////////////////////////////////////////////
int ledState = LOW;     
void ledOn();
void ledOff();
String ip2str(IPAddress address);

/////////////////////////////////////////////////////////////////////////////
// setup ////////////////////////////////////////////////////////////////////
void setup() {
  // huzzah LED
  pinMode(0, OUTPUT);
  blink(3);
  Serial.begin(115200);

  // TODO don't do this if wifi config'd and connected
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);

  Serial.print("SSID: ");
  Serial.print(ssid);
  Serial.print("; IP address(es): local=");
  Serial.print(WiFi.localIP());
  Serial.print("; AP=");
  Serial.println(WiFi.softAPIP());

  // TODO don't do this if wifi config'd and connected
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("DNS server started");

  webServer.on("/", handle_root);
  webServer.on("/generate_204", handle_root); // Android support
  webServer.on("/L0", handle_root);
  webServer.on("/L2", handle_root);
  webServer.on("/ALL", handle_root);
  webServer.onNotFound(handleNotFound);

  webServer.on("/wifi", handle_wifi);
  webServer.on("/wifistatus", handle_wifistatus);
  webServer.on("/chz", handle_chz);
  webServer.on("/data", handle_data);
  webServer.on("/actuate", handle_actuate);
  webServer.begin();
  Serial.println("HTTP server started");

  // TODO if this keeps failing move it into loop? or need to pick up config
  // from ESP ROM somehow, and do WiFi.begin?
  if(WiFi.hostname("waterelf"))
    Serial.println("set hostname succeeded");
  else
    Serial.println("set hostname failed");
}

/////////////////////////////////////////////////////////////////////////////
// looooooooooooooooooooop //////////////////////////////////////////////////
void loop() {
  dnsServer.processNextRequest(); // TODO don't do this if wifi config'd and connected
  webServer.handleClient();
  delay(100);

  if(loopCounter == TICK_MONITOR) {
    // ledOn();
    updateSensorData(monitorData);
    delay(100);
    // ledOff();
    postSensorData(monitorData);
  } 
  if(loopCounter == TICK_WIFI_DEBUG) {
    Serial.print("SSID: "); Serial.print(ssid);
    Serial.print("; IP address(es): local="); Serial.print(WiFi.localIP());
    Serial.print("; AP="); Serial.println(WiFi.softAPIP());
  }
  if(loopCounter == TICK_HEAP_DEBUG) {
    Serial.print("free heap="); Serial.println(ESP.getFreeHeap());
  }
  loopCounter++;
}

/////////////////////////////////////////////////////////////////////////////
// wifi management stuff ////////////////////////////////////////////////////
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
  delay(100);
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
  delay(100);
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
    f += "<form method='POST' action='chz'> ";
    for(int i = 0; i < n; ++i) {
      // print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);

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
  delay(100);
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

  toSend += "</ul></p>";

  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
  delay(100);
}

void handle_chz() {
  Serial.println("serving page at /chz");
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
    toSend += "<h2>Done! Joining...</h2>";
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
  delay(100);
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

  // TODO trigger the 433 transmitter

  toSend += "<h2>Actuator triggered</h2>\n";
  toSend += "<p>(New state is ";
  toSend += (newState) ? "on" : "off";
  toSend += ".)</p>\n";
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
  delay(100);
}

/////////////////////////////////////////////////////////////////////////////
// sensor stuff /////////////////////////////////////////////////////////////
void updateSensorData(monitor_t *monitorData) {
  // Serial.print("monitorCursor = "); Serial.print(monitorCursor);
  // Serial.print(" monitorSize = ");  Serial.println(monitorSize);

  monitor_t* now = &monitorData[monitorCursor];
  if(monitorSize < MONITOR_POINTS)
    monitorSize++;
  now->timestamp = millis();
  if(GOT_TEMP_SENSOR)
    getTemperature(&now->celsius, &now->fahrenheit);

  if(++monitorCursor == MONITOR_POINTS)
    monitorCursor = 0;
}

void postSensorData(monitor_t *monitorData) {
  // create a JSON form and ping 
  // 192.168.1.151:5984
  Serial.println("postSensorData");

  if(googleClient.connect(googleServer, 80)) {
    Serial.println("connected to google server");
  } 
  if(client.connect(couchServer, 5984)) {
    Serial.println("connected to server");
    client.println("POST /fishydata HTTP/1.1");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println("{ \"key\": \"value\" }");
  } else {
    Serial.println("no couch server");
  }

  client.stop();
  return;
}

void printMonitorEntry(monitor_t m, String* buf) {
  buf->concat("timestamp: ");
  buf->concat(m.timestamp);
  buf->concat(", celsius: ");
  buf->concat(m.celsius);
  buf->concat(", fahrenheit: ");
  buf->concat(m.fahrenheit);
}

void getTemperature(float* celsius, float* fahrenheit) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float _celsius = *celsius;
  float _fahrenheit = *fahrenheit;
  Serial.println("getTemperature()...");

  while(!ds.search(addr)) {
    Serial.print("  no addresses; resetting. ");
    ds.reset_search();
    delay(250);
  }

  if(OneWire::crc8(addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!");
      return;
  }

  // the first ROM byte indicates which chip
  switch(addr[0]) {
    case 0x10:
      Serial.print("chip=DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.print("chip=DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.print("chip=DS1822");
      type_s = 0;
      break;
    default:
      Serial.print("device is not a DS18x20 family device :-(");
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);    // start conversion, with parasite power on at the end

  delay(1000);          // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);       // read scratchpad

  Serial.print("; data=");
  Serial.print(present, HEX);
  Serial.print(" ");
  for(i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("; CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);

  // convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor
  int16_t raw = (data[1] << 8) | data[0];
  if(type_s) {
    raw = raw << 3;     // 9 bit resolution default
    if(data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if(cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if(cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if(cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  _celsius = (float) raw / 16.0;
  _fahrenheit = _celsius * 1.8 + 32.0;
  Serial.print("; temp: ");
  Serial.print(_celsius);
  Serial.print(" C, ");
  Serial.print(_fahrenheit);
  Serial.println(" F");

  *celsius = _celsius;
  *fahrenheit = _fahrenheit;
  return;
}

/////////////////////////////////////////////////////////////////////////////
// misc utils ///////////////////////////////////////////////////////////////
void ledOn() {
  if(ledState == LOW) {
    ledState = HIGH;
    digitalWrite(BUILTIN_LED, ledState);
  }
}
void ledOff() {
  if(ledState == HIGH) {
    ledState = LOW;
    digitalWrite(BUILTIN_LED, ledState);
  }
}
void blink(int times) {
  ledOff();
  delay(10);
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
