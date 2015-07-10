/* waterelf.ino */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
  
const char* ssid = "WaterElf";
const char* html =
  "<html><head><title>WaterElf Aquaponics Helper</title>"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
  "</head><body>"
  "<h2>Welcome to WaterElf</h2>"
  "<h2>Control</h2>"
  "<p><ul>"
  "<li><a href='/wifi'>join a wifi network</a></li>"
  "<li>actuator links go here...</li>"
  "</ul></p>"
  "<h2>Monitor</h2>"
  "<p><ul>"
  "<li>data goes here...</li>"
  "</ul></p>"
  "</body></html>";

const char* apChoice = 
  "<html><head><title>WaterElf Aquaponics Helper: Wifi Config</title>"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
  "</head><body>"
  "<h2>Choose a wifi access point to join</h2><p><form method='POST' action='chz'> "
  "_ITEMS_<br/>Pass key: <input type='textarea' name='key'><br/><br/> "
  "<input type='submit' value='Submit'></form></p></body></html>";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("DNS Server started");

  server.on("/", handle_root);
  server.on("/generate_204", handle_root); // Android support
  server.on("/L0", handle_L0);
  server.on("/L2", handle_L2);
  server.on("/ALL", handle_ALL);
  server.on("/wifi", handle_wifi);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

void handleNotFound() {
  Serial.print("\t\t\t\t URI Not Found: ");
  Serial.println(server.uri());
  server.send ( 200, "text/plain", "URI Not Found" );
}

void handle_root() {
  Serial.println("serving page notionally at /");
  String toSend = html;
  //toSend.replace("TGT0", LEDstate[0] ? "y" : "b");
  //toSend.replace("TGT2", LEDstate[2] ? "y" : "b");
  server.send(200, "text/html", toSend);
  delay(100);
}

String genAPForm() {
  String f = apChoice;
  String buf = "";
  const char *checked = " checked";

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    f =
      "<html><head><title>WaterElf Aquaponics Helper: Wifi Config</title>"
      "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
      "</head><body><p>No AP data :-(<br/>"
      "<a href='/'>Back</a><br/><a href='/wifi'>Try again</a></p>"
      "</body></html>";
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);

      buf.concat("<input type='radio' name='ssid' value='");
      buf.concat(WiFi.SSID(i));
      buf.concat("'");
      buf.concat(checked);
      buf.concat(">");
      buf.concat(WiFi.SSID(i));
      buf.concat("<br/>\n");
      checked = "";
    }
    f.replace("_ITEMS_", buf);
  }
  Serial.println("");

  return f;
}

void handle_wifi() {
  Serial.println("serving page at /wifi");
  String toSend = genAPForm();
  server.send(200, "text/html", toSend);
  delay(100);
}

void handle_L0() { handle_root(); } 
void handle_L2() { handle_root(); }
void handle_ALL() { handle_root(); }
