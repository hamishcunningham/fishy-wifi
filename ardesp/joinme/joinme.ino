/*
 * joinme.ino --
 * a port of the Lua Joinme wifi config utility to Arduino ESP IDE
 */
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// TODO
// wifi config form
// sensor data report, and actuator set form
// fix the DNS server, http://www.esp8266.com/viewtopic.php?p=21169

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
  "be redirected here.</p></body></html>";

void setup() {
  Serial.begin(9600);
  Serial.println("doing setup");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("FishyWifi");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // reply code, no error
  // dnsServer.setErrorReplyCode(DNSReplyCode::NoError);

  // reply to all requests with same HTML
  webServer.onNotFound([]() {
    Serial.println("onNotFound called");
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.on("/", []() {
    Serial.println("on / called");
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
  Serial.println("setup done");

/* scanner code
  Serial.begin(9600);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
*/
}

void loop() {
  Serial.println("looping...");
  dnsServer.processNextRequest();
  webServer.handleClient();
  delay(1000);
/* scanner code
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
*/
}
