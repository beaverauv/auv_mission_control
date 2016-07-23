#ifndef TASK_BUOY_H
#define TASK_BUOY_H

#include "auv_mission_control/PidManager.h"
#include <auv_mission_control/Camera.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>

class Task_Buoy{
public:
  Task_Buoy();
  Task_Buoy(PidManager* pm, Camera* cam);
  ~Task_Buoy();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  Camera cam_;

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


};

#endif
