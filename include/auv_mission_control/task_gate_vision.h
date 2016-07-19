#ifndef TASK_GATE_VISION_H
#define TASK_GATE_VISION_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>


int action = 0;
bool objectFound;

int counter = 0;

class Task_Gate_Vision{
public:
  Task_Gate_Vision();
  Task_Gate_Vision(Pid_Manager* pm, CameraManager* cam);
  ~Task_Gate_Vision();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  CameraManager cam_;
  Timer depthTimer;
  Timer forwardTimer;
  Timer finalTimer;//names and stuff
  double surgeSpeed = 25;
  double previousDepth;
  double distanceFromEdge_left;
  double distanceFromEdge_right;
  bool outOfSight;
  double plantState_sway;
  double plantState_heave;
  double setpoint_sway;
  double setpoint_heave;
  double setpoint_surge;
  double plantState_surge;

};

#endif
