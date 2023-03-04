#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>

#include "api_wrapper.h"
#include "settings.h"

void updateAPIWithIpAddress(String apiHost, int port, int nodeId, String ipAddress){
    WiFiClient client;
    HTTPClient http;

    String updateEndpoint = "http://" + apiHost;
    if(port != 80){
        updateEndpoint += ":" + (String)port;
    }
    updateEndpoint += "/node/ipUpdate";

    http.begin(client, updateEndpoint);

    DynamicJsonDocument doc(1024);

    doc["Id"] = nodeId;
    doc["IpAddress"] = ipAddress;

    String serializedString;
    serializeJson(doc, serializedString);

    int httpResponseCode = http.POST(serializedString);

    if(httpResponseCode != 200){
        Serial.print("Register request failed with response code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}