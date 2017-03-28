#ifndef MISSION_H_
#define MISSION_H_

#include <auv_mission_control/Task.hpp>

class Mission : public Logger {
public:
  Mission(std::shared_ptr<PointerHandler> ph) : ph_(ph) {}

  ~Mission() {}

  std::string getTaskTag() { return std::string("[Mission Default]"); }

private:
  std::shared_ptr<PointerHandler> ph_;
};

#endif
