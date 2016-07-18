#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "outcomes.h"

class Task_Gate{
public:
  Task_Gate();
  Task_Gate(Pid_Manager* pm, CameraManager* cam);
  ~Task_Gate();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  CameraManager cam_;
};

#endif
