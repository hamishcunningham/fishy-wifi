/////////////////////////////////////////////////////////////////////////////
// waterelf32.ino /////////////////////////////////////////////////////////////
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

/////////////////////////////////////////////////////////////////////////////
// OTA stuff ////////////////////////////////////////////////////////////////
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/////////////////////////////////////////////////////////////////////////////
// SPIFFS editor (ace.js) stuff /////////////////////////////////////////////
#include <SPIFFSEditor.h>
const char* SPIFFS_username = "admin";
const char* SPIFFS_password = "secretpassword";

/////////////////////////////////////////////////////////////////////////////
// resource management stuff ////////////////////////////////////////////////
int loopCounter = 0;
const int LOOP_ROLLOVER = 5000; // how many loops per action slice
const int TICK_MONITOR = 0;
const int TICK_WIFI_DEBUG = 500;
const int TICK_POST_DEBUG = 200;
const int TICK_HEAP_DEBUG = 100000;
bool reboot=false;  

// MAC address ///////////////////////////////////////////////////////////////
char MAC_ADDRESS[13]; // MAC addresses are 12 chars, plus the NULL terminator
char *getMAC(char *);

/////////////////////////////////////////////////////////////////////////////
// wifi management stuff ////////////////////////////////////////////////////
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "joinme.h"
IPAddress apIP(192, 168, 99, 1);
IPAddress netMsk(255, 255, 255, 0);
AsyncWebServer webServer(80);
String apSSIDStr = "WaterElf-" + String(getMAC(MAC_ADDRESS));
const char* apSSID = apSSIDStr.c_str();
String svrAddr = ""; // loaded from config.txt

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
  "<li><a href='/analogconf'>Configure analog sensor</a></li>\n"
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
  "<li><a href='/elfstatus'>Elf status</a></li>\n"
  "<li><a href='/data'>Sensor data</a></li>\n"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>WaterElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='/edit'>SPIFFS file editor</a>&nbsp;&nbsp;&nbsp;"
  "<a href='/waterelfoverview.html'>Documentation</a>&nbsp;&nbsp;&nbsp;"
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
  long waterLevel1; // TODO should be an array
  long waterLevel2;
  long waterLevel3;
  float analog;
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
// misc utils ///////////////////////////////////////////////////////////////
void ledOn();
void ledOff();
String ip2str(IPAddress address);
#define dbg(b, s) if(b) Serial.print(s)
#define dln(b, s) if(b) Serial.println(s)
#define dbf(b, s, v) if(b) Serial.printf(s, v)
#define startupDBG true
#define valveDBG false
#define monitorDBG false
#define netDBG true
#define miscDBG true
#define citsciDBG false
#define analogDBG false

/////////////////////////////////////////////////////////////////////////////
// temperature sensor stuff /////////////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire ds(27); // DS1820 pin (a 4.7K resistor is necessary)
DallasTemperature tempSensor(&ds);  // pass through reference to library
void getTemperature(float* waterCelsius);
boolean GOT_TEMP_SENSOR = false; // we'll change later if we detect sensor
DeviceAddress tempAddr; // array to hold device address

/////////////////////////////////////////////////////////////////////////////
// humidity sensor stuff ////////////////////////////////////////////////////
#include "DHT.h"
DHT dht(33, DHT22); // what digital pin we're on, plus type DHT22 aka AM2302
boolean GOT_HUMID_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// light sensor stuff ///////////////////////////////////////////////////////
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // sensor id
boolean GOT_LIGHT_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// pH sensor stuff //////////////////////////////////////////////////////////
const byte pH_Add = 0x4A;  // change this to match ph ADC address
int pH7Cal = 2048; // assume ideal probe and amp conditions 1/2 of 4096
int pH4Cal = 1286; // ideal probe slope -> this many 12bit units on 4 scale
float pHStep = 59.16; // ideal probe slope
const float vRef = 4.096; // our vRef into the ADC wont be exact
// since you can run VCC lower than Vref its best to measure and adjust here
const float opampGain = 5.25; //what is our Op-Amps gain (stage 1)
boolean GOT_PH_SENSOR = false; // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// RC switch stuff //////////////////////////////////////////////////////////
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
const int RCSW_CHANNEL = 2; // which 433 channel to use (I-IV)
const int RCSW_HEATER = 2;  // which 433 device to switch (1-4)

