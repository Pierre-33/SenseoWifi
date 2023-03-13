#include "BrewingState.h"

#include "SenseoLed.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "constants.h"
#include "Fsm/Components/CupComponent.h"
#include "FSM/Components/SenseoLedComponent.h"

void BrewingState::onEnter(StateId previousState) {
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(2000,500));

    CupComponent * cupComponent = getComponent<CupComponent>();
    if (cupComponent != nullptr) {
        if (cupComponent->isAvailable()) {
            cupComponent->setFilling();
        }
        else {
            senseoNode.setProperty("debug").send("cup: Brewing without detected cup, will not report the filling->full process.");
        }
    }
    senseoNode.setProperty("brew").send("true");
    senseoNode.setProperty("brewedSize").send("0");
}
void BrewingState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}

void BrewingState::onExit(StateId nextState) {
    senseoNode.setProperty("brew").send("false");
    // Determine brewed cup size based on time in brewing state
    int brewedSeconds = (getTimeInState()+ 500) / 1000;
    int brewedSize = 0;

    if (brewedSeconds > 10) {
        // 0---------------------|-----+-----|-----+-----|-------100
        int tolerance = (BrewHeat2CupSeconds - BrewHeat1CupSeconds) / 2;

        if (nextState == ReadyState::s_StateId) {
            if (abs(brewedSeconds - BrewHeat1CupSeconds) < tolerance) {
                brewedSize = 1;
            }
            else if (abs(brewedSeconds - BrewHeat2CupSeconds) < tolerance) {
                brewedSize = 2;
            }
        }

        tolerance = (Brew2CupSeconds - Brew1CupSeconds) / 2;
        if (nextState == NoWaterState::s_StateId || nextState == OffState::s_StateId) {
            if (abs(brewedSeconds - Brew1CupSeconds) < tolerance) {
                brewedSize = 1;
            }
            else if (abs(brewedSeconds - Brew2CupSeconds) < tolerance) {
                brewedSize = 2;
            }
        }

        senseoNode.setProperty("brewedSize").send(String(brewedSize));
        if (brewedSize == 0) {
            senseoNode.setProperty("debug").send("brew: Unexpected time in SENSEO_BREWING state. Please adapt timings.");
        }
        CupComponent * cupComponent = getComponent<CupComponent>();
        if (cupComponent != nullptr) {
            if (cupComponent->isFilling()) cupComponent->setFull();
        }        
    }
}