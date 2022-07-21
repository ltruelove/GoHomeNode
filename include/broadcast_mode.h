#ifndef BROADCAST_MODE
#define BROADCAST_MODE

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "node_prefs.h"
#include "sensors.h"

void initBroadcast();
void broadcastData();

#endif