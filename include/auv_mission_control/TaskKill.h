#ifndef TASKKILL_H
#define TASKKILL_H

#include <auv_mission_control/PidManager.h>


class TaskKill{
public:
  TaskKill();
  TaskKill(PidManager* pm);
  ~TaskKill();

  int execute();

private:
  //variables go here;
  PidManager pm_;
};

#endif
