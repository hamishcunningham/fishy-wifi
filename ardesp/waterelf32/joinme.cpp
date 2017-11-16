/*
 * joinme.ino --
 * a port of the Lua Joinme wifi config utility to Arduino ESP IDE
 */

#include "joinme.h"
#include <WiFiClient.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
int TEMPORARY_REDIRECT = 307;
DNSServer dnsServer;
ESPWebServer* server_;
IPAddress apIP_;

void do_redirect(){
  //307 is temporary redirect. if we used 301 we'd probably bjork the user's browser until they cleared their cache
  server_->sendHeader("Location",apIP_.toString()+"/");
  server_->send(TEMPORARY_REDIRECT,"text/plain","");
}

void handle_L0() {
  do_redirect();
}

void handle_L2() {
  do_redirect();
}

void handle_ALL() {
  do_redirect();
}

void joinme_setup(ESPWebServer* server, IPAddress apIP) {
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.println("joinme captive dns server started");
  server->on("/generate_204", do_redirect);  //Android captive portal
  server->on("/L0", handle_L0);
  server->on("/L2", handle_L2);
  server->on("/ALL", handle_ALL);
  Serial.println("joinme http handlers added");
  server_ = server;
  apIP_ = apIP;
}

void joinme_turn() {
  dnsServer.processNextRequest();
}
