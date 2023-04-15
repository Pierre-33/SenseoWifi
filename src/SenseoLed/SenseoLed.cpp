/*
  SenseoLed.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoLed.h"
#include <Homie.h>
#include <Arduino.h>
#include "constants.h"

static SenseoLed *s_instance = nullptr;

void IRAM_ATTR ledChangedHandler()
{
    s_instance->pinStateToggled();
}

SenseoLed::SenseoLed(HomieNode &senseoNode, int pin)
    : ledPin(pin), senseoNode(senseoNode)
{
    ledChangeMillis = millis();
    pinMode(ledPin, INPUT_PULLUP);
    assert(s_instance == nullptr); // You can't have two instance of this class
    s_instance = this;
}

void SenseoLed::onMqttReady()
{
    senseoNode.setProperty("ledState").send(getStateAsString());
}

void SenseoLed::attachInterrupt()
{
    ::attachInterrupt(digitalPinToInterrupt(ledPin), ledChangedHandler, CHANGE);
}

void SenseoLed::detachInterrupt()
{
    ::detachInterrupt(digitalPinToInterrupt(ledPin));
}

void SenseoLed::pinStateToggled()
{
    unsigned long now = millis();
    if (now - ledChangeMillis <= LedIgnoreChangeDuration)
        return; // simple debouncer
    prevLedChangeMillis = ledChangeMillis;
    ledChangeMillis = now;
    ledChanged = true;
}

int SenseoLed::getLastPulseDuration() const
{
    return (ledChangeMillis - prevLedChangeMillis);
}

void SenseoLed::updateState()
{
    ledStatePrev = ledState;
    if (ledChanged)
    {
        // When there was an interrupt from the Senseo LED pin
        int pulseDuration = ledChangeMillis - prevLedChangeMillis;
        if (true)
            Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;

        // decide if LED is blinking fast or slow
        if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance)
        {
            ledState = LED_FAST;
        }
        else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance)
        {
            ledState = LED_SLOW;
        }
        else
        {
            // Nothing to do here.
            // pulseDuration could be below (user interaction) or above (end of a continuous state) the known times.
            // No actions needed.
        }
        ledChanged = false;
    }
    // decide if LED is not blinking but in a continuous state
    int t = (unsigned long)(millis() - ledChangeMillis);
    if ((t > pulseContThreshold) && (t < 3 * pulseContThreshold || ledState == LED_unknown))
    {
        ledState = !digitalRead(ledPin) ? LED_ON : LED_OFF;
    }

    if (hasChanged())
    {
        Homie.getLogger() << "LED state machine, new LED state: " << getStateAsString() << endl;
        senseoNode.setProperty("ledState").send(getStateAsString());
    }
}

bool SenseoLed::hasChanged() const
{
    // did the LED state change during last updateState() execution?
    return (ledStatePrev != ledState);
}

ledStateEnum SenseoLed::getState() const
{
    return ledState;
}

const char *SenseoLed::getStateAsString() const
{
    if (ledState == LED_OFF) return "LED_OFF";
    else if (ledState == LED_SLOW) return "LED_SLOW";
    else if (ledState == LED_FAST) return "LED_FAST";
    else if (ledState == LED_ON) return "LED_ON";
    else return "LED_unknown";
}
