#pragma once
#include "ModularFsm/ModularFsm.h"
#include "Components/CommandComponent.h"

class ISenseoLed;
class HomieNode;

class SenseoFsm : public ModularFsm
{
    public:
        SenseoFsm(HomieNode & node) : ModularFsm() , senseoNode(node) {}
        void setup(const ISenseoLed & led,bool useCupDetector, bool useBuzzer, bool useCustomizableButton);
        bool isOff();

        bool sendCommands(CommandComponent::CommandBitFields commands);

        void onStateChange(FsmState * prevState, FsmState * nextState) override;

    protected:
        HomieNode & senseoNode;
};