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

  AUV_CREATE_EMPTY_STATE(Nowhere);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  AUV_CREATE_FUNCTIONS(Example);
  typedef Logger INHERITED;
};
}

#endif
