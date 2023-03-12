#ifndef FsmComponent_h
#define FsmComponent_h

#include "FsmClassId.h"

class BaseFsmComponent
{
  public:
    virtual void update() {}
};

/**
 * Base class for FsmComponent
 */
template <typename Derived> class FsmComponent : public BaseFsmComponent
{
  public:
    static FsmClassId getClassId() {
      static FsmClassId id = FsmClassIdGenerator::nextId++;
      return id;
    }
};

#endif
