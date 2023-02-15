#ifndef NODE_PREFS
#define NODE_PREFS

#include "Arduino.h"
#include "settings.h"

void clearPreferences();
void saveNodeId(int id);

String getSSID();
String getPass();
String getApiHost();
String getName();
String getControlPointMac();

uint8_t * getControlPointMacArray(String controlPointMac);

uint16_t getApiPort();

int getDhtType();
int getDhtPin();
int getMoisturePin();
int getResistorPin();
int getMagneticPin();
int getNodeId();
int getTogglePin();
int getMomentaryPin();
bool getIsClosedOn();

void setSSID(String _ssid);
void setPass(String _pass);
void setApiHost(String _apiHost);
void setName(String _name);
void setControlPointMac(String _controlPointMac);

void setApiPort(uint16_t _apiPort);

void setDhtType(int _dhtType);
void setDhtPin(int _dhtPin);
void setMoisturePin(int _moisturePin);
void setResistorPin(int _resistorPin);
void setMagneticPin(int _magneticPin);
void setNodeId(int _nodeId);
void setTogglePin(int _togglePin);
void setMomentaryPin(int _momentaryPin);
void setIsclosedOn(bool _isClosedOn);

void clearPreferences();

#endif