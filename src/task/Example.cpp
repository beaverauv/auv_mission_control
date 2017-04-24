#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Example.hpp>

namespace Task {

int Example::execute() {

  if (checkEventQueue()) {
    sm_->run();
  }
}

void Example::Init::run() {
  //
  self().queueState<Timer<Whatever>>(1.0);
}

void Example::Whatever::run() { ph().queueTask("Marker"); }
}
