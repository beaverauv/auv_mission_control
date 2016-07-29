#ifndef TaskBuoy_H
#define TaskBuoy_H

#include <unistd.h>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>

class TaskBuoy{
public:
  TaskBuoy();
  TaskBuoy(PidManager* pm, Camera* cam);
  ~TaskBuoy();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;

  int ColorSpace = 0;
  int currentCamera = INPUT_CAM_FRONT;


  cv::Scalar redMin = cv::Scalar(73, 153, 143);
  cv::Scalar redMax = cv::Scalar(107, 187, 255);

  cv::Scalar greenMin = cv::Scalar(0, 0, 0);
  cv::Scalar greenMax = cv::Scalar(255, 255, 255);

  int minR = 0; // R_MIN
  int maxR = 255; // R_MAX
  int minG = 0; // G_MIN
  int maxG = 255; // G_MAX
  int minB = 0; // B_MIN
  int maxB = 255; // B_MAX

  const int minObjectArea = 20*20; //20x20 blob
  bool objectFound;
  int currentColor;

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

  double redDepth; //depth of red buoy

  Timer ramRed;
  int ramRedCounter = 0;

};

#endif
