#pragma once

#include <functional>
#include <vector>
#include <set>
#include <memory>
#include <type_traits>
#include <map>
#include "FsmComponentId.h"
#include "FsmStateId.h"

class BaseFsmComponent;
class FsmState;

typedef std::function<void (FsmState * prevState, FsmState * newState)> StateChangeHandler;

class ModularFsm
{
    public: 
        template<class T> 
        bool addComponent(std::unique_ptr<T> && component){
            static_assert(std::is_base_of<BaseFsmComponent,T>::value, "T must derive from BaseFsmComponent");
            FsmComponentId classId = T::getClassId();
            if (components.find(classId) != components.end()) return false;
            else {
                components[classId] = std::move(component);
                return true;
            }
        }

        template<class T> 
        bool addState(std::unique_ptr<T> && state){
            static_assert(std::is_base_of<FsmState,T>::value, "T must derive from FsmState");
            StateId stateId = T::s_StateId;
            if (states.find(stateId) != states.end()) return false;
            else {
                state->initialize(this);
                states[stateId] = std::move(state);
                return true;
            }
        }

        template<class T> T * getComponent() {
            auto iter = components.find(T::getClassId());
            if (iter != components.end()) return (T*)iter->second.get();
            else return nullptr;
        }

        // The function will be call after the prevState->OnExit() and before nextState->onEntry()
        // Calling prevState->getTimeInState() will properly return the time spend in that state
        void registerStateChangeHandler(const StateChangeHandler &handler);
        
        void update(unsigned long currentTime);

        void setInitialState(StateId stateId);
        template<class T> void setInitialState() { setInitialState(T::s_StateId); }

        bool isInState(StateId stateId) const;
        template <class T> bool isInState() const { return isInState(T::s_StateId); }
        unsigned long getTimeInState() const { return lastUpdateMillis - lastStateChangeMillis; }

    private:
        friend class FsmState;
        void changeState(StateId classId); //changeState should not be called from outside a state

        void updateComponents();
        void updateFsm();

        unsigned long lastStateChangeMillis = 0;
        unsigned long lastUpdateMillis = 0;

        FsmState * currentState = nullptr;
        FsmState * nextState = nullptr;
        std::vector<StateChangeHandler> stateChangeHandlers;

        std::map<FsmComponentId,std::unique_ptr<BaseFsmComponent>> components;
        std::map<StateId,std::unique_ptr<FsmState>> states;

};