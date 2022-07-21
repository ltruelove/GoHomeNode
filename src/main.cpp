#include <Arduino.h>

#include "node_prefs.h"
#include "sensors.h"
#include "initial_server.h"
#include "register_web.h"
#include "broadcast_mode.h"

unsigned long previousMillis = 0;
unsigned long interval = 30000;
unsigned long previousMessageMillis = 0;   // Stores last time data was published
const long broadcastInterval = 300000;        // Interval at which to publish sensor readings 300000 millis = 5 minutes
bool isConnected = false;
bool hasPreferences = false;
String isGarage = "";
String macAddress;
int statusCode;

void connectToWifi(const char *ssid, const char *key){
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(getName().c_str());
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

  WiFi.softAP("GoHome Node", "");
  launchWeb();
}

void setup() {
  Serial.begin(115200);
  
  WiFi.disconnect();

  initPreferences();
  hasPreferences = true;

  macAddress = WiFi.macAddress();

  String ssid = getSSID();
  String pass = getPass();

  if(ssid.isEmpty()){
    setupAccessPoint();
  }else{
    int nodeId = getNodeId();
    String mac = getControlPointMac();

    if(mac.isEmpty() || nodeId < 1){
      connectToWifi(ssid.c_str(), pass.c_str());
    
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("Connected to WiFi");
        isConnected = true;

        launchRegisterWeb();
      } else {
        Serial.println("WiFi did not connect with given data");
        setupAccessPoint();
      }
    }else{
      initSensors();
      initBroadcast();
      broadcastData();
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
    String mac = getControlPointMac();

    if(!mac.isEmpty()){
      if(currentMillis - previousMessageMillis >= broadcastInterval){
        previousMessageMillis = currentMillis;

        broadcastData();
      }
    }else{
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
}