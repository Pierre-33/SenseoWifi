#pragma once
#include "ModularFsm/FsmState.h"

class SenseoLed;

class ReadyState : public FsmState
{    
    public:
        DECLARE_STATE("SENSEO_READY");
        
        ReadyState(SenseoLed & led) : senseoLed(led) {}
        virtual void onEnter(StateId previousState) override;
        //virtual void onExit(FsmComponentId nextState) override;
        virtual void onUpdate() override;
    private:
        SenseoLed & senseoLed;
};
