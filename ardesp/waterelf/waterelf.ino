/* waterelf.ino */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
ESP8266WebServer server(80);

const char* ssid = "WaterElf";

const char* pageTop =
  "<html><head><title>WaterElf Aquaponics Helper";
const char* pageTop2 = "</title>\n"
  "<style>body{background:#FFF;color: #000;font-family: sans-serif;}</style>"
  "</head><body>\n";
const char* pageDefault =
  "<h2>Welcome to WaterElf</h2>\n"
  "<h2>Control</h2>"
  "<p><ul>"
  "<li><a href='/wifi'>join a wifi network</a></li>"
  "<li><a href='/wifistatus'>wifi status</a></li>"
  "<li>TODO actuator links go here...</li>"
  "</ul></p>"
  "<h2>Monitor</h2>"
  "<p><ul>"
  "<li>TODO data goes here...</li>"
  "</ul></p>\n";
const char* pageFooter =
  "\n<p><a href='/'>WaterElf</a>&nbsp;&nbsp;&nbsp;"
  "<a href='https://www.fish4tea.net/'>Fish4Tea</a></p></body></html>";

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
  server.on("/L0", handle_root);
  server.on("/L2", handle_root);
  server.on("/ALL", handle_root);
  server.onNotFound(handleNotFound);

  server.on("/wifi", handle_wifi);
  server.on("/wifistatus", handle_wifistatus);
  server.on("/chz", handle_chz);
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
  // TODO send redirect to /? or just use handle_root?
  server.send(200, "text/plain", "URI Not Found");
}

void handle_root() {
  Serial.println("serving page notionally at /");
  String toSend = pageTop;
  toSend += pageTop2;
  toSend += pageDefault;
  toSend += pageFooter;
  server.send(200, "text/html", toSend);
  delay(100);
}

String genAPForm() {
  String f = pageTop;
  f += ": Wifi Config";
  f += pageTop2;
  f += "<h2>Choose a wifi access point to join</h2><p>\n";

  const char *checked = " checked";

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    f += "No wifi access points found :-( ";
    f += "<a href='/'>Back</a><br/><a href='/wifi'>Try again?</a></p>\n";
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    f += "<form method='POST' action='chz'> ";
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
  Serial.println("");

  f += pageFooter;
  return f;
}

void handle_wifi() {
  Serial.println("serving page at /wifi");
  String toSend = genAPForm();
  server.send(200, "text/html", toSend);
  delay(100);
}

void handle_wifistatus() {
  Serial.println("serving page at /wifistatus");

  String toSend = pageTop;
  toSend += ": Wifi Status";
  toSend += pageTop2;
  toSend += "<h2>Wifi Status</h2><p><ul>";

  toSend += "<li>SSID: ";
  toSend += WiFi.SSID();
  toSend += "</li>";
  toSend += "<li>Status: ";
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
  toSend += "</ul></p>";

  toSend += pageFooter;
  server.send(200, "text/html", toSend);
  delay(100);
}

void handle_chz() {
  Serial.println("serving page at /chz");
  String toSend = pageTop;
  toSend += ": joining wifi network";
  toSend += pageTop2;
  String ssid = "";
  String key = "";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    Serial.println(" " + server.argName(i) + ": " + server.arg(i) + "\n");
    if(server.argName(i) == "ssid")
      ssid = server.arg(i);
    else if(server.argName(i) == "key")
      key = server.arg(i);
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
  }

  toSend += pageFooter;
  server.send(200, "text/html", toSend);
  delay(100);
}
