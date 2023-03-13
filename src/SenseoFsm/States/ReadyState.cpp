#include "ReadyState.h"
#include "SenseoLed.h"
#include "BrewingState.h"
#include "NoWaterState.h"
#include "OffState.h"
#include "SenseoFsm/Components/BuzzerComponent.h"
#include "SenseoFsm/Components/SenseoLedComponent.h"

void ReadyState::onEnter(StateId previousState) {
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOn());
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody1"));
}

void ReadyState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (ledState == LED_SLOW) changeState<BrewingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
}