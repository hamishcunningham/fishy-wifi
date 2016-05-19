/////////////////////////////////////////////////////////////////////////////
// waterelf.ino /////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "./DNSServer.h"      // patched lib
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include <RCSwitch.h>
#include "Adafruit_MCP23008.h"

/////////////////////////////////////////////////////////////////////////////
// resource management stuff ////////////////////////////////////////////////
int loopCounter = 0;
const int LOOP_ROLLOVER = 5000; // how many loops per action slice
const int TICK_MONITOR = 0;
const int TICK_WIFI_DEBUG = 500;
const int TICK_POST_DEBUG = 200;
const int TICK_HEAP_DEBUG = 1000;

/////////////////////////////////////////////////////////////////////////////
// wifi management stuff ////////////////////////////////////////////////////
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 99, 1);
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
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;font-size: 150%;}</style>"
  "</head><body>\n";
const char* pageDefault = // TODO build the growbeds according to their num
  "<h2>Welcome to WaterElf</h2>\n"
  "<h2>Control</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifi'>Join a wifi network</a></li>\n"
  "<li><a href='/serverconf'>Configure data sharing</a></li>\n"
  "<li>"
    "<form method='POST' action='valve1'>\n"
    "Growbed 1: "
    "fill <input type='radio' name='state' value='on'>\n"
    "drain <input type='radio' name='state' value='off'>\n"
    "<input type='submit' value='Submit'></form>\n"
  "</li>"
  "<li>"
    "<form method='POST' action='valve2'>\n"
    "Growbed 2: "
    "fill <input type='radio' name='state' value='on'>\n"
    "drain <input type='radio' name='state' value='off'>\n"
    "<input type='submit' value='Submit'></form>\n"
  "</li>"
  "<li>"
    "<form method='POST' action='valve3'>\n"
    "Growbed 3: "
    "fill <input type='radio' name='state' value='on'>\n"
    "drain <input type='radio' name='state' value='off'>\n"
    "<input type='submit' value='Submit'></form>\n"
  "</li>"
  "<li>\n"
    "<form method='POST' action='actuate'>\n"
    "Power: "
    "on <input type='radio' name='state' value='on'>\n"
    "off <input type='radio' name='state' value='off'>\n"
    "<input type='submit' value='Submit'></form>\n"
  "</li>"
  "</ul></p>\n"
  "<h2>Monitor</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifistatus'>Wifi status</a></li>\n"
  "<li><a href='/data'>Sensor data</a></li>\n"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>WaterElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='https://wegrow.social/'>WeGrow</a></p></body></html>";

/////////////////////////////////////////////////////////////////////////////
// data monitoring stuff ////////////////////////////////////////////////////
const boolean SEND_DATA = true;  // turn off posting of data if required here
const int MONITOR_POINTS = 60; // number of data points to store
typedef struct {
  unsigned long timestamp;
  float waterCelsius;
  float airCelsius;
  float airHumid;
  uint16_t lux;
  float pH;
  long waterLevel1;
  long waterLevel2;
  long waterLevel3;
  // TODO add last reboot
  // TODO add free memory?
} monitor_t;
monitor_t monitorData[MONITOR_POINTS];
int monitorCursor = 0;
int monitorSize = 0;
const int DATA_ENTRIES = 4; // size of /data rpt; must be <= MONITOR_POINTS
void updateSensorData(monitor_t *monitorData);
void postSensorData(monitor_t *monitorData);
void printMonitorEntry(monitor_t m, String* buf);
void formatMonitorEntry(monitor_t *m, String* buf, bool JSON);

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
OneWire ds(2); // DS1820 on pin 2 (a 4.7K resistor is necessary)
DallasTemperature tempSensor(&ds);  // pass through reference to library
void getTemperature(float* waterCelsius);
boolean GOT_TEMP_SENSOR = false; // we'll change later if we detect sensor
DeviceAddress tempAddr; // array to hold device address

