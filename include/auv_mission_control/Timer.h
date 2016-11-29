#ifndef TIMER_H
#define TIMER_H

#include <auv_mission_control/Macho.hpp>

namespace Timer {

template<class T>
TSUBSTATE(Timer, T) {
        struct Box {
                Box() : waitTime(0), currentState(T::alias()), startTime(0) {
                }
                double waitTime;
                Macho::Alias currentState;
                double startTime;
        };
        TSTATE(Timer)
        inline void run(){
                if ((ros::Time::now().toSec() - box().startTime) > box().waitTime) {
                        T::setState(box().currentState);
                }
        }
private:
        inline void init(double waitTime, Macho::Alias currentState){
                box().waitTime = waitTime;
                box().currentState = currentState;
                box().startTime = ros::Time::now().toSec();
        }
};


//Experimental version that allows calling events from timer, doesn't work.
// setState<Timer::Timer<Top> >(3.0, Macho::Event(&TaskTest::Top::here));

// template<class T>
// TSUBSTATE(Timer, T) {
//         struct Box {
//                 Box() : waitTime(0), startTime(0), isStateSet(false), isEventSet(false), currentState(T::alias()), event(0) {
//                 }
//                 double waitTime;
//                 double startTime;
//                 bool isStateSet;
//                 bool isEventSet;
//                 Macho::Alias currentState;
//                 Macho::IEvent<T> * event;
//         };
//         TSTATE(Timer)
//         inline void run(){
//                 if ((ros::Time::now().toSec() - box().startTime) > box().waitTime) {
//                         if (box().isStateSet)
//                                 T::setState(box().currentState);
//
//                         if (box().isEventSet)
//                                 dispatch(box().event);
//                 }
//         }
// private:
//         inline void init(double waitTime, Macho::Alias currentState){
//                 box().waitTime = waitTime;
//                 box().startTime = ros::Time::now().toSec();
//                 box().isStateSet = true;
//                 box().currentState = currentState;
//         }
//         inline void init(double waitTime, auto * event){
//                 box().waitTime = waitTime;
//                 box().startTime = ros::Time::now().toSec();
//                 box().isEventSet = true;
//                 box().event = event;
//         }
//
// };


}


#endif
