#pragma once
#include <ErriezCRC32.h>
#include "FsmWithComponents.h"
#include "FsmStateId.h"

#define EXECUTE_IF_COMPONENT_EXIST(component,...) if (getComponent<component>() != nullptr) { getComponent<component>()->__VA_ARGS__; }
#define DECLARE_STATE(stateName)                                        \
    static inline const char* const s_StateName = stateName;            \
    static inline StateId const s_StateId = crc32String(stateName);     \
    virtual const char * getStateName() const { return this->s_StateName;  }  \
    virtual StateId getStateId() const { return this->s_StateId; }



class BaseFsmState
{
    public:
        virtual void onInitialized() {}
        virtual void onEnter(StateId previousState) {}
        virtual void onExit(StateId nextState) {}
        virtual void onUpdate() {}

        unsigned long getTimeInState() const { return myFsm->getTimeInState(); }

        virtual StateId getStateId() const { assert(!"please use DECLARE_STATE to declare your state"); return INVALID_STATE_ID; }
        virtual const char * getStateName() const { assert(!"please use DECLARE_STATE to declare your state"); return nullptr; }

    protected:
        void changeState(StateId classId);
        template<class T> void changeState() { changeState(T::s_StateId); }
        template<class T> T * getComponent() { return myFsm->getComponent<T>(); };

    private:
        friend class FsmWithComponents;
        void initialize(FsmWithComponents * fsm);
        FsmWithComponents * myFsm = nullptr;
};

/*template <typename Derived> class FsmState : public BaseFsmState
{
    public:
        virtual FsmClassId getStateId() { return getClassId();  }
        static FsmClassId getClassId() 
        {
            static FsmClassId id = FsmClassIdGenerator::nextId++;
            return id;
        }
};*/