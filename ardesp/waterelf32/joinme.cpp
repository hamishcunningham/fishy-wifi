/*
 * joinme.ino --
 * a port of the Lua Joinme wifi config utility to Arduino ESP IDE
 */

#include "joinme.h"
#include <WiFiClient.h>
#include <DNSServer.h>

//these 2 includes required to change default dhcps behaviour until
//https://github.com/espressif/esp-idf/issues/1285 is resolved
#include <apps/dhcpserver_options.h>
#include <tcpip_adapter.h>

const byte DNS_PORT = 53;
//307 is temporary redirect. if we used 301 we'd probably break the user's
//browser for sites they were captured from until they cleared their cache
int TEMPORARY_REDIRECT = 307;
DNSServer dnsServer;
ESPWebServer* server_;
IPAddress apIP_;

void do_redirect(){
  Serial.printf(
    "joinme redirecting captured client to: %s",
    apIP_.toString().c_str()
  );
  server_->sendHeader("Location","http://"+apIP_.toString()+"/");
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

void joinme_dhcps_hack(){
  //Do not use a custom dns server, which (possibly due to a bug) we can't even
  //set. Instead use the default, which is the gateway (in this case us)
  //may be removed if
  //https://github.com/espressif/esp-idf/issues/1285 is resolved
  uint8_t no = 0;
  tcpip_adapter_dhcps_option(
    TCPIP_ADAPTER_OP_SET,
    (tcpip_adapter_option_id_t) DOMAIN_NAME_SERVER,
    (void *) &no,
    sizeof(dhcps_offer_t)
  );
}

void joinme_setup(ESPWebServer* server, IPAddress apIP) {
  assert(server != NULL);
  server_ = server;
  apIP_ = apIP;
  Serial.printf(
    "joinme will direct captured clients to: %s\n",
    apIP_.toString().c_str()
  );
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP_);
  Serial.println("joinme captive dns server started");
  server_->on("/generate_204", do_redirect);  //Android captive portal
  server_->on("/L0", handle_L0);
  server_->on("/L2", handle_L2);
  server_->on("/ALL", handle_ALL);
  Serial.println("joinme http handlers added");
}

void joinme_turn() {
  dnsServer.processNextRequest();
}
