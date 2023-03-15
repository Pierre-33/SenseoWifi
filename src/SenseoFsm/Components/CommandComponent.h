#pragma once

#include "ModularFsm/FsmComponent.h"
#include <map>
#include <Arduino.h>

class HomieNode;


class CommandComponent : public  FsmComponent<CommandComponent>
{
    public:
        CommandComponent(HomieNode & node) : senseoNode(node) {}
        using CommandBitFields = unsigned char;
        enum Command : CommandBitFields
        {
            None = 0,
            TurnOn = 1,
            TurnOff = 2,
            Brew1Cup = 4,
            Brew2Cup = 8,
            TurnOffAfterBrewing = 16,
            All = 0xff,
        };

        void sendCommands(CommandBitFields newCommands) { commands = newCommands; updateSenseoNode(); }
        void clearCommand(CommandBitFields commandsToClear) { commands &= ~commandsToClear; updateSenseoNode(); }
        bool hasCommand(Command command) const { return hasCommand(command); }
        static bool hasCommand(CommandBitFields commands, Command command) { return (commands & command) == command; }
        bool hasAnyCommands(CommandBitFields command) const { return (commands & command) != 0; }
        CommandBitFields getCommands() const { return commands; }

        static const char * commandToString(Command command);
        String getCommandsAsString() const { return getCommandsAsString(commands); };
        static String getCommandsAsString(CommandBitFields commands);
        
    private:
        void updateSenseoNode();
        CommandBitFields commands = 0;
        HomieNode & senseoNode;

        static inline const std::map<Command,const char *> commandName = {{TurnOn,"TurnOn"},{TurnOff,"TurnOff"},{Brew1Cup,"Brew1Cup"},{Brew2Cup,"Brew2Cup"},{TurnOffAfterBrewing,"TurnOffAfterBrewing"}};
};