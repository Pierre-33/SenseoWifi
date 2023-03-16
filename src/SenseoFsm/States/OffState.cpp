#include "OffState.h"
#include "SenseoLed.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "UnknownState.h"
#include "../Components/BuzzerComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/ControlComponent.h"

void OffState::onEnter(StateId previousState) 
{
    clearCommands(CommandComponent::All);
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOff());
    senseoNode.setProperty("brewedSize").send("0");
    senseoNode.setProperty("power").send("false");
}

void OffState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
    else if (hasPendingCommands(CommandComponent::TurnOn))
    {
        controlComponent->pressPowerButton();
        processCommands(CommandComponent::TurnOn);
    }
}

void OffState::onExit(StateId nextState) 
{
    if (nextState != UnknownState::s_StateId) clearCommands(CommandComponent::TurnOn);
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody3"));
    senseoNode.setProperty("power").send("true");
    senseoNode.setProperty("outOfWater").send("false");
    senseoNode.setProperty("brew").send("false");
}