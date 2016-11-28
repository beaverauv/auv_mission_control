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
}


#endif
