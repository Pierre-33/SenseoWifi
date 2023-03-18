#include "SenseoLed2.h"
#include "Arduino.h"
#include "constants.h"
#include <Homie.h>

int SenseoLed2::ledPin = 0;
int SenseoLed2::pulseDuration = 0;
unsigned long SenseoLed2::ledChangeMillis = 0;
ledStateEnum SenseoLed2::ledState = LED_unknown;

ledStateEnum SenseoLed2::prevState = LED_unknown;
bool SenseoLed2::hasPulse = false;

static uint32_t timerTick = int(pulseDurLedSlow * 1.1)/*seconds*/ * 100000 /*to us*/ * 5 /*to tick*/;

void IRAM_ATTR SenseoLed2::ledChangedIsr() 
{
    unsigned long now = millis();
    int pulseDuration = now - ledChangeMillis;
    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance) ledState = LED_FAST;
    else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance) ledState = LED_SLOW;

    //each time the led blink, we push back the timer
    timer1_write(timerTick);
    ledChangeMillis = now;
    
    //use to trigger debug log
    hasPulse = true;
}

void IRAM_ATTR SenseoLed2::timerElapseIsr() 
{
    ledState = !digitalRead(ledPin) ? LED_ON : LED_OFF;
}

void SenseoLed2::attachInterrupt()
{
    ::attachInterrupt(digitalPinToInterrupt(ledPin), SenseoLed2::ledChangedIsr, CHANGE);
    timer1_attachInterrupt(SenseoLed2::timerElapseIsr);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(timerTick); // 2500000 / 5 ticks per us from TIM_DIV16 == 500,000 us interval 
}

void SenseoLed2::detachInterrupt()
{
    ::detachInterrupt(digitalPinToInterrupt(ledPin));
    timer1_detachInterrupt();
    timer1_disable();
}

String SenseoLed2::getStateAsString() 
{
    if (ledState == LED_OFF) return "LED_OFF";
    else if (ledState == LED_SLOW) return "LED_SLOW";
    else if (ledState == LED_FAST) return "LED_FAST";
    else if (ledState == LED_ON) return "LED_ON";
    else return "LED_unknown";
}

void SenseoLed2::debugLog()
{
    if (hasPulse) 
    {
        //pulseDuration is not thread safe. So the debug value display here could be glitchy every now and then
        Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;
        hasPulse = false;
    }
    if (ledState != prevState)
    {
        Homie.getLogger() << "LED state machine, new LED state: " << getStateAsString() << endl;
        prevState = ledState;
    }
}
