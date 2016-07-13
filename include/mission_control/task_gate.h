#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "task.h"

class Task_Gate: public Task{
public:
  Task_Gate(pid_manager* pm);
  virtual ~Task_Gate();

  void execute();

private:
  //variables go here;
};

#endif
