#include "FsmState.h"

void FsmState::initialize(FsmWithComponents * fsm)
{
    assert(fsm != nullptr);
    myFsm = fsm;
    onInitialized();
}

void FsmState::changeState(StateId classId)
{
    assert (myFsm != nullptr);
    myFsm->changeState(classId);
}