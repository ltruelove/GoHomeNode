#include "node_prefs.h"

Preferences prefs;

const char *prefsName = "nodePrefs";

String ssid, pass, apiHost, name, controlPointMac;
uint16_t apiPort;
int dhtType, dhtPin, moisturePin, nodeId, togglePin, momentaryPin;

void initPreferences(){
    prefs.begin(prefsName, true);
    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    apiHost = prefs.getString("apiHost", "");
    name = prefs.getString("name", "");
    controlPointMac = prefs.getString("controlPointMac", "");
    apiPort = prefs.getUInt("apiPort", 80);
    dhtType = prefs.getInt("dhtType", 0);
    dhtPin = prefs.getInt("dhtPin", -1);
    moisturePin = prefs.getInt("moisturePin", -1);
    nodeId = prefs.getInt("nodeId", 0);
    togglePin = prefs.getInt("togglePin", -1);
    momentaryPin = prefs.getInt("momentaryPin", -1);
    prefs.end();

    Serial.println("Pins:");
    Serial.println(moisturePin);
    Serial.println(togglePin);
    Serial.println("");
}

void saveAllPreferences(){
    prefs.begin(prefsName, false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.putString("apiHost", apiHost);
    prefs.putString("name", name);
    prefs.putString("controlPointMac", controlPointMac);
    prefs.putUInt("apiPort", apiPort);
    prefs.putInt("dhtType", dhtType);
    prefs.putInt("dhtPin", dhtPin);
    prefs.putInt("moisturePin", moisturePin);
    prefs.putInt("nodeId", nodeId);
    prefs.putInt("togglePin", togglePin);
    prefs.putInt("momentaryPin", momentaryPin);
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

String getControlPointMac(){
    return controlPointMac;
}

uint8_t * getControlPointMacArray(){
    static uint8_t macArray[6];
    char delim[] = ":";
    char *strings[30];
    int i = 0;

    char *mac_arr = new char[controlPointMac.length() + 1];
    strcpy(mac_arr, controlPointMac.c_str());
    Serial.println(mac_arr);

    strings[i] = strtok(mac_arr, delim);
    while(strings[i] != NULL){
        strings[++i] = strtok(NULL, delim);
    }

    delete [] mac_arr;

    for(int j = 0; j < i; j++){
        macArray[j] = (int)strtol(strings[j], NULL, 16);
    }

    return macArray;
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

int getMoisturePin(){
    return moisturePin;
}

int getNodeId(){
    return nodeId;
}

int getTogglePin(){
    return togglePin;
}

int getMomentaryPin(){
    return momentaryPin;
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

void setControlPointMac(String _controlPointMac){
    controlPointMac = _controlPointMac;
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

void setMoisturePin(int _moisturePin){
    moisturePin = _moisturePin;
}

void setNodeId(int _nodeId){
    nodeId = _nodeId;
}

void setTogglePin(int _togglePin){
    togglePin = _togglePin;
}

void setMomentaryPin(int _momentaryPin){
    momentaryPin = _momentaryPin;
}