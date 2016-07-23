#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "auv_mission_control/PidManager.h"
#include <auv_mission_control/Camera.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>


class Task_Gate{



public:
  Task_Gate();
  Task_Gate(PidManager* pm, Camera* cam);
  ~Task_Gate();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;


  int minR = 0; // R_MIN
  int maxR = 255; // R_MAX
  int minG = 0; // G_MIN
  int maxG = 255; // G_MAX
  int minB = 0; // B_MIN
  int maxB = 255; // B_MAX
  int ColorSpace = 0;
  const int minObjectArea = 20*20; //20x20 blob
  bool objectFound;



  int action = 0;
  Timer goToDepth_time;
  Timer driveForwards_time;
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
