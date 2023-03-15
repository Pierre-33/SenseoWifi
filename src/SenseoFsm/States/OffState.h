#pragma once
#include "SenseoState.h"

class SenseoLed;
class HomieNode;

class OffState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_OFF");
        OffState(const SenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;

    private: 
        //const SenseoLed & senseoLed;
        //HomieNode & senseoNode;
    
};
