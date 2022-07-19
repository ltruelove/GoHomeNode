#include <Arduino.h>

#include "node_prefs.h"
#include "initial_server.h"
#include "register_web.h"

unsigned long previousMillis = 0;
unsigned long interval = 30000;
bool isConnected = false;
bool hasPreferences = false;
String isGarage = "";
String macAddress;
int statusCode;

void connectToWifi(const char *ssid, const char *key){
  WiFi.mode(WIFI_STA);
  Serial.println("connecting");
  WiFi.begin(ssid, key);
  int c = 0;

  while(c < 20){
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("connected");
      IPAddress ip = WiFi.localIP();
      Serial.println(ip.toString());
      return;
    }
    delay(500);
    Serial.print(".");
    c++;
  }
}


void setupAccessPoint(void)
{
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(100);
  scanNetworks();

  WiFi.softAP("GoHome Node", "");
  launchWeb();
}

void setup() {
  Serial.begin(115200);
  
  WiFi.disconnect();

  initPreferences();

  macAddress = WiFi.macAddress();

  String ssid = getSSID();
  String pass = getPass();

  if(ssid.isEmpty()){
    setupAccessPoint();
  }else{
    connectToWifi(ssid.c_str(), pass.c_str());
    
    hasPreferences = true;
    initSensors();

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected to WiFi");
      isConnected = true;
      int nodeId = getNodeId();

      if(nodeId < 1){
        launchRegisterWeb();
      }else{
        Serial.println("Begin standard operation");
        // this will eventually start the esp now code
        launchRegisterWeb();
        //this should be where the device operates most of the time
      }
    } else {
      Serial.println("WiFi did not connect with given data");
      setupAccessPoint();
    }
  }
}

void loop() {
  if(!hasPreferences){
    delay(1);
  }else{
    delay(100);

    // increment our time counter
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >=interval)) {
      Serial.println(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      if(WiFi.reconnect()){
        int nodeId = getNodeId();

        if(nodeId > 0){
          Serial.println("Updating API with new IP");
          //updateIPWithAPI();
        }else{
          Serial.println("Attempt to register sensor with the API");
          //registerSensorWithAPI(nodePrefs.apiHost, nodePrefs.apiPort);
        }
      }
      previousMillis = currentMillis;
    }
  }
}