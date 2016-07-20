#ifndef PIDTEST
#define PIDTEST

#include <auv_mission_control/pid_manager.h>

class pidtest{
public:
  pidtest();
  pidtest(Pid_Manager* pm);
  ~pidtest();

  int execute();

private:
  //variables go here;
  Pid_Manager pm_;


};

#endif
