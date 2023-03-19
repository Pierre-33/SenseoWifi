#pragma once
#include "SenseoState.h"

class SenseoLed;
class HomieNode;

class BrewingState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_BREWING");

        BrewingState(const ISenseoLed & led, HomieNode & node) : SenseoState(led,node) {}

        virtual void onEnter(StateId previousState) override;
        virtual void onExit(StateId nextState) override;
        virtual void onUpdate() override;
};
