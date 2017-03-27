#ifndef TASKEXAMPLE_H_
#define TASKEXAMPLE_H_

#include <auv_mission_control/task/Task.hpp>

class TaskExample : public TaskBase {
public:
  TaskExample(std::shared_ptr<PointerHandler> ph)
      : ph_(ph), sm_(Macho::State<Top>(this, ph_)) {}

  ~TaskExample() {}

  AUV_CREATE_TOP_STATE(TaskExample);

  AUV_LOG_TAG(TaskExampl);

  AUV_CREATE_STATE(Init);

  AUV_CREATE_EMPTY_STATE(Idle);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  AUV_CREATE_FUNCTIONS(TaskExample);
};

#endif
