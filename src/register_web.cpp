#include "register_web.h"

AsyncWebServer registration_server(80);

void registerHome(AsyncWebServerRequest *request) {
    request->send(200, "text/html", register_index_html);
}

void registerNode(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", register_node_html, processor);
}

void webSetControlPointMac(AsyncWebServerRequest *request){
    if(!request->hasParam("controlPointMac", true)){
        request->send(400, "text/html", "Missing controlPointMac argument");
        return;
    }
    
    String mac;
    mac = request->getParam("controlPointMac", true)->value();
    
    String regContent;
    if (!mac.isEmpty()) {
        setControlPointMac(mac);
        saveAllPreferences();

        regContent = "{\"Success\":\"saved control point mac to eeprom\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void webSetNodeId(AsyncWebServerRequest *request){
    if(!request->hasParam("nodeId", true)){
        request->send(400, "text/html", "Missing id argument");
        return;
    }
    
    int nodeId = atoi(request->getParam("nodeId", true)->value().c_str());
    
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
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing id argument");
        return;
    }

    int dhtPin = atoi(request->getParam("pin", true)->value().c_str());
    int dhtType = atoi(request->getParam("dhtType", true)->value().c_str());
    
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
    registration_server.on("/setControlPointMac", HTTP_POST, webSetControlPointMac);
    registration_server.onNotFound(handleNotFound);
    registration_server.begin();
}