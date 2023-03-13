#ifndef UnknownState_h
#define UnknownState_h

#include "FSM/FsmState.h"

class SenseoLed;

class UnknownState : public FsmState
{
    public:
        DECLARE_STATE("SENSEO_UNKNOWN");
        UnknownState(SenseoLed & led) : senseoLed(led) {}
        virtual void onUpdate() override;
        virtual void onEnter(StateId previousState) override;
    
    private:
        SenseoLed & senseoLed;
};

#endif

