#include <auv_mission_control/Timer.hpp>

template<class T>
void Timer::TimerOld<T>::init(double waitTime, int i){
        TimerOld<T>::box().waitTime_ = waitTime;
        TimerOld<T>::box().startTime_ = ros::Time::now().toSec();
        TimerOld<T>::box().isStateMachineSet = true;
}
