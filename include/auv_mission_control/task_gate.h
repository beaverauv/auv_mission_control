#ifndef TASK_GATE_H
#define TASK_GATE_H

#include "task.h"
#include "auv_mission_control/pid_manager.h"

class Task_Gate: public Task{
public:
  Task_Gate (){}
  Task_Gate(Pid_Manager* pm);
  virtual ~Task_Gate();

  void execute();

private:
  //variables go here;
};

#endif
