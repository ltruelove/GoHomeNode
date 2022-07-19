#ifndef REGISTER_WEB
#define REGISTER_WEB

#include <ArduinoJson.h>
#include <DHT.h>

#include "node_prefs.h"
#include "pages.h"
#include "common_web.h"

void launchRegisterWeb();
void initSensors();

#define DHTTYPE DHT11
#define DHTTYPE22 DHT22

#endif