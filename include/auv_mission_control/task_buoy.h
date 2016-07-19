#ifndef TASK_BUOY_H
#define TASK_BUOY_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>

class Task_Buoy{
public:
  Task_Buoy();
  Task_Buoy(Pid_Manager* pm, CameraManager* cam);
  ~Task_Buoy();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  CameraManager cam_;
};

#endif