/////////////////////////////////////////////////////////////////////////////
// Valve stuff ///////////////////////////////////////////////////////////
const int valvePins[] = { 32, 14, 21, }; // pins for valves
const int valvePinsUsed = sizeof(valvePins)/sizeof(int); // length of valvePins array

/////////////////////////////////////////////////////////////////////////////
// level sensing stuff //////////////////////////////////////////////////////
const int LEVEL_TRIG_PIN=26;
const int LEVEL_ECHO_PIN1=39;
const int LEVEL_ECHO_PIN2=34;
const int LEVEL_ECHO_PIN3=25;
boolean GOT_LEVEL_SENSOR = false;  // we'll change later if we detect sensor

/////////////////////////////////////////////////////////////////////////////
// analog sensor stuff //////////////////////////////////////////////////////
#include "EmonLib.h" // Emon Library, see openenergymonitor.org
String ANALOG_SENSOR_NONE = "analog_none";
String ANALOG_SENSOR_MAINS = "analog_mains";
String ANALOG_SENSOR_PRESSURE = "analog_pressure";
String analogSensor; // loaded from config.txt
EnergyMonitor emon1; // instance of energy monitor, for mains current sensor

/////////////////////////////////////////////////////////////////////////////
// valves and flow control //////////////////////////////////////////////////
/*
user settable parameters:
- per elf:
  - number of (controlled) beds
  - cycle length (minutes)
  - max simultaneous drainers (if there may be return pipe contention)
  - min beds wet (to prevent sump overflow)
  - max beds wet (to avoid dry pump)
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
  int dryMins = 45;             // mins to leave bed drained per cycle
  // in beds with overflows can be 0; else will be cycle time minus fill time
  long startTime = -1;          // when to start cycling (after boot)    
  bool gotOverflow = false;     // does the growbed have an overflow?
  int fillLevel = 7;            // cms below the level sensor: drain point
  bool filling = false;         // true when closed / on
  long lastFlip = -1;           // millis at last state change
  int floodMins = 18;

  Valve() { number = counter++; }
  void stateChange(bool newState) { // turn on or off
    dbg(valveDBG, "valve ");
    dbg(valveDBG, number);

    // trigger MOSFETs
    if(newState == true){
      digitalWrite(valvePins[number-1], HIGH);
      filling = true; // TODO if set from UI will interfere with timing?
      dln(valveDBG, " on");
    } else {
      digitalWrite(valvePins[number-1], LOW);
      filling = false;
      dln(valveDBG, " off");
    }
    dbg(valveDBG, "pump Pin = "); dln(valveDBG, valvePins[number-1]);

    lastFlip = millis();
  }
  void on()  { stateChange(true); }  // fill time
  void off() { stateChange(false); } // drain time
  void step(monitor_t* now, char cycleMins) {   // check conditions and adjust
    dbg(valveDBG, "\nvalve[].step - number = "); dln(valveDBG, number-1);
    int t = millis();
    dbg(valveDBG, "t = "); dbg(valveDBG, t);
    dbg(valveDBG, "; filling = "); dln(valveDBG, filling);
    if(filling &&
        (
          ( lastFlip + (floodMins * 60 * 1000) ) <= t  || full(now)
        )
      ) { // flood over
      dbg(valveDBG, "t = "); dln(valveDBG, t);
      dbg(valveDBG, "lastFlip = "); dln(valveDBG, lastFlip);
      dbg(valveDBG, "floodMins = "); dln(valveDBG, floodMins);
      dbg(valveDBG, "floodMins * 60 * 1000 = ");
      dln(valveDBG, floodMins * 60 * 1000);
      off();
    } else if(!filling && ( startTime <= t )) { // time to start filling
      on();
      startTime += (cycleMins * 60 * 1000);
      dbg(valveDBG, "startTime = "); dln(valveDBG, startTime);
    } 
  }
  bool full(monitor_t *now) {
    int l = -1;
    switch(number) {
      case 1: l = now->waterLevel1; break;
      case 2: l = now->waterLevel2; break;
      case 3: l = now->waterLevel3; break;
    }
    dbg(valveDBG, "full? l = "); dln(valveDBG, l);

    return ( l <= fillLevel );
  }
};
int Valve::counter = 1;         // definition (the above only declares)

class FlowController { // the set of valves and their config ////////////////
  public:
  int numValves = valvePinsUsed;         // WARNING! call init if resetting!
  int cycleMins = 30;        // how long is a flood/drain cycle?
  int maxSimultDrainers = 1; // how many beds can drain simultaneously?
  int minBedsWet = 1;        // min beds that are full or filling
  int maxBedsWet = 2;        // max beds that are full or filling
  int staggerMins;           // gap to leave between valve startups
  Valve* valves = 0;         // the valves and their states

  int getStaggerMillis() { return staggerMins * 60 * 1000; }
  void init() {
    Valve::counter = 1;
    if(valves != 0) delete(valves);
    valves = new Valve[numValves];

    // set up staggered valve starts
    staggerMins = cycleMins / numValves;
    dbg(valveDBG, "cycleMins = "); dbg(valveDBG, cycleMins);
    dbg(valveDBG, " staggerMins = "); dln(valveDBG, staggerMins);
    long t = millis();
    dbg(valveDBG, "millis = "); dln(valveDBG, t);
    long nextCycleStart = t;
    for(int i = 0; i < numValves; i++) {
      valves[i].startTime = nextCycleStart;
      dbg(valveDBG, "valves["); dbg(valveDBG, i);
      dbg(valveDBG, "].startTime = "); dln(valveDBG, nextCycleStart);
      nextCycleStart += getStaggerMillis();
    }
  }
  void step(monitor_t* now) {
    for(int i = 0; i < numValves; i++)
      valves[i].step(now, cycleMins);
  }
};
FlowController flowController;

/////////////////////////////////////////////////////////////////////////////
// config utils /////////////////////////////////////////////////////////////
template< typename G> G getConfig( G configName );
template< typename S, typename SS > bool setConfig( S configName, SS configData );

/////////////////////////////////////////////////////////////////////////////
// setup ////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT); // turn built-in LED on
  blink(3);                     // signal we're starting setup
  WiFi.begin();  // lets hope the stored credentials work...

  //ArduinoOTA.setPasswordHash("1dee0c92b097b253f201a7da39dce6df");
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      SPIFFS.end();
    }
    dln(miscDBG, "Start updating " + type);
  })
  .onEnd([]() {
    dln(miscDBG, "\nEnd");
    SPIFFS.begin();  // in case we unmounted it earlier...
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    dbf(miscDBG, "Progress: %u%%\r", (progress / (total / 100)));
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    dbf(miscDBG, "Error[%u]: ", error);
    //Serial.printf("Error[%u]: ", error);
    SPIFFS.begin();  // in case we unmounted it earlier...
    if (error == OTA_AUTH_ERROR) dln(miscDBG, "Auth Failed");
    else if (error == OTA_BEGIN_ERROR) dln(miscDBG, "Begin Failed");
    else if (error == OTA_CONNECT_ERROR)dln(miscDBG, "Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) dln(miscDBG, "Receive Failed");
    else if (error == OTA_END_ERROR) dln(miscDBG, "End Failed");
  });
  ArduinoOTA.begin();
  
  if (!MDNS.begin(apSSID)) {
    dln(startupDBG, "Error setting up mDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  dln(startupDBG, "mDNS responder started");

  // read persistent config
  SPIFFS.begin(true); // passing true triggers format if flash unpartitioned
  analogSensor = getConfig("analogSensorType");
  svrAddr = getConfig("citsciServerAddress");


  // start the sensors, the DNS and webserver, etc.
  startPeripherals();
  joinme_dhcps_hack();
  startAP();
  printIPs();
  joinme_setup(&webServer,WiFi.softAPIP());
  startWebServer();

  // initialise the valve pins and the flow controller
  dln(startupDBG, "doing flow controller and valve init...");

  flowController.init();
  for(int i = 0; i < valvePinsUsed; i++) {
    pinMode(valvePins[i], OUTPUT);
  }
  // try and set the host name
/*
  if(WiFi.hostname("waterelf"))
    dln(netDBG, "set hostname succeeded");
  else
    dln(netDBG, "set hostname failed");
*/
  delay(300); blink(3);         // signal we've finished config
}

