#ifndef TaskBuoy_H
#define TaskBuoy_H

#include <unistd.h>

#include <auv_mission_control/PidManager.h>
#include <auv_mission_control/Camera.h>
#include <auv_mission_control/StateMachine.h>

class TaskBuoy{
public:
  TaskBuoy();
  TaskBuoy(PidManager* pm, TaskVision* vision);
  ~TaskBuoy();

  int execute();

private:
  //variables go here;
  PidManager pm_;
  TaskVision vision_;

  int currentColor;
  int objectFound;
  int action = 0;
  Timer goToDepth_time;
  int depthCounter = 0;
  Timer driveForwards_time;
  int forwardCounter = 0;
  Timer waitTimer;
  int waitCounter = 0;
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
