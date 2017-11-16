#ifdef ESP32
#include <ESPWebServer.h>
#elif
#include <ESP8266WebServer.h>
typedef ESP8266WebServer ESPWebServer;
#endif


/*
 * Setup joinme to capture clients on the softAP
 * You *must* regularly call joinme_turn and server.handleClient()
 * server *must* live until after you last call joinme_turn
 */
void joinme_setup(ESPWebServer* server, IPAddress apIP);
void joinme_turn();
