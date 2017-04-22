#include <auv_mission_control/task/Marker.hpp>
#include <auv_mission_control/TemplateStates.hpp>

namespace Task {

int Marker::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Marker::Init::run() { setState<Timer<Init>>(3.0); }
}
