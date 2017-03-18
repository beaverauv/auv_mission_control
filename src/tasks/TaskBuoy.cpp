#include <auv_mission_control/TaskBuoy.hpp>
#include <auv_mission_control/template_states.hpp>

int TaskBuoy::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void TaskBuoy::Init::run() { setState<Timer<Init>>(3.0); }
