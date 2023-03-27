#pragma once

#include "SenseoLed.h"
class String;
class HomieNode;

class SenseoLed2 : public ISenseoLed
{
    public:
        static void initialize(int pin) { ledPin = pin; };
        static void attachInterrupt();
        static void detachInterrupt();
        virtual ledStateEnum getState() const override;
        static String getStateAsString();
        static void debugLog(HomieNode & senseoNode);
    private:
        static void timerElapseIsr();
        static void ledChangedIsr();

        static int ledPin;
};