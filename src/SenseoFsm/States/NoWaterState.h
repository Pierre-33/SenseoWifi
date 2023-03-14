#pragma once
#include "ModularFsm/FsmState.h"

class SenseoLed;
class HomieNode;

class NoWaterState : public FsmState
{
    public:
        DECLARE_STATE("SENSEO_NOWATTER");
        NoWaterState(SenseoLed & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    private:
        SenseoLed & senseoLed;
        HomieNode & senseoNode;
};
