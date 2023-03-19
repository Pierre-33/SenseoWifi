#pragma once
#include "SenseoState.h"

class ISenseoLed;
class HomieNode;

class NoWaterState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_NOWATTER");
        NoWaterState(const ISenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    private:
        //const SenseoLed & senseoLed;
        //HomieNode & senseoNode;
};
