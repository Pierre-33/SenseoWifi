#ifndef FsmWithComponents_h
#define FsmWithComponents_h

#include <memory>
#include <type_traits>
#include <map>
#include "FsmClassId.h"

class BaseFsmComponent;
class BaseFsmState;

class FsmWithComponents
{
    public: 
        template<class T> 
        bool addComponent(std::unique_ptr<T> && component){
            static_assert(std::is_base_of<BaseFsmComponent,T>::value, "T must derive from BaseFsmComponent");
            FsmClassId classId = T::getClassId();
            if (components.find(classId) != components.end()) return false;
            else {
                components[classId] = std::move(component);
                return true;
            }
        }

        template<class T> 
        bool addState(std::unique_ptr<T> && state){
            static_assert(std::is_base_of<BaseFsmState,T>::value, "T must derive from BaseFsmState");
            FsmClassId classId = T::getClassId();
            if (states.find(classId) != states.end()) return false;
            else {
                state->initialize(this);
                states[classId] = std::move(state);
                return true;
            }
        }

        template<class T> T * getComponent() {
            auto iter = components.find(T::getClassId());
            if (iter != components.end()) return (T*)iter->second.get();
            else return nullptr;
        }
        
        void update(unsigned long currentTime);
        void setInitialState(FsmClassId classId);
        template<class T> void setInitialState() { setInitialState(T::getClassId()); }
        unsigned long getTimeInState() { return lastUpdateMillis - lastStateChangeMillis; }

    private:
        friend class BaseFsmState;
        void changeState(FsmClassId classId); //changeState should not be called from outside a state

        void updateComponents();
        void updateFsm();

        unsigned long lastStateChangeMillis = 0;
        unsigned long lastUpdateMillis = 0;

        BaseFsmState * currentState = nullptr;
        BaseFsmState * nextState = nullptr;

        std::map<FsmClassId,std::unique_ptr<BaseFsmComponent>> components;
        std::map<FsmClassId,std::unique_ptr<BaseFsmState>> states;

};

#endif