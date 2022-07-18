#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <HTTPClient.h>

#define DHTTYPE DHT11
#define DHTTYPE22 DHT22

Preferences prefs;
WebServer server(80);

const char *prefsName = "nodePrefs";

unsigned long previousMillis = 0;
unsigned long interval = 30000;
int doorState = 0;
bool isConnected = false;
bool hasPreferences = false;
String ssid;
String pass = "";
String apiHost = "";
uint16_t apiPort = 8080;
String name = "";
int nodeId = 0;
String isGarage = "";
String st;
String macAddress;
String content;
int statusCode;
int dhtType = 11;
int dhtPin = -1;

DHT dht(dhtPin, DHTTYPE); // we have to instantiate this class in order for it to be available

void clearPreferences()
{
  prefs.begin(prefsName, false);
  prefs.clear();
  prefs.end();
  server.send(200, "text/html", "Preferences Reset");
  delay(100);
  ESP.restart();
}

bool registerSensorWithAPI(String host, int port){
  bool registerSuccess = false;
  String registerEndpoint = "http://" + host;
  if(port != 80){
    registerEndpoint = registerEndpoint + ":" + (String)port;
  }
  registerEndpoint = registerEndpoint + "/temps/registersensor";

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Not connected to WiFi. Cannot register.");
    return registerSuccess;
  }

  WiFiClient client;
  HTTPClient http;

  Serial.println(WiFi.localIP());
  http.begin(client, registerEndpoint);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(1024);

  doc["name"] = name;
  doc["isGarage"] = isGarage == "yes" ? 1:0;
  doc["ipAddress"] = WiFi.localIP().toString();

  String serializedString;
  serializeJson(doc, serializedString);

  Serial.println("Payload:");
  Serial.println(serializedString);
  int httpResponseCode = http.POST(serializedString);

  if(httpResponseCode != 200){
    http.end();
    Serial.print("Register request failed with response code: ");
    Serial.println(httpResponseCode);
    clearPreferences();
    return registerSuccess;
  }

  registerSuccess = true;
  String sensor = http.getString();
  Serial.println(sensor);
  DynamicJsonDocument responseDoc(1024);
  deserializeJson(responseDoc, sensor);
  nodeId = responseDoc["nodeId"];
  Serial.println(nodeId);

  http.end();

  prefs.begin(prefsName, false);
  prefs.putUInt("nodeId", nodeId);
  prefs.end();
  
  return registerSuccess;
  
  delay(100);
  ESP.restart();
}

void updateIPWithAPI(){
  String updateEndpoint = "http://" + apiHost;
  if(apiPort != 80){
    updateEndpoint = updateEndpoint + ":" + (String)apiPort;
  }
  updateEndpoint = updateEndpoint + "/temps/updatesensor";

  WiFiClient client;
  HTTPClient http;

  http.begin(client, updateEndpoint);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(1024);

  doc["name"] = name;
  doc["isGarage"] = isGarage == "yes" ? 1:0;
  doc["ipAddress"] = WiFi.localIP().toString();
  doc["nodeId"] = nodeId;

  String serializedString;
  serializeJson(doc, serializedString);
  Serial.println("Payload:");
  Serial.println(serializedString);
  int httpResponseCode = http.PUT(serializedString);
  http.end();
}

void restart(){
  Serial.println("called restart");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", "restarting");
  delay(100);
  ESP.restart();
}

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

