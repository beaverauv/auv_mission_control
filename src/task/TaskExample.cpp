#include <auv_mission_control/task/TaskExample.hpp>
#include <auv_mission_control/template_states.hpp>

int TaskExample::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void TaskExample::Init::run() {
  //
  self().queueState<Timer<Whatever>>(1.0);
}

void TaskExample::Whatever::run() {
  setState<Timer<Init>>(0.0, StateMachine::Marker::alias());
}
