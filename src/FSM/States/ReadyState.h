#pragma once
#include "FSM/FsmState.h"

class SenseoLed;

class ReadyState : public FsmState<ReadyState>
{
    public:
        ReadyState(SenseoLed & led) : senseoLed(led) {}
        //virtual void onEnter(FsmClassId previousState);
        //virtual void onExit(FsmClassId nextState);
        virtual void onUpdate();
    private:
        SenseoLed & senseoLed;
};
