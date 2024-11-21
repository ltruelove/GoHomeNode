#include "node_prefs.h"

const char *prefsName = "nodePrefs";

const char *SSID = "ssid", *PASS = "pass", *API_HOST = "apiHost", *NODE_NAME = "name", *CTRL_PT_MAC = "controlPointMac", *API_PORT = "apiPort",
*DHT_TYPE = "dhtType", *DHT_PIN = "dhtPin", *MOISTURE_PIN = "moisturePin", *RESISTOR_PIN = "resistorPin", *MAGNETIC_PIN = "magneticPin", *NODE_ID = "nodeId",
*TOGGLE_PIN = "togglePin", *MOMENTARY_PIN = "momentaryPin", *IS_CLOSED_ON = "isClosedOn", *VERSION = "version";

String ssid;
String pass;
String apiHost;
String name;
String controlPointMac;
String version;
uint16_t apiPort;
int dhtType;
int dhtPin;
int moisturePin;
int resistorPin;
int magneticPin;
int nodeId;
int togglePin;
int momentaryPin;
bool isClosedOn;

void clearPreferences()
{
    eraseSettings();
}

void initPreferences()
{
    ssid = getStringValue(SSID);
    pass = getStringValue(PASS);
    apiHost = getStringValue(API_HOST);
    name = getStringValue(NODE_NAME);
    controlPointMac = getStringValue(CTRL_PT_MAC);
    version = getStringValue(VERSION);
    apiPort = getIntValue(API_PORT);
    dhtType = getIntValue(DHT_TYPE);
    dhtPin = getIntValue(DHT_PIN);
    moisturePin = getIntValue(MOISTURE_PIN);
    resistorPin = getIntValue(RESISTOR_PIN);
    magneticPin = getIntValue(MAGNETIC_PIN);
    nodeId = getIntValue(NODE_ID);
    togglePin = getIntValue(TOGGLE_PIN);
    momentaryPin = getIntValue(MOMENTARY_PIN);
    isClosedOn = getIntValue(IS_CLOSED_ON) == 1;
}

void saveNodeId(int id){
    setIntValue(id, NODE_ID);
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

String getVersion(){
    return version;
}

String getControlPointMac(){
    return controlPointMac;
}

uint8_t * getControlPointMacArray(String controlPointMac){
    static uint8_t macArray[6];
    char delim[] = ":";
    char *Strings[30];
    int i = 0;

    char *mac_arr = new char[controlPointMac.length() + 1];
    strcpy(mac_arr, controlPointMac.c_str());
    Serial.println(mac_arr);

    Strings[i] = strtok(mac_arr, delim);
    while(Strings[i] != NULL){
        Strings[++i] = strtok(NULL, delim);
    }

    delete [] mac_arr;

    for(int j = 0; j < i; j++){
        macArray[j] = (int)strtol(Strings[j], NULL, 16);
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

int getResistorPin(){
    return resistorPin;
}

int getMagneticPin(){
    return magneticPin;
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

bool getIsClosedOn(){
    return isClosedOn;
}

void setSSID(String _ssid){
    setStringValue(_ssid.c_str(), SSID);
}

void setPass(String _pass){
    setStringValue(_pass.c_str(), PASS);
}

void setApiHost(String _apiHost){
    setStringValue(_apiHost.c_str(), API_HOST);
}

void setName(String _name){
    setStringValue(_name.c_str(), NODE_NAME);
}

void setControlPointMac(String _controlPointMac){
    setStringValue(_controlPointMac.c_str(), CTRL_PT_MAC);
}

void setApiPort(uint16_t _apiPort){
    setIntValue(_apiPort, API_PORT);
}

void setDhtType(int _dhtType){
    setIntValue(_dhtType, DHT_TYPE);
}

void setDhtPin(int _dhtPin){
    setIntValue(_dhtPin, DHT_PIN);
}

void setMoisturePin(int _moisturePin){
    setIntValue(_moisturePin, MOISTURE_PIN);
}

void setResistorPin(int _resistorPin){
    setIntValue(_resistorPin, RESISTOR_PIN);
}

void setMagneticPin(int _magneticPin){
    setIntValue(_magneticPin, MAGNETIC_PIN);
}

void setNodeId(int _nodeId){
    setIntValue(_nodeId, NODE_ID);
}

void setTogglePin(int _togglePin){
    setIntValue(_togglePin, TOGGLE_PIN);
}

void setMomentaryPin(int _momentaryPin){
    setIntValue(_momentaryPin, MOMENTARY_PIN);
}

void setIsclosedOn(bool _isClosedOn){
    setIntValue(_isClosedOn ? 1 : 0, IS_CLOSED_ON);
}

void setVersion(String _version){
    setStringValue(_version.c_str(), VERSION);
}