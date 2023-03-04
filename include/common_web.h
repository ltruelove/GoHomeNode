#ifndef COMMON_WEB
#define COMMON_WEB

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "node_prefs.h"

String processor(const String& var);
String scanNetworks();
void clearAllPreferences(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
void nodeVersion(AsyncWebServerRequest *request);

#endif