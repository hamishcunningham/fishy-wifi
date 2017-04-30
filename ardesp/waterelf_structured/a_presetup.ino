#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
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
IPAddress apIP(192, 168, 99, 1);
IPAddress netMsk(255, 255, 255, 0);
ESP8266WebServer webServer(80);
String apSSIDStr = "WaterElf-" + String(ESP.getChipId());
const char* apSSID = apSSIDStr.c_str();
String svrAddr = ""; // address of a local server TODO delete?

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
  long waterLevel1; // TODO should be an array
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
// misc utils ///////////////////////////////////////////////////////////////
void ledOn();
void ledOff();
String ip2str(IPAddress address);
#define dbg(b, s) if(b) Serial.print(s)
#define dln(b, s) if(b) Serial.println(s)
#define startupDBG true
#define valveDBG false
#define monitorDBG false
#define netDBG false
#define miscDBG false
#define citsciDBG true

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
const byte pH_Add = 0x4E;  // change this to match ph ADC address
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
    int pumpMcpPin = valvePinMap[number][0];
    int solenoidMcpPin = valvePinMap[number][1];

    // trigger MOSFETs via the MCP
    if(newState == true){
      mcp.digitalWrite(pumpMcpPin, HIGH);
      mcp.digitalWrite(solenoidMcpPin, HIGH);
      filling = true; // TODO if set from UI will interfere with timing?
      dln(valveDBG, " on");
    } else {
      mcp.digitalWrite(pumpMcpPin, LOW);
      mcp.digitalWrite(solenoidMcpPin, LOW);
      filling = false;
      dln(valveDBG, " off");
    }
    // dbg(valveDBG, "pump Pin = "); dbg(valveDBG, pumpMcpPin);
    // dbg(valveDBG, ", solen = "); dln(valveDBG, solenoidMcpPin);

    lastFlip = millis();
  }
  void on()  { stateChange(true); }  // fill time
  void off() { stateChange(false); } // drain time
  void step(monitor_t* now, char cycleMins) {   // check conditions and adjust
    dbg(valveDBG, "\nvalve[].step - number = "); dln(valveDBG, number);
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
  int numValves = 3;         // WARNING! call init if resetting!
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
boolean getCloudShare();
void setCloudShare(boolean b);
String getSvrAddr();
void setSvrAddr(String s);
