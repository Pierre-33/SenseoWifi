#include "SenseoLedComponent.h"
#include "Arduino.h"

/*#include "Homie.h"
#include "enums.h"
#include "constants.h"*/


SenseoLedComponent::SenseoLedComponent(int pLedPin) 
{
  ledPin = pLedPin;
  timestampChanged = millis();
  digitalWrite(ledPin, LOW);  
}

void SenseoLedComponent::turnOn()
{
  milliOff = 0;
  milliOn = 0;
  if (currentState != true) 
  {
    digitalWrite(ledPin, HIGH);
    currentState = true;
    timestampChanged = millis();
  }
}

void SenseoLedComponent::turnOff()
{
  milliOff = 0;
  milliOn = 0;
  if (currentState != false) 
  {
    digitalWrite(ledPin, LOW);
    currentState = false;
    timestampChanged = millis();
  }
}

void SenseoLedComponent::blink(unsigned long _milliOn, unsigned long _milliOff /*=0*/)
{
  milliOn = _milliOn;
  milliOff = _milliOff > 0 ?  _milliOff : _milliOn;  
}

void SenseoLedComponent::update()
{
  if (milliOn > 0 && milliOff > 0)
  {
    unsigned long currentMillis = millis();
    if (currentState == true && currentMillis - timestampChanged >= milliOn)
    {
      digitalWrite(ledPin, LOW);
      currentState = false;
      timestampChanged = millis();
    }
    else if (currentState == false && currentMillis - timestampChanged >= milliOff)
    {
      digitalWrite(ledPin, HIGH);
      currentState = true;
      timestampChanged = millis();
    }
  }
}
