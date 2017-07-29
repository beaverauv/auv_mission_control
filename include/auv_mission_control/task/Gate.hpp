#ifndef TASKGATE_H
#define TASKGATE_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Gate : public Logger {
public:
  Gate(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Gate() {}

  AUV_LOG_TAG(Gate);

  AUV_CREATE_TOP_STATE(Gate);

  AUV_CREATE_STATE(Init);
  AUV_CREATE_STATE(SwitchTask);
  AUV_CREATE_STATE(GoToDepth);
  AUV_CREATE_STATE(DriveForwards);
  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_FUNCTIONS(Gate);

private:
  // variables go here;
  float timer = 0;
  float sketchZero;
  bool firstRun = true;
};
}

#endif
