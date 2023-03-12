#include "FsmWithComponents.h"
#include "FsmComponent.h"
#include "FsmState.h"

void FsmWithComponents::update(unsigned long currentTime) {
    lastUpdateMillis = currentTime;
    updateComponents();
    updateFsm();
}

void FsmWithComponents::updateComponents() {
    for(auto & component: components) {
        component.second->update();
    }
}

void FsmWithComponents::updateFsm() {
    if (nextState != nullptr) {
        if (currentState != nullptr) currentState->onExit(nextState->getStateId());
        lastStateChangeMillis = lastUpdateMillis;
        nextState->onEnter(currentState ? currentState->getStateId() : INVALID_FSM_CLASS_ID);
        currentState = nextState;
        nextState = nullptr;
        
  }
}

void FsmWithComponents::changeState(FsmClassId stateId) {
    if (currentState != nullptr && currentState->getStateId() != stateId) {
        if (nextState == nullptr || nextState->getStateId() != stateId) {
            nextState = states[stateId].get();
        }
    }
}

void FsmWithComponents::setInitialState(FsmClassId classId) {
    assert(false);
}