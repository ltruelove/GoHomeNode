#include <Arduino.h>

#include "node_prefs.h"
#include "settings.h"
#include "sensors.h"
#include "switches.h"
#include "initial_server.h"
#include "register_web.h"
#include "broadcast_mode.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300        /* Time ESP32 will go to sleep (in seconds) */

unsigned long previousMillis = 0;
unsigned long interval = 30000;
unsigned long resetMillis = 0;
unsigned long resetInterval = 4000;
unsigned long restartInterval = 86400000;
unsigned long previousMessageMillis = 0;   // Stores last time data was published
const long broadcastInterval = 300000;        // Interval at which to publish sensor readings 300000 millis = 5 minutes
bool isConnected = false;
bool hasPreferences = false;
String isGarage = "";
String macAddress;
String controlPointMacAddress;
int statusCode;
int NodeID = 0;
int resetButtonPin = 12;
int resetState;
int lastResetState = LOW;
int toggleButtonPin = 13;
int toggleButtonState;
int lastToggleButtonState = LOW;
int momentaryButtonPin = 14;
int momentaryButtonState;
int lastMomentaryButtonState = LOW;

int togglePinSetting = 0;
int momentaryPinSetting = 0;

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
  delay(500);
  Serial.begin(115200);
  
  pinMode(resetButtonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  WiFi.disconnect();

  //eraseSettings();
  //esp_restart();

  if(!initSettings()){
    Serial.println("There was an error initializing settings");
    esp_restart();
  }

  String ssid = getSSID();

  if(!ssid.isEmpty()){
    hasPreferences = true;
  }

  if(!hasPreferences){
    setupAccessPoint();
  }else{

    macAddress = WiFi.macAddress();

    String pass = getPass();
    togglePinSetting = getTogglePin();
    momentaryPinSetting = getMomentaryPin();

    NodeID = getNodeId();
    controlPointMacAddress = getControlPointMac();

    if(controlPointMacAddress.isEmpty() || NodeID < 1){
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
      initSwitches();
      initBroadcast();
      broadcastData();

      delay(500);

      // Can't wake up from deep sleep for incoming esp now commands
      //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
      //esp_deep_sleep_start();
    }
  }
}

void loop() {
  if(!hasPreferences){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }else{
    delay(100);

    // increment our time counter
    unsigned long currentMillis = millis();

    if(digitalRead(resetButtonPin) == LOW){
      resetMillis = currentMillis;
    }

    if(togglePinSetting > 0){
      toggleButtonState = digitalRead(toggleButtonPin);

      if(toggleButtonState == HIGH){
        lastToggleButtonState = HIGH;
      }else{
        if(lastToggleButtonState == HIGH){
          lastToggleButtonState = LOW;
          flipToggleSwitch();
        }
      }
    }

    if(momentaryPinSetting > 0){
      momentaryButtonState = digitalRead(momentaryButtonPin);
      
      if(momentaryButtonState == HIGH){
        setMomentary(HIGH);
        lastMomentaryButtonState = HIGH;
      }else{
        if(lastMomentaryButtonState == HIGH){
          lastMomentaryButtonState = LOW;
          setMomentary(LOW);
        }
      }
    }

    // restart every once in a while, currently 1 day
    if(currentMillis  >= restartInterval){
      ESP.restart();
    }

    if(currentMillis - resetMillis >= resetInterval){
      clearPreferences();
      delay(100);
      ESP.restart();
    }

    if(!controlPointMacAddress.isEmpty() && NodeID > 0){
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