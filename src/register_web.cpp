#include "register_web.h"

AsyncWebServer registration_server(80);

DHT dht(getDhtPin(), DHTTYPE); // we have to instantiate this class in order for it to be available

void registerHome(AsyncWebServerRequest *request) {
    request->send(200, "text/html", register_index_html);
}

void registerNode(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", register_node_html, processor);
}

void webSetNodeId(AsyncWebServerRequest *request){
    if(!request->hasParam("nodeId")){
        request->send(400, "text/html", "Missing id argument");
        return;
    }
    
    int nodeId = atoi(request->getParam("nodeId")->value().c_str());
    
    String regContent;
    if (nodeId > 0) {
        setNodeId(nodeId);
        saveAllPreferences();

        regContent = "{\"Success\":\"saved Node ID to eeprom\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setDht(AsyncWebServerRequest *request){
    if(!request->hasParam("pin")){
        request->send(400, "text/html", "Missing id argument");
        return;
    }

    int dhtPin = atoi(request->getParam("pin")->value().c_str());
    int dhtType = atoi(request->getParam("dhtType")->value().c_str());
    
    String regContent;
    if (dhtPin > 0 && dhtType > 0) {
        setDhtPin(dhtPin);
        setDhtType(dhtType);
        saveAllPreferences();

        regContent = "{\"Success\":\"saved DHT settings to eeprom\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

String getDhtJson(){
  StaticJsonDocument<500> doc;
  Serial.println(dht.readTemperature());
  doc["humidity"] = dht.readHumidity();
  doc["celcius"] = dht.readTemperature();
  doc["fahrenheit"] = dht.readTemperature(true);
  
  String output;
  serializeJson(doc, output);

  return output;
}

void handleDht(AsyncWebServerRequest *request){
  String output = getDhtJson();
  request->send(200, "application/json", output);
}

void restart(AsyncWebServerRequest *request){
  request->send(200, "application/json", "restarting");

  delay(100);
  ESP.restart();
}

void launchRegisterWeb(){
    registration_server.on("/", registerHome);
    registration_server.on("/register", registerNode);
    registration_server.on("/clear", clearAllPreferences);
    registration_server.on("/setNodeId", webSetNodeId);
    registration_server.on("/setDht", setDht);
    registration_server.on("/restart", restart);
    registration_server.on("/dht", handleDht);
    registration_server.onNotFound(handleNotFound);
    registration_server.begin();
}

void initSensors(){
    int dhtPin = getDhtPin();
    int dhtType = getDhtType();
    
    // Only start DHT if we're actually using it
    if(dhtPin > -1){
      dht.setPin(dhtPin);
      
      if(dhtType == 11){
        dht.setType(DHTTYPE);
      }else{
        dht.setType(DHTTYPE22);
      }
      
      dht.begin();
    }
}