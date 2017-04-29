#ifndef TASKMARKER_H
#define TASKMARKER_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Marker : public Logger {
public:
  Marker(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Marker() {}

  AUV_LOG_TAG(Marker);

  AUV_CREATE_TOP_STATE(Marker);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_STATE(SwitchTask);

  AUV_CREATE_FUNCTIONS(Marker);

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
}

#endif
