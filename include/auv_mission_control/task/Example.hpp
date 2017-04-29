#ifndef TASKEXAMPLE_H_
#define TASKEXAMPLE_H_

#include <auv_mission_control/Task.hpp>

namespace Task {

class Example : public Logger {
public:
  Example(std::shared_ptr<PointerHandler> ph)
      : INHERITED(ph), sm_(Macho::State<Top>(this)) {}

  ~Example() {}

  AUV_CREATE_TOP_STATE(Example);

  AUV_LOG_TAG(Exmpl);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_CREATE_STATE(SwitchTask);

  AUV_CREATE_FUNCTIONS(Example);
};
}

#endif
