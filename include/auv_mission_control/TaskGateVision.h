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
  Timer goToDepth_time;
  Timer forwardTimer;
  bool startTimer;
  int rosInfoCounter;
  Timer finalTimer;//names and stuff
  Timer dedReckonTimer;
  int dedTimer = 0;
  int depthCounter = 0;
  int goCounter = 0;
  int finalCounter = 0;
  int maxJump = 45;
  
  double killSwitch;
  double currentDepth;
  double startSwitch;

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

  double prevPosX;
  double prevPosY;
  double changePosX;
  double changePosY;



  int camInUse;

  int minR = 0;
  int maxR = 255;
  int minG = 0;
  int maxG = 255;
  int minB = 0;
  int maxB = 255;


};

#endif
