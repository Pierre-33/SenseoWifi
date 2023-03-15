#pragma once

#include "SenseoState.h"

class SenseoLed;

class UnknownState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_UNKNOWN");
        UnknownState(const SenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onUpdate() override;
        virtual void onEnter(StateId previousState) override;
};
