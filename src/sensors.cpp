#include <Arduino.h>
#include "sensors.h"

DHT dht(getDhtPin(), DHTTYPE); // we have to instantiate this class in order for it to be available

void initSensors(){
    int dhtPin = getDhtPin();
    int dhtType = getDhtType();
    int moisturePin = getMoisturePin();
    
    // Only start DHT if we're actually using it
    if(dhtPin > -1){
      dht.setPin(dhtPin);
      
      if(dhtType == 11){
        dht.setType(DHTTYPE);
      }else{
        dht.setType(DHTTYPE22);
      }
      
      dht.begin();
      Serial.println(dhtPin);
    }

    if(moisturePin > 0){
      pinMode(moisturePin, INPUT);
      adcAttachPin(moisturePin);
    }
}

float getTemperatureF(){
      Serial.println(dht.readTemperature(true));
    return dht.readTemperature(true);
}

float getTemperatureC(){
    return dht.readTemperature();
}

float getHumidity(){
    return dht.readHumidity();
}

int getMoisture(){
  int pin = getMoisturePin();
  if(pin > -1){
    return analogRead(pin);
  }else{
    return 0;
  }
}

int getResistor(){
  int pin = getResistorPin();
  if(pin > -1){
    return analogRead(pin);
  }else{
    return 0;
  }
}

int getMagnetic(){
  int pin = getMagneticPin();
  if(pin > -1){
    return digitalRead(pin);
  }else{
    return 0;
  }
}