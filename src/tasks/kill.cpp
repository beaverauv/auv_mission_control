#include "auv_mission_control/kill.h"
#include "auv_mission_control/PidManager.h"

Task_Gate::Task_Gate(){
}


Task_Gate::Task_Gate(PidManager* pm) : pm_(*pm){
}

Task_Gate::~Task_Gate(){

}

int Task_Gate::execute(){
  pm_.setPidEnabled(AXIS_SURGE, false);
  pm_.setPidEnabled(AXIS_SWAY, false);
  pm_.setPidEnabled(AXIS_HEAVE, false);
  pm_.setPidEnabled(AXIS_YAW, false);

  pm_.controlEffort_set(AXIS_SURGE, 0);
  pm_.controlEffort_set(AXIS_SWAY, 0);
  pm_.controlEffort_set(AXIS_HEAVE, 0);
  pm_.controlEffort_set(AXIS_YAW, 0);

}
