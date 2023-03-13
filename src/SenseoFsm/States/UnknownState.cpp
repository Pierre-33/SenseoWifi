#include "UnknownState.h"
#include "SenseoLed.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "../Components/SenseoLedComponent.h"

void UnknownState::onEnter(StateId previousState) {
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(200,1000));
}

void UnknownState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}