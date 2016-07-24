#ifndef TaskGate_H
#define TaskGate_H

<<<<<<< HEAD:include/auv_mission_control/task_gate.h
#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/Camera.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
=======

#include <iostream>
#include <cmath>
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:include/auv_mission_control/TaskGate.h
#include <unistd.h>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>


class TaskGate{
public:
<<<<<<< HEAD:include/auv_mission_control/task_gate.h
  Task_Gate();
  Task_Gate(Pid_Manager* pm, Camera* cam);
  ~Task_Gate();
=======
  TaskGate();
  TaskGate(PidManager* pm, Camera* cam);
  ~TaskGate();
>>>>>>> aecae700d55f4c671ffd932ac9a8c9c3b7a29d3d:include/auv_mission_control/TaskGate.h

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  Camera cam_;

  bool startTimer = 0;

  int ColorSpace = 0;
  const int minObjectArea = 20*20; //20x20 blob
  bool objectFound;



  int action = 0;
  Timer goToDepth_time;
  int depthCounter = 0;
  Timer driveForwards_time;
  int forwardCounter = 0;
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
