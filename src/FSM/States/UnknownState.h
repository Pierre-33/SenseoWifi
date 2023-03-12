#ifndef UnknownState_h
#define UnknownState_h

#include "FSM/FsmState.h"

class SenseoLed;

class UnknownState : public FsmState<UnknownState>
{
    public:
        UnknownState(SenseoLed & led) : senseoLed(led) {}
        virtual void onUpdate();
    
    private:
        SenseoLed & senseoLed;
};

#endif

