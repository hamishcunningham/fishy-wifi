/////////////////////////////////////////////////////////////////////////////
// waterelf.ino /////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

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
String apSSIDStr = "FormaldElf-" + String(ESP.getChipId());
const char* apSSID = apSSIDStr.c_str();
String svrAddr = ""; // address of a local server TODO delete?

/////////////////////////////////////////////////////////////////////////////
// page generation stuff ////////////////////////////////////////////////////
String pageTopStr = String(
  "<html><head><title>FormaldElf Aquaponics Helper [ID: " + apSSIDStr + "]"
);
const char* pageTop = pageTopStr.c_str();
const char* pageTop2 = "</title>\n"
  "<meta charset=\"utf-8\">"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;font-size: 150%;}</style>"
  "</head><body>\n";
const char* pageDefault = // TODO build the growbeds according to their num
  "<h2>Welcome to FormaldElf</h2>\n"
  "<h2>Control</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifi'>Join a wifi network</a></li>\n"
  "<li><a href='/serverconf'>Configure data sharing</a></li>\n"
  "</ul></p>\n"
  "<h2>Monitor</h2>\n"
  "<p><ul>\n"
  "<li><a href='/wifistatus'>Wifi status</a></li>\n"
  "<li><a href='/data'>Sensor data</a></li>\n"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>FormaldElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='https://wegrow.social/'>WeGrow</a></p></body></html>";

/////////////////////////////////////////////////////////////////////////////
// data monitoring stuff ////////////////////////////////////////////////////
const boolean SEND_DATA = true;  // turn off posting of data if required here
const int MONITOR_POINTS = 60; // number of data points to store
typedef struct {
  unsigned long timestamp;
  long rawADC;
  float formaldehyde;
} monitor_t;
monitor_t monitorData[MONITOR_POINTS];
int monitorCursor = 0;
int monitorSize = 0;
const int DATA_ENTRIES = 10; // size of /data rpt; must be <= MONITOR_POINTS
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
#define monitorDBG true
#define netDBG false
#define miscDBG true
#define citsciDBG true


/////////////////////////////////////////////////////////////////////////////
// config utils /////////////////////////////////////////////////////////////
boolean getCloudShare();
void setCloudShare(boolean b);
String getSvrAddr();
void setSvrAddr(String s);

/////////////////////////////////////////////////////////////////////////////
// setup ////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT); // turn built-in LED on
  blink(3);                     // signal we're starting setup

  // read persistent config
  SPIFFS.begin();
  svrAddr = getSvrAddr();

  // start the sensors, the DNS and webserver, etc.
  startPeripherals();
  startAP();
  printIPs();
  startWebServer();

  // initialise the MCP and the flow controller
  dln(startupDBG, "doing flow controller and mcp init...");
 
  // try and set the host name
  if(WiFi.hostname("waterelf"))
    dln(netDBG, "set hostname succeeded");
  else
    dln(netDBG, "set hostname failed");
  delay(300); blink(3);         // signal we've finished config
}

/////////////////////////////////////////////////////////////////////////////
// looooooooooooooooooooop //////////////////////////////////////////////////
void loop() {
  webServer.handleClient();
  //delay(5);
  if(loopCounter == TICK_MONITOR) { // monitor levels, step valves, push data
    monitor_t* now = &monitorData[monitorCursor];
    if(monitorSize < MONITOR_POINTS)
      monitorSize++;
    now->timestamp = millis();
    getFormaldehyde(&now->rawADC, &now->formaldehyde);
    yield();
    
    if(SEND_DATA) {                     // push data to the cloud
      postSensorData(&monitorData[monitorCursor]); yield();
    }
      
    if(++monitorCursor == MONITOR_POINTS)
      monitorCursor = 0;
  }

  if(loopCounter == TICK_WIFI_DEBUG) {
    dbg(netDBG, "SSID: "); dbg(netDBG, apSSID);
    dbg(netDBG, "; IP address(es): local="); dbg(netDBG, WiFi.localIP());
    dbg(netDBG, "; AP="); dln(netDBG, WiFi.softAPIP());
  }
  if(loopCounter == TICK_HEAP_DEBUG) {
    dbg(miscDBG, "free heap="); dln(miscDBG, ESP.getFreeHeap());
  }

  if(loopCounter++ == LOOP_ROLLOVER) loopCounter = 0;
}

/////////////////////////////////////////////////////////////////////////////
// wifi and web server management stuff /////////////////////////////////////
void startAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID);
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
  webServer.begin();
  dln(startupDBG, "HTTP server started");
}
void handleNotFound() {
  dbg(netDBG, "URI Not Found: ");
  dln(netDBG, webServer.uri());
  // TODO send redirect to /? or just use handle_root?
  webServer.send(200, "text/plain", "URI Not Found");
}
void handle_root() {
  dln(netDBG, "serving page notionally at /");
  String toSend = pageTop;
  toSend += pageTop2;
  toSend += pageDefault;
  toSend += pageFooter;
  webServer.send(200, "text/html", toSend);
}
void handle_data() {
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
      dln(netDBG, (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");

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
  dln(netDBG, "serving page at /wifi");
  String toSend = genAPForm();
  webServer.send(200, "text/html", toSend);
}

void handle_wifistatus() {
  dln(netDBG, "serving page at /wifistatus");

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
  dln(netDBG, "serving page at /wfchz");
  String toSend = pageTop;
  toSend += ": joining wifi network";
  toSend += pageTop2;
  String ssid = "";
  String key = "";

  for(uint8_t i = 0; i < webServer.args(); i++ ) {
    //dln(netDBG, " " + webServer.argName(i) + ": " + webServer.arg(i));
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
  dln(netDBG, "serving page at /serverconf");
  String toSend = genServerConfForm();
  webServer.send(200, "text/html", toSend);
}
void handle_svrchz() { // TODO delete?
  dln(netDBG, "serving page at /svrchz");
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

/////////////////////////////////////////////////////////////////////////////
// sensor/actuator stuff ////////////////////////////////////////////////////
void startPeripherals() {
  dln(monitorDBG, "\nstartPeripherals...");
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
  String citsciAddr = "citsci.wegrow.social";
  formatMonitorEntry(monitorData, &jsonBuf, true);
  String envelope =
    "GET /collect/"; envelope += apSSIDStr; envelope += " HTTP/1.1\r\n";
  envelope += "User-Agent: FormaldElf/0.000001\r\n";
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
    dbg(netDBG, citsciAddr);
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
  buf->concat("^ ~raw ADC~+ ");
  buf->concat(m->rawADC);
  buf->concat("^ ~sensor absolute~+ ");
  buf->concat(m->rawADC*2.0916);
  if(! JSON) buf->concat("\tmV");  
  buf->concat("^ ~formaldehyde~+ ");
  buf->concat(m->formaldehyde);
  if(! JSON) buf->concat("\tppm");
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
void getFormaldehyde(long* rawADC,float* formaldehyde) {
  for (int i=0; i < 10; i++){
    (*rawADC) += analogRead(A0);
    delay(10);
  }
  *rawADC=*rawADC/10;
  (*formaldehyde) = *rawADC*0.0081109; // Magic number converts 10-bit ADC to ppm
  dbg(monitorDBG, "raw ADC: ");
  dbg(monitorDBG, *rawADC);
  dbg(monitorDBG, "Formaldehyde: ");
  dbg(monitorDBG, *formaldehyde);
  dln(monitorDBG, " ppm, ");
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
