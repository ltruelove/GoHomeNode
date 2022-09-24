#include "broadcast_mode.h"
#include "switches.h"

struct NodeReading {
    int nodeId;
    float TemperatureF;
    float TemperatureC;
    float Humidity;
    int Moisture;
    int ResistorValue;
    bool IsClosed;
};

struct SwitchCommand {
    bool triggerToggle;
    bool pressMomentary;
    int msMomentaryPress;
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
    if(inst.triggerToggle){
        Serial.print("Flipping toggle");
        flipToggleSwitch();
    }

    if(inst.pressMomentary){
        Serial.print("Pressing momentary");
        pressMomentary(inst.msMomentaryPress);
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

void initBroadcast(){
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

    broadcastAddress = getControlPointMacArray();

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.encrypt = false;
    
    //Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void broadcastData(){
    messageData.nodeId = getNodeId();
    messageData.TemperatureF = getTemperatureF();
    messageData.TemperatureC = getTemperatureC();
    messageData.Humidity = getHumidity();
    messageData.Moisture = getMoisture();

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &messageData, sizeof(messageData));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}