#ifndef TASK_GATE_VISION_H
#define TASK_GATE_VISION_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/Camera.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>


int action = 0;
bool objectFound;

int counter = 0;

class Task_Gate_Vision{
public:
  Task_Gate_Vision();
  Task_Gate_Vision(Pid_Manager* pm, Camera* cam);
  ~Task_Gate_Vision();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  Camera cam_;
  Timer depthTimer;
  Timer forwardTimer;
  Timer finalTimer;//names and stuff
  int depthCounter = 0;
  int goCounter = 0;
  int finalCounter = 0;

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
  int camInUse;

  int minR = 0;
  int maxR = 255;
  int minG = 0;
  int maxG = 255;
  int minB = 0;
  int maxB = 255;


};

#endif
