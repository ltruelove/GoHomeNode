#include <Arduino.h>
#include "switches.h"

bool toggleStatus; // false is open, true is closed
int selectedTogglePin, selectedMomentaryPin;

void initSwitches(){
    selectedTogglePin = getTogglePin();
    Serial.println(selectedTogglePin);
    selectedMomentaryPin = getMomentaryPin();
    bool isClosedOn = getIsClosedOn();

    if(selectedTogglePin > -1){
      pinMode(selectedTogglePin, OUTPUT);
      //default to off;
      if(isClosedOn){
        digitalWrite(selectedTogglePin, LOW);
        toggleStatus = false;
      }else{
        //use this for devices that use an active current to
        //make a connection open instead of closed
        //still defaults to off in that respect
        digitalWrite(selectedTogglePin, HIGH);
        toggleStatus = true;
      }
    }

    if(selectedMomentaryPin > -1){
      pinMode(selectedMomentaryPin, OUTPUT);
    }
}

void flipToggleSwitch(){
  if(selectedTogglePin > -1){
    int status = digitalRead(selectedTogglePin);
    digitalWrite(selectedTogglePin, !status);
    //if the status we got from the read is 0
    //we have then switched to HIGH (or 1)
    //therefore the status would be true/closed
    toggleStatus = status < 1;
  }
}

void setMomentary(int setting){
  digitalWrite(selectedMomentaryPin, setting);
}

void pressMomentary(int msTime){
  digitalWrite(selectedMomentaryPin, HIGH);

  delay(msTime);

  digitalWrite(selectedMomentaryPin, LOW);
}

bool isToggleOn(){
  return toggleStatus;
}