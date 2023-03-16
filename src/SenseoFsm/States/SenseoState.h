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

        bool hasOffCommands() const;
        bool hasPendingCommands(CommandComponent::CommandBitFields commands) const { return commandComponent->hasPendingCommands(commands); }
        bool hasProcessedCommands(CommandComponent::CommandBitFields commands) const { return commandComponent->hasProcessedCommands(commands); }
        void processCommands(CommandComponent::CommandBitFields commands);
        void clearCommands(CommandComponent::CommandBitFields commands);

    protected:
        const SenseoLed & senseoLed;
        HomieNode & senseoNode;
        CommandComponent * commandComponent = nullptr;    
        ControlComponent * controlComponent = nullptr;    
};