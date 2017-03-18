// #include <auv_mission_control/StateMachine.hpp>
#include <auv_mission_control/TaskGate.hpp>
#include <auv_mission_control/template_states.hpp>

int TaskGate::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void TaskGate::Init::run() { setState<Timer<Init>>(3.0); }
