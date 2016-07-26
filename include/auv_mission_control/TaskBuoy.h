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


  cv::Scalar bottomLowRed = cv::Scalar(109, 0, 97);
  cv::Scalar bottomHighRed = cv::Scalar(123, 95, 204);

  cv::Scalar upperLowRed = cv::Scalar(100, 98, 0);
  cv::Scalar upperHighRed = cv::Scalar(180, 95, 204);

  cv::Scalar bottomLowGreen = cv::Scalar(79, 218, 136);
  cv::Scalar bottomHighGreen = cv::Scalar(80, 255, 255);

  cv::Scalar upperLowGreen = cv::Scalar(79, 228, 0);
  cv::Scalar upperHighGreen = cv::Scalar(85, 255, 157);

  cv::Scalar lowYellow = cv::Scalar(0,0,0);
  cv::Scalar highYellow = cv::Scalar(79, 193, 255);


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
  int lastAngle;

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

  double redDepth; //depth of red buoy

  Timer ramRed;
  int ramRedCounter = 0;

};

#endif
