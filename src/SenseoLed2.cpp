#include "SenseoLed2.h"
#include "Arduino.h"
#include "constants.h"
#include <Homie.h>

int SenseoLed2::ledPin = 0;

//variable use both inside and outside the ISR should be volatile as they could be modified from outside the current code path
volatile static ledStateEnum ledState = LED_unknown;
//volatile static int pulseDuration = 0; // don't forget to remove the variable declaration in ledChangedIsr()

static uint32_t timerTick = uint32_t(float((pulseDurLedSlow + pulseDurTolerance)/*ms*/ * 1000 /*to us*/) / 3.2 /*to tick*/);

ledStateEnum SenseoLed2::getState() const 
{ 
    return ledState; 
}

void IRAM_ATTR SenseoLed2::ledChangedIsr() 
{
    static unsigned long ledChangeMillis = 0;

    unsigned long now = millis();
    int pulseDuration = now - ledChangeMillis;
    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance) ledState = LED_FAST;
    else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance) ledState = LED_SLOW;
    else pulseDuration = 0; //longer pulse duration means "no pulse" (continuous on or continuous off)

    //each time the led blink, we push back the timer
    timer1_write(timerTick);
    ledChangeMillis = now;
}

void IRAM_ATTR SenseoLed2::timerElapseIsr() 
{
    ledState = !digitalRead(ledPin) ? LED_ON : LED_OFF;
}

void SenseoLed2::attachInterrupt()
{
    ::attachInterrupt(digitalPinToInterrupt(ledPin), SenseoLed2::ledChangedIsr, CHANGE);
    timer1_attachInterrupt(SenseoLed2::timerElapseIsr);
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
    timer1_write(timerTick); 
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

void SenseoLed2::debugLog(HomieNode & senseoNode)
{
    static ledStateEnum prevState = LED_unknown;
    
    /*static int lastPulseDuration = 0;
    if (lastPulseDuration != pulseDuration) 
    {
        //pulseDuration is not thread safe. So the debug value display here could be glitchy every now and then
        if (pulseDuration > 0)
        {
            //pulse duration of 0 means continuous state
            Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;
        }
        lastPulseDuration = pulseDuration;
    }*/

    if (ledState != prevState)
    {
        Homie.getLogger() << "LED state machine, new LED state: " << getStateAsString() << endl;
        senseoNode.setProperty("ledState").send(getStateAsString());
        prevState = ledState;
    }
}
