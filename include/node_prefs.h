#ifndef NODE_PREFS
#define NODE_PREFS

#include <Preferences.h>

void initPreferences();
void clearPreferences();
void saveNodeId(int id);
void saveAllPreferences();

String getSSID();
String getPass();
String getApiHost();
String getName();

uint16_t getApiPort();

int getDhtType();
int getDhtPin();
int getNodeId();

void setSSID(String _ssid);
void setPass(String _pass);
void setApiHost(String _apiHost);
void setName(String _name);

void setApiPort(uint16_t _apiPort);

void setDhtType(int _dhtType);
void setDhtPin(int _dhtPin);
void setNodeId(int _nodeId);

#endif