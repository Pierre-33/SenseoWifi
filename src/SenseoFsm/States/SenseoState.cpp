#include "SenseoState.h"
#include <Homie.h>

#include "../Components/ControlComponent.h"

void SenseoState::onInitialized()
{
    commandComponent = getComponent<CommandComponent>();
    controlComponent = getComponent<ControlComponent>();
    assert(commandComponent != nullptr);
    assert(controlComponent != nullptr);
}

bool SenseoState::hasOffCommands()
{
    if (commandComponent->hasCommand(CommandComponent::TurnOff)) return true;
    else if (commandComponent->hasCommand(CommandComponent::TurnOffAfterBrewing) && commandComponent->hasAnyCommands(CommandComponent::Brew1Cup | CommandComponent::Brew2Cup) == false) return true;

    return false;
}

void SenseoState::clearCommands(CommandComponent::CommandBitFields commands) 
{ 
    if ((commands & commandComponent->getCommands()) != 0)
    {
        Homie.getLogger() << getStateName() << ": Clearing commands " << commandComponent->getCommandsAsString(commands & commandComponent->getCommands()) << endl;
    }
    commandComponent->clearCommand(commands); 
}