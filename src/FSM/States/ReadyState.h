#pragma once
#include "FSM/FsmState.h"

class SenseoLed;

class ReadyState : public FsmState
{    
    public:
        DECLARE_STATE("SENSEO_READY");
        
        ReadyState(SenseoLed & led) : senseoLed(led) {}
        virtual void onEnter(StateId previousState);
        //virtual void onExit(FsmClassId nextState);
        virtual void onUpdate();
    private:
        SenseoLed & senseoLed;
};
