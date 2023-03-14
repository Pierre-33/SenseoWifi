#pragma once

#include "ModularFsm/FsmState.h"

class SenseoLed;

class UnknownState : public FsmState
{
    public:
        DECLARE_STATE("SENSEO_UNKNOWN");
        UnknownState(const SenseoLed & led) : senseoLed(led) {}
        virtual void onUpdate() override;
        virtual void onEnter(StateId previousState) override;
    
    private:
        const SenseoLed & senseoLed;
};
