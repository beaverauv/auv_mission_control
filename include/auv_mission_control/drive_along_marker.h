#ifndef DRIVE_ALONG_MARKER_H
#define DRIVE_ALONG_MARKER_H

#include "auv_mission_control/pid_manager.h"
#include <auv_mission_control/CameraManager.h>
#include "auv_mission_control/state_machine.h"
#include "outcomes.h"
#include <unistd.h>


int action = 0;
bool objectFound;
double plantState_sway;
double setpoint_sway;
int counter_sway = 0;
int counter_depth = 0;

class Follow_Marker{
public:
  Follow_Marker();
  Follow_Marker(Pid_Manager* pm, CameraManager* cam);
  ~Follow_Marker();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;
  CameraManager cam_;
  Timer swayTimer;
  Timer depthTimer;
  Timer yawTimer;

};

#endif
