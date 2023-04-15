/*
  SenseoLed.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoLed_h
#define SenseoLed_h

#include "ISenseoLed.h"

class HomieNode;

class SenseoLed : public ISenseoLed
{
public:
    SenseoLed(HomieNode &senseoNode, int ledPin);

    void attachInterrupt() override;
    void detachInterrupt() override;

    void updateState() override;
    void onMqttReady() override;

    ledStateEnum getState() const override;
    const char *getStateAsString() const override;

    void pinStateToggled();

private:
    int getLastPulseDuration() const;
    bool hasChanged() const;

    int ledPin;
    bool ledChanged = false;
    unsigned long ledChangeMillis;
    unsigned long prevLedChangeMillis = 0;
    ledStateEnum ledState = LED_unknown;
    ledStateEnum ledStatePrev = LED_unknown;
    HomieNode &senseoNode;
};

#endif
