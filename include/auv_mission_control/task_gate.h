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
};

#endif
