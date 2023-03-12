#pragma once
#include "FSM/FsmState.h"

class SenseoLed;

class HeatingState : public FsmState<HeatingState>
{
    public:
        HeatingState(SenseoLed & led) : senseoLed(led) {}
        //virtual void onEnter(FsmClassId previousState);
        //virtual void onExit(FsmClassId nextState);
        virtual void onUpdate();
    private:
        SenseoLed & senseoLed;
};
