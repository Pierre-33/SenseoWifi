#include "FsmState.h"

void BaseFsmState::initialize(FsmWithComponents * fsm)
{
    assert(fsm != nullptr);
    myFsm = fsm;
    onInitialized();
}

void BaseFsmState::changeState(StateId classId)
{
    assert (myFsm != nullptr);
    myFsm->changeState(classId);
}