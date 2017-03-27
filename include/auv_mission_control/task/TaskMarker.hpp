#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <auv_mission_control/task/Task.hpp>

class TaskMarker : public TaskBase {
public:
  TaskMarker(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~TaskMarker() {}

  AUV_LOG_TAG(TaskMarker);

  AUV_CREATE_TOP_STATE(TaskMarker);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  AUV_CREATE_FUNCTIONS(TaskMarker);

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