/////////////////////////////////////////////////////////////////////////////
// humidity sensor stuff ////////////////////////////////////////////////////
DHT dht(0, DHT22); // what digital pin we're on, plus type DHT22 aka AM2302
boolean GOT_HUMID_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// light sensor stuff ///////////////////////////////////////////////////////
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // sensor id
boolean GOT_LIGHT_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// pH sensor stuff //////////////////////////////////////////////////////////
const byte pH_Add = 0x4D;  // change this to match ph ADC address
int pH7Cal = 2048; // assume ideal probe and amp conditions 1/2 of 4096
int pH4Cal = 1286; // ideal probe slope -> this many 12bit units on 4 scale
float pHStep = 59.16; // ideal probe slope
const float vRef = 4.096; // our vRef into the ADC wont be exact
// since you can run VCC lower than Vref its best to measure and adjust here
const float opampGain = 5.25; //what is our Op-Amps gain (stage 1)
boolean GOT_PH_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// RC switch stuff //////////////////////////////////////////////////////////
RCSwitch mySwitch = RCSwitch();
const int RCSW_CHANNEL = 2; // which 433 channel to use (I-IV)
const int RCSW_HEATER = 2;  // which 433 device to switch (1-4)

/////////////////////////////////////////////////////////////////////////////
// MCP23008 stuff ///////////////////////////////////////////////////////////
Adafruit_MCP23008 mcp; // create object for MCP23008
const int mcpPins[] = { 0, 1, 2, 3, 6, 7, }; // pins to init in setup
const int mcpPinsUsed = 6; // length of mcpPins array
const int valvePinMap[][2] = {
  { -1, -1 }, // there's no valve numbered 0...
  { 0, 2 },   // valve 1: pump on pin 0, solenoid on pin 2
  { 3, 6 },   // valve 2: ...
  { 7, 1 },   // valve 3: ...
};

/////////////////////////////////////////////////////////////////////////////
// level sensing stuff //////////////////////////////////////////////////////
const int LEVEL_TRIG_PIN=12;
const int LEVEL_ECHO_PIN1=13;
const int LEVEL_ECHO_PIN2=14;
const int LEVEL_ECHO_PIN3=16;
boolean GOT_LEVEL_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// valves and flow control //////////////////////////////////////////////////
/*
user settable parameters:
- per elf:
  - number of (controlled) beds
  - cycle length (minutes)
  - max simultaneous drainers
  - min beds wet
  - max beds wet
  - stagger minutes (default: cycle length / num beds)
- per growbed:
  - dry minutes
  - got overflow
  - fill level (cms below ultrasound sensor)

maybe: fixed cycle time, irrespective of actual fill time, and the
differences are absorbed into the dry time?
*/
class Valve { // each valve /////////////////////////////////////////////////
  public:
  static int counter;           // counter for setting valve number
  int number;                   // id of this valve, counting from 1
  char dryMins = 45;            // mins to leave bed drained per cycle
  // in beds with overflows can be 0; else will be cycle time minus fill time
  long startTime = -1;          // when to start cycling (after boot)    
  bool running = false;         // true when cycling started
  bool gotOverflow = false;     // does the growbed have an overflow?
  char fillLevel = 5;           // cms below the level sensor: drain point

  Valve() { number = counter++; }
  void stateChange(bool newState) { // turn on or off
    Serial.print("valve ");
    Serial.print(number);
    int pumpMcpPin = valvePinMap[number][0];
    int solenoidMcpPin = valvePinMap[number][1];

    // trigger MOSFETs via the MCP
    if(newState == true){
      mcp.digitalWrite(pumpMcpPin, HIGH);
      mcp.digitalWrite(solenoidMcpPin, HIGH);
      Serial.println(" on");
    } else {
      mcp.digitalWrite(pumpMcpPin, LOW);
      mcp.digitalWrite(solenoidMcpPin, LOW);
      Serial.println(" off");
    }
  }
  void on()  { stateChange(true); } // fill time
  void off() { stateChange(false); }// drain time
  void step(monitor_t* now) {       // check conditions and adjust state
    if(! running) {
      if(startTime >= millis())
        running = true;
      else
        return;
    }
  }
};
int Valve::counter = 1;             // definition (the above only declares)