/////////////////////////////////////////////////////////////////////////////
// looooooooooooooooooooop //////////////////////////////////////////////////
void loop() {
  joinme_turn();      // keep the access point stuff serviced
  ArduinoOTA.handle();  // and keep the OTA handler serviced
  if(reboot==true) {
    SPIFFS.end();
    ESP.restart();
  }
  if(loopCounter == TICK_MONITOR) { // monitor levels, step valves, push data
    monitor_t* now = &monitorData[monitorCursor];
    if(monitorSize < MONITOR_POINTS)
      monitorSize++;
    now->timestamp = millis();
    if(GOT_TEMP_SENSOR)
      getTemperature(&now->waterCelsius);               
    if(GOT_HUMID_SENSOR)
      getHumidity(&now->airCelsius, &now->airHumid);
    if(GOT_LIGHT_SENSOR)
      getLight(&now->lux);
    if(GOT_PH_SENSOR)
      getPH(&now->pH);
    if(GOT_LEVEL_SENSOR) {
      getLevel(LEVEL_ECHO_PIN1, &now->waterLevel1);     
      getLevel(LEVEL_ECHO_PIN2, &now->waterLevel2);     
      getLevel(LEVEL_ECHO_PIN3, &now->waterLevel3);     
      dln(valveDBG, "");
      dbg(valveDBG, "wL1: "); dbg(valveDBG, now->waterLevel1);
      dbg(valveDBG, "; wL2: "); dbg(valveDBG, now->waterLevel2);
      dbg(valveDBG, "; wL3: "); dln(valveDBG, now->waterLevel3);
    }
    if(analogSensor!=ANALOG_SENSOR_NONE) getAnalog(&now->analog);

    flowController.step(now);   // set valves on and off etc.
    
    if(SEND_DATA)
      postSensorData(&monitorData[monitorCursor]);  // push data to the cloud

    if(++monitorCursor == MONITOR_POINTS)
      monitorCursor = 0;
  }

  if(loopCounter == TICK_WIFI_DEBUG) {
    /* TODO a way to only trigger on less loops
    dbg(netDBG, "SSID: "); dbg(netDBG, apSSID);
    dbg(netDBG, "; IP address(es): local="); dbg(netDBG, WiFi.localIP());
    dbg(netDBG, "; AP="); dln(netDBG, WiFi.softAPIP());
    */
  }
  if(loopCounter == TICK_HEAP_DEBUG) {
    dbg(miscDBG, "free heap="); dln(miscDBG, ESP.getFreeHeap());
  }

  if(loopCounter++ == LOOP_ROLLOVER)
    loopCounter = 0;
}

