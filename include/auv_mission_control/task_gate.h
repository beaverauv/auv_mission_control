#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>


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
<<<<<<< Updated upstream

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

  int minR = 0; // R_MIN
  int maxR = 255; // R_MAX
  int minG = 0; // G_MIN
  int maxG = 255; // G_MAX
  int minB = 0; // B_MIN
  int maxB = 255; // B_MAX
  int ColorSpace = 0;
  const int minObjectArea = 20*20; //20x20 blob
  bool objectFound;

  double surgeSpeed = 40;
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

<<<<<<< Updated upstream
  cv::Scalar bottomLowRed(109, 0, 97);
  cv::Scalar bottomHighRed(123, 95, 204);

  cv::Scalar upperLowRed(100, 98, 0);
  cv::Scalar upperHighRed(180, 95, 204);


  cv::Scalar bottomLowGreen(79, 218, 136);
  cv::Scalar bottomHighGreen(80, 255, 255);

  cv::Scalar upperLowGreen(79, 228, 0);
  cv::Scalar upperHighGreen(85, 255, 157);


  cv::Scalar lowYellow(0,0,0);
  cv::Scalar highYellow(79, 193, 255);
=======
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






>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
};

#endif
