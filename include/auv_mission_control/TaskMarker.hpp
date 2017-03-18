#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <auv_mission_control/Task.hpp>

class TaskMarker : public Task {
public:
  createTaskTag(TaskMarker);

  createTopState(TaskMarker);

  createState(Init);

  createNullState(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  createTaskFunctions(TaskMarker);

private:
  // variables go here;
  // Timer swayTimer;
  // Timer depthTimer;
  // Timer yawTimer;
  int action = 0;
  bool objectFound;
  double plantState_sway;
  double setpoint_sway;
  int counter_sway = 0;
  int counter_depth = 0;
};

#endif
