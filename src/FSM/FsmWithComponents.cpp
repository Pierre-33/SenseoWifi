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
        
        //call state change handlers
        for (const auto & handler: stateChangeHandlers) {
            handler(currentState,nextState);
        }

        lastStateChangeMillis = lastUpdateMillis;
        nextState->onEnter(currentState ? currentState->getStateId() : INVALID_FSM_CLASS_ID);
        currentState = nextState;
        nextState = nullptr;
        
  }
}

void FsmWithComponents::changeState(StateId stateId) {
    if (currentState != nullptr && currentState->getStateId() != stateId) {
        if (nextState == nullptr || nextState->getStateId() != stateId) {
            nextState = states[stateId].get();
        }
    }
}

bool FsmWithComponents::isInState(StateId stateId) const {
    return stateId == currentState->getStateId(); 
}

void FsmWithComponents::setInitialState(StateId stateId) {
    assert(currentState == nullptr);
    nextState = states[stateId].get();
}

void FsmWithComponents::registerStateChangeHandler(const StateChangeHandler &handler)
{
    stateChangeHandlers.push_back(handler);
}