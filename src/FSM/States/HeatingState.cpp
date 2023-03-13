#include "HeatingState.h"

#include "SenseoLed.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "BrewingState.h"
#include "FSM/Components/SenseoLedComponent.h"

void HeatingState::onEnter(StateId previousState) {
  EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(1000));
}

void HeatingState::onUpdate() {
  ledStateEnum ledState = senseoLed.getState();

  if (ledState == LED_OFF)  changeState<OffState>();
  else if (ledState == LED_FAST) changeState<NoWaterState>();
  else if (ledState == LED_ON) changeState<ReadyState>();
  else if (getTimeInState() > (1000 * (HeatingTime + HeatingTimeTol))) {
    // Heating takes more time then expected, assume immediate brew.
    changeState<BrewingState>();
  }
}