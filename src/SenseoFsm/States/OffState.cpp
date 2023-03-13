#include "OffState.h"
#include "SenseoLed.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "SenseoFsm/Components/BuzzerComponent.h"
#include "SenseoFsm/Components/SenseoLedComponent.h"

void OffState::onEnter(StateId previousState) {
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOff());
    senseoNode.setProperty("brewedSize").send("0");
    senseoNode.setProperty("power").send("false");
}

void OffState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}

void OffState::onExit(StateId nextState) {
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody3"));
    senseoNode.setProperty("power").send("true");
    senseoNode.setProperty("outOfWater").send("false");
    senseoNode.setProperty("brew").send("false");
}