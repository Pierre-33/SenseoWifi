#include "SenseoFsm.h"

#include "ModularFsm/ModularFsm.h"
#include "Components/SenseoLedComponent.h"
#include "Components/BuzzerComponent.h"
#include "Components/CupComponent.h"
#include "States/BrewingState.h"
#include "States/HeatingState.h"
#include "States/NoWaterState.h"
#include "States/OffState.h"
#include "States/ReadyState.h"
#include "States/UnknownState.h"
#include "pins.h"


void SenseoFsm::setup(const SenseoLed & led, HomieNode & node,bool useCupDetector, bool useBuzzer, bool useCustomizableButton) {
    if (useBuzzer) addComponent(std::make_unique<BuzzerComponent>(beeperPin));
    if (useCupDetector) addComponent(std::make_unique<CupComponent>(cupDetectorPin));
    if (useCustomizableButton) addComponent(std::make_unique<SenseoLedComponent>(senseoLedOutPin));

    addState(std::make_unique<BrewingState>(led,node));
    addState(std::make_unique<HeatingState>(led));
    addState(std::make_unique<NoWaterState>(led,node));
    addState(std::make_unique<OffState>(led,node));
    addState(std::make_unique<ReadyState>(led));
    addState(std::make_unique<UnknownState>(led));

    setInitialState<UnknownState>();
}

bool SenseoFsm::isOff() {
    return isInState<OffState>() || isInState<UnknownState>();
}