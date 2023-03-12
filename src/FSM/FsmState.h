#pragma once
#include "FsmClassId.h"
#include "FsmWithComponents.h"

#define EXECUTE_IF_COMPONENT_EXIST(component,...) if (getComponent<component>() != nullptr) { getComponent<component>()->__VA_ARGS__; }

class BaseFsmState
{
    public:
        virtual void onInitialized() {}
        virtual void onEnter(FsmClassId previousState) {}
        virtual void onExit(FsmClassId nextState) {}
        virtual void onUpdate() {}

        unsigned long getTimeInState() { return myFsm->getTimeInState(); }

        virtual FsmClassId getStateId() { return INVALID_FSM_CLASS_ID; }

    protected:
        void changeState(FsmClassId classId);
        template<class T> void changeState() { changeState(T::getClassId()); }
        template<class T> T * getComponent() { return myFsm->getComponent<T>(); };

    private:
        friend class FsmWithComponents;
        void initialize(FsmWithComponents * fsm);
        FsmWithComponents * myFsm = nullptr;
};

template <typename Derived> class FsmState : public BaseFsmState
{
    public:
        virtual FsmClassId getStateId() { return getClassId();  }
        static FsmClassId getClassId() 
        {
            static FsmClassId id = FsmClassIdGenerator::nextId++;
            return id;
        }
};