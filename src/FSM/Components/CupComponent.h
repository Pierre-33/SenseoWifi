/*
  Cup.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#pragma once

#include "FSM/FsmComponent.h"

class CupComponent : public  FsmComponent<CupComponent>
{
  public:
    CupComponent(int pin);
    void update();
    void setFilling();
    void setFull();
    bool isAvailableChanged();
    bool isFullChanged();
    bool isAvailable();
    bool isNotAvailable();
    bool isFilling();
    bool isFull();
    bool isEmpty();
  private:
    int detectorPin;
    bool lastChangeValue = false;
    unsigned long lastChangeMillis = 0;
    bool availableChanged = false;
    bool fullChanged = false;
    bool cupAvailable = false;
    bool cupFilling = false;
    bool cupFull = false;
};