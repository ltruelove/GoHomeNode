#include "broadcast_mode.h"
#include "switches.h"
#include "settings.h"

struct NodeReading {
    int nodeId;
    float TemperatureF;
    float TemperatureC;
    float Humidity;
    int Moisture;
    int ResistorValue;
    int MagneticValue;
    bool IsClosed;
};

struct SwitchCommand {
    bool triggerToggle;
    bool pressMomentary;
    int msMomentaryPress;
    bool triggerUpdate;
    bool eraseSettings;
    bool updateMode;
};

NodeReading messageData;
SwitchCommand instructions;
esp_now_peer_info_t peerInfo;
uint8_t *broadcastAddress;

int32_t getWiFiChannel(const char *ssid) {
    if (int32_t n = WiFi.scanNetworks()) {
        for (uint8_t i=0; i<n; i++) {
        if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
            return WiFi.channel(i);
        }
        }
    }
    return 0;
}

void followInstructions(SwitchCommand inst){
    if(inst.eraseSettings){
        eraseSettings();
        esp_restart();
    }

    if(inst.triggerToggle){
        Serial.print("Flipping toggle");
        flipToggleSwitch();
        //make sure we update the status
        broadcastData();
    }

    if(inst.pressMomentary){
        Serial.print("Pressing momentary");
        pressMomentary(inst.msMomentaryPress);
    }

    if(inst.triggerUpdate){
        broadcastData();
    }
    
    // puts the node into OTA update mode
    if(inst.updateMode){
        setStringValue("update", "updateMode");
        delay(100);
        esp_restart();
    }
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
    // Copies the sender mac address to a string
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);
    memcpy(&instructions, incomingData, sizeof(instructions));
    
    // store the reading for fetching later
    followInstructions(instructions);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void addPeer(uint8_t* mac){
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void initBroadcast(){
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    String ssid = getSSID();
    int32_t channel = getWiFiChannel(ssid.c_str());

    //WiFi.printDiag(Serial); // Uncomment to verify channel number before
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    //WiFi.printDiag(Serial); // Uncomment to verify channel change after

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);

    broadcastAddress = getControlPointMacArray(getControlPointMac());

    if(!esp_now_is_peer_exist(broadcastAddress)){
        addPeer(broadcastAddress);
    }
}

void broadcastData(){
    messageData.nodeId = getNodeId();
    messageData.TemperatureF = getTemperatureF();
    messageData.TemperatureC = getTemperatureC();
    messageData.Humidity = getHumidity();
    messageData.Moisture = getMoisture();
    messageData.ResistorValue = getResistor();
    messageData.MagneticValue = getMagnetic();
    messageData.IsClosed = isToggleOn();

    if(!esp_now_is_peer_exist(broadcastAddress)){
        addPeer(broadcastAddress);
    }

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &messageData, sizeof(messageData));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}