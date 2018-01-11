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
IPAddress apIP_;

void do_redirect(AsyncWebServerRequest* request){
  Serial.printf(
    "joinme redirecting captured client to: %s",
    apIP_.toString().c_str()
  );
  auto response = request->beginResponse(TEMPORARY_REDIRECT,"text/plain","");
  response->addHeader("Location","http://"+apIP_.toString()+"/");
  request->send(response);
}

void handle_L0(AsyncWebServerRequest* request) {
  do_redirect();
}

void handle_L2(AsyncWebServerRequest* request) {
  do_redirect();
}

void handle_ALL(AsyncWebServerRequest* request) {
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

void joinme_setup(AsyncWebServer* server, IPAddress apIP) {
  assert(server != NULL);
  apIP_ = apIP;
  Serial.printf(
    "joinme will direct captured clients to: %s\n",
    apIP_.toString().c_str()
  );
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP_);
  Serial.println("joinme captive dns server started");
  server->on("/generate_204", do_redirect);  //Android captive portal
  server->on("/L0", handle_L0);
  server->on("/L2", handle_L2);
  server->on("/ALL", handle_ALL);
  Serial.println("joinme http handlers added");
}

void joinme_turn() {
  dnsServer.processNextRequest();
}
