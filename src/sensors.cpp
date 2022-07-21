#include "sensors.h"

DHT dht(getDhtPin(), DHTTYPE); // we have to instantiate this class in order for it to be available

void initSensors(){
    int dhtPin = getDhtPin();
    int dhtType = getDhtType();
    
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
}

float getTemperatureF(){
    return dht.readTemperature(true);
}

float getTemperatureC(){
    return dht.readTemperature();
}

float getHumidity(){
    return dht.readHumidity();
}