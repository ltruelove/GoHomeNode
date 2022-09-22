#include "common_web.h"

String processor(const String& var){

  if(var == "IPPLACEHOLDER"){
    IPAddress ip = WiFi.softAPIP();
    return ip.toString();
  }

  if(var == "WIFIOPTIONS"){
    return scanNetworks();
  }

  if(var == "APIHOSTIP"){
    return getApiHost();
  }

  if(var == "APIHOST"){
    String host = "http://";
    host += getApiHost();
    int port = getApiPort();

    if(port > 0 && port != 80){
      host += ":";
      host.concat(port);
    }
  
    return host;
  }

  if(var == "NODEMAC"){
    return WiFi.macAddress();
  }

  if(var == "NODENAME"){
    return getName();
  }

  if(var == "NODEIP"){
    return WiFi.localIP().toString();
  }

  if(var == "DHTPIN"){
    return (String)getDhtPin();
  }

  return String();
}

String scanNetworks(){
  String options = "";
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      delay(10);
    }
  }

  Serial.println("");
  options = "<select name='ssid'>";

  for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
    options += "<option value='";
    options += WiFi.SSID(i);
    options += "'>";
    options += WiFi.SSID(i);
    options += " (";
    options += WiFi.RSSI(i);

    options += ")";
    options += "</option>";
  }

  options += "</select>";

return options;
}

void clearAllPreferences(AsyncWebServerRequest *request){
  clearPreferences();
  request->send(200, "text/html", "Preferences Reset");

  delay(100);
  ESP.restart();
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "File not found");
}