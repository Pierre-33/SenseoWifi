#pragma once

#include <ezBuzzer.h>
#include "FSM/FsmComponent.h"

class BuzzerComponent : public  FsmComponent<BuzzerComponent>
{
    public:
        BuzzerComponent(int buzzerPin) : myBuzzer(buzzerPin) {}
        bool buzz(const String & tune);
        void update();

    private:
        ezBuzzer myBuzzer;
};