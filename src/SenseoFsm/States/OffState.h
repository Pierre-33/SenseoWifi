#pragma once
#include "SenseoState.h"

class ISenseoLed;
class HomieNode;
class ProgramComponent;

class OffState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_OFF");
        OffState(const ISenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onInitialized() override;
        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;

    private: 
        ProgramComponent * programComponent;
    
};
