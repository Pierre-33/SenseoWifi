#pragma once
#include "ModularFsm/FsmState.h"

class SenseoLed;

class HeatingState : public FsmState
{
    public:
        DECLARE_STATE("SENSEO_HEATING");
        HeatingState(const SenseoLed & led) : senseoLed(led) {}
        virtual void onEnter(StateId previousState) override;
        //virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
    private:
        const SenseoLed & senseoLed;
};
