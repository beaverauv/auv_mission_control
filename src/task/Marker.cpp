#include <auv_mission_control/TemplateStates.hpp>
#include <auv_mission_control/task/Marker.hpp>

namespace Task {

int Marker::execute() {
  if (checkEventQueue()) {
    sm_->run();
  }
}

void Marker::Init::run() {
  QUEUE_ACTION(Timer, 3.0);
  QUEUE_STATE(Init);
}

void Marker::SwitchTask::run(){};
}