/////////////////////////////////////////////////////////////////////////////
// wifi and web server management stuff /////////////////////////////////////
void startAP() {
  WiFi.mode(WIFI_AP_STA);
  if (!WiFi.softAPConfig(apIP, apIP, netMsk)){
    dbg(startupDBG,"WARNING: Failed to set AP IP");
  }
  WiFi.softAP(apSSID,"secretpassword");
  dln(startupDBG, "Soft AP started");
}

void printIPs() {
  dbg(startupDBG, "AP SSID: ");
  dbg(startupDBG, apSSID);
  dbg(startupDBG, "; IP address(es): local=");
  dbg(startupDBG, WiFi.localIP());
  dbg(startupDBG, "; AP=");
  dln(startupDBG, WiFi.softAPIP());
}
void startWebServer() {
  webServer.on("/", handle_root);
  webServer.onNotFound(handleNotFound);
  webServer.on("/wifi", handle_wifi);
  webServer.on("/elfstatus", handle_elfstatus);
  webServer.on("/serverconf", handle_serverconf);
  webServer.on("/analogconf", handle_analogconf);
  webServer.on("/wfchz", handle_wfchz);
  webServer.on("/svrchz", handle_svrchz);
  webServer.on("/algchz", handle_algchz);
  webServer.on("/data", handle_data);
  webServer.on("/actuate", handle_actuate);
  webServer.on("/valve1", handle_valve1);
  webServer.on("/valve2", handle_valve2);
  webServer.on("/valve3", handle_valve3);
  webServer.on("/reboot", handle_reboot);
  webServer.serveStatic("/", SPIFFS, "/");
  webServer.addHandler(new SPIFFSEditor(SPIFFS, SPIFFS_username,SPIFFS_password));

  webServer.begin();
  dln(startupDBG, "HTTP server started");
}
void handleNotFound(AsyncWebServerRequest *request) {
  dbg(netDBG, "URI Not Found: ");
  dln(netDBG, request->url());
  // TODO send redirect to /? or just use handle_root?
  request->send(200, "text/plain", "URI Not Found");
}
void handle_root(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page notionally at /");
  String toSend = pageTop;
  toSend += pageTop2;
  toSend += pageDefault;
  toSend += pageFooter;
  request->send(200, "text/html", toSend);
}
void handle_data(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /data");
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
  request->send(200, "text/html", toSend);
}
String genAPForm() {
  String f = pageTop;
  f += ": Wifi Config";
  f += pageTop2;
  f += "<h2>Choose a wifi access point to join</h2>\n";
  f += "<h3>Signal strength in brackets, lower is better</h3><p>\n";
  
  const char *checked = " checked";

  int n = WiFi.scanNetworks();
  dbg(netDBG, "scan done: ");
  if(n == 0) {
    dln(netDBG, "no networks found");
    f += "No wifi access points found :-( ";
    f += "<a href='/'>Back</a><br/><a href='/wifi'>Try again?</a></p>\n";
  } else {
    dbg(netDBG, n);
    dln(netDBG, " networks found");
    f += "<form method='POST' action='wfchz'> ";
    for(int i = 0; i < n; ++i) {
      // print SSID and RSSI for each network found
      dbg(netDBG, i + 1);
      dbg(netDBG, ": ");
      dbg(netDBG, WiFi.SSID(i));
      dbg(netDBG, " (");
      dbg(netDBG, WiFi.RSSI(i));
      dbg(netDBG, ")");
      dln(netDBG, (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");

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
void handle_wifi(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /wifi");
  String toSend = genAPForm();
  request->send(200, "text/html", toSend);
}

void handle_elfstatus(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /elfstatus");

  String toSend = pageTop;
  toSend += ": Elf Status";
  toSend += pageTop2;
  toSend += "\n<h2>Elf Status</h2><p><ul>\n";

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
  toSend += "\n<li>Data sharing server address: "; toSend += svrAddr;
  toSend += "</li>\n";
  toSend += "\n<li>Analog sensor type: "; toSend += analogSensor;
  toSend += "</li>\n";
  toSend += "\n<li>Software Version: "; toSend += "waterelf32-OTA v1.08";
  toSend += "</li>\n";
  
  toSend += "<h3>Reboot WaterElf?</h3>"; 
  toSend += "<form method='POST' action='reboot'> ";
  toSend += "<input type='submit' value='Reboot'></form></p>";
  toSend += "</ul></p>";

  toSend += pageFooter;
  request->send(200, "text/html", toSend);
}
void handle_wfchz(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /wfchz");
  String toSend = pageTop;
  toSend += ": joining wifi network";
  toSend += pageTop2;
  String ssid = "";
  String key = "";

  for(uint8_t i = 0; i < request->args(); i++ ) {
    //dln(netDBG, " " + request->argName(i) + ": " + webServer.arg(i));
    if(request->argName(i) == "ssid")
      ssid = request->arg(i);
    else if(request->argName(i) == "key")
      key = request->arg(i);
  }

  if(ssid == "") {
    toSend += "<h2>Ooops, no SSID...?</h2>";
    toSend += "<p>Looks like a bug :-(</p>";
  } else {
    toSend += "<h2>Done! Now trying to join network...</h2>";
    toSend += "<p>Check <a href='/elfstatus'>wifi status here</a>.</p>";
    char ssidchars[ssid.length()+1];
    char keychars[key.length()+1];
    ssid.toCharArray(ssidchars, ssid.length()+1);
    key.toCharArray(keychars, key.length()+1);
    WiFi.begin(ssidchars, keychars);
  }

  toSend += pageFooter;
  request->send(200, "text/html", toSend);
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
String genAnalogConfForm() {
  String f = pageTop;
  f += ": Analog Sensor Config";
  f += pageTop2;
  f += "<h2>Configure Analog Sensor</h2><p>\n";

  f += "<form method='POST' action='algchz'> ";
  f += "<br/>Analog sensor:\n<ul>\n";
  f += "<li>none <input type='radio' name='analog_sensor' value='analog_none' checked>\n";
  f += "<li>mains current <input type='radio' name='analog_sensor' value='analog_mains'>\n";
  f += "<li>water pressure <input type='radio' name='analog_sensor' value='analog_pressure'>\n";
  f += "<input type='submit' value='Submit'></form></ul></p>";

  f += pageFooter;
  return f;
}
void handle_serverconf(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /serverconf");
  String toSend = genServerConfForm();
  request->send(200, "text/html", toSend);
}
void handle_analogconf(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /analogconf");
  String toSend = genAnalogConfForm();
  request->send(200, "text/html", toSend);
}
void handle_svrchz(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /svrchz");
  String toSend = pageTop;
  toSend += ": data sharing configured";
  toSend += pageTop2;

  boolean cloudShare = false;
  for(uint8_t i = 0; i < request->args(); i++) {
    if(request->argName(i) == "svraddr") {
      svrAddr = request->arg(i);
      if(svrAddr.length() == 0) // default cloud server address
        svrAddr = "citsci.wegrow.social";
      toSend += "<h2>Added local server config...</h2>";
      toSend += "<p>...at ";
      toSend += svrAddr;
      toSend += "</p>";
    } else if(request->argName(i) == "key") {
      if(request->arg(i) == "on")
        cloudShare = true;
    }
  }

  // persist the config
  if(setConfig("citsciServerAddress",svrAddr)){
    toSend += "<h2>Saved to config</h2>";
  } else {
    toSend += "<h2>NOT SAVED!</h2>";
  }

  toSend += pageFooter;
  request->send(200, "text/html", toSend);
}
void handle_algchz(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /algchz");
  String toSend = pageTop;
  toSend += ": analog sensor configured";
  toSend += pageTop2;

  for(uint8_t i = 0; i < request->args(); i++) {
    if(request->argName(i) == "analog_sensor") { // remember/persist the type
      String argVal = request->arg(i);
      if(argVal == "analog_mains") {
        analogSensor = ANALOG_SENSOR_MAINS;
      } else if(argVal == "analog_pressure") {
        analogSensor = ANALOG_SENSOR_PRESSURE;
      } else {
        analogSensor = ANALOG_SENSOR_NONE; // the default is...
        dln(analogDBG,"no analog sensor");
      }
      toSend += "<h2>Added analog sensor config...</h2>";
      toSend += "<p>...for ";
      toSend += analogSensor;
      toSend += "</p>";

      if(setConfig("analogSensorType",analogSensor)){
        toSend += "<h2>Saved to config</h2>";
      } else {
        toSend += "<h2>NOT SAVED!</h2>";
      }
    }
  }

  toSend += pageFooter;
  dln(analogDBG, analogSensor);
  request->send(200, "text/html", toSend);
}
void handle_actuate(AsyncWebServerRequest *request) {
  dln(netDBG, "serving page at /actuate");
  String toSend = pageTop;
  toSend += ": Setting Actuator";
  toSend += pageTop2;

  boolean newState = false;
  for(uint8_t i = 0; i < request->args(); i++ ) {
    if(request->argName(i) == "state") {
      if(request->arg(i) == "on")
        newState = true;
    }
  }

  // now we trigger the 433 transmitter
  if(newState == true){
    mySwitch.switchOn(RCSW_CHANNEL, RCSW_HEATER);
    dln(netDBG, "Actuator on");
  } else {
    mySwitch.switchOff(RCSW_CHANNEL, RCSW_HEATER);
    dln(netDBG, "Actuator off");
  }

  toSend += "<h2>Actuator triggered</h2>\n";
  toSend += "<p>(New state should be ";
  toSend += (newState) ? "on" : "off";
  toSend += ".)</p>\n";
  toSend += pageFooter;
  request->send(200, "text/html", toSend);
}
void handle_valve1(AsyncWebServerRequest *request) { handle_valve(request,0); } // valves in the UI are...
void handle_valve2(AsyncWebServerRequest *request) { handle_valve(request,1); } // ...numbered from 1, but...
void handle_valve3(AsyncWebServerRequest *request) { handle_valve(request,2); } // ...from 0 in the FlowController
void handle_valve(AsyncWebServerRequest *request, int valveNum) {
  dbg(valveDBG, "serving page at /valve");
  dln(valveDBG, valveNum + 1);
  String toSend = pageTop;
  toSend += ": Setting Water Valve ";
  toSend += valveNum + 1;
  toSend += pageTop2;

  boolean newState = false;
  for(uint8_t i = 0; i < request->args(); i++ ) {
    if(request->argName(i) == "state") {
      if(request->arg(i) == "on")
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
  request->send(200, "text/html", toSend);
}

void handle_reboot(AsyncWebServerRequest *request) {
  String toSend = pageTop;
  toSend += "<h1>Rebooting Waterelf...</h1>";
  toSend += pageFooter;
  request->send(200, "text/html", toSend);
  reboot=true;  
}
/////////////////////////////////////////////////////////////////////////////
// sensor/actuator stuff ////////////////////////////////////////////////////
void startPeripherals() {
  dln(monitorDBG, "\nstartPeripherals...");
  mySwitch.enableTransmit(4);   // RC transmitter is connected to Pin 4

  taskDISABLE_INTERRUPTS();
  tempSensor.begin();     // start the onewire temperature sensor
  taskENABLE_INTERRUPTS();
  if(tempSensor.getDeviceCount()==1) {
    GOT_TEMP_SENSOR = true;
    dln(monitorDBG, "Found temperature sensor");
    taskDISABLE_INTERRUPTS();
    tempSensor.getAddress(tempAddr, 0);
    tempSensor.setResolution(tempAddr, 12); // 12 bit res (DS18B20 does 9-12)
    taskENABLE_INTERRUPTS();
  } else {
    dln(monitorDBG, "Temperature sensor FAIL");
  }

  dht.begin();    // start the humidity and air temperature sensor
  float airHumid = dht.readHumidity();
  float airCelsius = dht.readTemperature();
  if (isnan(airHumid) || isnan(airCelsius)) {
    dln(monitorDBG, "Humidity sensor FAIL");
  } else {
    GOT_HUMID_SENSOR = true;
    dln(monitorDBG, "Found humidity sensor");
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
  
  Wire.beginTransmission(pH_Add);
  error = Wire.endTransmission();
  if(error==0){
    GOT_PH_SENSOR = true;
    dln(monitorDBG, "Found pH sensor");
  }

  if(analogSensor == ANALOG_SENSOR_MAINS) {
    emon1.current(A0, 111.1); // current: input pin, calibration
  }
}

/*
postSensorData: construct & post (via GET, to confuse), sensor data like this:
GET /collect/WaterElf-10865861 HTTP/1.1
User-Agent: WaterElf/0.000001
Host: citsci.wegrow.social:8000
Accept: application/json
Content-Type: application/json
Content-Length: 146

{ "timestamp": 39653, "airTemp": 25.90, "humidity": 49.40, "light": 8, "pH": 11.12, "waterLevel1": 257, "waterLevel2": 257, "waterLevel3": 257 }
*/
void postSensorData(monitor_t *monitorData) {

// TODO if we failed to connect last time, wait another few calls before
// trying again? to increase web liveness in disconnected devices

  dln(citsciDBG, "\npostSensorData");
  String jsonBuf = "";
  String citsciAddr = svrAddr;
  if(citsciAddr.length() == 0) // default cloud server address
    citsciAddr = "citsci.wegrow.social";
  formatMonitorEntry(monitorData, &jsonBuf, true);
  String envelope =
    "GET /collect/"; envelope += apSSIDStr; envelope += " HTTP/1.1\r\n";
  envelope += "User-Agent: WaterElf/0.000001\r\n";
  envelope += "Host: "; envelope += citsciAddr; envelope += ":8000\r\n";
  envelope += "Accept: application/json\r\n";
  envelope += "Content-Type: application/json\r\n";
  envelope += "Content-Length: " ;
  envelope += jsonBuf.length() + 2; // + 2 for the cr/nls
  envelope += "\r\n\r\n";
  envelope += jsonBuf;
  
  WiFiClient citsciClient;
  if(citsciClient.connect(citsciAddr.c_str(), 8000)) {
    dln(citsciDBG, "connected to citsci server; doing GET");
    citsciClient.print(envelope);
  } else {
    dbg(citsciDBG, citsciAddr);
    dln(citsciDBG, " - no citsci server");
  }
  citsciClient.stop();
  // dln(citsciDBG, envelope);

  dln(citsciDBG, "");
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
  if(analogSensor!=ANALOG_SENSOR_NONE){
    buf->concat("^ ~analog~+ ");
    buf->concat(m->analog);
    if(! JSON) buf->concat("\tanalog");
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
  taskDISABLE_INTERRUPTS(); 
  tempSensor.requestTemperatures(); // Send the command to get temperatures
  (*waterCelsius) = tempSensor.getTempC(tempAddr);
  taskENABLE_INTERRUPTS();
  dbg(monitorDBG, "Water temp: ");
  dbg(monitorDBG, *waterCelsius);
  dln(monitorDBG, " C, ");
  return;
}
void getHumidity(float* airCelsius, float* airHumid) {
  (*airCelsius) = dht.readTemperature();
  (*airHumid) = dht.readHumidity();
  dbg(monitorDBG, "Air Temp: ");
  dbg(monitorDBG, *airCelsius);
  dbg(monitorDBG, " C, ");
  dbg(monitorDBG, "Humidity: ");
  dbg(monitorDBG, *airHumid);
  dln(monitorDBG, " %RH, ");
  return;
}
void getLight(uint16_t* lux) {
  sensors_event_t event;
  tsl.getEvent(&event);
  (*lux) = event.light; 
  dbg(monitorDBG, "Light: ");
  dbg(monitorDBG, *lux);
  dln(monitorDBG, " Lux");
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
  dbg(monitorDBG, "pH: ");
  dbg(monitorDBG, *pH);
  dln(monitorDBG, " pH");
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
  delay(35);                                    // anti-interference measure

  dbg(monitorDBG, "Water Level: ");
  dbg(monitorDBG, *waterLevel);
  dln(monitorDBG, " cm, ");
  return;
}
void getAnalog(float* a) {
  dbg(analogDBG, "getAnalog of type: ");
  dln(analogDBG, analogSensor);
  if(analogSensor==ANALOG_SENSOR_NONE) {
    (*a) = 0.0;
    dln(analogDBG, "no analog sensor");

  } else if(analogSensor == ANALOG_SENSOR_MAINS) {
    (*a) = (float) ( emon1.calcIrms(1480) / 10 /*fudge!*/ );
    dbg(analogDBG, "mains reading is ");
    dbg(analogDBG, (*a)); dbg(analogDBG, "A\n");
  } else if(analogSensor == ANALOG_SENSOR_PRESSURE) {
    int analogValue = analogRead(A0);

    // conversion/"calibration" because sensor 4.5v=1.2MPa
    (*a) = (analogValue - 25) * .19;

    dbg(analogDBG, "value: ");
    dbg(analogDBG, analogValue);
    dbg(analogDBG, "    pressure: ");
    dbg(analogDBG, (*a));
    dbg(analogDBG, " PSI\n");
  }

  return;
}

/////////////////////////////////////////////////////////////////////////////
// config utils /////////////////////////////////////////////////////////////
//bool loadConfig() {
template< typename G> G getConfig( G configName ){
  File configFile = SPIFFS.open("/config.txt", FILE_READ);
  if (!configFile) {
    dln(miscDBG, "Failed to open config file for reading");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    dln(miscDBG, "Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    dln(miscDBG, "Failed to parse config file");
    return false;
  }

  configFile.close();
  return json[configName];
}

template< typename S, typename SS > bool setConfig( S configName, SS configData ){
  File configFileR = SPIFFS.open("/config.txt", FILE_READ);
  if (!configFileR) {
    dln(miscDBG, "Failed to open config file for reading before writing");
    return false;
  }
  
  size_t size = configFileR.size();
  if (size > 1024) {
    dln(miscDBG, "Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFileR.readBytes(buf.get(), size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  configFileR.close();

  File configFile = SPIFFS.open("/config.txt", FILE_WRITE);
  if (!configFile) {
    dln(miscDBG, "Failed to open config file for writing");
    return false;
  }
  
  long saves = json["configUpdateCount"];
  saves ++;
  json["configUpdateCount"] = saves;
  dbg(miscDBG, "New config update count: ");
  dln(miscDBG, saves);

  json[configName] = configData;
  json.prettyPrintTo(configFile);
  configFile.close();
  return true;
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
char *getMAC(char *buf) { // the MAC is 6 bytes, so needs careful conversion...
  uint64_t mac = ESP.getEfuseMac(); // ...to string (high 2, low 4):
  sprintf(buf, "%04X%08X", (uint16_t) (mac >> 32), (uint32_t) mac);
  return buf;
}
