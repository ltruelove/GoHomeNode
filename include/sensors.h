#ifndef NODE_SENSORS
#define NODE_SENSORS

#include <Arduino.h>
#include <DHT.h>
#include "node_prefs.h"

void initSensors();

float getTemperatureF();
float getTemperatureC();
float getHumidity();
int getMoisture();
int getResistor();
int getMagnetic();

#define DHTTYPE DHT11
#define DHTTYPE22 DHT22

#endif