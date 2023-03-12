#pragma once
#include "FSM/FsmState.h"

class SenseoLed;
class HomieNode;

class NoWaterState : public FsmState<NoWaterState>
{
    public:
        NoWaterState(SenseoLed & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}
        //virtual void onEnter(FsmClassId previousState);
        virtual void onExit(FsmClassId nextState);
        virtual void onUpdate();
    private:
        SenseoLed & senseoLed;
        HomieNode & senseoNode;
};
