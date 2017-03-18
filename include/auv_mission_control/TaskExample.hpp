#ifndef TASKEXAMPLE_H_
#define TASKEXAMPLE_H_

#include <auv_mission_control/Task.hpp>

class TaskExample : public Task {
public:
  createTopState(TaskExample);

  createTaskTag(TaskExampl);

  createState(Init);

  createNullState(Nowhere);

  SUBSTATE(Whatever, Top) {

    STATE(Whatever);

    void run();
  };

  createTaskFunctions(TaskExample);
};

#endif
