#include "update_mode_web.h"
#include "SPIFFS.h"
#include "api_wrapper.h"
#include "Update.h"
#include <AsyncElegantOTA.h>

AsyncWebServer update_mode_server(80);

void updateModeHome(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/update_mode.html");
}

void exitUpdateMode(AsyncWebServerRequest *request){
    setStringValue("broadcast", "updateMode");
    delay(100);

    String regContent = "{\"message\":\"leaving update mode\"}";
    request->send(200, "application/json", regContent);
    esp_restart();
}

void launchUpdateModeWeb(){
    update_mode_server.on("/", updateModeHome);
    update_mode_server.on("/exit", exitUpdateMode);
    update_mode_server.on("/version", nodeVersion);
    update_mode_server.onNotFound(handleNotFound);

    AsyncElegantOTA.begin(&update_mode_server);

    update_mode_server.begin();

    updateAPIWithIpAddress(getApiHost(), getApiPort(), getNodeId(), WiFi.localIP().toString());
}