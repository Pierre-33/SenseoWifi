#include "NoWaterState.h"

#include "SenseoLed.h"
#include "HeatingState.h"
#include "ReadyState.h"
#include "OffState.h"

void NoWaterState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
    else if (ledState == LED_OFF)  changeState<OffState>();
}

void NoWaterState::onExit(FsmClassId nextState) {
    if (nextState != OffState::getClassId()) {
        senseoNode.setProperty("outOfWater").send("false");
      }
}