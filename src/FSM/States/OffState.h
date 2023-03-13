#pragma once
#include "FSM/FsmState.h"

class SenseoLed;
class HomieNode;

class OffState : public BaseFsmState
{
    public:
        DECLARE_STATE("SENSEO_OFF");
        OffState(SenseoLed & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}
        virtual void onEnter(StateId previousState);
        virtual void onExit(StateId nextState);
        virtual void onUpdate();

    private: 
        SenseoLed & senseoLed;
        HomieNode & senseoNode;
    
};
