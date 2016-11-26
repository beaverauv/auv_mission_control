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
        void run();
private:
        void entry(){
                //AUV_DEBUG("Timer::entry");
        }
        void init(double waitTime, Macho::Alias currentState){
                box().waitTime = waitTime;
                box().currentState = currentState;
                box().startTime = ros::Time::now().toSec();
        }
};

template<class T>
void Timer<T>::run() {
        if ((ros::Time::now().toSec() - box().startTime) > box().waitTime) {
                //AUV_DEBUG("Waiting done, switching states");
                //setState(Macho::Anchor<box().currentState, 1>);
                T::setState(box().currentState);
                //std::cout << "here" << std::endl;
        }

}


}


#endif
