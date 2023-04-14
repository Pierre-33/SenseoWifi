#pragma once

enum ledStateEnum : char //this garanty that accessing the enum is atomic (ie one instruction)
{
  LED_unknown,
  LED_OFF,
  LED_SLOW,
  LED_FAST,
  LED_ON
};

class ISenseoLed
{
  public:
    virtual ledStateEnum getState() const = 0;
    virtual const char * getStateAsString() const = 0;

    virtual void attachInterrupt() {}
    virtual void detachInterrupt() {}
    virtual void updateState() {} 
    virtual void onMqttReady() {}
};