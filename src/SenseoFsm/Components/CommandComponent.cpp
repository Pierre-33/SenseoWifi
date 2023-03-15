#include "CommandComponent.h"
#include <Homie.h>

const char * CommandComponent::commandToString(Command command)
{ 
    auto iter = commandName.find(command);
    if (iter != commandName.end()) return iter->second;
    return nullptr;
}

String CommandComponent::getCommandsAsString(CommandBitFields commands)
{
    String commandString;
    for(const auto & it: commandName) 
    {
        if (hasCommand(commands,it.first))
        {
            if (commandString.isEmpty() == false) commandString += " | ";
            commandString += it.second;
        }
    }
    return commandString;
}

void CommandComponent::updateSenseoNode()
{
    senseoNode.setProperty("commands").send(getCommandsAsString());
}