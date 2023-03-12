#include "FsmState.h"

void BaseFsmState::initialize(FsmWithComponents * fsm)
{
    assert(fsm != nullptr);
    myFsm = fsm;
    onInitialized();
}

void BaseFsmState::changeState(FsmClassId classId)
{
    assert (myFsm != nullptr);
    myFsm->changeState(classId);
}