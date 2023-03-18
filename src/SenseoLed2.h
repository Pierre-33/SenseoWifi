#pragma once

#include "enums.h"
class String;

class SenseoLed2
{
    public:
        static void initialize(int pin) { ledPin = pin; };
        static void attachInterrupt();
        static void detachInterrupt();
        static ledStateEnum getState() { return ledState; }
        static String getStateAsString();
        static void debugLog();
    private:
        static void timerElapseIsr();
        static void ledChangedIsr();

        static int ledPin;
        static unsigned long ledChangeMillis;
        static ledStateEnum ledState;

        //use for debug log
        static bool hasPulse;
        static int  pulseDuration;
        static ledStateEnum prevState;
};