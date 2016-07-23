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


};

#endif
