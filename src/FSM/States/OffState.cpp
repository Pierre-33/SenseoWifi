#include "OffState.h"
#include "SenseoLed.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "FSM/Components/BuzzerComponent.h"

void OffState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}

void OffState::onExit(FsmClassId nextState) {
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,buzz("melody3"));
    senseoNode.setProperty("power").send("true");
    senseoNode.setProperty("outOfWater").send("false");
    senseoNode.setProperty("brew").send("false");
}