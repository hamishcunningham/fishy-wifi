#ifndef JOINME_H
#define JOINME_H

#ifdef ESP32
#include <ESPAsyncWebServer.h>
#elif
#include <ESP8266WebServer.h>
typedef ESP8266WebServer ESPWebServer;
#endif

//must be called before starting access points in setup
void joinme_dhcps_hack();
/*
 * Setup joinme to capture clients on the softAP
 * You *must* regularly call joinme_turn and server.handleClient()
 * server *must* live until after you last call joinme_turn
 */
void joinme_setup(AsyncWebServer* server, IPAddress apIP);
//joinme_turn must be run once per main loop iteration
void joinme_turn();

#endif //JOINME_H
