#include "BrewingState.h"

#include "SenseoLed.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "constants.h"

void BrewingState::onEnter(FsmClassId previousState) {

}
void BrewingState::onUpdate() {
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}

void BrewingState::onExit(FsmClassId nextState) {
    /*senseoNode.setProperty("brew").send("false");
    // Determine brewed cup size based on time in brewing state
    int brewedSeconds = mySenseoSM.getSecondsInLastState();
    int brewedSize = 0;

    if (brewedSeconds > 10) {
        // 0---------------------|-----+-----|-----+-----|-------100
        int tolerance = (BrewHeat2CupSeconds - BrewHeat1CupSeconds) / 2;

        if (mySenseoSM.getState() == SENSEO_READY) {
            if (abs(brewedSeconds - BrewHeat1CupSeconds) < tolerance) {
                brewedSize = 1;
            }
            else if (abs(brewedSeconds - BrewHeat2CupSeconds) < tolerance) {
                brewedSize = 2;
            }
        }

        tolerance = (Brew2CupSeconds - Brew1CupSeconds) / 2;
        if (mySenseoSM.getState() == SENSEO_NOWATER || mySenseoSM.getState() == SENSEO_OFF) {
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
        if (CupDetectorAvailableSetting.get() && myCup.isFilling()) myCup.setFull();        
    }*/
}