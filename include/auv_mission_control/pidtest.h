#ifndef PIDTEST
#define PIDTEST

#include <auv_mission_control/PidManager.h>

class pidtest{
public:
  pidtest();
  pidtest(PidManager* pm);
  ~pidtest();

  int execute();

private:
  //variables go here;
  PidManager pm_;


};

#endif
