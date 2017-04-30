/////////////////////////////////////////////////////////////////////////////
// wifi and web server management stuff /////////////////////////////////////
void startAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(apSSID,"wegrowdotsocial");
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
  webServer.on("/actuate", handle_actuate);
  webServer.on("/valve1", handle_valve1);
  webServer.on("/valve2", handle_valve2);
  webServer.on("/valve3", handle_valve3);
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
void handle_actuate() {
  dln(netDBG, "serving page at /actuate");
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
  webServer.send(200, "text/html", toSend);
}
void handle_valve1() { handle_valve(0); } // valves in the UI are...
void handle_valve2() { handle_valve(1); } // ...numbered from 1, but...
void handle_valve3() { handle_valve(2); } // ...from 0 in the FlowController
void handle_valve(int valveNum) {
  dbg(valveDBG, "serving page at /valve");
  dln(valveDBG, valveNum + 1);
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
  dln(monitorDBG, "\nstartPeripherals...");
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
    dln(monitorDBG, "failed to find humidity sensor");
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
    dln(monitorDBG, "Found pH sensor");
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
//String citsciAddr = "citsci.wegrow.social"; //Hardcoded? Also changed port from 8000 to 5984
  String citsciAddr = svrAddr;
  formatMonitorEntry(monitorData, &jsonBuf, true);
  String envelope =
    "GET /collect/"; envelope += apSSIDStr; envelope += " HTTP/1.1\r\n";
  envelope += "User-Agent: WaterElf/0.000001\r\n";
  envelope += "Host: "; envelope += citsciAddr; envelope += ":5984\r\n";
  envelope += "Accept: application/json\r\n";
  envelope += "Content-Type: application/json\r\n";
  envelope += "Content-Length: " ;
  envelope += jsonBuf.length() + 2; // + 2 for the cr/nls
  envelope += "\r\n\r\n";
  envelope += jsonBuf;
  
  WiFiClient citsciClient;
  if(citsciClient.connect(citsciAddr.c_str(), 5984)) {
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
