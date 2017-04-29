#ifndef TASKTEST_H
#define TASKTEST_H

#include <auv_mission_control/Task.hpp>

namespace Task {

class Test : public Logger {
public:
  Test(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Test() {}

  AUV_LOG_TAG(Test);

  AUV_CREATE_TOP_STATE(Test);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_STATE(SwitchTask);

  AUV_CREATE_FUNCTIONS(Test);
};
}

#endif
