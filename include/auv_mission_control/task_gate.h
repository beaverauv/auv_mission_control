#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>
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
