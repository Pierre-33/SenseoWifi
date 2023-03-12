#pragma once
using FsmClassId = int;

const FsmClassId INVALID_FSM_CLASS_ID = -1;

struct FsmClassIdGenerator {
  static FsmClassId nextId;
};