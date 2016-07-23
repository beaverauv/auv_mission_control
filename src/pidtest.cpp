#include <auv_mission_control/pidtest.h>


pidtest::pidtest(PidManager* pm) : pm_(*pm){
}

pidtest::pidtest(){

}

pidtest::~pidtest(){

}


int pidtest::execute(){
  pm_.setSetpoint(AXIS_SWAY, INPUT_CAM_FRONT, 10.0);
  ROS_INFO("%f", pm_.getDepth());
  ROS_INFO("%d", pm_.getKill());
  ROS_INFO("%d", pm_.getStart());

}