class FlowController { // the set of valves and their config ////////////////
  public:
  int numValves = 3;                // WARNING! call init if resetting!
  char cycleMinutes = 60;           // how long is a flood/drain cycle?
  char maxSimultaneousDrainers = 1; // how many beds can drain simultaneously
  char minBedsWet = 1;              // min beds that are full or filling
  char maxBedsWet = 2;              // max beds that are full or filling
  char staggerMins = 10;            // gap to leave between valve startups
  Valve* valves = 0;                // the valves and their states

  FlowController() { init(); }
  int getStaggerMillis() { return staggerMins * 60 * 1000; }
  void init() {
    Valve::counter = 1;
    if(valves != 0) delete(valves);
    valves = new Valve[numValves];

    long t = millis();
    long nextCycleStart = t;
    for(int i = 0; i < numValves; i++) {
      valves[i].startTime = nextCycleStart;
      nextCycleStart += getStaggerMillis();
    }
  }
  void step(monitor_t* now) {
    for(int i = 0; i < numValves; i++)
      valves[i].step(now);
  }
};
FlowController flowController;

/////////////////////////////////////////////////////////////////////////////
// config utils /////////////////////////////////////////////////////////////
boolean getCloudShare();
void setCloudShare(boolean b);
String getSvrAddr();
void setSvrAddr(String s);

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
  blink(3); // signal we're starting setup

  // read persistent config
  SPIFFS.begin();
  svrAddr = getSvrAddr();

  startPeripherals();
  startAP();
  printIPs();
  startDNS();

  startWebServer();

  mcp.begin();      // use default address 0 for mcp23008
  for(int i = 0; i < mcpPinsUsed; i++)
    mcp.pinMode(mcpPins[i], OUTPUT);

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

  if(loopCounter == TICK_MONITOR) { // monitor levels, step valves, push data
    monitor_t* now = &monitorData[monitorCursor];
    if(monitorSize < MONITOR_POINTS)
      monitorSize++;
    now->timestamp = millis();
    if(GOT_TEMP_SENSOR) {
      getTemperature(&now->waterCelsius);               yield();
    }
    if(GOT_HUMID_SENSOR) {
      getHumidity(&now->airCelsius, &now->airHumid);    yield();
    }
    if(GOT_LIGHT_SENSOR) { getLight(&now->lux);         yield(); }
    if(GOT_PH_SENSOR) { getPH(&now->pH);                yield(); }
    if(GOT_LEVEL_SENSOR) {
      getLevel(LEVEL_ECHO_PIN1, &now->waterLevel1);     yield();
      getLevel(LEVEL_ECHO_PIN2, &now->waterLevel2);     yield();
      getLevel(LEVEL_ECHO_PIN3, &now->waterLevel3);     yield();
    }

    flowController.step(now); yield();  // set valves on and off etc.
    if(SEND_DATA) {                     // push data to the cloud
      postSensorData(&monitorData[monitorCursor]); yield();
    }
      
    if(++monitorCursor == MONITOR_POINTS)
      monitorCursor = 0;
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
  webServer.on("/valve1", handle_valve1);
  webServer.on("/valve2", handle_valve2);
  webServer.on("/valve3", handle_valve3);
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
  toSend += "\n<h2>Sensor Data</h2><p><pre>\n";

  int mSize = monitorSize;
  for(
    int i = monitorCursor - 1, j = 1;
    j <= DATA_ENTRIES && j <= monitorSize;
    i--, j++
  ) {
    formatMonitorEntry(&monitorData[i], &toSend, false);
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
  f += "<h2>Choose a wifi access point to join</h2>\n";
  f += "<h3>Signal strength in brackets, lower is better</h3><p>\n";
  
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
      f.concat(" (");
      f.concat(WiFi.RSSI(i));
      f.concat(" dBm)");
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
    //Serial.println(" " + webServer.argName(i) + ": " + webServer.arg(i));
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
    char ssidchars[ssid.length()+1];
    char keychars[key.length()+1];
    ssid.toCharArray(ssidchars, ssid.length()+1);
    key.toCharArray(keychars, key.length()+1);
    WiFi.begin(ssidchars, keychars);
  }

  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
String genServerConfForm() {
  String f = pageTop;
  f += ": Server Config";
  f += pageTop2;
  f += "<h2>Configure data sharing</h2><p>\n";

  f += "<form method='POST' action='svrchz'> ";
  f += "<br/>Local server IP address: ";
  f += "<input type='textarea' name='svraddr'><br/><br/> ";
  //f += "Sharing on WeGrow.social: ";
  // TODO set checked dependent on getCloudShare()
  //f += "on <input type='radio' name='wegrow' value='on' checked>\n";
  //f += "off <input type='radio' name='wegrow' value='off'><br/><br/>\n";
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
  toSend += ": data sharing configured";
  toSend += pageTop2;

  boolean cloudShare = false;
  for(uint8_t i = 0; i < webServer.args(); i++) {
    if(webServer.argName(i) == "svraddr") {
      svrAddr = webServer.arg(i);
      toSend += "<h2>Added local server config...</h2>";
      toSend += "<p>...at ";
      toSend += svrAddr;
      toSend += "</p>";
    } else if(webServer.argName(i) == "key") {
      if(webServer.arg(i) == "on")
        cloudShare = true;
    }
  }

  // persist the config
  if(svrAddr.length() > 0) setSvrAddr(svrAddr);
  setCloudShare(cloudShare);

  // TODO some way of verifying if server config worked
  // add srvstatus, or roll that into wifistatus, or...?

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
    mySwitch.switchOn(RCSW_CHANNEL, RCSW_HEATER);
    Serial.println("Actuator on");
  } else {
    mySwitch.switchOff(RCSW_CHANNEL, RCSW_HEATER);
    Serial.println("Actuator off");
  }

  toSend += "<h2>Actuator triggered</h2>\n";
  toSend += "<p>(New state should be ";
  toSend += (newState) ? "on" : "off";
  toSend += ".)</p>\n";
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
void handle_valve1() { handle_valve(0); } // valves in the UI are...
void handle_valve2() { handle_valve(1); } // ...numbered from 1, but...
void handle_valve3() { handle_valve(2); } // ...from 0 in the FlowController
void handle_valve(int valveNum) {
  Serial.print("serving page at /valve");
  Serial.println(valveNum + 1);
  String toSend = pageTop;
  toSend += ": Setting Water Valve ";
  toSend += valveNum + 1;
  toSend += pageTop2;

  boolean newState = false;
  for(uint8_t i = 0; i < webServer.args(); i++ ) {
    if(webServer.argName(i) == "state") {
      if(webServer.arg(i) == "on")
        newState = true;
    }
  }
  flowController.valves[valveNum].stateChange(newState);

  toSend += "<h2>Water Valve ";
  toSend += valveNum + 1;
  toSend += " triggered</h2>\n";
  toSend += "<p>(New state should be ";
  toSend += (newState) ? "on" : "off";
  toSend += ".)</p>\n";
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}

/////////////////////////////////////////////////////////////////////////////
// sensor/actuator stuff ////////////////////////////////////////////////////
void startPeripherals() {
  Serial.println("\nstartPeripherals...");
  mySwitch.enableTransmit(15);   // RC transmitter is connected to Pin 15

  tempSensor.begin();     // start the onewire temperature sensor
  if(tempSensor.getDeviceCount()==1) {
    GOT_TEMP_SENSOR = true;
    tempSensor.getAddress(tempAddr, 0);
    tempSensor.setResolution(tempAddr, 12); // 12 bit res (DS18B20 does 9-12)
  }
  
  dht.begin();    // start the humidity and air temperature sensor
  float airHumid = dht.readHumidity();
  float airCelsius = dht.readTemperature();
  if (isnan(airHumid) || isnan(airCelsius)) {
    Serial.println("failed to find humidity sensor");
  } else {
    GOT_HUMID_SENSOR = true;
  }

  // configure the level sensors
  pinMode(LEVEL_TRIG_PIN, OUTPUT);
  pinMode(LEVEL_ECHO_PIN1, INPUT);
  pinMode(LEVEL_ECHO_PIN2, INPUT);
  pinMode(LEVEL_ECHO_PIN3, INPUT);
  GOT_LEVEL_SENSOR = true;

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
  
  //Wire.begin();
  Wire.beginTransmission(pH_Add);
  error = Wire.endTransmission();
  if(error==0){
    GOT_PH_SENSOR = true;
    Serial.println("Found pH sensor");
  }
}
void postSensorData(monitor_t *monitorData) {
  //Serial.println("\npostSensorData");

  // create a JSON form
  String jsonBuf = "";
  formatMonitorEntry(monitorData, &jsonBuf, true);
  String envelope = "POST /fishydata HTTP/1.1\n";
  envelope += "Content-Type: application/json\n";
  envelope += "Content-Length: " ;
  envelope += jsonBuf.length();
  envelope += "\nConnection: close\n\n";
  envelope += jsonBuf;
  //Serial.println(envelope);
  
  WiFiClient couchClient;
  if(couchClient.connect(svrAddr.c_str(), 5984)) {
    Serial.print(svrAddr);
    Serial.println(" - connected as couch server");
    couchClient.print(envelope);
  } else {
    Serial.print(svrAddr);
    Serial.println(" - no couch server");
  }

  Serial.println("");
  return;
}
void formatMonitorEntry(monitor_t *m, String* buf, bool JSON) {
  if(JSON) buf->concat("{ ");
  buf->concat("~timestamp~+ ");
  buf->concat(m->timestamp);
  if(GOT_TEMP_SENSOR){
    buf->concat("^ ~waterTemp~+ ");
    buf->concat(m->waterCelsius);
    if(! JSON) buf->concat("\t\u00B0C");
  }
  if(GOT_HUMID_SENSOR){  
    buf->concat("^ ~airTemp~+ ");
    buf->concat(m->airCelsius);
    if(! JSON) buf->concat("\t\u00B0C");
    buf->concat("^ ~humidity~+ ");
    buf->concat(m->airHumid);
    if(! JSON) buf->concat("\t%RH");
  }
  if(GOT_LIGHT_SENSOR){
    buf->concat("^ ~light~+ ");
    buf->concat(m->lux);
    if(! JSON) buf->concat("\tlux");
  }
  if(GOT_PH_SENSOR){
    buf->concat("^ ~pH~+ ");
    if(! JSON) buf->concat("\t ");
    buf->concat(m->pH);
  }
  if(GOT_LEVEL_SENSOR){
    buf->concat("^ ~waterLevel1~+ "); buf->concat(m->waterLevel1);
    if(! JSON) buf->concat("\tcm");
    buf->concat("^ ~waterLevel2~+ "); buf->concat(m->waterLevel2);
    if(! JSON) buf->concat("\tcm");
    buf->concat("^ ~waterLevel3~+ "); buf->concat(m->waterLevel3);
    if(! JSON) buf->concat("\tcm");
  }
  if(JSON) {
    buf->concat(" }");
    buf->replace('~', '"');
    buf->replace('^', ',');
    buf->replace('+', ':');
  } else { // remove quotes and commas
    buf->replace('~', ' ');
    buf->replace('^', '\n');
    buf->replace('+', '\t');
    buf->concat("\n");
  }
}
void getTemperature(float* waterCelsius) {
  tempSensor.requestTemperatures(); // send command to get temperatures
  (*waterCelsius) = tempSensor.getTempC(tempAddr);
  Serial.print("Water temp: ");
  Serial.print(*waterCelsius);
  Serial.println(" C, ");
  return;
}
void getHumidity(float* airCelsius, float* airHumid) {
  (*airCelsius) = dht.readTemperature();
  (*airHumid) = dht.readHumidity();
  Serial.print("Air Temp: ");
  Serial.print(*airCelsius);
  Serial.print(" C, ");
  Serial.print("Humidity: ");
  Serial.print(*airHumid);
  Serial.println(" %RH, ");
  return;
}
void getLight(uint16_t* lux) {
  sensors_event_t event;
  tsl.getEvent(&event);
  (*lux) = event.light; 
  Serial.print("Light: ");
  Serial.print(*lux);
  Serial.println(" Lux");
  return;
}
void getPH(float* pH) {
// this is our I2C ADC interface section
// assign 2 BYTES variables to capture the LSB & MSB (or Hi Low in this case)
  byte adc_high;
  byte adc_low;
  // we'll assemble the 2 in this variable
  int adc_result;
  
  Wire.requestFrom(pH_Add, 2); // requests 2 bytes
  while(Wire.available() < 2); // while two bytes to receive
  adc_high = Wire.read();      // set...
  adc_low = Wire.read();       // ...them
  // now assemble them, remembering byte maths; a Union works well here too
  adc_result = (adc_high * 256) + adc_low;
  // we have a our Raw pH reading from the ADC; now figure out what the pH is  
  float milliVolts = (((float)adc_result/4096)*vRef)*1000;
  float temp = ((((vRef*(float)pH7Cal)/4096)*1000) - milliVolts) / opampGain;
  (*pH) = 7-(temp/pHStep); 
  Serial.print("pH: ");
  Serial.print(*pH);
  Serial.println(" pH");
  return;
}
void getLevel(int echoPin, long* waterLevel) {
  long duration;
  int TIMEOUT = 15000;                          // how long to wait for pulse

  digitalWrite(LEVEL_TRIG_PIN, LOW);            // prepare for ping
  delayMicroseconds(2);
  digitalWrite(LEVEL_TRIG_PIN, HIGH);           // start ping
  delayMicroseconds(10);                        // allow 10ms ping
  digitalWrite(LEVEL_TRIG_PIN, LOW);            // stop ping
  duration = pulseIn(echoPin, HIGH, TIMEOUT);   // wait for response

  (*waterLevel) = (duration/2) / 29.1;

  Serial.print("Water Level: ");
  Serial.print(*waterLevel);
  Serial.println(" cm, ");
  return;
}

/////////////////////////////////////////////////////////////////////////////
// config utils /////////////////////////////////////////////////////////////
boolean getCloudShare() {
  boolean b = false;
  File f = SPIFFS.open("/cloudShare.txt", "r");
  if(f) {
    b = true;
    f.close();
  }
  return b;
}
void setCloudShare(boolean b) {
  if(b) {
    File f = SPIFFS.open("/cloudShare.txt", "w");
    f.println("");
    f.close();
  } else {
    SPIFFS.remove("/cloudShare.txt");
  }
}
String getSvrAddr() {
  String s = "";
  File f = SPIFFS.open("/svrAddr.txt", "r");
  if(f) {
    s = f.readString();
    s.trim();
    f.close();
  }
  return s;
}
void setSvrAddr(String s) {
  File f = SPIFFS.open("/svrAddr.txt", "w");
  f.println(s);
  f.close();
}

/////////////////////////////////////////////////////////////////////////////
// misc utils ///////////////////////////////////////////////////////////////
void ledOn()  { digitalWrite(BUILTIN_LED, LOW); }
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
