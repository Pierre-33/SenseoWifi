/*
  SenseoControl.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#ifndef SenseoLedControl_h
#define SenseoLedControl_h

#include "Homie.h"
#include "enums.h"
#include "constants.h"
#include "FSM/FsmComponent.h"

/**
 * SenseoLedControl is use to control the Led of Senseo
 */
class SenseoLedControl : public FsmComponent<SenseoLedControl>
{
  public:
    SenseoLedControl(int pLedPin);
    void update();
    void turnOn();
    void turnOff();
    void blink(unsigned long milliOn, unsigned long milliOff = 0); // 0 means milliOff will be equal to milliOn

    void test() { Homie.getLogger() << "SenseoLedControl ClassId = " << getClassId() << endl; }

  private:
    unsigned long timestampChanged;
    int ledPin;
    bool currentState = false;
    unsigned long milliOn = 0;  //0 means the Led won't blink
    unsigned long milliOff = 0; //0 means the Led won't blink
};

#endif
