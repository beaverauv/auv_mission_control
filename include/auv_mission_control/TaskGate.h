#ifndef TaskGate_H
#define TaskGate_H


#include <iostream>
#include <cmath>
#include <unistd.h>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>


class TaskGate{
public:
  TaskGate();
  TaskGate(PidManager* pm, Camera* cam);
  ~TaskGate();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;


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
