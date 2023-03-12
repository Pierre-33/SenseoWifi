#include "SenseoLedControl.h"

SenseoLedControl::SenseoLedControl(int pLedPin) {
  ledPin = pLedPin;
  timestampChanged = millis();
  digitalWrite(ledPin, LOW);  
}

void SenseoLedControl::turnOn(){
  milliOff = 0;
  milliOn = 0;
  if (currentState != true) {
    digitalWrite(ledPin, HIGH);
    currentState = true;
    timestampChanged = millis();
  }
}

void SenseoLedControl::turnOff(){
  milliOff = 0;
  milliOn = 0;
  if (currentState != false) {
    digitalWrite(ledPin, LOW);
    currentState = false;
    timestampChanged = millis();
  }
}

void SenseoLedControl::blink(unsigned long _milliOn, unsigned long _milliOff /*=0*/){
  milliOn = _milliOn;
  milliOff = _milliOff > 0 ?  _milliOff : _milliOn;  
}

void SenseoLedControl::update(){
  if (milliOn > 0 && milliOff > 0){
    unsigned long currentMillis = millis();
    if (currentState == true && currentMillis - timestampChanged >= milliOn){
      digitalWrite(ledPin, LOW);
      currentState = false;
      timestampChanged = millis();
    }
    else if (currentState == false && currentMillis - timestampChanged >= milliOff){
      digitalWrite(ledPin, HIGH);
      currentState = true;
      timestampChanged = millis();
    }
  }
}
