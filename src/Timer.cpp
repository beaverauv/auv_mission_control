#include "auv_mission_control/Timer.h"

Timer::Timer(){

}

Timer::~Timer(){

}

void Timer::start(){
  startTime = ros::Time::now().toSec();
}

double Timer::getTime(){
  return ros::Time::now().toSec() - startTime;
}
