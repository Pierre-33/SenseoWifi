#pragma once
#include "SenseoState.h"

class SenseoLed;

class ReadyState : public SenseoState
{    
    public:
        DECLARE_STATE("SENSEO_READY");
        
        ReadyState(const SenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        //virtual void onExit(FsmComponentId nextState) override;
        virtual void onUpdate() override;
    private:
        //const SenseoLed & senseoLed;
};
