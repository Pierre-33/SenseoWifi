/*
  Cup.h - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/
#pragma once

#include "ModularFsm/FsmComponent.h"

class HomieNode;

class CupComponent : public  FsmComponent<CupComponent>
{
  public:
    CupComponent(HomieNode & node, int pin);
    void update();
    void setFilling();
    void setFull();
    bool isAvailable() const;
    bool isNotAvailable() const;
    bool isFilling() const;
    bool isFull() const;
    bool isEmpty() const;
  private:
    HomieNode & senseoNode;
    int detectorPin;
    bool lastChangeValue = false;
    unsigned long lastChangeMillis = 0;
    bool cupAvailable = false;
    bool cupFilling = false;
    bool cupFull = false;
};