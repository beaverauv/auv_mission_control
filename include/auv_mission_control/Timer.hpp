#ifndef TIMER_H
#define TIMER_H

#include <memory>
#include <auv_mission_control/Macho.hpp>
#include <auv_mission_control/StateMachine.hpp>

namespace Timer {

//EXAMPLES

//Wait a specified amount of time and call an event while switching to any state
//Event must be virtual function in the Top state, with the same event implemented in the state being called
//It will call the version in the state you are switching too
//setState<Timer::Timer<Whatever> >(3.0, Macho::Event(&Top::here));
//setState<Timer::Timer<StateToSwitchTo> >(TimeToWait, Macho::Event(&Top::eventToCall));


//Wait a specified amount of time and switch to any state
//setState<Timer::Timer<Init> >(3.0);
//setState<Timer::Timer<StateToSwitchTo> >(TimeToWait);


//Queue state switch in main state machine
//setState<Timer::Timer<Init> >(3.0, StateMachine::Gate::alias());
//setState<Timer::Timer<CurrentState> >(3.0, StateToSwitchTo::alias());

template<class T>
TSUBSTATE(Timer, T) {
        struct Box {
                Box() : waitTime_(0), startTime_(0), event_(0), isEventSet(false), statemachine_(), isStateMachineSet(false), alias_(TOP::alias()), isAliasSet(false) {
                }
                double waitTime_;
                double startTime_;

                Macho::IEvent<typename T:: Top> * event_;
                bool isEventSet;

                std::shared_ptr<StateMachine> statemachine_;
                bool isStateMachineSet;

                Macho::Alias alias_;
                bool isAliasSet;
        };
        TSTATE(Timer)
        inline void run(){
                if ((ros::Time::now().toSec() - box().startTime_) > box().waitTime_) {
                        if (box().isEventSet) {
                                LINK::dispatch(box().event_);
                        }
                        if (box().isAliasSet) {
                                TOP::box().statemachine_->template queueState(box().alias_);
                                return;
                        }


                        TOP::setState(SUPER::alias());
                }
        }
private:
        inline void init(double waitTime){
                box().waitTime_ = waitTime;
                box().startTime_ = ros::Time::now().toSec();
        }
        inline void init(double waitTime, Macho::IEvent<typename T:: Top> * event){
                box().waitTime_ = waitTime;
                box().startTime_ = ros::Time::now().toSec();
                box().isEventSet = true;
                box().event_ = event;
        }
        // void init(double waitTime, int i);
        inline void init(double waitTime, Macho::Alias alias){
                box().waitTime_ = waitTime;
                box().startTime_ = ros::Time::now().toSec();
                box().alias_ = alias;
                box().isAliasSet = true;
        }

};


}


#endif
