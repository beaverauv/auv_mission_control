#ifndef BACKPLANE_MANAGER_HPP_
#define BACKPLANE_MANAGER_HPP_

#include <auv_mission_control/Task.hpp>

class BackplaneManager : public Logger {
public:
  BackplaneManager();
  ~BackplaneManager();

  void setBackplaneState(int state);

  void setBuoyOrder(int order);

  bool getStartSwitchState();
};

#endif
