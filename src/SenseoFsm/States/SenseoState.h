#pragma once
#include "ModularFsm/FsmState.h"

#include "SenseoFsm/Components/CommandComponent.h"

class SenseoLed;
class HomieNode;
class ControlComponent;

class SenseoState : public FsmState
{    
    public:
        SenseoState(const SenseoLed & led, HomieNode & node) : senseoLed(led),senseoNode(node) {}

        void onInitialized() override;

        bool hasOffCommands();
        bool hasAnyCommands(CommandComponent::CommandBitFields commands) { return commandComponent->hasAnyCommands(commands); }
        bool hasCommand(CommandComponent::Command command) { return commandComponent->hasCommand(command); }
        void clearCommands(CommandComponent::CommandBitFields commands);

    protected:
        const SenseoLed & senseoLed;
        HomieNode & senseoNode;
        CommandComponent * commandComponent = nullptr;    
        ControlComponent * controlComponent = nullptr;    
};