// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskMarker.hpp>
#include <auv_mission_control/template_states.hpp>

int TaskMarker::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void TaskMarker::Init::run() { setState<Timer<Init>>(3.0); }
