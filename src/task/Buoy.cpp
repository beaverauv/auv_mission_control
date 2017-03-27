#include <auv_mission_control/task/Buoy.hpp>
#include <auv_mission_control/template_states.hpp>

namespace Task {

int Buoy::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Buoy::Init::run() { setState<Timer<Init>>(3.0); }
}
