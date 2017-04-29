#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Gate.hpp>

namespace Task {

int Gate::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Gate::Init::run() { setState<Timer<Init>>(3.0); }

void Gate::SwitchTask::run() {}
}
