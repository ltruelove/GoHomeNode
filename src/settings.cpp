#include "Arduino.h"
#include "settings.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

nvs_handle_t my_handle;

bool initSettings(){
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
  }
  ESP_ERROR_CHECK( err );
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    return false;
  }

  return true;
}

bool eraseSettings(){
    esp_err_t err = nvs_flash_erase();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        return false;
    }

    ESP_ERROR_CHECK(err);
    return true;
}

bool openHandle(){
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return false;
    }

    return true;
}

void setIntValue(int32_t value, const char *name){
    if(!openHandle()){
        return;
    }

    esp_err_t err = nvs_set_i32(my_handle, name, value);
    Serial.printf((err != ESP_OK) ? "Set int value failed!\n" : "Set int value succeeded\n");

    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void setStringValue(const char *value, const char *name){
    openHandle();
    if(!openHandle()){
        return;
    }

    esp_err_t err = nvs_set_str(my_handle, name, value);
    Serial.printf("%s set status: ", name);
    Serial.printf((err != ESP_OK) ? "Set string value failed!\n" : "Set string value succeeded\n");

    nvs_commit(my_handle);
    nvs_close(my_handle);
}

int32_t getIntValue(const char *name){
    int32_t intVal = 0;

    openHandle();
    if(!openHandle()){
        return intVal;
    }

    esp_err_t err = nvs_get_i32(my_handle, name, &intVal);

    switch (err) {
        case ESP_ERR_NVS_NOT_FOUND:
            Serial.printf("The %s value is not initialized yet!\n", name);
            break;
        default :
            Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    nvs_close(my_handle);

    return intVal;
}

char* getStringValue(const char *name){
    size_t requiredSize;
    char* defaultString = new char[1];
    strcpy(defaultString, "");

    openHandle();
    if(!openHandle()){
        return defaultString;
    }

    esp_err_t err = nvs_get_str(my_handle, name, NULL, &requiredSize);

    switch (err) {
        case ESP_OK:
            Serial.println("size found");
            Serial.println(requiredSize);
        break;
        case ESP_ERR_NVS_NOT_FOUND:
            Serial.printf("Required size not found for %s!\n", name);

            nvs_close(my_handle);

            return defaultString;
            break;
        default :
            Serial.printf("Error (%s) reading required size!\n", esp_err_to_name(err));

            nvs_close(my_handle);

            return defaultString;
    }


    char* stringVal = (char *)malloc(requiredSize);
    err = nvs_get_str(my_handle, name, stringVal, &requiredSize);

    switch (err) {
        case ESP_OK:
            Serial.println("value found");
        break;
        case ESP_ERR_NVS_NOT_FOUND:
            Serial.printf("The %s value is not initialized yet!\n", name);
            break;
        default :
            Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    nvs_close(my_handle);

    return stringVal;
}