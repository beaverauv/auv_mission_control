#ifndef TaskGateVision_H
#define TaskGateVision_H

#include <cmath>
#include <unistd.h>

#include <opencv2/imgproc/imgproc.hpp>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>



class TaskGateVision{
public:
  TaskGateVision();
  TaskGateVision(PidManager* pm, Camera* cam);
  ~TaskGateVision();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;
  Timer depthTimer;
  Timer forwardTimer;
  Timer finalTimer;//names and stuff
  int depthCounter = 0;
  int goCounter = 0;
  int finalCounter = 0;

  int action = 0;
  bool objectFound;

  int counter = 0;

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
