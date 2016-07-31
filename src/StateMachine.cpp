#include <auv_mission_control/StateMachine.h>


StateMachine::StateMachine(){
  AUV_INFO("Init");
  pm_ = PidManager(&nh_);
  vision_ = TaskVision(&cam_);
  //gate_ = TaskGate(&pm_, &vision_);

}

StateMachine::~StateMachine(){

}

std::string StateMachine::getTag(){
  return std::string("[State Machine]");
}

int StateMachine::execute(){
  AUV_INFO("Execute");
}
