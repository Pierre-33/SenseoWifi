/*
CupComponent.cpp - Library for the SenseoWifi project.
Created by Thomas Dietrich, 2016-03-05.
Released under some license.
*/

#include "CupComponent.h"

#include "Homie.h"
#include "constants.h"


CupComponent::CupComponent(int pin)
{
  detectorPin = pin;
  cupAvailable = !digitalRead(detectorPin);
}

void CupComponent::update() {
  bool value = !digitalRead(detectorPin);
  //debounce
  if (value != lastChangeValue) {
    lastChangeValue = value;
    lastChangeMillis = millis();
  }
  if (millis() - lastChangeMillis <= CupDebounceInterval) return;
  // process debounced detector reading
  if (value != cupAvailable) {
    cupAvailable = value;
    cupFilling = false;
    availableChanged = true;
    if (cupAvailable || (!cupAvailable && cupFull)) {
      cupFull = false;
      fullChanged = true;
    }
  }
}

void CupComponent::setFilling() {
  cupFilling = true;
  cupFull = false;
}

void CupComponent::setFull() {
  cupFilling = false;
  cupFull = true;
  fullChanged = true;
}

bool CupComponent::isAvailableChanged() {
  if (availableChanged) {
    availableChanged = false;
    return true;
  };
  return false;
}

bool CupComponent::isFullChanged() {
  if (fullChanged) {
    fullChanged = false;
    return true;
  };
  return false;
}

bool CupComponent::isAvailable() {
  return cupAvailable;
}

bool CupComponent::isNotAvailable() {
  return !cupAvailable;
}

bool CupComponent::isFilling() {
  return cupFilling;
}

bool CupComponent::isFull() {
  return cupFull;
}

bool CupComponent::isEmpty() {
  return !cupFull;
}
