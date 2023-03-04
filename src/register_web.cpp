#include "register_web.h"
#include "SPIFFS.h"

AsyncWebServer registration_server(80);

void registerHome(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/register_index.html");
}

void registerNode(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/register_node.html", String(), false, processor);
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

        regContent = "{\"Success\":\"saved control point mac to flash\"}";
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

        regContent = "{\"Success\":\"saved Node ID to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setDht(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    int dhtPin = atoi(request->getParam("pin", true)->value().c_str());
    int dhtType = atoi(request->getParam("dhtType", true)->value().c_str());
    
    String regContent;
    if (dhtPin > 0 && dhtType > 0) {
        setDhtPin(dhtPin);
        setDhtType(dhtType);

        regContent = "{\"Success\":\"saved DHT settings to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setMoisture(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    int moisturePin = atoi(request->getParam("pin", true)->value().c_str());
    
    String regContent;
    if (moisturePin > 0) {
        setMoisturePin(moisturePin);

        regContent = "{\"Success\":\"saved sensor settings to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setResistor(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    int resistorPin = atoi(request->getParam("pin", true)->value().c_str());
    
    String regContent;
    if (resistorPin > 0) {
        setResistorPin(resistorPin);

        regContent = "{\"Success\":\"saved sensor settings to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setMagnetic(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    int magneticPin = atoi(request->getParam("pin", true)->value().c_str());
    
    String regContent;
    if (magneticPin > 0) {
        setMagneticPin(magneticPin);

        regContent = "{\"Success\":\"saved sensor settings to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setToggle(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    if(!request->hasParam("IsClosedOn", true)){
        request->send(400, "text/html", "Missing IsClosedOn argument");
        return;
    }

    int pin = atoi(request->getParam("pin", true)->value().c_str());
    bool isClosedOn = request->getParam("IsClosedOn", true)->value() == "true";
    Serial.print("Is closed on: ");
    Serial.println(isClosedOn);
    
    String regContent;
    if (pin > 0) {
        setTogglePin(pin);
        setIsclosedOn(isClosedOn);

        regContent = "{\"Success\":\"saved switch settings to flash\"}";
        request->send(200, "application/json", regContent);
        
        return;
    }

    regContent = "{\"Error\":\"404 not found\"}";
    request->send(404, "application/json", regContent);
}

void setMomentary(AsyncWebServerRequest *request){
    if(!request->hasParam("pin", true)){
        request->send(400, "text/html", "Missing pin argument");
        return;
    }

    int pin = atoi(request->getParam("pin", true)->value().c_str());
    
    String regContent;
    if (pin > 0) {
        setMomentaryPin(pin);

        regContent = "{\"Success\":\"saved switch settings to flash\"}";
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
    registration_server.on("/setMoisture", setMoisture);
    registration_server.on("/setResistor", setResistor);
    registration_server.on("/setMagnetic", setMagnetic);
    registration_server.on("/setToggle", setToggle);
    registration_server.on("/setMomentary", setMomentary);
    registration_server.on("/restart", restart);
    registration_server.on("/setControlPointMac", HTTP_POST, webSetControlPointMac);
    registration_server.onNotFound(handleNotFound);
    registration_server.begin();
}