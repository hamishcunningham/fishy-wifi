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
  flowController.init();
  mcp.begin();                  // use default address 0 for mcp23008
  for(int i = 0; i < mcpPinsUsed; i++)
    mcp.pinMode(mcpPins[i], OUTPUT);

  // try and set the host name
  if(WiFi.hostname("waterelf"))
    dln(netDBG, "set hostname succeeded");
  else
    dln(netDBG, "set hostname failed");
  delay(300); blink(3);         // signal we've finished config
}
