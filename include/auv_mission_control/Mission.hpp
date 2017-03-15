#ifndef MISSION_H_
#define MISSION_H_

#include <auv_mission_control/Task.hpp>

class Mission : public Task {
public:
  Mission();
  ~Mission();

  std::string getTaskTag() { return std::string("[Mission Default]"); }

private:
};

#endif
