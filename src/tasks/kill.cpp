#include "auv_mission_control/kill.h"
#include "auv_mission_control/pid_manager.h"

Task_Gate::Task_Gate(){
}


Task_Gate::Task_Gate(Pid_Manager* pm) : pm_(*pm){
}

Task_Gate::~Task_Gate(){

}

int Task_Gate::execute(){
  pm_.pidEnable(AXIS_SURGE, false);
  pm_.pidEnable(AXIS_SWAY, false);
  pm_.pidEnable(AXIS_HEAVE, false);
  pm_.pidEnable(AXIS_YAW, false);

  pm_.controlEffort_set(AXIS_SURGE, 0);
  pm_.controlEffort_set(AXIS_SWAY, 0);
  pm_.controlEffort_set(AXIS_HEAVE, 0);
  pm_.controlEffort_set(AXIS_YAW, 0);

}
