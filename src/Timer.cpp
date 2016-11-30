#include <auv_mission_control/Timer.hpp>

template<class T>
void Timer::Timer<T>::init(double waitTime, int i){
        Timer<T>::box().waitTime_ = waitTime;
        Timer<T>::box().startTime_ = ros::Time::now().toSec();
        Timer<T>::box().isStateMachineSet = true;
}
