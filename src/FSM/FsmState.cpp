#include "FsmState.h"

void FsmState::initialize(FsmWithComponents * fsm)
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