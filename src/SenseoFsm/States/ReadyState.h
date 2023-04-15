#pragma once
#include "SenseoState.h"

class ISenseoLed;

class ReadyState : public SenseoState
{    
    public:
        DECLARE_STATE("SENSEO_READY");
        
        ReadyState(const ISenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        //virtual void onExit(FsmComponentId nextState) override;
        virtual void onUpdate() override;
    private:
        void processBrewingCommand(CommandComponent::Command command);
        bool waitingForACup = false;
};
