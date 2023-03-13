#include "NoWaterState.h"

#include "SenseoLed.h"
#include "HeatingState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "../Components/BuzzerComponent.h"
#include "../Components/SenseoLedComponent.h"

void NoWaterState::onEnter(StateId previousState) {
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(100));
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody2"));
    senseoNode.setProperty("outOfWater").send("true");
}

void NoWaterState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
    else if (ledState == LED_OFF)  changeState<OffState>();
}

void NoWaterState::onExit(StateId nextState) {
    if (nextState != OffState::s_StateId) {
        senseoNode.setProperty("outOfWater").send("false");
      }
}