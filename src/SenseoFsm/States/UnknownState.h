#pragma once

#include "SenseoState.h"

class ISenseoLed;

class UnknownState : public SenseoState
{
    public:
        DECLARE_STATE("SENSEO_UNKNOWN");
        UnknownState(const ISenseoLed & led, HomieNode & node) : SenseoState(led,node) {}
        virtual void onUpdate() override;
        virtual void onEnter(StateId previousState) override;
};
