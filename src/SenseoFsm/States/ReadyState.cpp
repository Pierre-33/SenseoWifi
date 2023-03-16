#include "ReadyState.h"
#include "SenseoLed.h"
#include "BrewingState.h"
#include "NoWaterState.h"
#include "OffState.h"
#include "../Components/BuzzerComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/CommandComponent.h"
#include "../Components/ControlComponent.h"

void ReadyState::onEnter(StateId previousState) 
{
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOn());
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody1"));
}

void ReadyState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (hasOffCommands()) 
    {
        Homie.getLogger() << "Turning Off" << endl;
        controlComponent->pressPowerButton();
    }
    else if (ledState == LED_SLOW) changeState<BrewingState>(); //TODO overload this state and remove this statement when the Button addon is used
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (hasPendingCommands(CommandComponent::Brew1Cup))
    {
        controlComponent->pressLeftButton();
        processCommands(CommandComponent::Brew1Cup);
        changeState<BrewingState>();        
    }
    else if (hasPendingCommands(CommandComponent::Brew2Cup))
    {
        controlComponent->pressRightButton();
        processCommands(CommandComponent::Brew2Cup);
        changeState<BrewingState>();
    }
}