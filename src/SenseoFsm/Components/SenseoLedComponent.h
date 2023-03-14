#pragma once

#include "ModularFsm/FsmComponent.h"

/**
 * SenseoLedComponent is use to control the Led of Senseo
 */
class SenseoLedComponent : public FsmComponent<SenseoLedComponent>
{
  public:
    SenseoLedComponent(int pLedPin);
    void update();
    void turnOn();
    void turnOff();
    void blink(unsigned long milliOn, unsigned long milliOff = 0); // 0 means milliOff will be equal to milliOn

  private:
    unsigned long timestampChanged;
    int ledPin;
    bool currentState = false;
    unsigned long milliOn = 0;  //0 means the Led won't blink
    unsigned long milliOff = 0; //0 means the Led won't blink
};
