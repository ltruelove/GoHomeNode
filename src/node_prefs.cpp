#include "node_prefs.h"

Preferences prefs;

const char *prefsName = "nodePrefs";

String ssid, pass, apiHost, name;
uint16_t apiPort;
int dhtType, dhtPin, nodeId;

void initPreferences(){
    prefs.begin(prefsName, true);
    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    apiHost = prefs.getString("apiHost", "");
    name = prefs.getString("name", "");
    apiPort = prefs.getUInt("apiPort", 80);
    dhtType = prefs.getInt("dhtType", 0);
    dhtPin = prefs.getInt("dhtPin", -1);
    nodeId = prefs.getInt("nodeId", 0);
    prefs.end();
}

void saveAllPreferences(){
    prefs.begin(prefsName, false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.putString("apiHost", apiHost);
    prefs.putString("name", name);
    prefs.putUInt("apiPort", apiPort);
    prefs.putInt("dhtType", dhtType);
    prefs.putInt("dhtPin", dhtPin);
    prefs.putInt("nodeId", nodeId);
    prefs.end();
}

void clearPreferences()
{
  prefs.begin(prefsName, false);
  prefs.clear();
  prefs.end();
}

void saveNodeId(int id){
  prefs.begin(prefsName, false);
  prefs.putUInt("nodeId", id);
  prefs.end();
}

String getSSID(){
    return ssid;
}

String getPass(){
    return pass;
}

String getApiHost(){
    return apiHost;
}

String getName(){
    return name;
}

uint16_t getApiPort(){
    return apiPort;
}

int getDhtType(){
    return dhtType;
}

int getDhtPin(){
    return dhtPin;
}

int getNodeId(){
    return nodeId;
}

void setSSID(String _ssid){
    ssid = _ssid;
}

void setPass(String _pass){
    pass = _pass;
}

void setApiHost(String _apiHost){
    apiHost = _apiHost;
}

void setName(String _name){
    name = _name;
}

void setApiPort(uint16_t _apiPort){
    apiPort = _apiPort;
}

void setDhtType(int _dhtType){
    dhtType = _dhtType;
}

void setDhtPin(int _dhtPin){
    dhtPin = _dhtPin;
}

void setNodeId(int _nodeId){
    nodeId = _nodeId;
}