#include "SenseoInputButtons.h"

SenseoInputButtons::SenseoInputButtons(int pLedPin) 
{
  analogPin = analogPin;
}

void SenseoInputButtons::addButtonPushHandler(int buttonValue,const ButtonHandler &handler)
{
  handlers.push_back({.type = ON_PUSH,.buttonValue = buttonValue,.handler = handler});
  handlersByValue.insert(buttonValue);
}
void SenseoInputButtons::addButtonHoldHandler(int buttonValue,unsigned long time,const ButtonHandler &handler)
{
  handlers.push_back({.type = ON_HOLD,.buttonValue = buttonValue,.handler = handler,.time = time});
  handlersByValue.insert(buttonValue);
}

void SenseoInputButtons::addButtonReleaseHandler(int buttonValue,unsigned long time,const ButtonHandler &handler)
{
  handlers.push_back({.type = ON_RELEASE,.buttonValue = buttonValue,.handler = handler,.time = time});
  handlersByValue.insert(buttonValue);
}

bool SenseoInputButtons::hasHandler(int reading)
{
  if (reading <= A0buttonThreeshold) return true; //when no button is press, we don't need a handler
  for(const auto &handler: handlersByValue) 
  {
    if (abs(handler - reading) <= A0buttonThreeshold) return true;
  }
  return false;
}

void SenseoInputButtons::update()
{
  if (millis() - lastReadingTime < 10) return; //too much reading of A0 break the wifi

  int reading = analogRead(analogPin);
  unsigned long previousReadingTime = lastReadingTime;
  lastReadingTime = millis();

  if (abs(previousReading - reading) > A0buttonThreeshold) 
  {
    //Manage push handlers
    for(const auto & handlerData: handlers) 
    {
        if (handlerData.type == ON_PUSH && abs(handlerData.buttonValue - reading) <= A0buttonThreeshold) handlerData.handler();
    }

    //Manage release handlers
    if (previousReading > A0buttonThreeshold) 
    {
      const HandlerData * bestHandler = nullptr;
      unsigned long releaseTime = lastReadingTime - lastReadingChangeTime;
      for (const auto & handlerData: handlers) 
      {
        if (handlerData.type == ON_RELEASE && abs(handlerData.buttonValue - previousReading) <= A0buttonThreeshold) 
        {
          // we press the button long enough
          if (releaseTime >= handlerData.time) 
          {
            // we want to trigger only the matching handler with the longest push time
            if (bestHandler == nullptr || handlerData.time > bestHandler->time) 
            {
              bestHandler = &(handlerData);
            }
          }
        }
      }
      if (bestHandler != nullptr) bestHandler->handler();
    }

    lastReadingChangeTime = lastReadingTime; // need to stay at the end of the "if"
  }
  else
  {
    //Manage Hold handlers
    for (const auto & handlerData: handlers) 
    {
        if (handlerData.type == ON_HOLD && abs(handlerData.buttonValue - previousReading) <= A0buttonThreeshold) 
        {
          unsigned long triggerTime = lastReadingChangeTime + handlerData.time;
          if (previousReadingTime < triggerTime && lastReadingTime >= triggerTime) handlerData.handler();
        }
      }
  }

  if (!hasHandler(reading)) Homie.getLogger() << "Button Handler not found for " << reading << endl;
  previousReading = reading;
}
