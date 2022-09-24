#ifndef NODE_SWITCHES
#define NODE_SWITCHES

#include <Arduino.h>
#include "node_prefs.h"

void initSwitches();
void flipToggleSwitch();
void pressMomentary(int msTime);

bool isToggleOn();

#endif