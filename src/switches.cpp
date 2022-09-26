#include <Arduino.h>
#include "switches.h"

bool toggleStatus; // false is open, true is closed
int selectedTogglePin, selectedMomentaryPin;

void initSwitches(){
    selectedTogglePin = getTogglePin();
    selectedMomentaryPin = getMomentaryPin();

    if(selectedTogglePin > -1){
      pinMode(selectedTogglePin, OUTPUT);
      //default to off;
      digitalWrite(selectedTogglePin, LOW);
      toggleStatus = false;
    }

    if(selectedMomentaryPin > -1){
      pinMode(selectedMomentaryPin, OUTPUT);
    }
}

void flipToggleSwitch(){
  int status = digitalRead(selectedTogglePin);
  digitalWrite(selectedTogglePin, !status);
  //if the status we got from the read is 0
  //we have then switched to HIGH (or 1)
  //therefore the status would be true/closed
  toggleStatus = status < 1;
}

void pressMomentary(int msTime){
  digitalWrite(selectedMomentaryPin, HIGH);

  delay(msTime);

  digitalWrite(selectedMomentaryPin, LOW);
}

bool isToggleOn(){
  return toggleStatus;
}