/*
  SenseoLed.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoLed_h
#define SenseoLed_h

#include "Homie.h"
#include "enums.h"
#include "constants.h"

class ISenseoLed
{
  public:
    virtual ledStateEnum getState() const = 0;
};

class SenseoLed : public ISenseoLed
{
  public:
    SenseoLed(int ledPin);
    void pinStateToggled();
    int getLastPulseDuration() const;
    void updateState();
    bool hasChanged() const;
    ledStateEnum getState() const override;
    String getStateAsString() const;
  private:
    int ocSenseLedPin;
    bool ledChanged = false;
    unsigned long ledChangeMillis;
    unsigned long prevLedChangeMillis = 0;
    ledStateEnum ledState = LED_unknown;
    ledStateEnum ledStatePrev = LED_unknown;
};

#endif
