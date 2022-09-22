#include "node_prefs.h"

Preferences prefs;

const char *prefsName = "nodePrefs";

String ssid, pass, apiHost, name, controlPointMac;
uint16_t apiPort;
int dhtType, dhtPin, moisturePin, nodeId;

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
    prefs.end();
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
    //char *token;
    char *strings[30];
    int i = 0;

    char *mac_arr = new char[controlPointMac.length() + 1];
    strcpy(mac_arr, controlPointMac.c_str());
    //sprintf(mac, "%s",controlPointMac.c_str());
    Serial.println(mac_arr);

    strings[i] = strtok(mac_arr, delim);
    while(strings[i] != NULL){
        strings[++i] = strtok(NULL, delim);
    }

    delete [] mac_arr;

    for(int j = 0; j < i; j++){
        macArray[j] = (int)strtol(strings[j], NULL, 16);
    }

    /*
    token = strtok(&controlPointMac[0], delim);
    int i = 0;
    char *ptr;
    while(token != NULL){
        macArray[i] = strtol(token, &ptr, 16);
        i++;
        token = strtok(NULL, delim);
    }
    */

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