#ifndef TASKEXAMPLE_H_
#define TASKEXAMPLE_H_

#include <auv_mission_control/Task.hpp>

class TaskExample : public Task {
public:
  createTopState(TaskExample);

  createTaskTag(TaskExampl);

  createState(Init);

  createNullState(Nowhere);

  AUV_SUBSTATE(Whatever, Top) {

    AUV_STATE(Whatever);

    void run();
  };

  createTaskFunctions(TaskExample);
};

#endif