void scanNetworks(){
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
  st = "<select name='ssid'>";

  for (int i = 0; i < n; ++i) {
    // Print SSID and RSSI for each network found
    st += "<option value='";
    st += WiFi.SSID(i);
    st += "'>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += "</option>";
  }

  st += "</select>";
  delay(100);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

// returns the state of the temp/humidity sensor as well as
// the garage door's open/closed status if it's the garage sensor
void handleRoot() {
/*
  StaticJsonDocument<500> doc;
  doc["humidity"] = dht.readHumidity();
  doc["celcius"] = dht.readTemperature();
  doc["fahrenheit"] = dht.readTemperature(true);
  if(isGarage == "yes"){
    doc["doorClosed"] = doorState;
  }
  */
  String output;
  //serializeJson(doc, output);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", output);
}

void registerHome() {
  content = "<!DOCTYPE HTML>\r\n<html>GoHome Node Registration";
    content += "<p><a href='/register'>Go To Registration Form</a></p>";
    content += "<p><a href='/clear'>Clear Preferences (CAREFUL!)</a></p>";
    content += "</html>";
    
    server.send(200, "text/html", content);
}

void wifiHome() {
  content = "<!DOCTYPE HTML>\r\n<html>GoHome Node WiFi Selection";
    content += "<p><a href='/wifi-setup'>Go To WiFi Form</a></p>";
    content += "<p><a href='/clear'>Clear Preferences (CAREFUL!)</a></p>";
    content += "</html>";
    
    server.send(200, "text/html", content);
}

void wifiSetupPage(){
  scanNetworks();
    IPAddress ip = WiFi.softAPIP();

    prefs.begin(prefsName, true);
    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    apiHost = prefs.getString("apiHost", "");
    apiPort = prefs.getUInt("apiPort", 80);
    prefs.end();

    char port[8];
    String sPort;
    Serial.println("convert prefs func");
    itoa(apiPort, port, 10);

    sPort = port;

    Serial.println("build page");
    content = "<!DOCTYPE HTML>\r\n<html><h3>GoHome Node WiFi Setup</h3>";
    content += "<p>";
    content += ip.toString();
    content += "</p>";
    content += "<form method='get' action='setting'>";
    content += "<table><tr><td width='100'>";
    content += "<label>SSID: </label></td><td>" + st +"</td></tr>";
    content += "<tr><td><label>Key: </label></td><td><input type='password' name='pass' value='" + pass + "' length=64></td></tr>";
    content += "<tr><td><label>Node Name: </label></td><td><input type='text' name='name'></td></tr>";
    content += "<tr><td><label>API Host: </label></td><td><p>http:// <input type='text' name='apiHost' value='" + apiHost + "' length=32></p></td></tr>";
    content += "<tr><td><label>API PORT: </label></td><td><input type='number' name='apiPort' value='" + sPort + "'  length=5></td></tr>";
    content += "<tr><td>&nbsp;</td><td><input type='submit'></td></tr></form>";
    content += "</html>";

    Serial.println("send page");
    server.send(200, "text/html", content);
}

void registerNode(){
    prefs.begin(prefsName, true);
    apiHost = prefs.getString("apiHost", "");
    apiPort = prefs.getUInt("apiPort", 80);
    name = prefs.getString("name", "");
    prefs.end();

    char port[8];
    String sPort;
    Serial.println("convert prefs func");
    itoa(apiPort, port, 10);

    sPort = port;

    String host = "http://" + apiHost;
    
    if(apiPort > 0 && apiPort != 80){
      host += ":" + sPort;
    }

    Serial.println("build page");
    content = "<!DOCTYPE HTML>\r\n<html>";
    content += "<head>";
      content += "<script type='text/javascript'>";
        content += "var apiHost = '" + host +"';";
      content += "</script>";
    content += "<script src='https://code.jquery.com/jquery-3.6.0.min.js' integrity='sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=' crossorigin='anonymous'></script>\r\n";
    content += "<script type='text/javascript' src='" + host + "/js/registerNode.js'></script>\r\n";
    content += "<h3>GoHome Node Registration</h3>";
    content += "<form method='get' action='setting'>";
    content += "<input type='hidden' id='mac' value='" + macAddress + "'>";
    content += "<input type='hidden' id='nodeName' value='" + name + "'>";
    content += "<table>";
    content += "<tr>";
      content += "<td>";
      content += "<label>Name: </label></td>";
      content += "<td><input type='text' id='typeName' value=''></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td>";
      content += "<label>Type: </label></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td><label><input type='radio' name='sType' value='sensor' checked='checked' > Sensor</label></td>";
      content += "<td><label><input type='radio' name='sType' value='switch'> Switch</label></td>";
    content += "</tr>";
    content += "<tr id='sensorTypes'>";
      content += "<td>";
      content += "<label>Sensor Types: </label></td>";
      content += "<td><select id='sensorTypeOptions'></select></td>";
    content += "</tr>";
    content += "<tr class='dhtTypes'>";
      content += "<td>";
      content += "<label>Select DHT Type: </label></td>";
    content += "</tr>";
    content += "<tr class='dhtTypes'>";
      content += "<td><label><input type='radio' name='dhtType' value='11' checked='checked' > DHT11</label></td>";
      content += "<td><label><input type='radio' name='dhtType' value='22'> DHT22</label></td>";
    content += "</tr>";
    content += "<tr id='switchTypes'>";
      content += "<td>";
      content += "<label>Switch Types: </label></td>";
      content += "<td><select id='switchTypeOptions'></select></td>";
    content += "</tr>";
    content += "<tr class='switchOptions'>";
      content += "<td>";
      content += "<label>Switch Options: </label></td>";
    content += "</tr>";
    content += "<tr class='switchOptions'>";
      content += "<td>";
      content += "<label>Momentary press duration (ms): </label></td>";
      content += "<td><input type='number' id='MomentaryPressDuration' value='0'></td>";
    content += "</tr>";
    content += "<tr class='switchOptions'>";
      content += "<td>";
      content += "<label>Closed toggle status: </label></td>";
    content += "</tr>";
    content += "<tr class='switchOptions'>";
      content += "<td><label><input type='radio' name='IsClosedOn' value='1'> On</label></td>";
      content += "<td><label><input type='radio' name='IsClosedOn' value='0'> Off</label></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td>";
      content += "<label>Pin: </label></td>";
      content += "<td><input type='number' name='pin' id='pin' value=''></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td>";
      content += "<td><button id='addTypeButton'>Add</button></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td>";
      content += "<label>Sensors: </label></td>";
      content += "<td><div id='sensorList'></div></td>";
    content += "</tr>";
    content += "<tr>";
      content += "<td>";
      content += "<label>Switches: </label></td>";
      content += "<td><div id='switchList'></div></td>";
    content += "</tr>";
    content += "<tr><td>&nbsp;</td><td><input type='submit' value='Save'></td></tr></form>";
    content += "</html>";

    Serial.println("send page");
    server.send(200, "text/html", content);
}

void setParameters(){
  ssid = server.arg("ssid");
  pass = server.arg("pass");
  apiHost = server.arg("apiHost");
  name = server.arg("name");
  isGarage = server.arg("isGarage");

  Serial.println(apiHost);
  apiPort = atoi(server.arg("apiPort").c_str());

  if (ssid.length() > 0 && pass.length() > 0) {
      prefs.begin(prefsName, false);
      prefs.putString("ssid", ssid);
      prefs.putString("pass", pass);
      prefs.putString("apiHost", apiHost);
      prefs.putUInt("apiPort", apiPort);
      prefs.putString("name", name);
      prefs.end();

    content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
    statusCode = 200;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    
    delay(100);
    ESP.restart();
  } else {
    content = "{\"Error\":\"404 not found\"}";
    statusCode = 404;
    Serial.println("Sending 404");
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", content);
}

void setNodeId(){
  Serial.println("called set nodeId");
  if(!server.hasArg("nodeId")){
    server.send(400, "text/html", "Missing id argument");
    return;
  }
  
  nodeId = atoi(server.arg("nodeId").c_str());
  
  if (nodeId > 0) {
    prefs.begin(prefsName, false);
    prefs.putUInt("nodeId", nodeId);
    prefs.end();

    content = "{\"Success\":\"saved Node ID to eeprom\"}";
    statusCode = 200;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    
    return;
  } else {
    content = "{\"Error\":\"404 not found\"}";
    statusCode = 404;
    Serial.println("Sending 404");
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", content);
}

void setDht(){
  Serial.println("called set DHT");
  if(!server.hasArg("pin")){
    server.send(400, "text/html", "Missing id argument");
    return;
  }

  dhtPin = atoi(server.arg("pin").c_str());
  dhtType = atoi(server.arg("dhtType").c_str());
  
  if (dhtPin > 0 && dhtType > 0) {
    prefs.begin(prefsName, false);
    prefs.putInt("dhtPin", dhtPin);
    prefs.putUInt("dhtType", dhtType);
    prefs.end();

    content = "{\"Success\":\"saved DHT settings to eeprom\"}";
    statusCode = 200;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    
    return;
  } else {
    content = "{\"Error\":\"404 not found\"}";
    statusCode = 404;
    Serial.println("Sending 404");
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", content);
}

String getDhtJson(){
  StaticJsonDocument<500> doc;
  Serial.println(dht.readTemperature());
  doc["humidity"] = dht.readHumidity();
  doc["celcius"] = dht.readTemperature();
  doc["fahrenheit"] = dht.readTemperature(true);
  if(isGarage == "yes"){
    doc["doorClosed"] = doorState;
  }
  
  String output;
  serializeJson(doc, output);

  return output;
}

void handleDht(){
  Serial.println("get DHT readings");
  String output = getDhtJson();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", output);
}

void launchWeb(){
  Serial.println("");

  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", wifiHome);
  server.on("/wifi-setup", wifiSetupPage);
  server.on("/setting", setParameters);
  server.on("/clear", clearPreferences);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web Server started");
}

void launchRegisterWeb(){
  server.on("/", registerHome);
  server.on("/register", registerNode);
  server.on("/clear", clearPreferences);
  server.on("/setNodeId", setNodeId);
  server.on("/setDht", setDht);
  server.on("/restart", restart);
  server.on("/dht", handleDht);
  server.onNotFound(handleNotFound);
  server.begin();
}

void launchSensorWeb(){
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/clear", clearPreferences);
  server.begin();
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

  Serial.println("getting preferences");
  prefs.begin(prefsName, true);
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");
  apiHost = prefs.getString("apiHost", "");
  apiPort = prefs.getUInt("apiPort", 80);
  dhtPin = prefs.getInt("dhtPin", -1);
  dhtType = prefs.getUInt("dhtType", 11);
  nodeId = prefs.getUInt("nodeId", 0);
  name = prefs.getString("name");
  prefs.end();

  macAddress = WiFi.macAddress();

  Serial.println("Check for empty SSID");
  if(ssid.isEmpty()){
    Serial.println("empty ssid");
    setupAccessPoint();
    Serial.println("Web Server started");
  }else{
    Serial.println("Try to connect to WiFi");
    connectToWifi(ssid.c_str(), pass.c_str());
    
    hasPreferences = true;
    
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

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Connected to WiFi");
      isConnected = true;

      if(nodeId < 1){
        launchRegisterWeb();
      }else{
        Serial.println("Begin standard operation");
        launchRegisterWeb();
        //this should be where the device operates most of the time
        /*
        updateIPWithAPI();
        dht.begin();
        pinMode(led, OUTPUT);
        digitalWrite(led, 1);
        launchSensorWeb();
        */
      }
    } else {
      Serial.println("WiFi did not connect with given data");
      setupAccessPoint();
    }
  }
}

void loop() {
  server.handleClient();
  
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
        if(nodeId > 0){
          Serial.println("Updating API with new IP");
          updateIPWithAPI();
        }else{
          Serial.println("Attempt to register sensor with the API");
          registerSensorWithAPI(apiHost, apiPort);
        }
      }
      previousMillis = currentMillis;
    }
  }
}