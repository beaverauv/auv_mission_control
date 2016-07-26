#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <cmath>
#include <unistd.h>


#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>



class TaskMarker{
public:
  TaskMarker();
  TaskMarker(PidManager* pm, Camera* cam);
  ~TaskMarker();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;
  Timer swayTimer;
  Timer depthTimer;
  Timer yawTimer;
  int action = 0;
  bool objectFound;
  double plantState_sway;
  double setpoint_sway;
  int counter_sway = 0;
  int counter_depth = 0;

  const int minObjectArea = 20*20; //20x20 blob

  int minH = 0; // R_MIN
  int maxH = 255; // R_MAX
  int minS = 0; // G_MIN
  int maxS = 255; // G_MAX
  int minV = 0; // B_MIN
  int maxV = 255; // B_MAX

  int minR = 0; // R_MIN
  int maxR = 255; // R_MAX
  int minG = 0; // G_MIN
  int maxG = 255; // G_MAX
  int minB = 0; // B_MIN
  int maxB = 255; // B_MAX

  int minL = 0; // R_MIN
  int maxL = 255; // R_MAX
  int minA = 0; // G_MIN
  int maxA = 255; // G_MAX
  int minlaB = 0; // B_MIN
  int maxlaB = 255; // B_MAX

  int largest_area = 0;
  int largest_contour_index = 0;



};

#endif
