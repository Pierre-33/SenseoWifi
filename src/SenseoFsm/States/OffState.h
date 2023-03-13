#pragma once
#include "FsmWithComponent/FsmState.h"

class SenseoLed;
class HomieNode;

class OffState : public FsmState
{
    public:
        DECLARE_STATE("SENSEO_OFF");
        OffState(SenseoLed & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;

    private: 
        SenseoLed & senseoLed;
        HomieNode & senseoNode;
    
};
