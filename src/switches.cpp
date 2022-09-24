#include <Arduino.h>
#include "switches.h"

bool toggleStatus; // false is off, true is on
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
  toggleStatus = status > 0;
}

void pressMomentary(int msTime){
  digitalWrite(selectedMomentaryPin, HIGH);

  delay(msTime);

  digitalWrite(selectedMomentaryPin, LOW);
}

bool isToggleOn();