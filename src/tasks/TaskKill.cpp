#include <auv_mission_control/TaskKill.h>

TaskKill::TaskKill(){
}


TaskKill::TaskKill(PidManager* pm) : pm_(*pm){
}

TaskKill::~TaskKill(){

}

int TaskKill::execute(){
  pm_.setPidEnabled(AXIS_SURGE, false);
  pm_.setPidEnabled(AXIS_SWAY, false);
  pm_.setPidEnabled(AXIS_HEAVE, false);
  pm_.setPidEnabled(AXIS_YAW, false);

  pm_.setControlEffort(AXIS_SURGE, 0);
  pm_.setControlEffort(AXIS_SWAY, 0);
  pm_.setControlEffort(AXIS_HEAVE, 0);
  pm_.setControlEffort(AXIS_YAW, 0);

}
