#include "FsmState.h"

void FsmState::initialize(ModularFsm * fsm)
{
    assert(fsm != nullptr);
    myFsm = fsm;
    onInitialized();
}

void FsmState::changeState(StateId stateId)
{
    assert (myFsm != nullptr);
    myFsm->changeState(stateId);
}